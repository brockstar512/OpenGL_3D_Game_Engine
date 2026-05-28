#include "ShaderProgram.h"
#include "ShaderProgram.h"
#include "ShaderProgram.h"
#include "ShaderProgram.h"
#include "graphics/ShaderProgram.h"

namespace engine 
{

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(m_shaderProgramId);
	}

	ShaderProgram::ShaderProgram(GLuint shaderProgramID) : m_shaderProgramId(shaderProgramID){}

	void ShaderProgram::Bind()
	{
		glUseProgram(m_shaderProgramId);
	}

	GLint engine::ShaderProgram::GetUniformLocation(const std::string& name)
	{
		auto it = m_uniformLocationCache.find(name);
		if (it != m_uniformLocationCache.end())
		{
			return it->second;
		}
		GLint location = glGetUniformLocation(m_shaderProgramId,name.c_str());
		m_uniformLocationCache[name] = location;
		return location;
	}

	void engine::ShaderProgram::SetUniform(const std::string& name, float value)
	{
		auto location = glGetUniformLocation(m_shaderProgramId, name.c_str());
		glUniform1f(location, value);

	}
}