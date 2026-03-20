#pragma once

class Color
{
public:
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}
	Color() : r(0), g(0), b(0), a(0) {}
};

