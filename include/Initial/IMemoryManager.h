//---------------------------------------------
//                Initial Engine
//            Memory manager Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IMEMORYMANAGER_HEADER_
#define _IMEMORYMANAGER_HEADER_

#include "Initial/Config.h"
#include "Initial/Core/IString.h"
//#include "Initial/IO/IFileStream.h"

#include <fstream>
#include <map>
#include <stack>
#include <string>

namespace Initial
{
	class IMemoryManager
	{
		public :
		 //----------------------------------------------------------
		// Renvoie l'instance de la classe
		//----------------------------------------------------------
		static IMemoryManager& Instance();

		//----------------------------------------------------------
		// Ajoute une allocation m�moire
		//----------------------------------------------------------
		void* Allocate(std::size_t Size, const Core::IString& File, int Line, bool Array);

		//----------------------------------------------------------
		// Retire une allocation m�moire
		//----------------------------------------------------------
		void Free(void* Ptr, bool Array);

		//----------------------------------------------------------
		// Sauvegarde les infos sur la d�sallocation courante
		//----------------------------------------------------------
		void NextDelete(const Core::IString& File, int Line);

	private :
		//----------------------------------------------------------
		// Constructeur par d�faut
		//----------------------------------------------------------
		IMemoryManager();

		//----------------------------------------------------------
		// Destructeur
		//----------------------------------------------------------
		~IMemoryManager();

		//----------------------------------------------------------
		// Inscrit le rapport sur les fuites de m�moire
		//----------------------------------------------------------
		void ReportLeaks();

		//----------------------------------------------------------
		// Types
		//----------------------------------------------------------
		struct TBlock
		{
			std::size_t Size;  // Taille allou�e
			Core::IString       File;  // Fichier contenant l'allocation
			int         Line;  // Ligne de l'allocation
			bool        Array; // Est-ce un objet ou un tableau ?
		};
		typedef std::map<void*, TBlock> TBlockMap;

		//----------------------------------------------------------
		// Donn�es membres
		//----------------------------------------------------------
		std::ofstream		m_File;        // Fichier de sortie
		//Initial::IO::IFileStream	m_File;
		TBlockMap					m_Blocks;      // Blocs de m�moire allou�s
		std::stack<TBlock>			m_DeleteStack; // Pile dont le sommet contient la ligne et le fichier de la prochaine d�sallocation

		static IMemoryManager m_Instance;

		bool m_bLogAlloc;
	};
}

#endif
