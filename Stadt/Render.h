#pragma once

#include "Includes.h"
#include "Globals.h"

#pragma warning(disable : 4244)

class Render
{
private:

	ImDrawList* drawList;

public:

	// todo, maybe few draw lists with mutexes https://github.com/spirthack/CSGOSimple/blob/df4bb4ef20aef408b54b8eb2f99fb1637a5d97e8/CSGOSimple/render.hpp

	// 3d circle https://github.com/spirthack/CSGOSimple/blob/master/CSGOSimple/render.cpp#L114
	// or https://github.com/KebsCS/KBot/blob/9dd5894bb8141e90e2b6887eb93c9a34baa98838/KBot/Draw.cpp#L65

	void RenderScene();

	template <class T>
	inline void Text(std::string str, T x, T y, float size = 13.f, ImColor color = ImColor(1.f, 1.f, 1.f), bool center = true, bool outline = true, ImFont* pFont = Globals::pDefaultFont)
	{
		ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, str.c_str());
		if (!pFont->ContainerAtlas)
			return;// 0.f;
		drawList->PushTextureID(pFont->ContainerAtlas->TexID);

		if (center)
			x -= textSize.x / 2.0f;

		if (outline)
		{
			ImColor black = ImColor(0.f, 0.f, 0.f, color.Value.w);
			drawList->AddText(pFont, size, ImVec2(x + 1, y + 1), black, str.c_str());
			drawList->AddText(pFont, size, ImVec2(x - 1, y - 1), black, str.c_str());
			drawList->AddText(pFont, size, ImVec2(x + 1, y - 1), black, str.c_str());
			drawList->AddText(pFont, size, ImVec2(x - 1, y + 1), black, str.c_str());
		}

		drawList->AddText(pFont, size, ImVec2(x, y), color, str.c_str());

		drawList->PopTextureID();
	}

	template <class T>
	inline void CornerBox(T x1, T y1, T x2, T y2, ImColor color, float th = 1.f) {
		int w = x2 - x1;
		int h = y2 - y1;

		int iw = w / 4;
		int ih = h / 4;
		// top
		Line(x1, y1, x1 + iw, y1, color, th);					// left
		Line(x1 + w - iw, y1, x1 + w, y1, color, th);			// right
		Line(x1, y1, x1, y1 + ih, color, th);					// top left
		Line(x1 + w - 1, y1, x1 + w - 1, y1 + ih, color, th);	// top right
		// bottom
		Line(x1, y1 + h, x1 + iw, y1 + h, color, th);			// left
		Line(x1 + w - iw, y1 + h, x1 + w, y1 + h, color, th);	// right
		Line(x1, y1 + h - ih, x1, y1 + h, color, th);			// bottom left
		Line(x1 + w - 1, y1 + h - ih, x1 + w - 1, y1 + h, color, th);	// bottom right
	}

	template <class T>
	inline void Box(T x1, T y1, T x2, T y2, ImColor color, float thickness = 1.f, float rounding = 0.f)
	{
		drawList->AddRect(ImVec2(x1, y1), ImVec2(x2, y2), color, rounding, 15, thickness);
	}
	inline void Box(RECT r, ImColor color, float thickness = 1.f, float rounding = 0.f)
	{
		Box(r.left, r.top, r.right, r.bottom, color, thickness, rounding);
	}
	template <class T>
	inline void BoxFilled(T x1, T y1, T x2, T y2, ImColor color, float thickness = 1.f, float rounding = 0.f)
	{
		drawList->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), color, rounding, 15);
	}
	template <class T>
	inline void Line(T x1, T y1, T x2, T y2, ImColor color, float thickness = 1.f)
	{
		drawList->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), color, thickness);
	}
	template <class T>
	inline void Circle(T x, T y, float radius, int points, ImColor color, float thickness = 1.f)
	{
		drawList->AddCircle(ImVec2(x, y), radius, color, points, thickness);
	}
	template <class T>
	inline void CirlceFilled(T x, T y, float radius, int points, ImColor color)
	{
		drawList->AddCircleFilled(ImVec2(x, y), radius, color, points);
	}
};

inline Render render;
