#pragma once

struct Color
{
	float r, g, b, a;

	//Constructors
	Color() : r(0.0f), g(0.0f), b(0.0f), a(0.0f) {}
	Color(float R, float G, float B, float A) : r(R), g(G), b(B), a(A) {}

	// Alpha Colors
	static Color Alpha1;
	static Color Alpha2;

	//  Black and White
	static Color White;
	static Color Black;
	static Color Silver;
	static Color Gray;

	//  Primary Colors
	static Color Orange;
	static Color Blue;
	static Color Green;
	static Color Red;
	static Color Yellow;
	static Color Purple;
	static Color LightPurple;

	//  Tertiary Colors
	static Color Maroon;
	static Color Olive;
	static Color Lime;
	static Color Aqua;
	static Color Teal;
	static Color Navy;
	static Color Fuchsia;

	//static void White

	~Color() {};

	static Color LerpRGB(Color a, Color b, float t)
	{
		return Color
		(
			a.r + (b.r - a.r) * t,
			a.g + (b.g - a.g) * t,
			a.b + (b.b - a.b) * t,
			a.a + (b.a - a.a) * t
		);
	}

	float operator-(const Color& other)
	{
		return float(r - other.r + g - other.g + b - other.b + a - other.a);
	}
};
