//---------------------------------------------------------------------
#include <vcl.h>
#include <dir.h>
#pragma hdrstop

#include "AddRemarksDlg.h"
#include "common.h"
#include "MainForm.h"
#include "ViewForm.h"

//--------------------------------------------------------------------- 
#pragma resource "*.dfm"

extern	int	 g_RemarksCount;  //declare in common.cpp
extern  int  g_ColumnToSort;
TDlg_AddRemarks *Dlg_AddRemarks;
//---------------------------------------------------------------------
__fastcall TDlg_AddRemarks::TDlg_AddRemarks(TComponent* AOwner)
	: TForm(AOwner)
{   //�`�N! FormCreate �� Constructor ���o��
	m_strImagePath = (REMARKS_DEFAULT_IMG);  	//define in common.h
    m_pOldRemarks = NULL;
    CoID = "Common";
}
//---------------------------------------------------------------------

void __fastcall TDlg_AddRemarks::FormShow(TObject *Sender)
{
	CallCenter_Main_Form->ChangeImagePic(Image_RemarksImg,m_strImagePath.c_str());
        if(ComboBox_Class->ItemIndex == -1)      //���p�S�����w����,�Y�s�W�Ƶ��a�I�~���|���w
        {
                ComboBox_Class->ItemIndex = 0;
        }
}
//---------------------------------------------------------------------------

void __fastcall TDlg_AddRemarks::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	Action = caFree;	
}
//---------------------------------------------------------------------------

