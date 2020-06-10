#pragma once
#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

// Breaks the exe and points out line if x is false
#define ASSERT(x) if(!(x)) __debugbreak();

// Debugging wrapper for OpenGL calls
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

// Clear internal errors of OpenGL
void GLClearError();

// Log internal errors to console
bool GLLogCall(const char* function, const char* file, int line);

// Retrieve OpenGL version
std::string GetGLVersion();

class Renderer
{
public:
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void EnableBlend() const;
};