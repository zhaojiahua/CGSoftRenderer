#pragma once
#include "base.h"

void ZScrPoint::operator=(ZScrPoint inPoint)
{
	X = inPoint.X;
	Y = inPoint.Y;
	color = inPoint.color;
	uv = inPoint.uv;
}
