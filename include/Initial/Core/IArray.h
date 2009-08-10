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
			IArray() : Array(0),ItemCount(0){ }

			~IArray()
			{
				Clear();
			}
			
			bool Add( T item)
			{
				T** newArray=NULL;
				if (Array==NULL)
					newArray = (T**) malloc( sizeof(T*) );
				else
					newArray = (T**) realloc( Array,sizeof(T*)*(ItemCount+1) );

				/*if (!newArray)
				{
					newArray= (T**) malloc( sizeof(T**)*(ItemCount+1) );
					if (newArray)
					{
						memcpy(newArray,Array,sizeof(T**)*(ItemCount));
					}
				}*/

				if (newArray)
				{
					Array=newArray;
					Array[ItemCount]= new T;
					if (Array[ItemCount])
					{
						*Array[ItemCount]=item;
						ItemCount++;
						return true;
					}
				}
				//printf("Add error\n");
				return false;
			}

			bool DeleteItem( T item)
			{
				for (UINT i=0;i<Count();i++)
				{
					//printf("del %d %d\n",item,Array[i]);
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
					T** newArray=NULL;
					delete Array[id];

					if (id < ItemCount-1)
						memcpy(Array+id,Array+id+1,(ItemCount-id-1)*sizeof(T*));
					
					if (ItemCount==1)
					{
						free(Array);
						newArray=NULL;
					}else
						newArray = (T**) realloc(Array,sizeof(T*)*(ItemCount-1));

					if (newArray || ItemCount==1)
					{
						Array=newArray;
						ItemCount--;
						return true;
					}else{
						//printf("Delete error\n");
						return false;
					}
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
					return *Array[id];
				}//else return (T&)*NULL;
			}

			T& Last()
			{
				return *Array[ItemCount-1];
			}

			/*T& operator = (T& ptr)
			{
				for (size_t i=0;i<Count();i++)
				{				
					printf("Replace%d %d\n",ptr,this);
				}	
			}*/

			void operator+=(IArray<T>& Array)
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
				//T temp;
				if (pos>=0 && pos<ItemCount)
					return (*Array[pos]);
				return (*Array[0]);
			}

			void Sort(int (*)(T&,T&)) // Quick sort algo
			{

			}

			//int Sortfunction(T a, T b){return 0;}

		protected:

			T** Array;

			UINT ItemCount;
		};
	}
}

#endif