//---------------------------------------------------------------------------

#ifndef EstablishFormH
#define EstablishFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include <ScktComp.hpp>
//---------------------------------------------------------------------------
struct CServerInfo
{
        AnsiString SiteName;
        AnsiString SiteIPAddr;
        AnsiString ODBCName;
};
//---------------------------------------------------------------------------
class TForm_Establish_Connect : public TForm
{
__published:	// IDE-managed Components
        TBitBtn *Btn_Ignore;
        TBitBtn *Btn_Retry;
        TLabel *Label_Statu;
        TComboBox *ComboBox_BDE;         
        void __fastcall Btn_IgnoreClick(TObject *Sender);
        void __fastcall Btn_RetryClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall ComboBox_BDEChange(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations

public:		// User declarations
        __fastcall TForm_Establish_Connect(TComponent* Owner);
        bool __fastcall ConnectDB();
        bool __fastcall ConnectNet();
        void __fastcall ChooseDB();
        bool bConnected;
        int ActiveMode;         //�Ұʤ����A, 1:�s�u��Ʈw�_�u  2:�ϥΪ̤�����Ʈw
        TList* ServerList;      //������Ʈw���A�����
        void __fastcall DeleteServerList();     //�R��ServerList
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_Establish_Connect *Form_Establish_Connect;
//---------------------------------------------------------------------------
#endif
