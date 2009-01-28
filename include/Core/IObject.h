//---------------------------------------------
//                Initial Engine
//                Object  Module
//
//            By Thibault HENNEQUIN
//              January 12th  2008
//---------------------------------------------

#ifndef _IOBJECT_HEADER_
#define _IOBJECT_HEADER_

namespace Initial
{
	
	class IObject
	{
	public:
		IObject();

		virtual void Think();
		virtual void SetNextThink(unsigned int next); //-1 always
		
	protected:
		void _Init();
	protected:
		unsigned int NextTime;

		static IArray<IObject*> m_aObjects;
	};

}

#endif