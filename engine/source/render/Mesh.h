#pragma once
#include <GL/glew.h>
#include "graphics/VertexLayout.h"

namespace engine
{
    class Mesh
    {
    public:
        //This is for meshes where vertices are reused through an index buffer/EBO.
        Mesh(const VertexLayout& layout, const std::vector<float>& vertices, const std::vector<uint32_t>& indices);
        //This is for simple meshes where the vertex buffer already lists every vertex in drawing order.
        Mesh(const VertexLayout& layout, const std::vector<float>& vertices);
        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;

        void Bind();
        void Draw();

    private:
        VertexLayout m_vertexLayout;
        GLuint m_VBO = 0;
        GLuint m_EBO = 0;
        GLuint m_VAO = 0;

        size_t m_vertexCount = 0;
        size_t m_indexCount = 0;
    };
}




