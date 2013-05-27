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
////-1:�����^�� , 0:�w�ɦ^�� , 1:�ߧY�^�� , 2:��郞���K�X ,3:��ﱱ��߸��X , 4:��L�R�O, 5:�]�w�^���ˬd�W�v
//Change Layout for different CmdType
        Width = 348;
        switch(m_DlgCmdType)
        {
    	        case -1:LayoutDlgCmdType_N1();	break;	//-1:�����^��
    	        case 0:	LayoutDlgCmdType_0();	break;	//0:�w�ɦ^��
                case 1:	LayoutDlgCmdType_1();	break;  //1:�ߧY�^��
                case 2:	LayoutDlgCmdType_2();	break;  //2:��郞���K�X
                case 3:	LayoutDlgCmdType_3();	break;  //3:��ﱱ��߸��X
                case 4:	LayoutDlgCmdType_4();	break;  //4:��L�R�O
                case 5: LayoutDlgCmdType_5();   break;  //5:�]�w�^���ˬd�W�v
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
        //�����^��
        Dlg_RemoteCmd->Caption = "�����^��";
        Panel_SetTimer->Visible = true;
	MaskEdit_ReportTimer_Min->Visible = false;
        MaskEdit_ReportTimer_Sec->Visible = false;
        Label1->Visible = false;
        Label2->Visible = false;
        Caption = "�ߧY�ǰe�T�{";
        Label_ReportTimer->Width = 300;
        Label_ReportTimer->Caption = "�A�T�w�n�ǰe�����w�ɦ^��(�n�D�ɤ~�^��)�T����?";
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
{//-1:�����^�� , 0:�w�ɦ^�� , 1:�ߧY�^�� , 2:��郞���K�X ,3:��ﱱ��߸��X , 4:��L�R�O
	//�����^��
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
                        strcpy(Cmd.m_szOptions,"0");	//�����^��
                        strcpy(Cmd.m_szPassword,pCar->GetLocatorPwd());
                        Cmd.m_iCmdID = G_GetCmdID();
                        AnsiString tmpstr = Form_Login->LoginID;
                        strcpy(Cmd.m_szSender,tmpstr.c_str());
                        DataModule1->SendCmdToCar(Cmd, pCar);
//�������Database                        DataModule1->Update_DB_Value("tLatestStatus","ReportRate",0,"LocatorNo",(AnsiString)Cmd.m_szLocatorNo);
                        memset(&Cmd,0,sizeof(sCmdMsg));         //RESET WHEN FREE
                }
        }//for
}

//---------------------------------------------------------------------------

