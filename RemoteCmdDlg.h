//----------------------------------------------------------------------------
#ifndef RemoteCmdDlgH
#define RemoteCmdDlgH
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
#include <ComCtrls.hpp>
#include <Mask.hpp>

#include "Common.h"
//----------------------------------------------------------------------------
class TDlg_RemoteCmd : public TForm
{
__published:        
	TButton *OKBtn;
	TButton *CancelBtn;
	TBevel *Bevel1;
	TListView *ListView_CarsInfo;
	TGroupBox *GroupBox_ChgPwd;
	TPanel *Panel_SetTimer;
	TMaskEdit *MaskEdit_ReportTimer_Min;
	TMaskEdit *MaskEdit_ReportTimer_Sec;
	TLabel *Label2;
	TLabel *Label1;
	TLabel *Label_ReportTimer;
	TMaskEdit *MaskEdit_NewPwd;
	TLabel *Label4;
	TGroupBox *GroupBox_Cmd;
	TLabel *Label6;
	TComboBox *ComboBox_CmdType;
	TLabel *Label7;
	TEdit *Edit_Option;
	TRadioButton *RadioButton_Enable;
	TRadioButton *RadioButton_Disable;
	TLabel *Label_Option;
        TLabel *Label8;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall CancelBtnClick(TObject *Sender);
	void __fastcall OKBtnClick(TObject *Sender);
	void __fastcall ComboBox_CmdTypeChange(TObject *Sender);
private:
public:
	TList *m_CarList;
        int	m_DlgCmdType;     	//-1:取消回報 , 0:定時回報 , 1:立即回報 , 2:更改車機密碼 ,3:更改控制中心號碼 , 4:其他命令,5:設定定時回報頻率
	virtual __fastcall TDlg_RemoteCmd(TComponent* AOwner);
        void __fastcall AddCarsToList_RemoteCmdDlg();
        void __fastcall LayoutDlgCmdType_N1();
        void __fastcall LayoutDlgCmdType_0();
        void __fastcall LayoutDlgCmdType_5();
        void __fastcall LayoutDlgCmdType_1();
        void __fastcall LayoutDlgCmdType_2();
        void __fastcall LayoutDlgCmdType_3();
        void __fastcall LayoutDlgCmdType_4();
        void __fastcall HandleDlgCmdType_N1();
        void __fastcall HandleDlgCmdType_0();
        void __fastcall HandleDlgCmdType_5();
        void __fastcall HandleDlgCmdType_1();
        void __fastcall HandleDlgCmdType_2();
        void __fastcall HandleDlgCmdType_3();
        void __fastcall HandleDlgCmdType_4();
        void __fastcall HandleDlgCmdType_Common(int CmdID,char* strOption);

};
//----------------------------------------------------------------------------
extern PACKAGE TDlg_RemoteCmd *Dlg_RemoteCmd;
//----------------------------------------------------------------------------
#endif
