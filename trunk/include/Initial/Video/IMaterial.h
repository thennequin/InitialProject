//---------------------------------------------
//                Initial Engine
//               Material  Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#ifndef _IMATERIAL_HEADER_
#define _IMATERIAL_HEADER_

#include "Initial/Core/IString.h"
#include "Initial/Core/IList.h"
#include "Initial/Core/IArray.h"
#include "Initial/IRessource.h"
#include "Initial/Video/IMaterialExpression.h"

class MaterialGraph;
namespace Initial
{
	class IDevice;

	namespace Video
	{
		class IRenderDriver;
		class ITexture;
		class IShader;
	
		class IMaterial : public IRessource
		{
			friend class ::Initial::IRessourceManager;
			friend class MaterialGraph;
		public:
			enum MaterialType
			{
				IMT_OPAQUE,
				IMT_MASK,
				IMT_TRANSLUCENT,
			};
		public:
			virtual void Reload();

			Video::IShader*		GetShader();
			Video::ITexture*	GetTexture(int id);
			void				SetTexture(Video::ITexture* tex, int id);

			Core::IString		GetFilename();
			Core::IString		GetName();

			IMaterial*			GetParent();
			void				SetParent(IMaterial* mat);

			bool				IsTranslucent();

			IDevice *GetDevice();

			~IMaterial();
		//protected:
			IMaterial(IDevice *device);

			bool Load(Core::IString filename);
			bool LoadV2(Core::IString file);
			bool SaveV2(Core::IString file);

			bool CompileV2();

			virtual void OnPropertyChange(Core::IString name);

			Core::IList<IMaterialExpression*>& GetExpressions() {return m_aExpressions;}
		protected:
			IDevice					*m_pDevice;
			//Core::IString			m_sFilename;
			Core::IString			m_sName;
			Video::IShader*			m_pShader;
			ISmartPtr<Video::ITexture>		m_pTextures[8];
			//Video::ITexture*		m_pTextures[8];
			//Core::IArray<IVar>	m_aCustomVar;
			IMaterial*				m_pMaterialParent;
			bool					m_bTranslucent;
			MaterialType			m_iType;

			bool m_bOnPropChange;

			//Material V2
			//IMaterialExpression *m_eFinal;
			Core::IList<IMaterialExpression*> m_aExpressions;
			UINT NextId;
		};
	}
}
#endif