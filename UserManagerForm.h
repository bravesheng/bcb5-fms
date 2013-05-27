//---------------------------------------------------------------------------

#ifndef UserManagerFormH
#define UserManagerFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TForm_UserManager : public TForm
{
__published:	// IDE-managed Components
        TTreeView *TreeView1;
        TListView *ListView1;
        TPopupMenu *Popup_UserSetup;
        TMenuItem *SetPasswd;
        TPopupMenu *Popup_AddUser;
        TMenuItem *AddUser;
        TMenuItem *N1;
        void __fastcall TreeView1Click(TObject *Sender);
        void __fastcall SetPasswdClick(TObject *Sender);
        void __fastcall AddUserClick(TObject *Sender);
        void __fastcall N1Click(TObject *Sender);
        void __fastcall Popup_UserSetupPopup(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm_UserManager(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_UserManager *Form_UserManager;
//---------------------------------------------------------------------------
#endif
