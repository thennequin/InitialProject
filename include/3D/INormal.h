//---------------------------------------------
//                Initial Engine
//                Context Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _INORMAL_HEADER_
#define _INORMAL_HEADER_

namespace Initial
{

	class INormal
	{
	public:
		INormal(float x=1.0, float y=0.0, float z=0.0) 
		{ 
			pos[0]=x;pos[1]=y;pos[2]=z;
		}

		float GetX() { return pos[0]; }
		float GetY() { return pos[1]; }
		float GetZ() { return pos[2]; }

		void SetX(float x) { pos[0]=x; }
		void SetY(float y) { pos[1]=y; }
		void SetZ(float z) { pos[2]=z; }

		float& operator[](int val)
		{
			float temp=0.0;
			if (val>=0 && val<3)
				return pos[val];
			else return temp;
		}

		void operator= (INormal normal)
		{
			pos[0]=normal.pos[0];
			pos[1]=normal.pos[1];
			pos[2]=normal.pos[2];
		}

	private:
		float pos[3];
	};

}

#endif