//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "RemoteCmdDlg.h"
#include "MainForm.h"
#include "LoginForm.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TDlg_RemoteCmd *Dlg_RemoteCmd;
//---------------------------------------------------------------------
__fastcall TDlg_RemoteCmd::TDlg_RemoteCmd(TComponent* AOwner)
	: TForm(AOwner)
{
        m_CarList = new TList();
        m_DlgCmdType = 0;
}
//---------------------------------------------------------------------
void __fastcall TDlg_RemoteCmd::FormClose(TObject *Sender,
      TCloseAction &Action)
{   //free Dlg_TimerReport memory
	Action = caFree;
        delete m_CarList;
}
//---------------------------------------------------------------------------
void __fastcall TDlg_RemoteCmd::FormShow(TObject *Sender)
{
////-1:取消回報 , 0:定時回報 , 1:立即回報 , 2:更改車機密碼 ,3:更改控制中心號碼 , 4:其他命令, 5:設定回報檢查頻率
//Change Layout for different CmdType
        Width = 348;
        switch(m_DlgCmdType)
        {
    	        case -1:LayoutDlgCmdType_N1();	break;	//-1:取消回報
    	        case 0:	LayoutDlgCmdType_0();	break;	//0:定時回報
                case 1:	LayoutDlgCmdType_1();	break;  //1:立即回報
                case 2:	LayoutDlgCmdType_2();	break;  //2:更改車機密碼
                case 3:	LayoutDlgCmdType_3();	break;  //3:更改控制中心號碼
                case 4:	LayoutDlgCmdType_4();	break;  //4:其他命令
                case 5: LayoutDlgCmdType_5();   break;  //5:設定回報檢查頻率
                default:Close(); break;
        }
}

//---------------------------------------------------------------------------
void __fastcall TDlg_RemoteCmd::AddCarsToList_RemoteCmdDlg()
{
        TListItem *pItem;
        CCar* ptrCar=NULL;
	for(int i=0;i<m_CarList->Count;i++)
        {//Show Text CarID , LocatorNo , Old ReportTimer
    	        ptrCar = (CCar*)( m_CarList->Items[i]);
                if(ptrCar)
                {
			//Add new record
			pItem = ListView_CarsInfo->Items->Add();
			pItem->ImageIndex = 5;
			pItem->Caption = AnsiString(i+1);             			// No SubItem
			pItem->SubItems->Add(AnsiString(ptrCar->GetCarID()));    		//SubItems->Strings[0]
			pItem->SubItems->Add(AnsiString(ptrCar->GetLocatorNo()));		//SubItems->Strings[1]
                }
	}
}
//---------------------------------------------------------------------------
void __fastcall TDlg_RemoteCmd::LayoutDlgCmdType_N1()
{
        //取消回報
        Dlg_RemoteCmd->Caption = "取消回報";
        Panel_SetTimer->Visible = true;
	MaskEdit_ReportTimer_Min->Visible = false;
        MaskEdit_ReportTimer_Sec->Visible = false;
        Label1->Visible = false;
        Label2->Visible = false;
        Caption = "立即傳送確認";
        Label_ReportTimer->Width = 300;
        Label_ReportTimer->Caption = "你確定要傳送取消定時回報(要求時才回報)訊息嗎?";
        //Add Cars to ListView
        AddCarsToList_RemoteCmdDlg();
}
//---------------------------------------------------------------------------
void __fastcall TDlg_RemoteCmd::HandleDlgCmdType_Common(int CmdID,char* strOption)
{
   	CCar* pCar=NULL;
        for(int i=0;i<m_CarList->Count;i++)
        {
    	        pCar = (CCar*)( m_CarList->Items[i]);
                if(pCar)
                {
			sCmdMsg Cmd;
                        memset(&Cmd,0,sizeof(sCmdMsg));
                        strcpy(Cmd.m_szLocatorNo,pCar->GetLocatorNo());
                        GetNowDateTime(Cmd.m_szDate,Cmd.m_szTime);
                        Cmd.m_iCommand = CmdID;
                        strcpy(Cmd.m_szOptions,strOption);
                        strcpy(Cmd.m_szPassword,pCar->GetLocatorPwd());
                        Cmd.m_iCmdID = G_GetCmdID();
                        AnsiString tmpstr = Form_Login->LoginID;
                        strcpy(Cmd.m_szSender,tmpstr.c_str());
                        DataModule1->SendCmdToCar(Cmd, pCar);
                        memset(&Cmd,0,sizeof(sCmdMsg));         //RESET WHEN FREE
                }
        }//for
}
//---------------------------------------------------------------------------
void __fastcall TDlg_RemoteCmd::HandleDlgCmdType_N1()
{//-1:取消回報 , 0:定時回報 , 1:立即回報 , 2:更改車機密碼 ,3:更改控制中心號碼 , 4:其他命令
	//取消回報
   	CCar* pCar=NULL;
        for(int i=0;i<m_CarList->Count;i++)
        {
    	        pCar = (CCar*)( m_CarList->Items[i]);
                if(pCar)
                {
			sCmdMsg Cmd;
                        memset(&Cmd,0,sizeof(sCmdMsg));
                        strcpy(Cmd.m_szLocatorNo,pCar->GetLocatorNo());
                        GetNowDateTime(Cmd.m_szDate,Cmd.m_szTime);
                        Cmd.m_iCommand = 2;
                        strcpy(Cmd.m_szOptions,"0");	//取消回報
                        strcpy(Cmd.m_szPassword,pCar->GetLocatorPwd());
                        Cmd.m_iCmdID = G_GetCmdID();
                        AnsiString tmpstr = Form_Login->LoginID;
                        strcpy(Cmd.m_szSender,tmpstr.c_str());
                        DataModule1->SendCmdToCar(Cmd, pCar);
//取消更改Database                        DataModule1->Update_DB_Value("tLatestStatus","ReportRate",0,"LocatorNo",(AnsiString)Cmd.m_szLocatorNo);
                        memset(&Cmd,0,sizeof(sCmdMsg));         //RESET WHEN FREE
                }
        }//for
}