void __fastcall TDlg_AddRemarks::OKBtnClick(TObject *Sender)
{
        //�s�W�Ƶ��a�I
        if( (!m_pOldRemarks)&&(OKBtn->Caption == "�s�W") )
        {   //�Y������Ƨ� g_ColumnToSort != 0 , �s�W�� ListView�ɥ����]�� 0
                g_ColumnToSort = 0;   //Sort ListView for �Ƶ��a�I
                //�N�ȼg�JCRemarkPlace����
    	        CRemarkPlace* pRemark = new CRemarkPlace(m_Remark_Owner,m_Remark_tmX,m_Remark_tmY,m_strImagePath.c_str());
                pRemark->m_RemarkType = ComboBox_Class->ItemIndex;     //�Ӧa�I�����O
                strcpy(pRemark->m_strImagePath,m_strImagePath.c_str());
    	        strcpy(pRemark->m_strName,Edit_Name->Text.c_str());
    	        strcpy(pRemark->m_strPhone,Edit_Phone->Text.c_str());
    	        strcpy(pRemark->m_strAddr,Edit_Addr->Text.c_str());
    	        strcpy(pRemark->m_strEmail,Edit_Email->Text.c_str());
    	        strcpy(pRemark->m_strWebAddr,Edit_HomePage->Text.c_str());
    	        strcpy(pRemark->m_strRemarks,Memo_Remarks->Lines->GetText());
                strcpy(pRemark->m_szCoID,CoID.c_str());
                strcpy(pRemark->m_strCompanyName,ComboBox_Company->Text.c_str());
                CallCenter_Main_Form->m_pRemarksList->Add(pRemark);    //Add New to TList
                //Add New Data to ListView_Remarks
                Form_ViewInfo->AddNewListView_Remarks(pRemark,Form_ViewInfo->ListView_Remark,CallCenter_Main_Form->m_pRemarksList->Count);
                //���]�t�ܤ�r
    	        if( (Edit_Name->Text.Length()>0)||(Edit_Phone->Text.Length()>0)||(Edit_Addr->Text.Length()>0)||
                (Edit_Email->Text.Length()>0)||(Edit_HomePage->Text.Length()>0) )
                {
                        pRemark->SetHint();
                }
                DataModule1->Update_Table_Remarks(pRemark);     //�[�J��Ʈw
                //Add Count
                g_RemarksCount++;
        }
        else if( (m_pOldRemarks)&&(OKBtn->Caption == "�ק�") )
        {
                m_pOldRemarks->m_RemarkType = ComboBox_Class->ItemIndex;     //�Ӧa�I�����O
		strcpy(m_pOldRemarks->m_strImagePath,m_strImagePath.c_str());
    	        strcpy(m_pOldRemarks->m_strName,Edit_Name->Text.c_str());
    	        strcpy(m_pOldRemarks->m_strPhone,Edit_Phone->Text.c_str());
    	        strcpy(m_pOldRemarks->m_strAddr,Edit_Addr->Text.c_str());
    	        strcpy(m_pOldRemarks->m_strEmail,Edit_Email->Text.c_str());
    	        strcpy(m_pOldRemarks->m_strWebAddr,Edit_HomePage->Text.c_str());
    	        strcpy(m_pOldRemarks->m_strRemarks,Memo_Remarks->Lines->GetText());
    	        if( (Edit_Name->Text.Length()>0)||(Edit_Phone->Text.Length()>0)||(Edit_Addr->Text.Length()>0)||
                        (Edit_Email->Text.Length()>0)||(Edit_HomePage->Text.Length()>0) )
		{
                        m_pOldRemarks->SetHint();
                        m_pOldRemarks->m_pImage_Remarks->Picture->LoadFromFile(m_strImagePath);
                }
                else
                        m_pOldRemarks->m_pImage_Remarks->ShowHint = false;
                //�ק� ListView_Remarks ���e
                CRemarkPlace* pRemarks = NULL;
                TListView* pListView = Form_ViewInfo->ListView_Remark;
                int TotalCount = pListView->Items->Count;
                for(int i=0;i<TotalCount;i++)
                {
                        //in SubItem [7] is CRemarkPlace Pointer
                        pRemarks = (CRemarkPlace*)atol(pListView->Items->Item[i]->SubItems->Strings[RMK_PTR].c_str());
                        if( (pRemarks)&&(m_pOldRemarks==pRemarks) )
                        {
                                pListView->Items->Item[i]->SubItems->Strings[RMK_NAME]= pRemarks->m_strName;
                                pListView->Items->Item[i]->SubItems->Strings[RMK_CLASS]= pRemarks->GetClassName();
                                pListView->Items->Item[i]->SubItems->Strings[RMK_PHONE]= pRemarks->m_strPhone;
                                pListView->Items->Item[i]->SubItems->Strings[RMK_ADDR]= pRemarks->m_strAddr;
                                pListView->Items->Item[i]->SubItems->Strings[RMK_EMAIL]= pRemarks->m_strEmail;
                                pListView->Items->Item[i]->SubItems->Strings[RMK_HTTP]= pRemarks->m_strWebAddr;
                                pListView->Items->Item[i]->SubItems->Strings[RMK_MEMO]= pRemarks->m_strRemarks;
                                pListView->Items->Item[i]->SubItems->Strings[RMK_PATH]= pRemarks->m_strImagePath;
                                //update��Ʈw
                                TQuery* pQuery = new TQuery(Dlg_AddRemarks);     //free in next fiew line
                                pQuery->DatabaseName = "DB_DCH";
                                pQuery->SQL->Add("UPDATE tRemarks");
                                pQuery->SQL->Add("SET Name=:Name,RemarkType=:RemarkType,Phone=:Phone,Address=:Address,Email=:Email,WebAddress=:WebAddress,Memo=:Memo,ImagePath=:ImagePath");
                                pQuery->SQL->Add("WHERE SerialNo = :SerialNo");
                                pQuery->ParamByName("SerialNo")->AsInteger = pRemarks->i_SerialNo;
                                pQuery->ParamByName("Name")->AsString = (AnsiString)pRemarks->m_strName;
                                pQuery->ParamByName("RemarkType")->AsInteger = pRemarks->m_RemarkType;
                                pQuery->ParamByName("Phone")->AsString = (AnsiString)pRemarks->m_strPhone;
                                pQuery->ParamByName("Address")->AsString = (AnsiString)pRemarks->m_strAddr;
                                pQuery->ParamByName("Email")->AsString = (AnsiString)pRemarks->m_strEmail;
                                pQuery->ParamByName("WebAddress")->AsString = (AnsiString)pRemarks->m_strWebAddr;
                                pQuery->ParamByName("Memo")->AsString = (AnsiString)pRemarks->m_strRemarks;
                                pQuery->ParamByName("ImagePath")->AsString = (AnsiString)pRemarks->m_strImagePath;
                                try
                                {
                                        pQuery->ExecSQL();
                                }
                                catch(...)
                                {
                                        ShowMessage("�ק�Ƶ��a�I���Ѧb�g�J��Ʈw�ɵo�ͥ���!");
                                }
                                delete pQuery;
                                break;
                        }//if
                }//for

        }
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TDlg_AddRemarks::OnExecShell(TObject *Sender)
{//
	TEdit* pEdit = (TEdit*)Sender;
    if(pEdit->Text.Length() >0)
    {
    	AnsiString strExec;
    	if(pEdit->Name == "Edit_Email")
    		strExec = "mailto:"+pEdit->Text;
    	else
        	strExec = "http://"+pEdit->Text;
    	ShellExecute(Handle, "open",strExec.c_str(), NULL, NULL, SW_SHOWNORMAL);
    }
}
//---------------------------------------------------------------------------

void __fastcall TDlg_AddRemarks::ComboBox_CompanyEnter(TObject *Sender)
{
        ComboBox_Company->Items->Clear();
        TQuery* pQuery = DataModule1->Query_tCustInfo;
        try
        {
                pQuery->Close();
                pQuery->Open();
        }
        catch(...)
        {
                DataModule1->EstablishConnect(1);
                return;
        }
        for(int i=0; i<pQuery->RecordCount; i++)
        {
                ComboBox_Company->Items->Add(pQuery->FieldByName("FullName")->AsString);
                pQuery->Next();
        }
        ComboBox_Company->Items->Add("*�q�γƵ��a�I*");
}
//---------------------------------------------------------------------------

void __fastcall TDlg_AddRemarks::ComboBox_CompanyChange(TObject *Sender)
{
        if(ComboBox_Company->Text == "*�q�γƵ��a�I*")
        {
                CoID = "Common";
        }
        else
        {
                TQuery* pQuery = new TQuery(Dlg_AddRemarks);
                pQuery->DatabaseName = "DB_DCH";
                pQuery->SQL->Add("SELECT CoID FROM tCustInfo");
                pQuery->SQL->Add("WHERE FullName = :FullName");
                pQuery->ParamByName("FullName")->AsString = ComboBox_Company->Text;
                pQuery->Active = false;
                pQuery->Active = true;
                CoID = pQuery->FieldByName("CoID")->AsString;
                pQuery->Active = false;
                delete pQuery;
        }
}
//---------------------------------------------------------------------------


void __fastcall TDlg_AddRemarks::Edit_NameChange(TObject *Sender)
{
        if(Edit_Name->Text != "")
        {
                OKBtn->Enabled = true;
        }
}
//---------------------------------------------------------------------------


void __fastcall TDlg_AddRemarks::SpeedButton_ImageClick(TObject *Sender)
{
//�|���ܤu�@�ؿ�, �ϥΧ��г]�^
        chdir(CallCenter_Main_Form->WorkingPath);
        OpenPictureDialog_Remarks->InitialDir = ("..\\Data\\Remarks");
        if(DataModule1->DB_Check_Thread->bCanTerminate)
        {
                DataModule1->DB_Check_Thread->Suspend();
        }
        if(CallCenter_Main_Form->Maintain_Thread->bCanTerminate)
        {
                CallCenter_Main_Form->Maintain_Thread->Suspend();
        }
	if (OpenPictureDialog_Remarks->Execute())
   	{
                DataModule1->DB_Check_Thread->Resume();
                CallCenter_Main_Form->Maintain_Thread->Resume();
                AnsiString FileName = OpenPictureDialog_Remarks->FileName;
                int Pos = FileName.Pos("\\Data\\Remarks");
                if(Pos == 0)
                {
                        ShowMessage("��L���|���ϧάO���Q���\��!!");
                        return;
                }
                FileName.Delete(1,Pos-1);
                FileName.Insert("..",1);
                m_strImagePath = FileName;
                CallCenter_Main_Form->ChangeImagePic(Image_RemarksImg,FileName.c_str());
        }
        chdir(CallCenter_Main_Form->WorkingPath);	//�_�� Working Dir
        DataModule1->DB_Check_Thread->Resume();
        CallCenter_Main_Form->Maintain_Thread->Resume();        
}
//---------------------------------------------------------------------------


