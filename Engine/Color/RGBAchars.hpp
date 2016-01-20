//=====================================================
// RGBAchars.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_RGBAchars__
#define __included_RGBAchars__

struct RGBAchars{
public:
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	RGBAchars();
	explicit RGBAchars(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);

	bool operator == (const RGBAchars& colorToEqual) const;
	bool operator != (const RGBAchars& colorToNotEqual) const;

	const static RGBAchars RED;
	const static RGBAchars DARK_RED;
	const static RGBAchars PURPLE;
	const static RGBAchars BLUE;
	const static RGBAchars DARK_BLUE;
	const static RGBAchars CYAN;
	const static RGBAchars DARK_CYAN;
	const static RGBAchars GREEN;
	const static RGBAchars DARK_GREEN;
	const static RGBAchars COMBAT_GREEN;
	const static RGBAchars YELLOW;
	const static RGBAchars ORANGE;
	const static RGBAchars BROWN;
	const static RGBAchars WHITE;
	const static RGBAchars GRAY;
	const static RGBAchars DARK_GRAY;
	const static RGBAchars BLACK;
};

///=====================================================
/// 
///=====================================================
inline RGBAchars::RGBAchars()
	:r(255),
	g(255),
	b(255),
	a(255){
}

///=====================================================
/// 
///=====================================================
inline RGBAchars::RGBAchars(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha /*= 255*/)
	:r(red),
	g(green),
	b(blue),
	a(alpha){
}

///=====================================================
/// 
///=====================================================
inline bool RGBAchars::operator == (const RGBAchars& colorToEqual) const{
	return ((r == colorToEqual.r) && (g == colorToEqual.g) && (b == colorToEqual.b) && (a == colorToEqual.a));
}

///=====================================================
/// 
///=====================================================
inline bool RGBAchars::operator != (const RGBAchars& colorToNotEqual) const{
	return ((r != colorToNotEqual.r) || (g != colorToNotEqual.g) || (b != colorToNotEqual.b) || (a != colorToNotEqual.a));
}

#endif