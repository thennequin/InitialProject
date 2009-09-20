//---------------------------------------------
//                Initial Engine
//            Material  source Module
//
//            By Thibault HENNEQUIN
//              December 20th  2008
//---------------------------------------------

#include "Initial/Ressource/IMaterial.h"

#include "Initial/ILogger.h"
#include "Initial/IDevice.h"
#include "Initial/Core/IUtil.h"
#include "Initial/Video/IRenderDriver.h"
#include "Initial/Video/IShader.h"
#include "Initial/Ressource/ITexture.h"
#include "Initial/IO/IFileStream.h"
#include "Initial/IO/IXMLParser.h"

using namespace Initial::Core;
using namespace Initial::IO;
using namespace Initial::IO::XML;

namespace Initial
{
	namespace Video
	{

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

		IMaterial::IMaterial(IDevice *device)
		: IRessource("IMaterial")
		{
			m_pDevice=device;
			m_pShader=NULL;
			m_pMaterialParent=NULL;
			/*for (int i=0;i<8;i++)
			{
				m_pTextures[i]=NULL;	
			}*/
			m_bTranslucent=false;

			//V2
			IMaterialExpression *finalExpr=new IMaterialExpressionFinal;
			finalExpr->m_pMaterial=this;
			m_aExpressions.PushFront(finalExpr);
			NextId=1;

			m_bOnPropChange=false;

			AddProperty("Texture1",IPT_RESSOURCE_ITX);
			AddProperty("Texture2",IPT_RESSOURCE_ITX);
			AddProperty("Texture3",IPT_RESSOURCE_ITX);
			AddProperty("Texture4",IPT_RESSOURCE_ITX);
			AddProperty("Texture5",IPT_RESSOURCE_ITX);
			AddProperty("Texture6",IPT_RESSOURCE_ITX);
			AddProperty("Texture7",IPT_RESSOURCE_ITX);
			AddProperty("Texture8",IPT_RESSOURCE_ITX);

			Core::IList<Core::IString> strs;
			strs.PushBack(IString("Solid"));
			strs.PushBack(IString("Masked"));
			strs.PushBack(IString("Translucent"));
			AddProperty("Blend mode",IPT_CHOICE,NULL,strs);

			strs.Clear();
			strs.PushBack(IString("Lit"));
			strs.PushBack(IString("Unlit"));
			strs.PushBack(IString("Omnidirectional"));
			AddProperty("Light mode",IPT_CHOICE,NULL,strs);

			AddProperty("Mask clip",IPT_FLOAT);
			SetPropertyValue("Mask clip","0.5");
			AddProperty("Two side",IPT_BOOL);
		}

		IMaterial::~IMaterial()
		{
			//delete m_eFinal;
			for (UINT i=0;i<m_aExpressions.Count();i++)
				delete m_aExpressions[i];
		}

		void IMaterial::OnPropertyChange(IString name)
		{
			if (m_bOnPropChange==false)
			{
				m_bOnPropChange=true;

				if (GetDevice() && GetDevice()->GetRessourceManager())
				{
					//printf("OnPropertyChange\n");
					int Id=-1;
					if (name=="Texture1")
						Id=0;
					else if (name=="Texture2")
						Id=1;
					else if (name=="Texture3")
						Id=2;
					else if (name=="Texture4")
						Id=3;
					else if (name=="Texture5")
						Id=4;
					else if (name=="Texture6")
						Id=5;
					else if (name=="Texture7")
						Id=6;
					else if (name=="Texture8")
						Id=7;

					if (Id>=0)
					{
						IProperty *prop = GetPropertyList().GetProperty(name);
						if (prop)
						{
							IString tex = prop->GetString();
							//printf("%s\n",tex.c_str());
							ITexture *Tex=NULL;
							if (tex!="")
								Tex = GetDevice()->GetRessourceManager()->LoadTexture(tex);
							//printf("%s\n",GetFilename().c_str());
							SetTexture(Tex,Id);
						}
					}
				}

				CompileV2();
				m_bOnPropChange=false;
			}

			IRessource::OnPropertyChange(name);
		}

		IDevice *IMaterial::GetDevice()
		{
			return m_pDevice;
		}

		void IMaterial::Reload()
		{
			ILogger::LogDebug("Reload Material\n");
			Load(m_sFileName);
		}

