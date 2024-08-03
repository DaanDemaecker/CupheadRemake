#pragma once
#include <string>
class Texture;


class MenuWord final
{
public:
	MenuWord(const std::string& text, const std::string& fontPath, const Color4f& color1, const Color4f& color2, int textSize = 40);
	~MenuWord();

	MenuWord(MenuWord& rhs) = delete;
	MenuWord(MenuWord&& rhs) = delete;
	MenuWord& operator=(MenuWord& rhs) = delete;
	MenuWord& operator=(MenuWord&& rhs) = delete;

	void Draw(const Point2f& pos, bool selected);

	float GetHeight();
	float GetWidth();

private:
	Texture* m_pUnselectedTextrue;
	Texture* m_pSelectedTexture;
};

