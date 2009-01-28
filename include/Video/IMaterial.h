//---------------------------------------------
//                Initial Engine
//               Material  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IMATERIAL_HEADER_
#define _IMATERIAL_HEADER_

#include "Core/IString.h"
#include "Core/IArray.h"

namespace Initial
{
	class IRenderDriver;
	class ITexture;
	class IShader;

	class IMaterial
	{
	public:
		static IMaterial* LoadMaterial(IRenderDriver *device, IString filename);
		static IMaterial* LoadMaterialFromTexture(IRenderDriver *device, IString filename);
		static IMaterial* LoadMaterialFromTexture(IRenderDriver *device, ITexture *tex);

		IShader*	GetShader();
		ITexture*	GetTexture(int id);
		bool		TextureIsDefine(int id);
		void		SetTexture(ITexture* tex, int id);

		IString		GetFilename();
		IString		GetName();

		IMaterial*	GetParent();
		void		SetParent(IMaterial* mat);

		~IMaterial();
	protected:
		IMaterial();
	protected:
		IString			m_sFilename;
		IString			m_sName;
		IShader*		m_pShader;
		ITexture*		m_pTextures[8];
		bool			m_bTexturesDefine[8];
		//IArray<IVar>	m_aCustomVar;
		IMaterial*		m_pMaterialParent;

		static IArray<IMaterial*> MaterialList;
	};

}
#endif