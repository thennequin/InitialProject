//---------------------------------------------
//                Initial Engine
//            Material  source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Video/IRenderDriver.h"
#include "Video/IMaterial.h"
#include "Video/IShader.h"
#include "Video/ITexture.h"

using namespace Initial;

IArray<IMaterial*> IMaterial::MaterialList;

enum 
{
	MAT_NONE = 0,
	MAT_TEXT,
	MAT_NAME,
	MAT_BODY,
	MAT_DEFAULT_WAIT,
	MAT_DEFAULT_BRACKET,
	MAT_DEFAULT,
	MAT_END
};

IMaterial* IMaterial::LoadMaterial(IRenderDriver *device, IString filename)
{
	for (int i=0;i<MaterialList.Count();i++)
		if (MaterialList[i]->m_sFilename==filename)
			return MaterialList[i];

	if (device)
	{
		FILE *file = fopen(filename,"r");

		if (!file)
		{
			IString str="Material/";
			str+=filename;
			file = fopen(str,"r");
		}

		int level;
		int space=0;
		IString Buffer;

		IArray<IString> defaultStrs;
		IArray<IString> defaultStrsValue;
		int defaultId=0;	

		bool Comment=false;

		if (file)
		{
			IMaterial *mat = new IMaterial();
			mat->m_sFilename=filename;

			int state=MAT_NONE;
			int length=0;
			IString text;

			while(!feof(file))
			{
				text+=(char)fgetc(file);
			}
			fclose(file);
			//printf("%s\n",text.c_str());

			while (text[length]!='\0')
			{
				if (Comment==false)
					switch (state)
					{
					case MAT_NONE:
						if (text[length]=='"')
						{
							state = MAT_NAME;
							Buffer="";
						}else if (text[length]=='{')
						{
							state = MAT_BODY;
							Buffer="";
						}else if (text[length]=='#')
							Comment=true;
						break;
					case MAT_BODY:
						if (text[length]=='"')
						{
							state = MAT_TEXT;
							Buffer="";
						}else if (text[length]=='}')
							state=MAT_END;
						else if (text[length]=='#')
							Comment=true;
						break;
					case MAT_NAME:
						if (text[length]=='"')
						{
							state = MAT_NONE;
							mat->m_sName=Buffer;
						}else{
							Buffer+=text[length];
						}
						break;
					case MAT_TEXT:
						if (text[length]=='"')
						{
							for (int i=0;i<defaultStrs.Count();i++)
							{
								if (Buffer.ToLower().Cmp(defaultStrs[i])==0)
								{
									state=MAT_DEFAULT_WAIT;
									defaultId=i;								
								}
							}	
							if (state!=MAT_DEFAULT_WAIT)
							{
								defaultStrs.Add(IString(Buffer));
								defaultStrsValue.Add(IString(""));
								defaultId=defaultStrs.Count()-1;
								state=MAT_DEFAULT_WAIT;
							}
							space=0;
						}else{
							Buffer+=text[length];
						}
						break;
					case MAT_DEFAULT_WAIT:
						if (text[length]=='{')
						{
							state = MAT_DEFAULT_BRACKET;
							Buffer="";
							level=0;
						}else if (text[length]=='"')
						{
							state = MAT_DEFAULT;
							Buffer="";
						}else
							space++;
						break;
					case MAT_DEFAULT_BRACKET:
						if (text[length]=='}' && level==0)
						{
							state = MAT_BODY;
							defaultStrsValue[defaultId]=Buffer;
						}else{
							if (text[length]=='{')
								level++;

							if (text[length]=='}')
								level--;

							Buffer+=text[length];
						}
						break;
					case MAT_DEFAULT:
						if (text[length]=='"')
						{
							state = MAT_BODY;
							defaultStrsValue[defaultId]=Buffer;
						}else{
							Buffer+=text[length];
						}
						break;
					case MAT_END:
						break;
					}
				else{
					if (text[length]=='\n') // Le commentaire s'arréte en fin de ligne
						Comment=false;
				}
				length++;
			}

			if (state==MAT_END)
				printf("Aucun probleme dans le fichier\n");
			else{
				printf("Probleme detecte dans le fichier\n");
				return NULL;
			}

			/*for (int i=0;i<defaultStrs.Count();i++)
				printf("%s : %s \"%s\"\n",defaultStrs[i].c_str(),defaultStrsValue[i].c_str(),defaultStrs[i].Left(7).c_str());*/

			IString vertex,fragment;
			for (int i=0;i<defaultStrs.Count();i++)
			{
				if (defaultStrs[i].ToLower().Cmp("materialbase")==0)
				{
					if (mat->m_pMaterialParent==NULL)
						mat->m_pMaterialParent=LoadMaterial(device,defaultStrsValue[i]);
				}else if (defaultStrs[i].ToLower().Cmp("vertex")==0)
					vertex=defaultStrsValue[i];
				else if (defaultStrs[i].ToLower().Cmp("fragment")==0)
					fragment=defaultStrsValue[i];				
				else if (defaultStrs[i].ToLower().Left(7).Cmp("texture")==0)
				{
					int pos = atoi(defaultStrs[i].Right(7));
					if (pos>=0 && pos<8)
					{
						mat->m_bTexturesDefine[pos]=true;
						mat->m_pTextures[pos]=device->LoadTexture(defaultStrsValue[i]);
					}
				}				
			}

			if (vertex!="" || fragment!="")
			{
				mat->m_pShader = device->LoadShader(vertex,fragment);				
			}

			printf("Material Loaded\n");

			return mat;
		}
	}
	printf("Erreur de lecture du fichier %s\n",filename.c_str());
	return NULL;
}

IMaterial* IMaterial::LoadMaterialFromTexture(IRenderDriver *device, IString filename)
{
	ITexture *tex = device->LoadTexture(filename);

	if (!tex)
	{
		IString str="Textures/";
		str+=filename;
		tex = device->LoadTexture(str);
	}
	if (tex)
		return LoadMaterialFromTexture(device,tex);
	return NULL;
}

IMaterial* IMaterial::LoadMaterialFromTexture(IRenderDriver *device, ITexture *tex)
{
	if (tex)
	{
		IMaterial *mat = new IMaterial;
		mat->m_pTextures[0]=tex;
		return mat;
	}
	return NULL;
}

IMaterial::IMaterial()
{
	m_pShader=NULL;
	m_pMaterialParent=NULL;
	for (int i=0;i<8;i++)
	{
		m_pTextures[i]=NULL;	
		m_bTexturesDefine[i]=false;
	}

	MaterialList.Add(this);
}

IMaterial::~IMaterial()
{
	MaterialList.DeleteItem(this);
}

IShader* IMaterial::GetShader()
{
	return m_pShader;
}

ITexture* IMaterial::GetTexture(int id)
{
	if (id>=0 && id<8)
		return m_pTextures[id];
	return NULL;
}

bool IMaterial::TextureIsDefine(int id)
{
	if (id>=0 && id<8)
		return m_bTexturesDefine[id];
	return false;
}

void IMaterial::SetTexture(ITexture* tex, int id)
{
	if (id>=0 && id<8)
	{
		m_bTexturesDefine[id]= tex ? true : false;
		m_bTexturesDefine[id]=true;
		m_pTextures[id]=tex;
	}
}

IMaterial* IMaterial::GetParent()
{
	return m_pMaterialParent;
}

void IMaterial::SetParent(IMaterial* mat)
{
	m_pMaterialParent=mat;
}

IString IMaterial::GetFilename()
{
	return m_sFilename;
}

IString IMaterial::GetName()
{
	return m_sName;
}
