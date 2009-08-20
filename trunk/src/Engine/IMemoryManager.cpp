//==========================================================
// MemoryManager.cpp
//
// Implantation de la classe IMemoryManager
//
//==========================================================


//==========================================================
// En-t�tes
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
	// Constructeur par d�faut
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
		// On v�rifie que le fichier est bien ouvert
		//if (!m_File)
		//	throw CLoadingFailed("Memory leaks.log", "Impossible d'acc�der en �criture");

		// Inscription de l'en-t�te du fichier
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
			// Fuites m�moires =(
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
	// [retval] R�f�rence sur l'instance unique de la classe
	//
	////////////////////////////////////////////////////////////
	IMemoryManager& IMemoryManager::Instance()
	{
		return m_Instance;
	}


	////////////////////////////////////////////////////////////
	// Inscrit le rapport sur les fuites de m�moire
	//
	////////////////////////////////////////////////////////////
	void IMemoryManager::ReportLeaks()
	{
		// D�tail des fuites
		std::size_t TotalSize = 0;
		for (TBlockMap::iterator i = m_Blocks.begin(); i != m_Blocks.end(); ++i)
		{
			// Ajout de la taille du bloc au cumul
			TotalSize += i->second.Size;

			// Inscription dans le fichier des informations sur le bloc courant
			m_File << "-> 0x" << i->first
				   << " | "   << std::setw(7) << std::setfill(' ') << static_cast<int>(i->second.Size) << " octets"
				   << " | "   << i->second.File.c_str() << " (" << i->second.Line << ")" << EOL;

			// Lib�ration de la m�moire
			free(i->first);
		}

		// Affichage du cumul des fuites
		m_File << EOL << EOL << "-- "
			   << static_cast<int>(m_Blocks.size()) << " blocs non-lib�r�(s), "
			   << static_cast<int>(TotalSize)       << " octets --"
			   << EOL;
	}


	////////////////////////////////////////////////////////////
	// Ajoute une allocation m�moire
	//
	// [in] Size :  Taille allou�e
	// [in] File :  Fichier contenant l'allocation
	// [in] Line :  Ligne de l'allocation
	// [in] Array : True si c'est une allocation de tableau
	//
	// [retval] Pointeur sur la m�moire allou�e
	//
	////////////////////////////////////////////////////////////
	void* IMemoryManager::Allocate(std::size_t Size, const Core::IString& File, int Line, bool Array)
	{
		// Allocation de la m�moire
		void* Ptr = malloc(Size);

		// Ajout du bloc � la liste des blocs allou�s
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
	// Retire une allocation m�moire
	//
	// [in] Ptr :   Adresse de la m�moire desallou�e
	// [in] Array : True si c'est une d�sallocation de tableau
	//
	////////////////////////////////////////////////////////////
	void IMemoryManager::Free(void* Ptr, bool Array)
	{
		// Recherche de l'adresse dans les blocs allou�s
		TBlockMap::iterator It = m_Blocks.find(Ptr);

		// Si le bloc n'a pas �t� allou�, on g�n�re une erreur
		if (It == m_Blocks.end())
		{
			// En fait �a arrive souvent, du fait que le delete surcharg� est pris en compte m�me l� o� on n'inclue pas DebugNew.h,
			// mais pas la macro pour le new
			// Dans ce cas on d�truit le bloc et on quitte imm�diatement
			free(Ptr);
			return;
		}

		// Si le type d'allocation ne correspond pas, on g�n�re une erreur
		//ASSERT_MSG(It->second.Array == Array,"Error : memory free")
		ASSERT(It->second.Array == Array)
		if (It->second.Array != Array)
		{
			//throw CBadDelete(Ptr, It->second.File.c_str(), It->second.Line, !Array);
			//printf("Erreur allocation\n");			
		}

		// Finalement, si tout va bien, on supprime le bloc et on loggiz tout �a
		if (m_bLogAlloc)
			m_File << "-- D�sallocation | 0x" << Ptr
				   << " | " << std::setw(7) << std::setfill(' ') << static_cast<int>(It->second.Size) << " octets"
				   << " | " << m_DeleteStack.top().File.c_str() << " (" << m_DeleteStack.top().Line << ")" << EOL;
		m_Blocks.erase(It);
		m_DeleteStack.pop();
	    
		// Lib�ration de la m�moire
		free(Ptr);
	}


	////////////////////////////////////////////////////////////
	// Sauvegarde les infos sur la d�sallocation en cours
	//
	// [in] File :  Fichier contenant la d�sallocation
	// [in] Line :  Ligne de la d�sallocation
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
