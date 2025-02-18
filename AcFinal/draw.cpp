#include "draw.h"

void GL::SetupOrtho() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glViewport(0, 0, viewport[2], viewport[3]);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
}

void GL::RestoreGL() {
	glPopMatrix();
	glPopAttrib();
}

void GL::DrawFilledRect(vec2 v1, vec2 v2, vec2 v3, vec2 v4, const GLubyte color[3]) {
	glColor3ub(color[0], color[1], color[2]);
	glBegin(GL_QUADS);
	glVertex2f(v1.x, v1.y);
	glVertex2f(v2.x, v2.y);
	glVertex2f(v4.x, v4.y);
	glVertex2f(v3.x, v4.y);
	glEnd();
}

void GL::DrawOutline(vec2 v1, vec2 v2, vec2 v3, vec2 v4, float thickness, const GLubyte color[3]) {
	glLineWidth(thickness);
	glBegin(GL_LINE_STRIP);
	glColor3f(color[0], color[1], color[2]);
	glVertex2f(v1.x, v1.y);
	glVertex2f(v2.x, v2.y);
	glVertex2f(v4.x, v4.y);
	glVertex2f(v3.x, v3.y);
	glVertex2f(v1.x, v1.y);
	glEnd();
}

void GL::DrawLine(vec2 v1, vec2 v2, float thickness, const GLubyte color[3]) {
	glLineWidth(thickness);
	glBegin(GL_LINE_STRIP);
	glColor3f(color[0], color[1], color[2]);
	glVertex2f(v1.x, v1.y);
	glVertex2f(v2.x, v2.y);
	glEnd();
}

void GL::font::build(int height) {
	hdc = wglGetCurrentDC();
	base = glGenLists(96);
	HFONT hFont = CreateFontA(-height, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE | DEFAULT_PITCH, "Consolas");
	HFONT holdFont = (HFONT)SelectObject(hdc, hFont);
	wglUseFontBitmaps(hdc, 32, 96, base);
	SelectObject(hdc, holdFont);
	DeleteObject(hFont);

	bBuilt = true;
}

void GL::font::print(float x, float y, const unsigned char color[3], const char* format, ...) {
	glColor3ub(color[0], color[1], color[2]);
	glRasterPos2f(x, y);

	char text[100];
	va_list args;

	va_start(args, format);
	vsprintf_s(text, 100, format, args);
	va_end(args);

	glPushAttrib(GL_LIST_BIT);
	glListBase(base - 32);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
}

/*vec3 GL::font::centerText(float x, float y, float width, float height, float textWidth, float textHeight) {
	vec3 text; 
	text.x = x + (width - textWidth) / 2;
	text.y = y + textHeight;
	return text;
}*/

float GL::font::centerText(float x, float textWidth) {
	if (width > textWidth) {
		float difference = width - textWidth;
		return(x + (difference / 2));
	}
	else {
		float difference = textWidth - width;
		return (x - (difference / 2));
	}
}


bool GL::WorldToScreen(vec3 pos, vec2& screen, float matrix[16], int windowWidth, int windowHeight) {
	vec4 clipCoords;
	clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
	clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
	clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
	clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

	if (clipCoords.w < 0.1f)
		return false;

	//perspective division, dividing by clip.W = Normalized Device Coordinates
	vec3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	//Transform to window coordinates
	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	return true;
}
