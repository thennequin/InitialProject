//---------------------------------------------
//                Initial Engine
//                 Array Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IARRAY2_HEADER_
#define _IARRAY2_HEADER_

#include "Config.h"

#include <stdlib.h>
#include <memory.h>

namespace Initial
{

	template<class T>
	class IArray
	{
	public:
		IArray();
		~IArray();
		
		//void SetSize(unsigned int count);
		bool Add( const T item);
		bool DeleteItem( T item);
		bool Delete( unsigned int id);
		void Clear();

		size_t Count();
		T& GetItem(int id);
		T& Last();

		T& operator[] (int pos);

		void Sort(int (*)(T&,T&)); // Quick sort algo

		 enum exception { MEMFAIL }; // exception enum 

	protected:
		T* Array;

		unsigned int ItemCount;
		unsigned int RealItemCount;

		const static int StartItemCount=16;
	};

//

	template <class T>
	IArray<T>::IArray() 
	{
		RealItemCount = StartItemCount; // First, allocate step 
								   // for dyn_array_step items
		ItemCount = 0;
		Array = new T[RealItemCount];
	 
		if (Array == NULL)
			throw MEMFAIL;
	}

	template <class T>
	IArray<T>::~IArray()
	{
		if (Array)
		{
			delete [] Array;
			Array=NULL;
		}
	}

	/*template <class T>
	void IArray<T>::SetSize(unsigned int count)
	{
		ItemCount = count;

		if (ItemCount != 0)
		{
			if ((ItemCount > RealItemCount) || (ItemCount < RealItemCount/2))
			{
    			RealItemCount = ItemCount;
    			T** newArray = new T[ItemCount];

				for (unsigned int i=0;i<RealItemCount;i++)
					newArray[i] = Array[i];
				delete [] Array;
				Array = newArray;

				if (Array == NULL)
					throw MEMFAIL;
			}
		}
		else
			Clear();
	}*/

	template <class T>
	bool IArray<T>::Add( const T item )
	{
		ItemCount++;
	 
		if (ItemCount > RealItemCount)
		{
			int oldcount=RealItemCount;
			RealItemCount *= 2;
	 
			T* newArray = new T[ItemCount];

			for (unsigned int i=0;i<RealItemCount;i++)
				newArray[i] = Array[i];
			delete [] Array;
			Array = newArray;
	 
			if (Array == NULL)
				throw MEMFAIL;
		}
	 
		Array[ItemCount-1] = item;
		return true;
	}

	template <class T>
	bool IArray<T>::DeleteItem( T item)
	{
		for (size_t i=0;i<ItemCount-1;i++)
		{
			//printf("del %d %d\n",item,Array[i]);
			if (Array[i]==item)
			{
				Delete(i);
				return true;
			}
		}
		return false;
	}

	template <class T>
	bool IArray<T>::Delete( unsigned int id)
	{
		if (ItemCount == 1)
			Clear();
		else
		{
			//delete Array[id];
			//Array[id]=new T();
			for(unsigned int i=id; i<ItemCount-1; i++) 
				Array[i] = Array[i+1];
	 
			ItemCount--;
		}
		return true;
	}

	template <class T>
	void IArray<T>::Clear()
	{
		ItemCount = 0; 
		delete [] Array;
		Array = new T[StartItemCount];
		RealItemCount = StartItemCount;
	}

	template <class T>
	size_t IArray<T>::Count()
	{
		return ItemCount;
	}

	template <class T>
	T& IArray<T>::GetItem(int id)
	{
		if (id>=0 && id < ItemCount)
		{
			return Array[id];
		}else return T();
	}

	template <class T>
	T& IArray<T>::Last()
	{
		return Array[ItemCount-1];
	}

	template <class T>
	T& IArray<T>::operator[] (int pos)
	{
		T temp;
		if (pos>=0 && pos<ItemCount)
			return Array[pos];
		return temp;
	}

	template <class T>
	void IArray<T>::Sort(int (*)(T&,T&)) // Quick sort algo
	{

	}

}

#endif