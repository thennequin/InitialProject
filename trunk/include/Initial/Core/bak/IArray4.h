//---------------------------------------------
//                Initial Engine
//                 Array Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IARRAY_HEADER_
#define _IARRAY_HEADER_

#include "Config.h"

#include <list>
#include <vector>
#include <stdlib.h>
#include <memory.h>

namespace Initial
{

	template<class T>
	class IArray
	{
	public:
		IArray() 
		{
			iter=NULL;
			pos=0;
		}

		~IArray()
		{
			if (iter)
				delete iter;
			Clear();
		}
		
		bool Add( const T item)
		{
			vect.insert(vect.end(),item);
			if (iter)
			{
				*((std::list<T>::iterator*)iter)=vect.begin();
				pos=0;
			}
			return true;
		}

		bool DeleteItem( const T item)
		{
			for (size_t i=0;i<Count();i++)
			{
				//printf("del %d %d\n",item,Array[i]);
				if (GetItem(i)==item)
				{
					Delete(i);
					return true;
				}
			}
			return false;
		}

		bool Delete( unsigned int id)
		{
			if (id>=Count())
				return false;

			std::list<T>::iterator ite = vect.begin();
			for (int i=0;i<id;i++)
				ite++;

			vect.erase(ite);

			if (iter)
			{
				*((std::list<T>::iterator*)iter)=vect.begin();
				pos=0;
			}
			return true;
		}

		void Clear()
		{
			vect.clear();
			if (iter)
			{
				*((std::list<T>::iterator*)iter)=vect.begin();
				pos=0;
			}
		}

		size_t Count()
		{
			return vect.size();
		}

		T& GetItem(int id)
		{
			if (id>=0 && id<vect.size())
			{
				if (iter==NULL)
				{
					iter = new std::list<T>::iterator();
					*((std::list<T>::iterator*)iter)=vect.begin();
				}

				unsigned int i;
				std::list<T>::iterator ite;
				if (pos>id) 
				{
					i=0;
					ite=vect.begin();
				}else{
					i=pos;
					ite = *((std::list<T>::iterator*)iter);
				}

				while (i<id)
				{
					ite++;
					i++;
				}
				*((std::list<T>::iterator*)iter)=ite;
				pos=id;
				return ite._Mynode()->_Myval;
			}
			T* t=NULL;
			return *t;
		}

		T& Last()
		{
			return GetItem(Count()-1);
			//return ((vect.end()--)._Mynode()->_Myval);
		}

		/*void operator+=(IArray<T> Array)
		{
			for (size_t i=0;i<Array.Count();i++)
			{
				Add(Array[i]);
			}
		}*/

		void operator=(IArray<T> Array)
		{
			Clear();
			for (size_t i=0;i<Array.Count();i++)
			{
				Add(Array[i]);
			}
		}

		T& operator[] (int pos)
		{
			return GetItem(pos);
		}

		void Sort(int (*)(T&,T&)) // Quick sort algo
		{

		}

		//int Sortfunction(T a, T b){return 0;}

	protected:
		std::list<T> vect;
		void *iter;
		unsigned int pos;
	};

}

#endif