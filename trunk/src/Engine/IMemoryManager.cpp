//==========================================================
// MemoryManager.cpp
//
// Implantation de la classe IMemoryManager
//
//==========================================================


//==========================================================
// En-têtes
//==========================================================
#include <Initial/IMemoryManager.h>
#include <Initial/Core/IAssert.h>
#include <Initial/IO/IConfigINI.h>

#include <iomanip>
#include <sstream>

#define EOL "\n"

namespace Initial
{
	IMemoryManager IMemoryManager::m_Instance;

	////////////////////////////////////////////////////////////
	// Constructeur par défaut
	//
	////////////////////////////////////////////////////////////
	IMemoryManager::IMemoryManager() :
	m_File("Memory leaks.log")
	{
		IO::IConfigINI config("engine.ini");
		config.Load();
		m_bLogAlloc = config.ReadIntValue("MemoryLogAlloc",1,true);
		if (config.NeedToBeSave())
			config.Save();

		/*if (m_bLogByDate)
		{
			m_File.Open("Memory leaks.log",IO::IFileStream::IOM_WRITE);
		}else{
			m_File.Open("Memory leaks.log",IO::IFileStream::IOM_WRITE);
		}*/
		// On vérifie que le fichier est bien ouvert
		//if (!m_File)
		//	throw CLoadingFailed("Memory leaks.log", "Impossible d'accéder en écriture");

		// Inscription de l'en-tête du fichier
		m_File << "  ========================================" << EOL;
		m_File << "            Memory leak tracker           " << EOL;
		m_File << "  ========================================" << EOL << EOL;
	}


	////////////////////////////////////////////////////////////
	// Destructeur
	//
	////////////////////////////////////////////////////////////
	IMemoryManager::~IMemoryManager()
	{
		if (m_Blocks.empty())
		{
			// Aucune fuite, bravo !
			m_File << EOL;
			m_File << "  ========================================" << EOL;
			m_File << "               No leak detected           " << EOL;
			m_File << "  ========================================" << EOL;
		}
		else
		{
			// Fuites mémoires =(
			m_File << EOL;
			m_File << "  ========================================" << EOL;
			m_File << "   Error : Some leaks have been detected  " << EOL;
			m_File << "  ========================================" << EOL;
			m_File << EOL;

			ReportLeaks();
		}
	}


	////////////////////////////////////////////////////////////
	// Renvoie l'instance de la classe
	//
	// [retval] Référence sur l'instance unique de la classe
	//
	////////////////////////////////////////////////////////////
	IMemoryManager& IMemoryManager::Instance()
	{
		return m_Instance;
	}


	////////////////////////////////////////////////////////////
	// Inscrit le rapport sur les fuites de mémoire
	//
	////////////////////////////////////////////////////////////
	void IMemoryManager::ReportLeaks()
	{
		// Détail des fuites
		std::size_t TotalSize = 0;
		for (TBlockMap::iterator i = m_Blocks.begin(); i != m_Blocks.end(); ++i)
		{
			// Ajout de la taille du bloc au cumul
			TotalSize += i->second.Size;

			// Inscription dans le fichier des informations sur le bloc courant
			m_File << "-> 0x" << i->first
				   << " | "   << std::setw(7) << std::setfill(' ') << static_cast<int>(i->second.Size) << " octets"
				   << " | "   << i->second.File.c_str() << " (" << i->second.Line << ")" << EOL;

			// Libération de la mémoire
			free(i->first);
		}

		// Affichage du cumul des fuites
		m_File << EOL << EOL << "-- "
			   << static_cast<int>(m_Blocks.size()) << " blocs non-libéré(s), "
			   << static_cast<int>(TotalSize)       << " octets --"
			   << EOL;
	}


	////////////////////////////////////////////////////////////
	// Ajoute une allocation mémoire
	//
	// [in] Size :  Taille allouée
	// [in] File :  Fichier contenant l'allocation
	// [in] Line :  Ligne de l'allocation
	// [in] Array : True si c'est une allocation de tableau
	//
	// [retval] Pointeur sur la mémoire allouée
	//
	////////////////////////////////////////////////////////////
	void* IMemoryManager::Allocate(std::size_t Size, const Core::IString& File, int Line, bool Array)
	{
		// Allocation de la mémoire
		void* Ptr = malloc(Size);

		// Ajout du bloc à la liste des blocs alloués
		TBlock NewBlock;
		NewBlock.Size  = Size;
		NewBlock.File  = File;
		NewBlock.Line  = Line;
		NewBlock.Array = Array;
		m_Blocks[Ptr]  = NewBlock;

		// Loggization
		if (m_bLogAlloc)
			m_File << "++ Allocation    | 0x" << Ptr
				   << " | " << std::setw(7) << std::setfill(' ') << static_cast<int>(NewBlock.Size) << " octets"
				   << " | " << NewBlock.File.c_str() << " (" << NewBlock.Line << ")" << EOL;

		return Ptr;
	}


	////////////////////////////////////////////////////////////
	// Retire une allocation mémoire
	//
	// [in] Ptr :   Adresse de la mémoire desallouée
	// [in] Array : True si c'est une désallocation de tableau
	//
	////////////////////////////////////////////////////////////
	void IMemoryManager::Free(void* Ptr, bool Array)
	{
		// Recherche de l'adresse dans les blocs alloués
		TBlockMap::iterator It = m_Blocks.find(Ptr);

		// Si le bloc n'a pas été alloué, on génère une erreur
		if (It == m_Blocks.end())
		{
			// En fait ça arrive souvent, du fait que le delete surchargé est pris en compte même là où on n'inclue pas DebugNew.h,
			// mais pas la macro pour le new
			// Dans ce cas on détruit le bloc et on quitte immédiatement
			free(Ptr);
			return;
		}

		// Si le type d'allocation ne correspond pas, on génère une erreur
		//ASSERT_MSG(It->second.Array == Array,"Error : memory free")
		ASSERT(It->second.Array == Array)
		if (It->second.Array != Array)
		{
			//throw CBadDelete(Ptr, It->second.File.c_str(), It->second.Line, !Array);
			//printf("Erreur allocation\n");			
		}

		// Finalement, si tout va bien, on supprime le bloc et on loggiz tout ça
		if (m_bLogAlloc)
			m_File << "-- Désallocation | 0x" << Ptr
				   << " | " << std::setw(7) << std::setfill(' ') << static_cast<int>(It->second.Size) << " octets"
				   << " | " << m_DeleteStack.top().File.c_str() << " (" << m_DeleteStack.top().Line << ")" << EOL;
		m_Blocks.erase(It);
		m_DeleteStack.pop();
	    
		// Libération de la mémoire
		free(Ptr);
	}


	////////////////////////////////////////////////////////////
	// Sauvegarde les infos sur la désallocation en cours
	//
	// [in] File :  Fichier contenant la désallocation
	// [in] Line :  Ligne de la désallocation
	//
	////////////////////////////////////////////////////////////
	void IMemoryManager::NextDelete(const Core::IString& File, int Line)
	{
		TBlock Delete;
		Delete.File = File;
		Delete.Line = Line;

		m_DeleteStack.push(Delete);
	}
} // namespace Intial
