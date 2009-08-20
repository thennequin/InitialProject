
#include "Initial/IO/IXMLParser.h"

//#include "wx/filename.h"
//#include "Util.h"

using namespace Initial::Core;

namespace Initial
{
	namespace IO
	{
		namespace XML
		{
			//-----------------------------------------------
			// IXMLValue
			//-----------------------------------------------

			IXMLValue::IXMLValue(IString name, IString value)
			{
				m_sName=name;
				m_sValue=value;
			}

			IXMLValue::IXMLValue(IXMLValue& copy)
			{
				m_sName=copy.m_sName;
				m_sValue=copy.m_sValue;
			}

			IString IXMLValue::GetName()
			{
				return m_sName;
			}

			IString IXMLValue::GetValue()
			{
				return m_sValue;
			}

			//-----------------------------------------------
			// IXMLNode
			//-----------------------------------------------

			IXMLNode::IXMLNode(IXMLNode* parent)
			{
				m_pParent=parent;
			}

			IXMLNode::IXMLNode(IXMLNode& copy)
			{
				m_pParent=copy.m_pParent;
			}

			IXMLNode::~IXMLNode()
			{
				for (UINT i=0;i<m_aNodes.Count();i++)
				{
					if (m_aNodes[i])
						delete m_aNodes[i];
				}

				for (UINT i=0;i<m_aValues.Count();i++)
				{
					if (m_aValues[i])
						delete m_aValues[i];
				}
			}

			IString IXMLNode::GetName()
			{
				return m_sName;
			}

			IXMLNode* IXMLNode::GetParent()
			{
				return m_pParent;
			}

			IXMLNode* IXMLNode::GetNext()
			{
				if (m_pParent)
				{
					for (unsigned int i=0;i<m_pParent->m_aNodes.Count();i++)
					{
						if (m_pParent->m_aNodes[i]==this)
						{
							if (i+1<m_pParent->m_aNodes.Count())
								return m_pParent->m_aNodes[i+1];
							else return NULL;
						}
					}
				}
				return NULL;
			}

			unsigned int IXMLNode::GetChildCount()
			{
				return m_aNodes.Count();
			}

			unsigned int IXMLNode::GetValueCount()
			{
				return m_aValues.Count();
			}

			IXMLNode* IXMLNode::GetChild(unsigned int i)
			{
				if (i>=0 && i<m_aNodes.Count())
					return m_aNodes[i];
				return NULL;
			}

			IXMLValue* IXMLNode::GetValue(unsigned int i)
			{
				if (i>=0 && i<m_aValues.Count())
					return m_aValues[i];
				return NULL;
			}

			IString IXMLNode::GetValue(IString name,IString def)
			{
				for (unsigned int i=0;i<m_aValues.Count();i++)
				{
					if (m_aValues[i] && m_aValues[i]->GetName().ToLower()==name.ToLower())
						return m_aValues[i]->GetValue();
				}
				return def;
			}

			IString IXMLNode::print(int level)
			{
				IString str,tab;
				for (int i=0;i<level;i++)
					tab+="\t";
				str+=tab;
				str+=m_sName;
				str+="\n";
				for (UINT i=0;i<m_aNodes.Count();i++)
					str+=m_aNodes[i]->print(level+1);
				str+=tab;
				str+="/"+m_sName;
				str+="\n";
				return str;
			}

			//-----------------------------------------------
			// IXMLNode
			//-----------------------------------------------

			IXMLParser::IXMLParser(IString filename)
			{
				m_sFilename=filename;
			}

			IXMLParser::~IXMLParser()
			{
				for (UINT i=0;i<m_aNodes.Count();i++)
				{
					if (m_aNodes[i])
						delete m_aNodes[i];
				}
			}

			bool IXMLParser::Parse(IString filename)
			{
				m_sFilename=filename;
				return Parse();
			}

