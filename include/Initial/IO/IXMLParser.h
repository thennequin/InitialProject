
#ifndef _IXMLPARSER_HEADER_
#define _IXMLPARSER_HEADER_

#include "Initial/Core/IString.h"
#include "Initial/Core/IArray.h"
#include "Initial/IO/IFileStream.h"

namespace Initial
{
	namespace IO
	{
		namespace XML
		{
			class IXMLValue
			{
				friend class IXMLNode;
				friend class IXMLParser;
			public:
				Core::IString GetName();
				Core::IString GetValue();
			protected:
				IXMLValue(Core::IString name="", Core::IString value="");
				IXMLValue(IXMLValue& copy);
			protected:
				Core::IString m_sName;
				Core::IString m_sValue;
			};

			class IXMLNode
			{
				friend class IXMLParser;
			public:
				~IXMLNode();

				Core::IString GetName();
				unsigned int GetChildCount();
				unsigned int GetValueCount();
				IXMLNode* GetChild(unsigned int pos=0);
				IXMLValue* GetValue(unsigned int pos);
				Core::IString GetValue(Core::IString name,Core::IString def="");

				IXMLNode* GetParent();
				IXMLNode* GetNext();

				Core::IString print(int level=0);
			protected:
				IXMLNode(IXMLNode* parent);
				IXMLNode(IXMLNode& copy);
			protected:
				IXMLNode* m_pParent;
				Core::IString m_sName;

				Core::IArray<IXMLNode*> m_aNodes;
				Core::IArray<IXMLValue*> m_aValues;

				Core::IString m_sBody;
			};

			class IXMLParser
			{
			public:
				IXMLParser(Core::IString filename="");
				~IXMLParser();

				bool Parse();
				bool Parse(Core::IString filename);

				bool Save(Core::IString filename);

				IXMLNode* GetFirstNode();

				unsigned int GetChildCount();
				IXMLNode* GetChild(unsigned int pos);
			protected:
				void WriteNode(IFileStream& file, IXMLNode *node, int level);
			protected:
				Core::IString m_sFilename;
				Core::IArray<IXMLNode*> m_aNodes;

				enum
				{
					STATE_START,
					STATE_XML_NODE,
					//------------
					STATE_NONE,
					STATE_COMMENT,
					STATE_NODE_NAME,
					STATE_END_NODE_NAME,
					STATE_NODE,
					STATE_VALUE_NAME,
					STATE_VALUE_START,
					STATE_VALUE_DOUBLE_QUOTE,
					STATE_VALUE_SIMPLE_QUOTE,
					STATE_VALUE,
				};
			};
		}
	}
}

#endif