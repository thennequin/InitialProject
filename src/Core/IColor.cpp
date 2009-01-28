//---------------------------------------------
//                Initial Engine
//             Color  source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Core/IColor.h"

using namespace Initial;

IColor::IColor(float red, float green, float blue, float alpha)
{
	Set(red,green,blue,alpha);
}

IColor::IColor(IColor &color)
{
	Set(color);
}

void IColor::Set(float red, float green, float blue, float alpha)
{
	rgba[0]=red;
	rgba[1]=green;
	rgba[2]=blue;
	rgba[3]=alpha;
}

void IColor::Set(IColor& color)
{
	Set(color.rgba[0],color.rgba[1],color.rgba[2],color.rgba[3]);
}