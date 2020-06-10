#include "Renderer.h"

#include <iostream>

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError()) { 
		std::cout << "[OpenGL Error] (" << error << "): " << function << ": " << file << ": " << line << std::endl; 
		return false;
	}
	return true;
}

std::string GetGLVersion()
{
	GLCall(std::string glVersion = (const char*)glGetString(GL_VERSION));
	return glVersion;
}

void Renderer::Clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray & va, const IndexBuffer & ib, const Shader & shader) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr)); // For now, we are assuming ib will be uint and mode will be triangle
}

void Renderer::EnableBlend() const
{
	GLCall(glEnable(GL_BLEND)); // Enable blend
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); // Set up blend function
}
