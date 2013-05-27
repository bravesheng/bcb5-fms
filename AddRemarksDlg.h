//----------------------------------------------------------------------------
#ifndef AddRemarksDlgH
#define AddRemarksDlgH
//----------------------------------------------------------------------------
#include <vcl\System.hpp>
#include <vcl\Windows.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>

#include "RemarkPlace.h"
#include <Db.hpp>
#include <DBCtrls.hpp>
#include <DBTables.hpp>
//----------------------------------------------------------------------------
class TDlg_AddRemarks : public TForm
{
__published:        
	TButton *OKBtn;
	TButton *CancelBtn;
	TEdit *Edit_Name;
	TLabel *Label1;
	TEdit *Edit_HomePage;
	TLabel *Label2;
	TEdit *Edit_Phone;
	TLabel *Label3;
	TEdit *Edit_Addr;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TImage *Image_RemarksImg;
	TOpenPictureDialog *OpenPictureDialog_Remarks;
	TLabel *Label7;
	TEdit *Edit_Email;
	TMemo *Memo_Remarks;
        TLabel *Label8;
        TComboBox *ComboBox_Company;
        TTable *Table_tCustInfo;
        TComboBox *ComboBox_Class;
        TLabel *Label9;
        TSpeedButton *SpeedButton_Image;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall OKBtnClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
        void __fastcall ComboBox_CompanyEnter(TObject *Sender);
        void __fastcall ComboBox_CompanyChange(TObject *Sender);
        void __fastcall Edit_NameChange(TObject *Sender);
        void __fastcall SpeedButton_ImageClick(TObject *Sender);
private:
        AnsiString CoID;
public:
	virtual __fastcall TDlg_AddRemarks(TComponent* AOwner);
    void __fastcall OnExecShell(TObject *Sender);
    AnsiString m_strImagePath;
    //for Chage or Show Old Remarks Object
    CRemarkPlace* m_pOldRemarks;
    //for Add New Node
	TWinControl* m_Remark_Owner;
    long m_Remark_tmX;
    long m_Remark_tmY;
};
//----------------------------------------------------------------------------
extern PACKAGE TDlg_AddRemarks *Dlg_AddRemarks;
//----------------------------------------------------------------------------
#endif    
