//---------------------------------------------
//                Initial Engine
//                 Array Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _ILIST_HEADER_
#define _ILIST_HEADER_

#include "Initial/Config.h"
#include "Initial/IDebugMemoryOn.h"

#include <stdlib.h>
#include <memory.h>

#define FOREACH(T,iterator,list) for (Initial::Core::IList<T>::Iterator iterator=list.Ite();iterator;iterator++)

namespace Initial
{
	namespace Core
	{
		template<class T>
		class IList
		{
		public:
			class ListNode
			{
				friend class IList<T>;
			public:
				T& GetData()
				{
					return Data;
				}
				ListNode *GetPrev()
				{
					return pPrev;
				}
				ListNode *GetNext()
				{
					return pNext;
				}
			protected:
				T Data;
				ListNode *pPrev;
				ListNode *pNext;
			};

			class Iterator
			{
				friend class IList<T>;
			public:
				Iterator()
				{
					pCurrent=NULL;
				}

				Iterator(const Iterator& ite)
				{
					pCurrent=ite.pCurrent;
				}

				T& GetData()
				{
					if (pCurrent)
						return pCurrent->Data;
				}

				void operator++()
				{
					if (pCurrent)
						pCurrent=pCurrent->pNext;
				}

				void operator--()
				{
					if (pCurrent)
						pCurrent=pCurrent->pPrev;
				}

				operator ListNode*()
				{
					return pCurrent;
				}
			protected:
				ListNode *pCurrent;
			};
		public:
			IList()
			{
				pFirst=NULL;
				pLast=NULL;
			}

			~IList()
			{
				Clear();
			}

			void AddAfter(T Item, ListNode *after)
			{
				ListNode *newNode = new ListNode;
				newNode->Data=Item;
				newNode->pPrev=after;
				if (after)
				{
					newNode->pNext=after->pNext;
					after->pNext=newNode;
				}else{
					newNode->pNext=pFirst;
					if (pFirst)
						pFirst->pPrev=newNode;
					pFirst=newNode;
				}
				if (newNode->pNext==NULL)
					pLast=newNode;
			}

			void AddBefore(T Item, ListNode *before)
			{
				ListNode *newNode = new ListNode;
				newNode->Data=Item;
				newNode->pNext=before;
				if (before)
				{
					newNode->pPrev=before->pPrev;
					before->pPrev=newNode;
				}else{
					newNode->pPrev=pLast;
					if (pLast)
						pLast->pNext=newNode;
					pLast=newNode;
				}
				if (newNode->pPrev==NULL)
					pFirst=newNode;
			}
			
			bool DeleteItem(T item)
			{
				ListNode *node=pFirst;
				while (node)
				{
					if (node->Data==item)
					{
						return DeleteNode(node);
					}
					node=node->pNext;
				}
				return false;
			}

			bool Delete(UINT id)
			{
				return DeleteNode(GetNode(id));
			}

			bool DeleteNode(ListNode *node)
			{
				if (node)
				{
					if (node->pPrev)
					{
						node->pPrev->pNext=node->pNext;
					}else{
						pFirst=node->pNext;
					}

					if (node->pNext)
					{
						node->pNext->pPrev=node->pPrev;
					}else{
						pLast=node->pPrev;
					}
					delete node;
					return true;
				}
				return false;
			}

			void Clear()
			{
				ListNode *node=pFirst;
				ListNode *next=NULL;
				while (node)
				{
					next=node->pNext;
					delete node;
					node=next;
				}
				pFirst=NULL;
				pLast=NULL;
			}

			UINT Count()
			{
				UINT i=0;
				ListNode *node=pFirst;
				while (node)
				{
					i++;
					node=node->pNext;
				}
				return i;
			}

			ListNode *GetNode(UINT id)
			{
				UINT i=0;
				ListNode *node=pFirst;
				while (node)
				{
					if (i==id)
						return node;
					i++;
					node=node->pNext;
				}
				return NULL;
			}

			T* GetItem(UINT id)
			{
				ListNode *node = GetNode(id);
				if (node)
					return &(node->Data);
				return NULL;
			}

			ListNode* First()
			{
				return pFirst;
			}

			ListNode* Last()
			{
				return pLast;
			}

			Iterator Ite()
			{
				Iterator ite;
				ite.pCurrent=pFirst;
				return ite;
			}

			void operator+=(const IList<T>& Array)
			{
				ListNode *node = Array.pFirst;
				while (node)
				{
					PushBack(node->Data);
					node=node->pNext;
				}
			}

			void operator=(const IList<T>& Array)
			{
				Clear();
				ListNode *node = Array.pFirst;
				while (node)
				{
					PushBack(node->Data);
					node=node->pNext;
				}
			}

			T& operator[] (int pos)
			{
				return *GetItem(pos);
			}

			void Sort(int (*)(T&,T&)) // Quick sort algo
			{

			}

			void PushBack(T item)
			{
				AddBefore(item,NULL);
			}

			void PopBack()
			{
				Delete(pLast);
			}

			void PushFront(T item)
			{
				AddAfter(item,NULL);
			}

			void PopFront()
			{
				Delete(pFirst);
			}

			//int Sortfunction(T a, T b){return 0;}

		protected:
			ListNode *pFirst;
			ListNode *pLast;
		};
	}
}

//#include "Initial/IDebugMemoryOff.h"

#endif