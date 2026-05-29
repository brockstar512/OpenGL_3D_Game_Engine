# Project Architecture

## Overview

The project is split into two CMake targets: a static **Engine** library and a **Game** executable that consumes it.

```
GameDevelopmentProject (executable)
└── Engine (static library)
    ├── GLFW  (windowing + input events)
    └── GLEW  (OpenGL extension loading)
```

---

## Entry Point

`source/main.cpp`

1. Creates a `Game` instance (heap allocated)
2. Hands it to the `Engine` singleton via `SetApplication()`
3. Calls `Engine::Init()` → `Engine::Run()` → `Engine::Destroy()`

---

## Engine Layer

### `Engine` — `engine/source/Engine.h`
A singleton. Owns and controls everything at the top level.

- Initializes GLFW and creates the window
- Initializes GLEW (loads OpenGL function pointers)
- Owns the `InputManager` and `GraphicsAPI`
- Owns the `Application` via `unique_ptr`
- Runs the game loop: poll events → compute deltaTime → call `Application::Update()`

### `Application` — `engine/source/Application.h`
Abstract base class that the game implements.

```
virtual bool Init()
virtual void Update(float deltaTime)
virtual void Destroy()
```

The game inherits from this and overrides all three.

### `InputManager` — `engine/source/input/InputManager.h`
Tracks the pressed state of up to 256 keys in a `std::array<bool, 256>`.

- Only `Engine` can construct it (`Engine` is a `friend`)
- Copy and move are deleted — there is exactly one, owned by `Engine`
- GLFW key callbacks call `SetKeyPressed()` on it
- Game code calls `isKeyPressed(keyCode)` to read state

### `GraphicsAPI` — `engine/source/graphics/GraphicsAPI.h`
Wraps raw OpenGL calls behind a cleaner interface.

| Method | What it does |
|---|---|
| `CreateShaderProgram()` | Compiles vertex + fragment shaders, links them, returns a `shared_ptr<ShaderProgram>` |
| `CreateVertexBuffer()` | Uploads float vertex data to a VBO on the GPU, returns the handle |
| `CreateIndexBuffer()` | Uploads index data to an EBO on the GPU, returns the handle |
| `BindMaterial()` | Calls `Material::Bind()` |
| `BindMesh()` | Calls `Mesh::Bind()` |
| `DrawMesh()` | Calls `Mesh::Draw()` |

---

## Render Layer

### `ShaderProgram` — `engine/source/graphics/ShaderProgram.h`
Wraps a compiled and linked OpenGL shader program (`GLuint`).

- Non-copyable (GPU resource — copying the handle would cause double-free)
- Movable — zeroes out the moved-from handle so it doesn't delete the program on destruction
- Caches `glGetUniformLocation` results to avoid repeated GPU queries
- `Bind()` calls `glUseProgram()`

### `VertexLayout` — `engine/source/graphics/VertexLayout.h`
A plain struct describing how vertex data is laid out in a buffer.

```
VertexLayout
├── stride          — total byte size of one vertex
└── elements[]
    ├── index       — attribute location in the shader
    ├── size        — number of components (e.g. 3 for vec3)
    ├── type        — GL_FLOAT etc.
    └── offset      — byte offset of this attribute within one vertex
```

### `Mesh` — `engine/source/render/Mesh.h`
Owns a VAO, VBO, and optionally an EBO on the GPU.

Two constructors:
- `Mesh(layout, vertices, indices)` — indexed draw. Vertices can be shared between triangles. Uses `glDrawElements`.
- `Mesh(layout, vertices)` — non-indexed draw. Vertices listed in drawing order. Uses `glDrawArrays`.

`Draw()` checks `m_indexCount > 0` to decide which path to take.

Non-copyable (owns GPU handles).

### `Material` — `engine/source/render/Material.h`
Pairs a `ShaderProgram` with a set of uniform values.

- Holds a `shared_ptr<ShaderProgram>` (multiple materials can share a shader)
- Stores float params in an `unordered_map<string, float>`
- `Bind()` calls `ShaderProgram::Bind()` then pushes all stored params as uniforms

---

## Game Layer

### `Game` — `source/Game.h`
Inherits from `Application`. Implements the actual game logic.

**`Init()`**
1. Defines vertex and fragment shader source strings (GLSL 330 core)
   - Vertex shader: reads `position` (location 0) and `color` (location 1), passes color to fragment shader
   - Fragment shader: outputs the interpolated color
2. Creates a `ShaderProgram` via `GraphicsAPI::CreateShaderProgram()`
3. Assigns it to `m_material`
4. Defines a quad (4 vertices, 6 indices — two triangles) with per-vertex RGB color
5. Builds a `VertexLayout` (position: 3 floats, color: 3 floats, stride: 24 bytes)
6. Creates `m_mesh` from the layout, vertices, and indices

**`Update(deltaTime)`**
- Reads `InputManager` — logs to console when the A key is held

---

## Data Flow

```
main()
  └── Engine::Init()
        ├── glfwInit() + window creation
        ├── glewInit()
        └── Game::Init()
              ├── GraphicsAPI::CreateShaderProgram()  →  ShaderProgram (on GPU)
              ├── Material::SetShaderProgram()
              └── Mesh()  →  VAO/VBO/EBO (on GPU)

Engine::Run()  [loop]
  ├── glfwPollEvents()  →  keyCallback()  →  InputManager::SetKeyPressed()
  ├── deltaTime calculation
  └── Game::Update()
        └── InputManager::isKeyPressed()

Engine::Destroy()
  └── Game::Destroy()
  └── glfwTerminate()
```

---

## Convenience Header

`engine/source/eng.h` — single include for game code. Pulls in:
- `Application.h`
- `Engine.h`
- `InputManager.h`
- `graphics/ShaderProgram.h`
- `graphics/GraphicsAPI.h` *(if added)*
