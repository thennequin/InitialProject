//---------------------------------------------
//                Initial Engine
//             Color  source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/Core/IColor.h"

namespace Initial
{
	namespace Core
	{
		IColor::IColor(float red, float green, float blue, float alpha)
		{
			Set(red,green,blue,alpha);
		}

		IColor::IColor(const IColor &color)
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

		void IColor::Set(const IColor& color)
		{
			Set(color.rgba[0],color.rgba[1],color.rgba[2],color.rgba[3]);
		}
	}
}