			bool IXMLParser::Parse()
			{
				IFileStream file(m_sFilename,IFileStream::IOM_READ);

				if (file.IsOk())
				{
					for (UINT i=0;i<m_aNodes.Count();i++)
					{
						if (m_aNodes[i])
							delete m_aNodes[i];
					}

					IString content;
					UINT size = file.Size();
					content.AllocBuffer(size);
					file.Read((void*)content.c_str(),size,1);
					file.Close();

					content[(int)size]='\0';

					IXMLNode *currentNode=NULL;
					IXMLNode *tempNode;

					int State = STATE_START;
					IString Name, Value;
					Name.AllocBuffer(128);
					Value.AllocBuffer(128);

					bool OK=true;
					for (UINT i=0;i<size && OK;i++)
					{
						//printf("%c %d %d\n",content[(int)i],i,size);
						switch (State)
						{
						case STATE_START:
							if (i+4<size && content[(int)i]=='<' && 
								content[(int)i+1]=='?' && 
								content[(int)i+2]=='x' && 
								content[(int)i+3]=='m' && 
								content[(int)i+4]=='l')
								State=STATE_XML_NODE;
							else{
								//ASSERT_MSG(0,"balise <?xml> manquante");
								OK=false;
							}
							break;
						// On ne verifie pas la version de xml ni son encodage
						case STATE_XML_NODE:
							if (content[(int)i]=='>')
								State=STATE_NONE;
							break;
						//-----------------------------------------------
						case STATE_NONE:
							if (i+3<size && content[(int)i]=='<' && content[(int)i+1]=='!' && content[(int)i+2]=='-' && content[(int)i+3]=='-')
							{
								State=STATE_COMMENT;
							}else if (i+1<size && content[(int)i]=='<' && content[(int)i+1]=='/')
							{
								i++; // on pass le caractére suivant qui est /
								Name="";
								State=STATE_END_NODE_NAME;
							}else if (content[(int)i]=='<')
							{
								tempNode = new IXMLNode(currentNode);
								if (currentNode)
									currentNode->m_aNodes.Add(tempNode);
								else
									m_aNodes.Add(tempNode);
								currentNode=tempNode;
								Name="";
								State=STATE_NODE_NAME;
							}else{
								if (currentNode)
									currentNode->m_sBody+=content[(int)i];
							}
							break;
						case STATE_COMMENT:
							if (i-3+1>=0 && content[(int)i]=='>' && content[(int)i-1]=='-' && content[(int)i-2]=='-' /*&& content[(int)i-3]==' '*/)
							{
								State=STATE_NONE;
							}
							break;
						case STATE_NODE_NAME:
							if (content[(int)i]=='>' || content[(int)i]==' ')
							{
								currentNode->m_sName=Name;
								if (content[(int)i]=='>')
									State=STATE_NONE;
								else
									State=STATE_NODE;
							}else{
								Name+=content[(int)i];
							}
							break;
						case STATE_END_NODE_NAME:
							if (content[(int)i]=='>')
							{
								if (!(currentNode && Name==currentNode->m_sName))
									OK=false;
								//ASSERT(currentNode);
								//ASSERT(Name==currentNode->m_sName);
								if (currentNode)
									currentNode = currentNode->GetParent();
								State=STATE_NONE;
							}else{
								Name+=content[(int)i];
							}
							break;
						case STATE_NODE:
							if (i+1<size && content[(int)i]=='/' && content[(int)i+1]=='>')
							{
								if (currentNode)
									currentNode = currentNode->GetParent();
								State=STATE_NONE;
							}else if (content[(int)i]=='>')
							{
								State=STATE_NONE;
							}else{
								Name="";
								State=STATE_VALUE_NAME;
								i--;
							}
							break;
						case STATE_VALUE_NAME:
							if (content[(int)i]=='=')
							{
								State=STATE_VALUE_START;
								Value="";
							}else if (content[(int)i]==' ')
							{
								State=STATE_NODE;
							}else{
								Name+=content[(int)i];
							}
							break;
						case STATE_VALUE_START:
							if (content[(int)i]=='"')
							{
								State=STATE_VALUE_DOUBLE_QUOTE;
							}else if (content[(int)i]=='"')
							{
								State=STATE_VALUE_SIMPLE_QUOTE;
							}else{
								Value+=content[(int)i];
							}
							break;
						case STATE_VALUE_DOUBLE_QUOTE:
						case STATE_VALUE_SIMPLE_QUOTE:
						case STATE_VALUE:
							if (content[(int)i]=='"' && State==STATE_VALUE_DOUBLE_QUOTE)
							{
								//TO FIX
								//Value.Replace("&quot;","\"");
								currentNode->m_aValues.Add(new IXMLValue(Name,Value));
								State=STATE_NODE;
							}else if (content[(int)i]=='\'' && State==STATE_VALUE_SIMPLE_QUOTE)
							{
								currentNode->m_aValues.Add(new IXMLValue(Name,Value));
								State=STATE_NODE;
							}else if (content[(int)i]==' ' && State==STATE_VALUE)
							{
								currentNode->m_aValues.Add(new IXMLValue(Name,Value));
								State=STATE_NODE;
							}else{
								Value+=content[(int)i];
							}
							break;
						}
					}

					if (currentNode || State!=STATE_NONE) // le node finale doit etre nulle et l'état doit etre à NONE sinon c'est qu'il y a une erreur
					{
						OK=false;
					}
					return OK;
				}
				//wxLogMessage("File Not Found");
				return false;
			}

			bool IXMLParser::Save(Core::IString filename)
			{
				IFileStream file(m_sFilename,IFileStream::IOM_READ);

				if (file.IsOk())
				{
					file.Printf("<?xml ?>\n");
					for (int i=0;i<m_aNodes.Count();i++)
					{
						WriteNode(file,m_aNodes[i],0);
					}
					return true;
				}
				return false;
			}

			void IXMLParser::WriteNode(IFileStream& file, IXMLNode *node, int level)
			{
				if (node)
				{
					for (int i=0;i<level;i++)
					{
						file.Printf("\t");
					}
					file.Printf("<%s ",node->GetName().c_str());
					IXMLValue *value;
					for (int i=0;i<node->GetValueCount();i++)
					{
						value = node->GetValue(i);
						file.Printf("%s=\"%s\" ",value->GetName().c_str(),value->GetValue().c_str());
					}
					file.Printf(">\n");
					IXMLNode *child;
					for (int i=0;i<node->GetChildCount();i++)
					{
						child = node->GetChild(i);
						WriteNode(file,child,level++);
					}

				}
			}

			IXMLNode* IXMLParser::GetFirstNode()
			{
				if (m_aNodes.Count())
					return m_aNodes[0];
				return NULL;
			}

			unsigned int IXMLParser::GetChildCount()
			{
				return m_aNodes.Count();
			}

			IXMLNode* IXMLParser::GetChild(unsigned int i)
			{
				return m_aNodes[i];
			}
		}
	}
}
