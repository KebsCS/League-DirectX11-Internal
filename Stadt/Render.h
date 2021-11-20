#pragma once

#include "Includes.h"
#include "Globals.h"
#include "Image.h"
#include "Geometry.h"
#include "LeagueFuncs.h"

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
		if (str.empty())
			return;

		ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, str.c_str());
		if (!pFont->ContainerAtlas)
			return;

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

	inline void Image(ImVec2 pos, ImVec2 size, ID3D11ShaderResourceView* image)
	{
		this->Image(pos.x, pos.y, size.x, size.y, image);
	}

	template <class T>
	inline void Image(T x, T y, float width, float height, ID3D11ShaderResourceView* image, bool centered = false)
	{
		if (image == nullptr)
			return;

		if (centered)
		{
			x -= width / 2.f;
			y -= height / 2.f;
		}

		drawList->AddImage(image, ImVec2(x, y), ImVec2(x + width, y + height));
	}

	template <class T>
	inline void ImageRounded(T x, T y, float width, float height, ID3D11ShaderResourceView* image, float rounding, bool centered = false)
	{
		if (image == nullptr)
			return;

		if (centered)
		{
			x -= width / 2.f;
			y -= height / 2.f;
		}

		drawList->AddImageRounded(image, ImVec2(x, y), ImVec2(x + width, y + height), ImVec2(0, 0), ImVec2(1, 1), IM_COL32_WHITE, rounding);
	}

	inline void ImageBordered(int x, int y, float width, float height, ID3D11ShaderResourceView* image, bool centered = false)
	{
		if (centered)
		{
			x -= width / 2.f;
			y -= height / 2.f;
		}
		int x2 = x + width;
		int y2 = y + height;
		Box(x, y, x2, y2, ImColor(0.f, 0.f, 0.f), 5.f);

		// centered false, because we already pass correct x and y
		this->Image(x, y, width, height, image, false);
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
	inline void Line(ImVec2 pos1, ImVec2 pos2, ImColor color, float thickness = 1.f)
	{
		drawList->AddLine(pos1, pos2, color, thickness);
	}
	template <class T>
	inline void Circle(T x, T y, float radius, int points, ImColor color, float thickness = 1.f)
	{
		drawList->AddCircle(ImVec2(x, y), radius, color, points, thickness);
	}
	template <class T>
	inline void CircleFilled(T x, T y, float radius, int points, ImColor color)
	{
		drawList->AddCircleFilled(ImVec2(x, y), radius, color, points);
	}

	inline void Polygon(const Geometry::Polygon poly, ImColor color, float tick = 1.f)
	{
		static ImVec2 points[200];
		int i = 0;
		for (const auto& point : poly.Points)
		{
			ImVec2 pos = LeagueFuncs::WorldToScreen(point);
			points[i].x = pos.x;
			points[i].y = pos.y;
			i++;
		}

		drawList->AddPolyline(points, i, color, true, tick);
	}

	// xp 0.f to 1.f
	template <class T>
	inline void FancyIcon(T x, T y, std::string champ, float hp, float mana, float xp, int level, float ultCD, std::string summ1, float summ1CD, std::string summ2, float summ2CD)
	{
		// to see better
		//BoxFilled(x - 50, y - 50, 400, 400, ImColor(1.f, 1.f, 1.f));

		// x31, y6

		std::string squareName = champ + XorStr("_square");
		this->ImageRounded(x + 42, y + 26, 120, 120, imageManager.GetImageByName(squareName), 150.f);

		ImVec2 uv0 = ImVec2(0.f, ((120.f * xp) / 120.f));
		ImVec2 uv1 = ImVec2(1.f, 1.f);
		drawList->AddImage(imageManager.GetImageByName(XorStr("xpbarblack")), ImVec2(x + 136, y + 17), ImVec2(x + 136 + 47, y + 17 + 120));
		drawList->AddImage(imageManager.GetImageByName(XorStr("xpbar")), ImVec2(x + 136, y + 17 + 120 * xp), ImVec2(x + 136 + 47, y + 17 + 120), uv0, uv1);

		this->Image(x + 8, y + 10, 66, 143, imageManager.GetImageByName(XorStr("hpbarblack")));

		//this->Image(x + 6, y + 9, 49, 135, imageManager.GetImageByName("hpbar"));
		uv0 = ImVec2(0.f, ((135 * hp) / 135));
		drawList->AddImage(imageManager.GetImageByName(XorStr("hpbar")), ImVec2(x + 6, y + 9 + 135 * hp), ImVec2(x + 6 + 49, y + 9 + 135), uv0, uv1);

		//this->Image(x + 22, y + 16, 49, 122, imageManager.GetImageByName("manabar"));
		uv0 = ImVec2(0.f, ((122 * mana) / 122));
		drawList->AddImage(imageManager.GetImageByName(XorStr("manabar")), ImVec2(x + 22, y + 16 + 122 * mana), ImVec2(x + 22 + 49, y + 16 + 122), uv0, uv1);

		this->Image(x, y, 193, 176, imageManager.GetImageByName(XorStr("hudicon")));

		this->Image(x, y, 79, 167, imageManager.GetImageByName(XorStr("hudleft")));
		this->Image(x + 106, y + 27, 48, 48, imageManager.GetImageByName(XorStr("hudcircle")), true);

		std::string ultName = champ + "_r";
		this->ImageRounded(x + 106, y + 27, 36, 36, imageManager.GetImageByName(ultName), 150.f, true);

		int fontSize = 20;
		if (ultCD > 0.f)
		{
			CircleFilled(x + 106, y + 27, 20, 12, ImColor(0.f, 0.f, 0.f, 0.6f));
			Text(std::to_string((int)ultCD), x + 106, y + 27 - fontSize / 2, fontSize, ImColor(1.f, 1.f, 1.f), true, true);
		}

		this->Image(x + 47, y + 130, 48, 48, imageManager.GetImageByName(XorStr("hudcircle")), true);
		this->ImageRounded(x + 47, y + 130, 36, 36, imageManager.GetImageByName(summ1), 150.f, true);
		if (summ1CD > 0.f)
		{
			CircleFilled(x + 47, y + 130, 20, 12, ImColor(0.f, 0.f, 0.f, 0.6f));
			Text(std::to_string((int)summ1CD), x + 47, y + 130 - fontSize / 2, fontSize, ImColor(1.f, 1.f, 1.f), true, true);
		}

		this->Image(x + 86, y + 149, 48, 48, imageManager.GetImageByName(XorStr("hudcircle")), true);
		this->ImageRounded(x + 86, y + 149, 36, 36, imageManager.GetImageByName(summ2), 150.f, true);
		if (summ2CD > 0.f)
		{
			CircleFilled(x + 86, y + 149, 20, 12, ImColor(0.f, 0.f, 0.f, 0.6f));
			Text(std::to_string((int)summ2CD), x + 86, y + 149 - fontSize / 2, fontSize, ImColor(1.f, 1.f, 1.f), true, true);
		}

		Text(std::to_string(level), x + 141, y + 144 - fontSize / 2, fontSize, ImColor(1.f, 1.f, 1.f), true, true);
	}
};

inline Render render;