//---------------------------------------------------------------------------

void __fastcall TDlg_RemoteCmd::LayoutDlgCmdType_0()
{
	//定時回報
        Dlg_RemoteCmd->Caption = "定時回報";
        Panel_SetTimer->Visible = true;
        AddCarsToList_RemoteCmdDlg();
}
//---------------------------------------------------------------------------
void __fastcall TDlg_RemoteCmd::LayoutDlgCmdType_5()
{
	//定時回報檢查頻率
        Dlg_RemoteCmd->Caption = "設定定時回報檢查頻率";
        Panel_SetTimer->Visible = true;
        AddCarsToList_RemoteCmdDlg();
        Label1->Caption = "(小時 / 次)";
        Label2->Caption = "(分 / 次)";
        Label7->Caption = "設定值為(0)即表示取消監控";
        Label8->Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TDlg_RemoteCmd::HandleDlgCmdType_0()
{//-1:取消回報 , 0:定時回報 , 1:立即回報 , 2:更改車機密碼 ,3:更改控制中心號碼 , 4:其他命令
	//定時回報
        CCar* pCar=NULL;
        long timer = ((atol(MaskEdit_ReportTimer_Min->Text.c_str()))*60+(atol(MaskEdit_ReportTimer_Sec->Text.c_str())));
	AnsiString strTimer(timer);
	for(int i=0;i<m_CarList->Count;i++)
        {   //Show Text CarID
    	        pCar = (CCar*)( m_CarList->Items[i]);
                if(pCar)
                {
			sCmdMsg Cmd;
                        memset(&Cmd,0,sizeof(sCmdMsg));
                        strcpy(Cmd.m_szLocatorNo,pCar->GetLocatorNo());
                        GetNowDateTime(Cmd.m_szDate,Cmd.m_szTime);
                        Cmd.m_iCommand = 2;
                        strcpy(Cmd.m_szOptions,strTimer.c_str());	//定時回報
                        strcpy(Cmd.m_szPassword,pCar->GetLocatorPwd());
                        Cmd.m_iCmdID = G_GetCmdID();
                        AnsiString tmpstr = Form_Login->LoginID;
                        strcpy(Cmd.m_szSender,tmpstr.c_str());
                        DataModule1->SendCmdToCar(Cmd, pCar);
//取消更改Database                        DataModule1->Update_DB_Value("tLatestStatus","ReportRate",timer,"LocatorNo",(AnsiString)Cmd.m_szLocatorNo);
                        memset(&Cmd,0,sizeof(sCmdMsg));         //RESET WHEN FREE
                }
        }
}
//---------------------------------------------------------------------------
void __fastcall TDlg_RemoteCmd::HandleDlgCmdType_5()
{//-1:取消回報 , 0:定時回報 , 1:立即回報 , 2:更改車機密碼 ,3:更改控制中心號碼 , 4:其他命令, 5:定時回報檢查頻率
	//定時回報檢查頻率
        CCar* pCar=NULL;
        //在這裡MaskEdit_ReportTimer_Min為小時,MaskEdit_ReportTimer_Sec單位為分鐘
        long timer = ((atol(MaskEdit_ReportTimer_Min->Text.c_str()))*3600+(atol(MaskEdit_ReportTimer_Sec->Text.c_str()))*60);
	for(int i=0;i<m_CarList->Count;i++)
        {   //Show Text CarID
    	        pCar = (CCar*)( m_CarList->Items[i]);
                if(pCar)
                {
                        TQuery* pQ_Update = new TQuery(DataModule1);      //free in last line
                        pQ_Update->DatabaseName = "DB_DCH";
                        pQ_Update->SQL->Add("UPDATE tLatestStatus");
                        pQ_Update->SQL->Add("SET ReportChkRate = :TIMER");
                        pQ_Update->SQL->Add("WHERE LocatorNo = :LocatorNo");
                        pQ_Update->ParamByName("TIMER")->AsFloat = timer;
                        pQ_Update->ParamByName("LocatorNo")->AsString = pCar->GetLocatorNo();
                        try
                        {
                                pQ_Update->ExecSQL();
                        }
                        catch(...)
                        {
                        }
                        delete pQ_Update;
                }
        }
}
//---------------------------------------------------------------------------
void __fastcall TDlg_RemoteCmd::LayoutDlgCmdType_1()
{
	//立即回報
        Dlg_RemoteCmd->Caption = "立即回報";
        Panel_SetTimer->Visible = true;
	MaskEdit_ReportTimer_Min->Visible = false;
        MaskEdit_ReportTimer_Sec->Visible = false;
        Label1->Visible = false;
        Label2->Visible = false;
        Caption = "立即傳送確認";
        Label_ReportTimer->Width = 300;
        Label_ReportTimer->Caption = "你確定要傳送立即回報車況訊息嗎?";
        //Add Cars to ListView
        AddCarsToList_RemoteCmdDlg();
}
//---------------------------------------------------------------------------
void __fastcall TDlg_RemoteCmd::HandleDlgCmdType_1()
{//-1:取消回報 , 0:定時回報 , 1:立即回報 , 2:更改車機密碼 ,3:更改控制中心號碼 , 4:其他命令
	//立即回報
   	CCar* pCar=NULL;
        for(int i=0;i<m_CarList->Count;i++)
        {
    	        pCar = (CCar*)( m_CarList->Items[i]);
                if(pCar)
                {
			sCmdMsg Cmd;
                        memset(&Cmd,0,sizeof(sCmdMsg));
                        strcpy(Cmd.m_szLocatorNo,pCar->GetLocatorNo());
                        GetNowDateTime(Cmd.m_szDate,Cmd.m_szTime);
                        Cmd.m_iCommand = 2;
                        strcpy(Cmd.m_szOptions,"1");	//立即回報
                        strcpy(Cmd.m_szPassword,pCar->GetLocatorPwd());
                        Cmd.m_iCmdID = G_GetCmdID();
                        AnsiString tmpstr = Form_Login->LoginID;
                        strcpy(Cmd.m_szSender,tmpstr.c_str());
                        DataModule1->SendCmdToCar(Cmd, pCar);
                        memset(&Cmd,0,sizeof(sCmdMsg));         //RESET WHEN FREE
                }
        }//for
}

//---------------------------------------------------------------------------
void __fastcall TDlg_RemoteCmd::LayoutDlgCmdType_2()
{
	//更改車機密碼
        Dlg_RemoteCmd->Caption = "更改車機密碼";
        GroupBox_ChgPwd->Left = 11;
        GroupBox_ChgPwd->Top = 160;
        GroupBox_ChgPwd->Visible = true;
        //Add Cars to ListView
        AddCarsToList_RemoteCmdDlg();
}
//---------------------------------------------------------------------------
void __fastcall TDlg_RemoteCmd::HandleDlgCmdType_2()
{//-1:取消回報 , 0:定時回報 , 1:立即回報 , 2:更改車機密碼 ,3:更改控制中心號碼 , 4:其他命令
	//更改車機密碼
   	CCar* pCar=NULL;
        for(int i=0;i<m_CarList->Count;i++)
        {
    	        pCar = (CCar*)( m_CarList->Items[i]);
                if(pCar)
                {
			sCmdMsg Cmd;
                        memset(&Cmd,0,sizeof(sCmdMsg));
                        strcpy(Cmd.m_szLocatorNo,pCar->GetLocatorNo());
                        GetNowDateTime(Cmd.m_szDate,Cmd.m_szTime);
                        Cmd.m_iCommand = 1;
                        strcpy(Cmd.m_szOptions,MaskEdit_NewPwd->Text.c_str());	//更改車機密碼
                        strcpy(Cmd.m_szPassword,pCar->GetLocatorPwd());
                        Cmd.m_iCmdID = G_GetCmdID();
                        AnsiString tmpstr = Form_Login->LoginID;
                        strcpy(Cmd.m_szSender,tmpstr.c_str());
                        TQuery *pQuery = new TQuery(DataModule1);
                        pQuery->DatabaseName = "DB_DCH";
                        pQuery->SQL->Add("INSERT INTO tWaitingAck");
                        pQuery->SQL->Add("(LocatorNo, CmdID, CmdType, Password, Sender)");
                        pQuery->SQL->Add("VALUES (:LocatorNo, :CmdID, :CmdType, :Password, :Sender)");
                        pQuery->ParamByName("LocatorNo")->AsString = Cmd.m_szLocatorNo;
                        pQuery->ParamByName("CmdID")->AsInteger = Cmd.m_iCmdID;
                        pQuery->ParamByName("CmdType")->AsInteger = 1;
                        pQuery->ParamByName("Password")->AsString = Cmd.m_szOptions;
                        pQuery->ParamByName("Sender")->AsString = Cmd.m_szSender;
                        pQuery->ExecSQL();
                        delete pQuery;
                        DataModule1->SendCmdToCar(Cmd, pCar);
                        memset(&Cmd,0,sizeof(sCmdMsg));         //RESET WHEN FREE
                }
        }//for
}
//---------------------------------------------------------------------------
void __fastcall TDlg_RemoteCmd::LayoutDlgCmdType_3()
{
        //更改控制中心號碼
        Dlg_RemoteCmd->Caption = "更改控制中心號碼";
        //Add Cars to ListView
        AddCarsToList_RemoteCmdDlg();
}
//---------------------------------------------------------------------------
void __fastcall TDlg_RemoteCmd::HandleDlgCmdType_3()
{//-1:取消回報 , 0:定時回報 , 1:立即回報 , 2:更改車機密碼 ,3:更改控制中心號碼 , 4:其他命令
	//更改控制中心號碼

}
//---------------------------------------------------------------------------
void __fastcall TDlg_RemoteCmd::LayoutDlgCmdType_4()
{
	//其他命令
        Dlg_RemoteCmd->Caption = "其他命令";
        GroupBox_Cmd->Left = 11;
        GroupBox_Cmd->Top = 160;
        GroupBox_Cmd->Visible = true;
        ComboBox_CmdType->ItemIndex = 0;
        //set Init Layerout
        Label_Option->Visible = true;
        Label_Option->Caption = "控制中心號碼";
        Edit_Option->Visible = true;
        TQuery* pQuery = new TQuery(DataModule1);
        pQuery->DatabaseName = "DB_DCH";
        pQuery->SQL->Add("SELECT CenterNo FROM tConnect");
        pQuery->Active = false;
        pQuery->Active = true;
        Edit_Option->Text = pQuery->FieldByName("CenterNo")->AsString;
        pQuery->Active = false;
        delete pQuery;
        RadioButton_Enable->Visible = false;
        RadioButton_Disable->Visible = false;
        //Add Cars to ListView
        AddCarsToList_RemoteCmdDlg();
}
//---------------------------------------------------------------------------

void __fastcall TDlg_RemoteCmd::HandleDlgCmdType_4()
{//-1:取消回報 , 0:定時回報 , 1:立即回報 , 2:更改車機密碼 ,3:更改控制中心號碼 , 4:其他命令
	//其他命令

/*Command No
0:設定控制中心號碼   -> 0
1:要求回報參數       -> 3
2:設定里程數         -> 100
3:設定校正參數       -> 101
4:設定自動校正       -> 102
5:設定Output         -> 103
6:取消 Alarm回報     -> 104
*/
	switch(ComboBox_CmdType->ItemIndex)
        {
                case 0: HandleDlgCmdType_Common(0,Edit_Option ->Text.c_str());
                        break;
                case 1: HandleDlgCmdType_Common(3,"");
                        break;
                case 2: HandleDlgCmdType_Common(100,Edit_Option ->Text.c_str());
                        break;
                case 3: HandleDlgCmdType_Common(101,Edit_Option ->Text.c_str());
                        break;
                case 4: if(RadioButton_Enable->Checked == true)
    			        HandleDlgCmdType_Common(102,"1");
    		        else
				HandleDlgCmdType_Common(102,"0");
                        break;
                case 5: if(RadioButton_Enable->Checked == true)
    			        HandleDlgCmdType_Common(103,"00000001");
    		        else
				HandleDlgCmdType_Common(103,"00000000");
                        break;
                case 6: HandleDlgCmdType_Common(104,"");
                        break;
                default:return;
        }
}
//---------------------------------------------------------------------------

void __fastcall TDlg_RemoteCmd::CancelBtnClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TDlg_RemoteCmd::OKBtnClick(TObject *Sender)
{
        switch(m_DlgCmdType)
        {//-1:取消回報 , 0:定時回報 , 1:立即回報 , 2:更改車機密碼 ,3:更改控制中心號碼 , 4:其他命令, 5:定時回報檢查頻率
    	        case -1:HandleDlgCmdType_N1();	break;	//-1:取消回報
    	        case 0:	HandleDlgCmdType_0();	break;	//0:定時回報
                case 1:	HandleDlgCmdType_1();	break;  //1:立即回報
                case 2:	HandleDlgCmdType_2();	break;  //2:更改車機密碼
                case 3:	HandleDlgCmdType_3();	break;  //3:更改控制中心號碼
                case 4:	HandleDlgCmdType_4();	break;  //4:其他命令
                case 5: HandleDlgCmdType_5();   break;  //5:定時回報檢查頻率
                default:break;
        }
        Close();
}
//---------------------------------------------------------------------------

void __fastcall TDlg_RemoteCmd::ComboBox_CmdTypeChange(TObject *Sender)
{
/*                   Command No
0:設定控制中心號碼   -> 0
1:要求回報參數       -> 3
2:設定里程數         -> 100
3:設定校正參數       -> 101
4:設定自動校正       -> 102
5:設定Output         -> 103
6:取消 Alarm回報     -> 104
*/
        TQuery* pQuery = new TQuery(DataModule1);
        pQuery->DatabaseName = "DB_DCH";
        pQuery->SQL->Add("SELECT CenterNo FROM tCOnnect");
        pQuery->Active = false;
        pQuery->Active = true;
        Edit_Option ->Text = pQuery->FieldByName("CenterNo")->AsString;
        pQuery->Active = false;
        delete pQuery;
	switch(ComboBox_CmdType->ItemIndex)
        {
                case 0: Label_Option->Visible = true;
    		        Label_Option->Caption = "控制中心號碼";
                        Edit_Option->Visible = true;
                        RadioButton_Enable->Visible = false;
                        RadioButton_Disable->Visible = false;
                        break;
                case 1: Label_Option->Visible = false;
                        Edit_Option->Visible = false;
                        RadioButton_Enable->Visible = false;
                        RadioButton_Disable->Visible = false;
                        break;
                case 2: Label_Option->Visible = true;
    		        Label_Option->Caption = "里程數";
                        Edit_Option->Visible = true;
			Edit_Option ->Text = "1000";
                        RadioButton_Enable->Visible = false;
                        RadioButton_Disable->Visible = false;
                        break;
                case 3: Label_Option->Visible = true;
    		        Label_Option->Caption = "校正參數";
                        Edit_Option->Visible = true;
			Edit_Option ->Text = "0";
                        RadioButton_Enable->Visible = false;
                        RadioButton_Disable->Visible = false;
                        break;
                case 4: Label_Option->Visible = true;
    	        	Label_Option->Caption = "自動校正";
                        Edit_Option->Visible = false;
                        RadioButton_Enable->Visible = true;
                        RadioButton_Enable->Checked = true;
                        RadioButton_Disable->Visible = true;
                        break;
                case 5: Label_Option->Visible = true;
    		        Label_Option->Caption = "設定Output";
                        Edit_Option->Visible = false;
                        RadioButton_Enable->Visible = true;
			RadioButton_Enable->Checked = true;
                        RadioButton_Disable->Visible = true;
                        break;
                case 6: Label_Option->Visible = false;
                        Edit_Option->Visible = false;
                        RadioButton_Enable->Visible = false;
                        RadioButton_Disable->Visible = false;
                        break;
                default:Label_Option->Visible = false;
                        Edit_Option->Visible = false;
                        RadioButton_Enable->Visible = false;
                        RadioButton_Disable->Visible = false;
                        break;
        }
}
//---------------------------------------------------------------------------

