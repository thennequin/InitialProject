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
				ItemCount=0;
				Alloc=8;
				Array = (T**)malloc(sizeof(T*)*Alloc);
				for (UINT i=0;i<Alloc;i++)
					Array[i]=NULL;
			}

			~IArray()
			{
				Clear();
			}

			bool AllocBuffer(UINT size)
			{
				if (size>Alloc)
				{
					UINT newAlloc;
					newAlloc=Alloc;
					while(newAlloc<size)
						newAlloc=newAlloc*2;
					T** newArray=NULL;
					while (newArray==NULL)
						newArray = (T**) realloc( Array,sizeof(T*)*(newAlloc) );
					//if (newArray)
					{
						Array=newArray;
						for (UINT i=Alloc;i<newAlloc;i++)
							Array[i]=NULL;
						Alloc=newAlloc;
						return true;
					}
				}else{
					return true;
				}
				return false;
			}
			
			bool Add( T item)
			{
				if (ItemCount+1>Alloc)
					if (AllocBuffer(ItemCount+1)==false)
						return false;

				printf("%d %d %d\n",Alloc,ItemCount,Array[ItemCount]);

				if (Array[ItemCount]==NULL)
					Array[ItemCount]=new T(item);
				else
					*Array[ItemCount]=item;
				ItemCount++;
				return true;
			}

			bool DeleteItem( T item)
			{
				for (UINT i=0;i<Count();i++)
				{
					if (*Array[i]==item)
					{
						Delete(i);
						return true;
					}
				}
				return false;
			}

			bool Delete( UINT id)
			{
				if (id>=0 && id < ItemCount)
				{
					if (Array[id])
						delete Array[id];
					ItemCount--;
					for (UINT i=id;i<ItemCount;i++)
					{
						 Array[id]=Array[id+1];
					}
					return true;
				}else return false;
			}

			void Clear()
			{
				if (Array)
				{
					for (UINT i=0;i<ItemCount;i++)
						if (Array[i])
							delete Array[i];

					free(Array);
					Array=NULL;
				}
			}

			UINT Count()
			{
				return ItemCount;
			}

			T& GetItem(UINT id)
			{
				if (id>=0 && id < ItemCount)
				{
					if (Array[id]==NULL)
						Array[id]=new T;
					return *Array[id];
				}
				return *Array[0];
			}

			T& Last()
			{
				return GetItem(ItemCount-1);
			}

			/*T& operator = (T& ptr)
			{
				for (size_t i=0;i<Count();i++)
				{				
					printf("Replace%d %d\n",ptr,this);
				}	
			}*/

			void operator+=(IArray<T> Array)
			{
				for (UINT i=0;i<Array.Count();i++)
				{
					Add(Array[i]);
				}
			}

			void operator=(IArray<T> Array)
			{
				Clear();
				for (UINT i=0;i<Array.Count();i++)
				{
					Add(Array[i]);
				}
			}

			T& operator[] (UINT pos)
			{
				return GetItem(pos);
			}

			void Sort(int (*)(T&,T&)) // Quick sort algo
			{

			}

			//int Sortfunction(T a, T b){return 0;}

		protected:
			T** Array;
			UINT Alloc;
			UINT ItemCount;
		};
	}
}

#endif