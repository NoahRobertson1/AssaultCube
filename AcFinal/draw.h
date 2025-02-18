#pragma once
#pragma comment(lib, "OpenGL32.lib")
#include <Windows.h>
#include <stdio.h>
#include <gl/GL.h>
#include "vector.h"


namespace GL {
	void SetupOrtho();
	void RestoreGL();
	void DrawFilledRect(vec2 v1, vec2 v2, vec2 v3, vec2 v4, const GLubyte color[3]);
	void DrawOutline(vec2 v1, vec2 v2, vec2 v3, vec2 v4, float thickness, const GLubyte color[3]);
	void DrawLine(vec2 v1, vec2 v2, float thickness, const GLubyte color[3]);

	class font {
	public:
		bool bBuilt = false;
		unsigned int base;
		HDC hdc = nullptr;
		int height;
		int width;

		void build(int height);
		void print(float x, float y, const unsigned char color[3], const char* format, ...);

		vec3 centerText(float x, float y, float width, float height, float textWidth, float textHeight);
		float centerText(float x, float textWidth);
	};

	bool WorldToScreen(vec3 pos, vec2& screen, float matrix[16], int windowWidth, int windowHeight);
}
