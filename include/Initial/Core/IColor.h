//---------------------------------------------
//                Initial Engine
//                 Color Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _ICOLOR_HEADER_
#define _ICOLOR_HEADER_

#include "Initial/Config.h"

namespace Initial
{
	namespace Core
	{
		class IColor
		{
		public:
			IColor(float red=1.0, float green=1.0, float blue=1.0, float alpha=1.0); 
			IColor(const IColor &color);

			float GetRed() { return rgba[0]; }
			float GetGreen() { return rgba[1]; }
			float GetBlue() { return rgba[2]; }
			float GetAlpha() { return rgba[3]; }

			void Set(float red, float green, float blue, float alpha);
			void Set(const IColor& color);
			void SetRed(float val) { rgba[0]=val; }
			void SetGreen(float val) { rgba[1]=val; }
			void SetBlue(float val) { rgba[2]=val; }
			void SetAlpha(float val) { rgba[3]=val; }

			void operator= (const IColor& color)
			{
				rgba[0]=color.rgba[0];
				rgba[1]=color.rgba[1];
				rgba[2]=color.rgba[2];
				rgba[3]=color.rgba[3];
			}

			const IColor operator* (const IColor& color)
			{
				double r,g,b,a;
				r=rgba[0]*color.rgba[0];
				g=rgba[1]*color.rgba[1];
				b=rgba[2]*color.rgba[2];
				a=rgba[3]*color.rgba[3];
				return IColor(r,g,b,a);
			}

			operator float*() const
			{
				return (float*)rgba;
			}

			union
			{
				struct {float r,g,b,a;};
				struct {float x,y,z,w;};
				float rgba[4];
			};			
		};
	}
}

#endif