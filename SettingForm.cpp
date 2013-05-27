//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SettingForm.h"
#include "MainForm.h"
#include "common.h"
#include "CustRoute.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_Setting *Form_Setting;
//---------------------------------------------------------------------------
__fastcall TForm_Setting::TForm_Setting(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm_Setting::ComboBox_LayerChange(TObject *Sender)
{
        if(ComboBox_Layer->Text == ComboBox_Layer->Items->Strings[0])
        {
                LayerBox->State[0] = cbUnchecked;
                LayerBox->State[1] = cbChecked;
                LayerBox->State[2] = cbChecked;
                LayerBox->State[3] = cbChecked;
                LayerBox->State[4] = cbChecked;
                LayerBox->State[5] = cbChecked;
                LayerBox->State[6] = cbChecked;
                LayerBox->State[7] = cbChecked;
                LayerBox->State[8] = cbUnchecked;
        }
        else if(ComboBox_Layer->Text == ComboBox_Layer->Items->Strings[1])
        {
                LayerBox->State[0] = cbChecked;
                LayerBox->State[1] = cbChecked;
                LayerBox->State[2] = cbChecked;
                LayerBox->State[3] = cbChecked;
                LayerBox->State[4] = cbUnchecked;
                LayerBox->State[5] = cbChecked;
                LayerBox->State[6] = cbChecked;
                LayerBox->State[7] = cbChecked;
                LayerBox->State[8] = cbChecked;
        }
        for(int i=0; i < 9; i++)
        {
                if(LayerBox->Checked[i])
                        g_DrawFlag[i] = '1';
                else
                        g_DrawFlag[i] = '0';
        }
        ReDrawImage(CallCenter_Main_Form->Image_Map,CallCenter_Main_Form->m_dScale);
}
//---------------------------------------------------------------------------
void __fastcall TForm_Setting::LayerBoxClickCheck(TObject *Sender)
{
        for(int i=0; i < 9; i++)
        {
                if(LayerBox->Checked[i])
                        g_DrawFlag[i] = '1';
                else
                        g_DrawFlag[i] = '0';
        }
        ReDrawImage(CallCenter_Main_Form->Image_Map,CallCenter_Main_Form->m_dScale);
        ComboBox_Layer->Text = ComboBox_Layer->Items->Strings[2];
        TIniFile* ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ));
        ini->WriteString("MAP","Layer",(AnsiString)g_DrawFlag);
        delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TForm_Setting::MaskEdit_LockedAreaPixelsChange(
      TObject *Sender)
{   //Locked Area 最小 20 , 最大999 
        int LockArea = atoi(MaskEdit_LockedAreaPixels->Text.c_str());
        if(LockArea<20)
        {
    	        CallCenter_Main_Form->m_iLockedAreaPixels = 20;
                MaskEdit_LockedAreaPixels->Text = "20";
        }
        else
		CallCenter_Main_Form->m_iLockedAreaPixels = LockArea;
        if(CallCenter_Main_Form->m_bIsLockMonitor) 	//Update Locked Area
        {
		ReDrawImage(CallCenter_Main_Form->Image_Map,CallCenter_Main_Form->m_dScale);
                CallCenter_Main_Form->UpdateAllObjPos();
        }

}
//---------------------------------------------------------------------------
void __fastcall TForm_Setting::UpDown_LockedAreaPixelsChanging(
      TObject *Sender, bool &AllowChange)
{
        MaskEdit_LockedAreaPixelsChange(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm_Setting::BitBtn_ShowAllSelPathClick(TObject *Sender)
{
        Form_CustRoute->PageControl1->ActivePage = Form_CustRoute->Tab_Admin;
        Form_CustRoute->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm_Setting::FormCreate(TObject *Sender)
{
        Edit_PathArea->Text = DEF_PATH_WIDTH;
}
//---------------------------------------------------------------------------
void __fastcall TForm_Setting::BitBtn1Click(TObject *Sender)
{
        Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm_Setting::Edit_PathAreaExit(TObject *Sender)
{
        if(Edit_PathArea->Text.ToIntDef(-1) == -1)
        {
                ShowMessage("輸入格式錯誤,請輸入0~999之數字格式!");
        }
        else
        {
                PathArea = Edit_PathArea->Text.ToInt();
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm_Setting::Edit_PathAreaKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
        if(Key==VK_RETURN)
        {
                if(Edit_PathArea->Text.ToIntDef(-1) == -1)
                {
                        ShowMessage("輸入格式錯誤,請輸入0~999之數字格式!");
                }
                else
                {
                        PathArea = Edit_PathArea->Text.ToInt();
                }
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm_Setting::Edit_MaxTraceExit(TObject *Sender)
{
        if(Edit_MaxTrace->Text.ToIntDef(-1) == -1)
        {
                ShowMessage("輸入格式錯誤,請輸入0~999之數字格式!");
        }
        else
        {
                MaxTracePoint = Edit_MaxTrace->Text.ToInt();
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm_Setting::Edit_MaxTraceKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
        if(Key==VK_RETURN)
        {
                if(Edit_MaxTrace->Text.ToIntDef(-1) == -1)
                {
                        ShowMessage("輸入格式錯誤,請輸入0~999之數字格式!");
                }
                else
                {
                        MaxTracePoint = Edit_MaxTrace->Text.ToInt();
                }
        }
}
//---------------------------------------------------------------------------


void __fastcall TForm_Setting::Button1Click(TObject *Sender)
{
        CallCenter_Main_Form->N_Equipment_SearchClick(this);
}
//---------------------------------------------------------------------------

