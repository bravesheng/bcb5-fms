
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "LogoForm.h"
#include "MainForm.h"

USERES("CallCenter.res");
USEFORM("MainForm.cpp", CallCenter_Main_Form);
USEUNIT("GetTrackThread.cpp");
USEUNIT("common.cpp");
USEFORM("LogoForm.cpp", Form_Logo);
USEFORM("SubForm1.cpp", CallCenter_Sub_Form);
USEFORM("RemoteCmdDlg.cpp", Dlg_RemoteCmd);
USEUNIT("RemarkPlace.cpp");
USEFORM("AddRemarksDlg.cpp", Dlg_AddRemarks);
USEFORM("ReportForm.cpp", Form_Report);
USELIB("a.lib");
USEFORM("SmartQryForm.cpp", Form_SmartQry);
USEFORM("HisTracer.cpp", CHisTracer); /* TDataModule: File Type */
USEFORM("TraceForm.cpp", Form_Trace);
USEFORM("smscform.cpp", Form_SMSC);
USEFORM("SelectPath.cpp", CSelectPath); /* TDataModule: File Type */
USEFORM("CustRoute.cpp", Form_CustRoute);
USEFORM("DataMudule.cpp", DataModule1); /* TDataModule: File Type */
USEFORM("RouteEditorForm.cpp", Form_RouteEditor);
USEUNIT("MaintainThread.cpp");
USEFORM("SettingForm.cpp", Form_Setting);
USEFORM("EstablishForm.cpp", Form_Establish_Connect);
USELIB("C:\Program Files\Borland\CBuilder5\Lib\wininet.lib");
USEFORM("ViewForm.cpp", Form_ViewInfo);
USEFORM("FreeAllForm.cpp", Form_FreeAll);
USEFORM("ChooseCarForm.cpp", Form_ChooseCar);
USEFORM("LoginForm.cpp", Form_Login);
USEFORM("EditPasswordForm.cpp", Form_EditPassword);
USEFORM("StatManager.cpp", Form_ManageState);
USEFORM("PrintForm.cpp", Form_Print);
USEFORM("TripForm.cpp", Form_Trip);
USEFORM("CarinfoForm.cpp", Form_CarInfo);
USEFORM("UserManagerForm.cpp", Form_UserManager);
USEFORM("SetupPwdForm.cpp", Form_EditPassword1);
USEFORM("AddUserForm.cpp", Form_AddUser);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                Application->Initialize();
                //除錯用的LOG
                TStringList *TempList = new TStringList;
                TempList->SaveToFile("runfunction.log");
                delete TempList;
                //動態產生 Logo Form      
                Form_Logo = new TForm_Logo(Application);
//                Form_Logo->Show();
                Application->Title = "FMS Real-Time Monitor";
                 Application->ProcessMessages();
                Application->CreateForm(__classid(TDataModule1), &DataModule1);
                 Application->CreateForm(__classid(TCallCenter_Main_Form), &CallCenter_Main_Form);
                 Application->CreateForm(__classid(TCHisTracer), &CHisTracer);
                 Application->CreateForm(__classid(TForm_Setting), &Form_Setting);
                 Application->CreateForm(__classid(TForm_Establish_Connect), &Form_Establish_Connect);
                 Application->CreateForm(__classid(TForm_FreeAll), &Form_FreeAll);
                 Application->CreateForm(__classid(TForm_ChooseCar), &Form_ChooseCar);
                 Application->CreateForm(__classid(TForm_CustRoute), &Form_CustRoute);
                 Application->CreateForm(__classid(TForm_Login), &Form_Login);
                 Application->CreateForm(__classid(TForm_EditPassword), &Form_EditPassword);
                 Application->CreateForm(__classid(TForm_ManageState), &Form_ManageState);
                 Application->ProcessMessages();
                Form_Logo->Visible = false;
                Form_Logo->Repaint();
                Form_Logo->InitApp();
                Form_Logo->Hide();
                Form_Logo->Close();   //Free Memory in OnClose()
                Application->Run();
        }

        catch (Exception &exception)
        {
                Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------


