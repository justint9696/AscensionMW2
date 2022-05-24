#ifndef _DRAWING_
#define _DRAWING_

extern const float COLOR_WHITE[4], COLOR_BLACK[4], COLOR_BLACK_FADE[4], COLOR_WHITE_FADE[4], MENU_OVERLAY[4], COLOR_RED[4], COLOR_GREEN[4], COLOR_BLUE[4];

void DrawText(const char *text, const char *font, float fontScale, float x, float y, const float *color);
void DrawShader(float x, float y, float width, float height, const float *color, const char *shader);
void DrawTextWithGlow(const char *text, const char *font, float fontScale, float x, float y, const float *color, const float *glowColor);

#endif