void __fastcall TDlg_RemoteCmd::LayoutDlgCmdType_0()
{
	//�w�ɦ^��
        Dlg_RemoteCmd->Caption = "�w�ɦ^��";
        Panel_SetTimer->Visible = true;
        AddCarsToList_RemoteCmdDlg();
}
//---------------------------------------------------------------------------
void __fastcall TDlg_RemoteCmd::LayoutDlgCmdType_5()
{
	//�w�ɦ^���ˬd�W�v
        Dlg_RemoteCmd->Caption = "�]�w�w�ɦ^���ˬd�W�v";
        Panel_SetTimer->Visible = true;
        AddCarsToList_RemoteCmdDlg();
        Label1->Caption = "(�p�� / ��)";
        Label2->Caption = "(�� / ��)";
        Label7->Caption = "�]�w�Ȭ�(0)�Y��ܨ����ʱ�";
        Label8->Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TDlg_RemoteCmd::HandleDlgCmdType_0()
{//-1:�����^�� , 0:�w�ɦ^�� , 1:�ߧY�^�� , 2:��郞���K�X ,3:��ﱱ��߸��X , 4:��L�R�O
	//�w�ɦ^��
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
                        strcpy(Cmd.m_szOptions,strTimer.c_str());	//�w�ɦ^��
                        strcpy(Cmd.m_szPassword,pCar->GetLocatorPwd());
                        Cmd.m_iCmdID = G_GetCmdID();
                        AnsiString tmpstr = Form_Login->LoginID;
                        strcpy(Cmd.m_szSender,tmpstr.c_str());
                        DataModule1->SendCmdToCar(Cmd, pCar);
//�������Database                        DataModule1->Update_DB_Value("tLatestStatus","ReportRate",timer,"LocatorNo",(AnsiString)Cmd.m_szLocatorNo);
                        memset(&Cmd,0,sizeof(sCmdMsg));         //RESET WHEN FREE
                }
        }
}
//---------------------------------------------------------------------------
void __fastcall TDlg_RemoteCmd::HandleDlgCmdType_5()
{//-1:�����^�� , 0:�w�ɦ^�� , 1:�ߧY�^�� , 2:��郞���K�X ,3:��ﱱ��߸��X , 4:��L�R�O, 5:�w�ɦ^���ˬd�W�v
	//�w�ɦ^���ˬd�W�v
        CCar* pCar=NULL;
        //�b�o��MaskEdit_ReportTimer_Min���p��,MaskEdit_ReportTimer_Sec��쬰����
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
	//�ߧY�^��
        Dlg_RemoteCmd->Caption = "�ߧY�^��";
        Panel_SetTimer->Visible = true;
	MaskEdit_ReportTimer_Min->Visible = false;
        MaskEdit_ReportTimer_Sec->Visible = false;
        Label1->Visible = false;
        Label2->Visible = false;
        Caption = "�ߧY�ǰe�T�{";
        Label_ReportTimer->Width = 300;
        Label_ReportTimer->Caption = "�A�T�w�n�ǰe�ߧY�^�����p�T����?";
        //Add Cars to ListView
        AddCarsToList_RemoteCmdDlg();
}
//---------------------------------------------------------------------------
void __fastcall TDlg_RemoteCmd::HandleDlgCmdType_1()
{//-1:�����^�� , 0:�w�ɦ^�� , 1:�ߧY�^�� , 2:��郞���K�X ,3:��ﱱ��߸��X , 4:��L�R�O
	//�ߧY�^��
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
                        strcpy(Cmd.m_szOptions,"1");	//�ߧY�^��
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
	//��郞���K�X
        Dlg_RemoteCmd->Caption = "��郞���K�X";
        GroupBox_ChgPwd->Left = 11;
        GroupBox_ChgPwd->Top = 160;
        GroupBox_ChgPwd->Visible = true;
        //Add Cars to ListView
        AddCarsToList_RemoteCmdDlg();
}
//---------------------------------------------------------------------------
void __fastcall TDlg_RemoteCmd::HandleDlgCmdType_2()
{//-1:�����^�� , 0:�w�ɦ^�� , 1:�ߧY�^�� , 2:��郞���K�X ,3:��ﱱ��߸��X , 4:��L�R�O
	//��郞���K�X
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
                        strcpy(Cmd.m_szOptions,MaskEdit_NewPwd->Text.c_str());	//��郞���K�X
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
        //��ﱱ��߸��X
        Dlg_RemoteCmd->Caption = "��ﱱ��߸��X";
        //Add Cars to ListView
        AddCarsToList_RemoteCmdDlg();
}
//---------------------------------------------------------------------------
void __fastcall TDlg_RemoteCmd::HandleDlgCmdType_3()
{//-1:�����^�� , 0:�w�ɦ^�� , 1:�ߧY�^�� , 2:��郞���K�X ,3:��ﱱ��߸��X , 4:��L�R�O
	//��ﱱ��߸��X

}
//---------------------------------------------------------------------------
void __fastcall TDlg_RemoteCmd::LayoutDlgCmdType_4()
{
	//��L�R�O
        Dlg_RemoteCmd->Caption = "��L�R�O";
        GroupBox_Cmd->Left = 11;
        GroupBox_Cmd->Top = 160;
        GroupBox_Cmd->Visible = true;
        ComboBox_CmdType->ItemIndex = 0;
        //set Init Layerout
        Label_Option->Visible = true;
        Label_Option->Caption = "����߸��X";
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
{//-1:�����^�� , 0:�w�ɦ^�� , 1:�ߧY�^�� , 2:��郞���K�X ,3:��ﱱ��߸��X , 4:��L�R�O
	//��L�R�O

/*Command No
0:�]�w����߸��X   -> 0
1:�n�D�^���Ѽ�       -> 3
2:�]�w���{��         -> 100
3:�]�w�ե��Ѽ�       -> 101
4:�]�w�۰ʮե�       -> 102
5:�]�wOutput         -> 103
6:���� Alarm�^��     -> 104
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
        {//-1:�����^�� , 0:�w�ɦ^�� , 1:�ߧY�^�� , 2:��郞���K�X ,3:��ﱱ��߸��X , 4:��L�R�O, 5:�w�ɦ^���ˬd�W�v
    	        case -1:HandleDlgCmdType_N1();	break;	//-1:�����^��
    	        case 0:	HandleDlgCmdType_0();	break;	//0:�w�ɦ^��
                case 1:	HandleDlgCmdType_1();	break;  //1:�ߧY�^��
                case 2:	HandleDlgCmdType_2();	break;  //2:��郞���K�X
                case 3:	HandleDlgCmdType_3();	break;  //3:��ﱱ��߸��X
                case 4:	HandleDlgCmdType_4();	break;  //4:��L�R�O
                case 5: HandleDlgCmdType_5();   break;  //5:�w�ɦ^���ˬd�W�v
                default:break;
        }
        Close();
}
//---------------------------------------------------------------------------

void __fastcall TDlg_RemoteCmd::ComboBox_CmdTypeChange(TObject *Sender)
{
/*                   Command No
0:�]�w����߸��X   -> 0
1:�n�D�^���Ѽ�       -> 3
2:�]�w���{��         -> 100
3:�]�w�ե��Ѽ�       -> 101
4:�]�w�۰ʮե�       -> 102
5:�]�wOutput         -> 103
6:���� Alarm�^��     -> 104
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
    		        Label_Option->Caption = "����߸��X";
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
    		        Label_Option->Caption = "���{��";
                        Edit_Option->Visible = true;
			Edit_Option ->Text = "1000";
                        RadioButton_Enable->Visible = false;
                        RadioButton_Disable->Visible = false;
                        break;
                case 3: Label_Option->Visible = true;
    		        Label_Option->Caption = "�ե��Ѽ�";
                        Edit_Option->Visible = true;
			Edit_Option ->Text = "0";
                        RadioButton_Enable->Visible = false;
                        RadioButton_Disable->Visible = false;
                        break;
                case 4: Label_Option->Visible = true;
    	        	Label_Option->Caption = "�۰ʮե�";
                        Edit_Option->Visible = false;
                        RadioButton_Enable->Visible = true;
                        RadioButton_Enable->Checked = true;
                        RadioButton_Disable->Visible = true;
                        break;
                case 5: Label_Option->Visible = true;
    		        Label_Option->Caption = "�]�wOutput";
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

