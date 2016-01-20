//=====================================================
// RGBA.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_RGBA__
#define __included_RGBA__

struct RGBA{
public:
	float r;
	float g;
	float b;
	float a;

	RGBA();
	explicit RGBA(float red, float green, float blue, float alpha = 1.0f);

	bool operator == (const RGBA& colorToEqual) const;
	bool operator != (const RGBA& colorToNotEqual) const;

	const static RGBA RED;
	const static RGBA DARK_RED;
	const static RGBA PURPLE;
	const static RGBA BLUE;
	const static RGBA DARK_BLUE;
	const static RGBA CYAN;
	const static RGBA DARK_CYAN;
	const static RGBA GREEN;
	const static RGBA DARK_GREEN;
	const static RGBA COMBAT_GREEN;
	const static RGBA YELLOW;
	const static RGBA ORANGE;
	const static RGBA BROWN;
	const static RGBA WHITE;
	const static RGBA GRAY;
	const static RGBA DARK_GRAY;
	const static RGBA BLACK;
};

///=====================================================
/// 
///=====================================================
inline RGBA::RGBA()
:r(1.0f),
g(1.0f),
b(1.0f),
a(1.0f){
}

///=====================================================
/// 
///=====================================================
inline RGBA::RGBA(float red, float green, float blue, float alpha /*= 1.0f*/)
:r(red),
g(green),
b(blue),
a(alpha){
}

///=====================================================
/// 
///=====================================================
inline bool RGBA::operator == (const RGBA& colorToEqual) const{
	return ((r == colorToEqual.r) && (g == colorToEqual.g) && (b == colorToEqual.b) && (a == colorToEqual.a));
}

///=====================================================
/// 
///=====================================================
inline bool RGBA::operator != (const RGBA& colorToNotEqual) const{
	return ((r != colorToNotEqual.r) || (g != colorToNotEqual.g) || (b != colorToNotEqual.b) || (a != colorToNotEqual.a));
}

#endif