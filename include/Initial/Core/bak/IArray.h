//---------------------------------------------
//                Initial Engine
//                 Array Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IARRAY_HEADER_
#define _IARRAY_HEADER_

#include "Initial/Config.h"

#include <stdlib.h>
#include <memory.h>
#include <vector>

namespace Initial
{
	namespace Core
	{
		template<class T>
		class IArray
		{
		public:
			IArray()
			{ 
			}

			~IArray()
			{
				Clear();
			}
			
			bool Add( T item)
			{
				stdArray.push_back(item);
				return true;
			}

			bool DeleteItem( T item)
			{
				//stdArray.a
			}

			bool Delete( UINT id)
			{
				//stdArray.erase(stdArray.at(id));
				//stdArray.erase(id);
			}

			void Clear()
			{
				stdArray.clear();
			}

			UINT Count()
			{
				return stdArray.size();
			}

			T& GetItem(UINT id)
			{
				return stdArray.at(id);
			}

			T& Last()
			{
				return stdArray.back();
			}

			void operator+=(IArray<T> Array)
			{
				for (UINT i=0;i<Array.Count();i++)
				{
					Add(Array[i]);
				}
			}

			void operator=(IArray<T> Array)
			{
				stdArray=Array.stdArray;
			}

			T& operator[] (UINT pos)
			{
				return GetItem(pos);
			}

			void Sort(int (*)(T&,T&)) // Quick sort algo
			{

			}

			//int Sortfunction(T a, T b){return 0;}

			/*void PushBack(T& item)
			{

			}

			T& PopBack()
			{

			}*/
		protected:
			std::vector<T> stdArray;
		};
	}
}

#endif