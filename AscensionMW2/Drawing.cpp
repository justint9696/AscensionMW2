#include "Drawing.h"
#include "Functions.h"
#include "Game.h"
#include "Struct.h"

const float COLOR_WHITE[4] = { 1.f, 1.f, 1.f, 1.f };
const float COLOR_BLACK[4] = { 0.f, 0.f, 0.f, 1.f };
const float COLOR_BLACK_FADE[4] = { 0.f, 0.f, 0.f, .5f };
const float COLOR_WHITE_FADE[4] = { 1.f, 1.f, 1.f, .25f };
const float MENU_OVERLAY[4] = { 0.f, 0.f, 0.f, .25f };
const float COLOR_RED[4] = { 1.f, 0.f, 0.f, 1.f };
const float COLOR_GREEN[4] = { 0.f, 5.f, 0.f, 1.f };
const float COLOR_BLUE[4] = { 0.f, 0.f, 1.f, 1.f };

void DrawText(const char *text, const char *font, float fontScale, float x, float y, const float *color) {
	UI_DrawText(GetActiveScreenPlacement(), text, 0x7FFFFFFF, R_RegisterFont(font, 0), x, y, 0, 0, fontScale, color, 6);
}

void DrawTextWithGlow(const char *text, const char *font, float fontScale, float x, float y, const float *color, const float *glowColor) {
	UI_DrawTextWithGlow(GetActiveScreenPlacement(), text, 0x7FFFFFFF, R_RegisterFont(font, 0), x, y, 0, 0, fontScale, color, 6, glowColor, 0, 0, 0, 0, 0, 0, 0, 255);
}

void DrawShader(float x, float y, float width, float height, const float *color, const char *shader) {
	UI_DrawHandlePic(GetActiveScreenPlacement(), x, y, width, height, 0, 0, color, GetMaterial(shader));
}