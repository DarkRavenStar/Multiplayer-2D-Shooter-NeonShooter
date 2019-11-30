#pragma once
#include "AllHeader.h"


struct Scale
{
	float xScale;
	float yScale;

	Scale()
	{
		xScale = 0.0f;
		yScale = 0.0f;
	}

	Scale(float x, float y)
	{
		xScale = x;
		yScale = y;
	}
};



static float xCoord(float x, float y, float angle, float scale, float xCoord)
{
	return xCoord + (scale * (x * cos(angle) - y * sin(angle)));
}

static float yCoord(float x, float y, float angle, float scale, float yCoord)
{
	return yCoord + (scale * (y * cos(angle) + x * sin(angle)));
}

static float CircleXCoord(float x, float y, float angle, float scale, float xCoord)
{
	return xCoord + (scale * (x * cos(angle) - y * sin(angle)));
}

static float CircleYCoord(float x, float y, float angle, float scale, float yCoord)
{
	return yCoord + (scale * (y * cos(angle) + x * sin(angle)));
}

static float Lerping(float value, float start1, float stop1, float start2, float stop2)
{
	return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
}

static float Lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

