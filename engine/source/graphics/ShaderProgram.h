#pragma once
#include <GL/glew.h>
#include <string>
#include <unordered_map>
namespace engine
{
	class ShaderProgram
	{
	public:
		ShaderProgram() = delete;
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		~ShaderProgram();
		explicit ShaderProgram(GLuint shaderProgramID);
		void Bind();
		GLint GetUniformLocation(const std::string& name);
		void SetUniform(const std::string& name, float value);
	private:
		std::unordered_map<std::string, GLuint> m_uniformLocationCache;
		GLuint m_shaderProgramId = 0;
		
	};
}