		bool IMaterial::Load(Core::IString filename)
		{
			IFileStream file(filename,IFileStream::IOM_READ);

			/*if (!file.IsOk())
			{
				filename="Materials/"+filename;
				file.Open(filename,IFileStream::IOM_READ);
			}*/

			int level;
			int space=0;
			IString Buffer;
			Buffer.AllocBuffer(64);

			IArray<IString> defaultStrs;
			IArray<IString> defaultStrsValue;
			int defaultId=0;	

			bool Comment=false;

			if (file.IsOk())
			{
				//IMaterial *mat = new IMaterial();
				m_sFileName=GetFullPath(filename);
				ILogger::LogDebug("Material %s\n",m_sFileName.c_str());

				int state=MAT_NONE;
				int length=0;
				IString text;

				while(!file.EndOfFile())
				{
					char car = file.GetChar();
					//printf("%c",car);
					text+=car;
				}
				file.Close();
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
								m_sName=Buffer;
							}else{
								Buffer+=text[length];
							}
							break;
						case MAT_TEXT:
							if (text[length]=='"')
							{
								for (int i=0;i<defaultStrs.Count();i++)
								{
									if (Buffer.ToLower()==defaultStrs[i])
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
					ILogger::LogDebug("Aucun probleme dans le fichier\n");
				else{
					ILogger::LogWarning("Probleme detecte dans le fichier\n");
					return false;
				}

				/*for (int i=0;i<defaultStrs.Count();i++)
					printf("%s : %s \"%s\"\n",defaultStrs[i].c_str(),defaultStrsValue[i].c_str(),defaultStrs[i].Left(7).c_str());*/

				IString vertex,fragment;
				for (int i=0;i<defaultStrs.Count();i++)
				{
					if (defaultStrs[i].ToLower()=="materialbase")
					{
						if (m_pMaterialParent==NULL)							
							m_pMaterialParent = m_pDevice->GetRessourceManager()->LoadMaterial(defaultStrsValue[i]);
					}else if (defaultStrs[i].ToLower()=="vertex")
						vertex=defaultStrsValue[i];
					else if (defaultStrs[i].ToLower()=="fragment")
						fragment=defaultStrsValue[i];				
					else if (defaultStrs[i].ToLower().Left(7)=="texture")
					{
						int pos = atoi(defaultStrs[i].Right(7).c_str());
						if (pos>=0 && pos<8)
						{
							SetTexture(m_pDevice->GetRessourceManager()->LoadTexture(defaultStrsValue[i]),pos);
							//m_pTextures[pos]=m_pDevice->GetRessourceManager()->LoadTexture(defaultStrsValue[i]);
						}
					}else if (defaultStrs[i].ToLower()=="translucent")
					{
						ILogger::LogDebug("Material Translucent\n");
						m_bTranslucent=atoi(defaultStrsValue[i].c_str());
					}	
				}

				if (vertex!="" || fragment!="")
				{
					if (m_pShader)
					{
						m_pShader->SetFragmentCode(fragment);
						m_pShader->SetVertexCode(vertex);
						m_pShader->Compile();
					}else{
						m_pShader = m_pDevice->GetRenderDriver()->LoadShader(vertex,fragment);
						m_pShader->Compile();
					}
					if (m_pShader->IsCompiled()==false)
						ILogger::LogError("erreur de compilation %s\n",filename);
				}

				m_sFileName=filename;
				ILogger::LogMessage("Material Loaded %s\n",filename);

				return true;
			}
			ILogger::LogError("Fichier non trouver %s\n",filename.c_str());
			return false;
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

		void IMaterial::SetTexture(ITexture* tex, int id)
		{
			if (id>=0 && id<8)
			{
				m_pTextures[id]=tex;
				IString name;
				name.Printf("Texture%d",id+1);
				SetPropertyValue(name,tex!=NULL?tex->GetFilename():"");
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
			return m_sFileName;
		}

		IString IMaterial::GetName()
		{
			return m_sName;
		}

		bool IMaterial::IsTranslucent()
		{
			return m_bTranslucent;
		}

		bool IMaterial::LoadV2(IString file)
		{
			IXMLParser xmlreader(file);
			if (xmlreader.Parse())
			{
				Lock();
				/*FOREACH(IMaterialExpression*,ite,m_aExpressions)
				{
					printf("%s\n",ite.GetData()->GetClass()->GetName().c_str());
				}*/
				NextId=1;
				//Clear Expressions
				//m_aExpressions::Iterator ite;
				for (UINT i=1;m_aExpressions.Count()>1;)
				{
					delete m_aExpressions[i];
					m_aExpressions.Delete(1);
				}
				ILogger::LogDebug("Parse Ok\n");
				XML::IXMLNode* nodeIMA = xmlreader.GetFirstNode();
					XML::IXMLNode* nodeTex = NULL;
						XML::IXMLNode* nodeTexFile = NULL;
					XML::IXMLNode* nodeParams = NULL;
						XML::IXMLNode* nodeParamVal = NULL;
					XML::IXMLNode* nodeExprs = NULL;
						XML::IXMLNode* nodeExpr = NULL;
						XML::IXMLNode* nodeConnect = NULL;
				while(nodeIMA)
				{
					if (nodeIMA->GetName().ToLower()=="ima")
					{
						//Parse texture informations
						/*nodeTex = nodeIMA->GetChild();
						while (nodeTex)
						{
							if (nodeTex->GetName().ToLower()=="textures")
							{
								ILogger::LogDebug("Textures\n");
								nodeTexFile = nodeTex->GetChild();
								while (nodeTexFile)
								{
									if (nodeTexFile->GetName().ToLower()=="texture")
									{
										ILogger::LogDebug("Texture\n");
										int TexId = Core::Iatof(nodeTexFile->GetValue("Id",0));
										if (TexId>=0 && TexId<8)
										{
											//m_pTextures[TexId]=m_pDevice->GetRessourceManager()->LoadTexture(nodeTexFile->GetValue("File"));
											SetTexture(m_pDevice->GetRessourceManager()->LoadTexture(nodeTexFile->GetValue("File")),TexId);
											//ILogger::LogDebug("Tex %d %s %s\n", TexId, nodeTexFile->GetValue("File").c_str(),m_pTextures[TexId]->GetFilename().c_str());
										}
									}
									nodeTexFile = nodeTexFile->GetNext();
								}
								break;
							}
							nodeTex = nodeTex->GetNext();
						}*/

						nodeParams = nodeIMA->GetChild();
						while (nodeParams)
						{
							if (nodeParams->GetName().ToLower()=="parameters")
							{
								nodeParamVal = nodeParams->GetChild();
								while (nodeParamVal)
								{
									if (nodeParamVal->GetName().ToLower()=="parameter")
									{
										SetPropertyValue(nodeParamVal->GetValue("Name",""),nodeParamVal->GetValue("Value",""));
									}
									nodeParamVal = nodeParamVal->GetNext();
								}
								break;
							}
							nodeParams = nodeParams->GetNext();
						}

						//Parse Expressions informations
						nodeExprs = nodeIMA->GetChild();
						while (nodeExprs)
						{
							if (nodeExprs->GetName().ToLower()=="expressions")
							{
								NextId=1;
								//Create Expressions
								nodeExpr=nodeExprs->GetChild(0);
								while (nodeExpr)
								{
									if (nodeExpr->GetName().ToLower()=="expression")
									{
										IObject::IObjectInfo *info = IObject::GetStaticClassByName(nodeExpr->GetValue("type"));
										if (info && info->IsKindOf(IMaterialExpression::GetStaticClass()))
										{
											if (info->GetName()=="IMaterialExpressionFinal")
											{
												//m_eFinal->ParseNode(nodeExpr);
												m_aExpressions.First()->GetData()->ParseNode(nodeExpr);
											}else{
												IMaterialExpression *expr = (IMaterialExpression*)info->CreateInstance();
												if (expr)
												{
													expr->m_pMaterial=this;
													expr->ParseNode(nodeExpr);
													if (NextId<=expr->GetExprId())
														NextId=expr->GetExprId()+1;
													m_aExpressions.PushBack(expr);
													//expr->_Init;
												}
											}
										}
									}
									nodeExpr=nodeExpr->GetNext();
								}

								//Link Expressions
								nodeExpr=nodeExprs->GetChild(0);
								while (nodeExpr)
								{
									if (nodeExpr->GetName().ToLower()=="expression")
									{
										UINT id = Iatof(nodeExpr->GetValue("Id"));
										//IList<IMaterialExpression*>::ListNode *node = m_aExpressions.First();
										//while (node)
										for (IList<IMaterialExpression*>::Iterator ite=m_aExpressions.Ite();ite;ite++)
										{
											if (ite.GetData()->GetExprId()==id)
											{
												nodeConnect=nodeExpr->GetChild(0);
												while (nodeConnect)
												{
													if (nodeConnect->GetName().ToLower()=="connect")
													{
														UINT inId = Iatof(nodeConnect->GetValue("InputId"));
														UINT exprId = Iatof(nodeConnect->GetValue("ExprId"));
														UINT outId = Iatof(nodeConnect->GetValue("OutputId"));
														for (IList<IMaterialExpression*>::Iterator ite2=m_aExpressions.Ite();ite2;ite2++)
														{
															if(ite2.GetData()->GetExprId()==exprId)
															{
																ite.GetData()->ConnectInput(inId,ite2.GetData(),outId);
																break;
															}
														}
													}													
													nodeConnect=nodeConnect->GetNext();
												}
												break;
											}											
										}
									}
									nodeExpr=nodeExpr->GetNext();
								}
								break;
							}
							nodeExprs = nodeExprs->GetNext();
						}
						SetFilename(GetFullPath(file));
						CompileV2();
						Unlock();
						ILogger::LogMessage("Material Loaded %s\n",GetFullPath(file).c_str());
						return true;
					}
					nodeIMA=nodeIMA->GetNext();
				}
				Unlock();
				return false;
			}

			XML::IXMLNode* node = xmlreader.GetFirstNode();
			IString str="-------------------\n";
			while(node)
			{			
				str+=node->print();
				node=node->GetNext();
			}
			ILogger::LogDebug("parse error\n%s\n",str);
			return false;
		}

		bool IMaterial::SaveV2(IString file)
		{
			IFileStream hFile;
			if (hFile.Open(file,IFileStream::IOM_WRITE))
			{
				hFile.Printf("<?xml version=\"1.0\"?>\n");
				hFile.Printf("<IMA version=\"1.0\">\n");
				/*hFile.Printf("\t<Textures>\n");

				IString appPath = AppPath();
				
				for (int i=0;i<8;i++)
				{
					if (m_pTextures[i])
					{
						IString relativeFile=Core::RelativeToPath(appPath,m_pTextures[i]->GetFilename());
						//hFile.Printf("\t\t<Texture Id=\"%d\" File=\"%s\"/>\n",i,m_pTextures[i]->GetFilename().c_str());
						//hFile.Printf("\t\t<Texture Id=\"%d\" File=\"%s\"/>\n",i,relativeFile.c_str());
					}
				}
				hFile.Printf("\t</Textures>\n");*/

				hFile.Printf("\t<Parameters>\n");
				IPropertyList& list = GetPropertyList();
				for (UINT i=0;i<list.Count();i++)
				{
					IProperty *prop = list.GetProperty(i);
					if (prop)
					{
						hFile.Printf("\t\t<Parameter Name=\"%s\" Value=\"%s\"/>\n",prop->GetName().c_str(),prop->GetString().c_str());
					}
				}
				hFile.Printf("\t</Parameters>\n");

				hFile.Printf("\t<Expressions>\n");
					//Write Final expression
					//hFile.Printf("%s\n",m_eFinal->WriteNode(false).c_str());
					bool First=false;
					for (IList<IMaterialExpression*>::Iterator ite=m_aExpressions.Ite();ite;ite++)
					{
						hFile.Printf("%s\n",ite.GetData()->WriteNode(First).c_str());
						if (First==false)
							First=true;
					}
				hFile.Printf("\t</Expressions>\n");
				hFile.Printf("</IMA>");				

				return true;
			}
			return false;
		}

		bool IMaterial::CompileV2()
		{
			FOREACH(IMaterialExpression*,ite,m_aExpressions)
			{
				ite.GetData();
			}
			IString frag, vert;
			m_aExpressions[0]->GenerateShader(frag,vert);
			if (m_pShader)
			{
				m_pShader->SetFragmentCode(frag);
				m_pShader->SetVertexCode(vert);
				//m_pShader->Compile();
			}else{
				m_pShader = m_pDevice->GetRenderDriver()->LoadShader(vert,frag);
				/*if (m_pShader)
					m_pShader->Compile();*/
			}
			return true;
		}
	}
}
