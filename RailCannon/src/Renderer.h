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
	void SetClearColor(float v0, float v1, float v2, float v3) const;
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void EnableBlend() const;

	inline void SetClearColorBlack() const { SetClearColor(0.0f, 0.0f, 0.0f, 1.0f); }
	inline void SetClearColorWhite() const { SetClearColor(1.0f, 1.0f, 1.0f, 1.0f); }
};