
#ifdef WIN32
#include <windows.h>
#include "wx/wx.h"
#endif

#include "Initial/Core/IAssert.h"
#include "Initial/ILogger.h"

#define SIMPLE_ASSERT 0

namespace Initial
{
	namespace Core
	{
#ifdef WIN32
		//&& defined(WXWIDGET)
		class AssertDialog : public wxDialog
		{
			enum
			{
				BT_BREAK,
				BT_IGNORE,
				BT_ALWAYS,
				BT_ALL,
				BT_EXIT,
			};

			DECLARE_EVENT_TABLE()
		public:
			AssertDialog(IString assert)
			{
				Create(NULL,wxID_ANY,"Assert");
				m_pBreak = new wxButton(this,BT_BREAK,"Break",wxPoint(5,5),wxSize(120,20),wxSIMPLE_BORDER);
				if (!IsDebuggerPresent())
					m_pBreak->Enable(false);
				m_pIgnore = new wxButton(this,BT_IGNORE,"Ignore",wxPoint(125,5),wxSize(120,20),wxSIMPLE_BORDER);
				m_pAllwaysIgnore = new wxButton(this,BT_ALWAYS,"Always ignore",wxPoint(245,5),wxSize(120,20));
				m_pIgnoreAllAssert = new wxButton(this,BT_ALL,"Ignore all assert",wxPoint(365,5),wxSize(120,20));
				m_pExit = new wxButton(this,BT_EXIT,"exit",wxPoint(490,5),wxSize(60,20));

				m_pText = new wxStaticText(this,wxID_ANY,assert.c_str(),wxPoint(10,30),wxSize(420,150));

				SetSize(555,200);
			};

			~AssertDialog()
			{
				if (m_pBreak)
					delete m_pBreak;
				if (m_pIgnore)
					delete m_pIgnore;
				if (m_pAllwaysIgnore)
					delete m_pAllwaysIgnore;
				if (m_pIgnoreAllAssert)
					delete m_pIgnoreAllAssert;
				if (m_pExit)
					delete m_pExit;
				if (m_pText)
					delete m_pText;
			}

			void OnClose(wxCloseEvent& event)
			{
				EndModal(0);
			}

			void OnButton(wxCommandEvent& event)
			{
				switch (event.GetId())
				{
				case BT_BREAK:
					EndModal(4);
					break;
				case BT_IGNORE:
					EndModal(0);
					break;
				case BT_ALWAYS:
					EndModal(1);
					break;
				case BT_ALL:
					EndModal(2);
					break;
				case BT_EXIT:
					EndModal(3);
					break;
				default:
					EndModal(0);
				}
			}

		protected:	
			wxButton *m_pBreak;
			wxButton *m_pIgnore;
			wxButton *m_pAllwaysIgnore;
			wxButton *m_pIgnoreAllAssert;
			wxButton *m_pExit;
			wxStaticText *m_pText;
		};

		BEGIN_EVENT_TABLE(AssertDialog,wxDialog)	
			EVT_CLOSE(AssertDialog::OnClose)
			EVT_BUTTON(wxID_ANY,AssertDialog::OnButton)
		END_EVENT_TABLE()


		struct AssertIgnore
		{
			AssertIgnore(int Line, IString File)
			{
				line=Line;
				file=File;
			}

			int line;
			IString file;
		};

		WX_DEFINE_ARRAY_PTR(AssertIgnore*,wxArrayAssert);

		bool IgnoreAllAssert=false;
		wxArrayAssert IgnredAssert;

		void myassert(int line,IString file, IString message, IString message2)
		{
			if (IgnoreAllAssert)
				return;

			IString str;
			str.Printf("%d : %s\n	%s\n	%s",line,file.c_str(),message.c_str(),message2.c_str());
	#if SIMPLE_ASSERT
			wxMessageDialog *dialog = new wxMessageDialog(NULL,str.c_str(),"Assert");
			dialog->ShowModal();
			delete dialog;
	#else
			bool Ignored=false;
			for (unsigned long i=0;i<IgnredAssert.Count();i++)
				if (IgnredAssert[i] && IgnredAssert[i]->line==line && IgnredAssert[i]->file==file)
				{
					Ignored=true;
					break;
				}

			if (Ignored==false)
			{
				if (IsDebuggerPresent())
					OutputDebugString((str+"\n").c_str());
				if (IsDebuggerPresent())
					DebugBreak();
				AssertDialog *dial = new AssertDialog(str);
				int res = dial->ShowModal();
				delete dial;
				switch (res)
				{
				case 0:
					//On l'ignore
					break;
				case 1:
					//On l'ignore toujours
					IgnredAssert.Add(new AssertIgnore(line,file));
					break;
				case 2:
					// On ignore tout
					IgnoreAllAssert=true;
					break;
				case 3:
					exit(1);
					break;
				case 4:
					if (IsDebuggerPresent())
						DebugBreak();
					break;
				}
				//__asm int 3; // On déclenche un break point dans Visual Studio
			}
	#endif
		}
#else
		void myassert(int line,IString file, IString message, IString message2)
		{
			IString str;
			str.Printf("%d : %s\n	%s\n	%s",line,file.c_str(),message.c_str(),message2.c_str());
			ILogger::LogError("%s\n",str.c_str());
		}	
#endif
	}
}