//---------------------------------------------------------------------------

#ifndef RouteEditorFormH
#define RouteEditorFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Mask.hpp>
#include "SelectPath.h"
//---------------------------------------------------------------------------
class TForm_RouteEditor : public TForm
{
__published:	// IDE-managed Components
        TBitBtn *BitBtnOk;
        TBitBtn *BitBtnCancel;
        TEdit *Edit_RouteNo;
        TEdit *Edit_StartName;
        TEdit *Edit3;
        TEdit *Edit4;
        TEdit *Edit_EndName;
        TEdit *Edit_RouteName;
        TEdit *Edit7;
        TEdit *Edit8;
        TMaskEdit *Edit_Radius;
        TEdit *Edit1;
        TComboBox *ComboBox_Company;
        void __fastcall BitBtnOkClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall BitBtnCancelClick(TObject *Sender);
        void __fastcall Edit_RouteNameChange(TObject *Sender);
        void __fastcall Edit_StartNameChange(TObject *Sender);
        void __fastcall Edit_EndNameChange(TObject *Sender);
        void __fastcall Edit_RadiusChange(TObject *Sender);
        void __fastcall ComboBox_CompanyChange(TObject *Sender);
private:	// User declarations

public:		// User declarations
       TCSelectPath *pPath;
       int RouteNo;
       __fastcall TForm_RouteEditor(TComponent* Owner);
       void __fastcall Get_Value_EditMode();           //修改模式讀取資料
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_RouteEditor *Form_RouteEditor;
//---------------------------------------------------------------------------
#endif
