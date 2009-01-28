//---------------------------------------------
//                Initial Engine
//                Vertex Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IVERTEX_HEADER_
#define _IVERTEX_HEADER_

#include "INormal.h"

namespace Initial
{

	class IVertex
	{
	public:
		
		IVertex(float x=0.0, float y=0.0, float z=0.0);
		IVertex(const IVertex& src);

		float GetX();
		float GetY();
		float GetZ();

		void SetX(float x);
		void SetY(float y);
		void SetZ(float z);
		void Set(float x, float y, float z);
		void Set(IVertex& vert);

		void Normalize();

		float& operator[](int val);
		float& operator[](int val) const;


		void operator= (IVertex vertex);

		const IVertex operator+ (const IVertex vertex) const;
		const IVertex operator- (const IVertex vertex) const;
		const IVertex operator* (const IVertex vertex) const;
		const IVertex operator+ (INormal vertex) const;

		const IVertex operator* (const float scale) const;

		operator float*() const;

	protected:
		float pos[3];
	};

}

#endif