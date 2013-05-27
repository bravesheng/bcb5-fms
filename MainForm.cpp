//---------------------------------------------------------------------------

#include <vcl.h>
#include <mmsystem.h>	//FOR Use ::sndPlaySound , and must Set Link lib -- winmm.lib
#include <stdio.h>
#pragma hdrstop

#include "Common.h"
#include "MainForm.h"
#include "SubForm1.h"
#include "Simu_Msg.h"
#include "RemoteCmdDlg.h"     	//Show Timer Report Dialog
#include "LogoForm.h"
#include "AddRemarksDlg.h"
#include "SmartQryForm.h"
#include "ReportForm.h"
#include "HisTracer.h"
#include "TraceForm.h"
#include "smscform.h"
#include "CustRoute.h"
#include "SettingForm.h"
#include "ViewForm.h"
#include "FreeAllForm.h"
#include "RouteEditorForm.h"
#include "LoginForm.h"
#include "EditPasswordForm.h"
#include "StatManager.h"
#include "PrintForm.h"
#include "TripForm.h"
#include "CarinfoForm.h"
#include "UserManagerForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

extern int g_RemarksCount;
TCallCenter_Main_Form *CallCenter_Main_Form;

//---------------------------------------------------------------------------
__fastcall TCallCenter_Main_Form::TCallCenter_Main_Form(TComponent* Owner)
        : TForm(Owner)
{
        ThreadDeleteing = false;
        m_bInitialed = false;
	    m_bIsLockMonitor = false;
	    m_iLockedAreaPixels = 350;     // 350x350 Rectangle
	    m_pCarList = new TList();		//free Memory at OnDestroy()
        CCarList_SortByLocatorNo = new TList();     //free memory at onDestory()
        m_pRemarksList = new TList();	//free Memory at OnDestroy()
        m_pSubFormList = new TList();	//free Memory at OnDestroy()
        m_pSelPathGroupList = new TList();//���w���u��List���X free Memory at OnDestroy()
        m_pDefaultSelPath = NULL;
        m_pDrawTraceCarList = new TList();   //�ݭn�e�Y�ɭy�񪺨��l���� List
        DockSiteWidth = 255;
        LateReportRate = 1.5;
        ACC_OFF_Delay_Time = 43200;
}
//---------------------------------------------------------------------------

void __fastcall TCallCenter_Main_Form::FormCreate(TObject *Sender)
{
        KeyPreview = true;	//Capture Key Down Event
        //2001.03.21 New Display Test for DCH
        // iMapType=0 -> TW Local Test , iMapType=1 -> HK Map
        InitDispDLL(g_iMapType);

	//Load Custom's Cursor
        Screen->Cursors[crDragHandCur] = LoadCursor(HInstance, "DragHand");
        Screen->Cursors[crDragMoveCur] = LoadCursor(HInstance, "DragMove");

	//Set Default Pointer Type
        m_PointerType = 1;		//0:Pointer , 1:DragMap , 2:SetPath
        //Image_Map->Cursor = (TCursor)crDefault;      //crDragHandCur

        //Initialize  Cursor State
        m_bIsBeginDragMap = false;
        m_bIsBeginDrawPath = false;

        m_dScale = 40.0;  //5.0
        m_dScale_ZoomInWnd = 10.0;
        //Draw init
        dc_bitmap_Map = new Graphics::TBitmap();  	//free at FormDestroy()
        dc_bitmap_Map->Width = ClientWidth;
        dc_bitmap_Map->Height= ClientHeight;
        Image_Map->Picture->Graphic = dc_bitmap_Map;
        Image_ZoomIn->Picture->Graphic = dc_bitmap_Map;
        ReDrawImage(Image_ZoomIn,m_dScale_ZoomInWnd);
        getcwd( WorkingPath, sizeof(WorkingPath));      //���o�u�@�ؿ�
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::FormDestroy(TObject *Sender)
{
        FreeDispDLL();  //Free Display DLL
        //���w���u�� ~CSelectPath()�|�N���l�� m_pSelPath �]�� NULL
        //�ҥH������ delete m_pSelPathGroupList  �A delete Car
        if(m_pSubFormList)
                delete	m_pSubFormList;
        if(m_pDefaultSelPath)
                delete	m_pDefaultSelPath;
        for (int j = 0; j < m_pSelPathGroupList->Count; j++)
                delete ((TCSelectPath*) m_pSelPathGroupList->Items[j]);
        delete m_pSelPathGroupList;
        delete CCarList_SortByLocatorNo;
        //Save Car info for next open
        //free Cars List Memory
        CCar* pCar=NULL;
        if(m_pCarList)
        {
                for (int i = 0; i < m_pCarList->Count; i++)
                {
                        pCar = (CCar*) m_pCarList->Items[i];
                        delete pCar;
                }
                delete m_pCarList;
        }
        if(m_pDrawTraceCarList)     //�ݭn�e�Y�ɭy�񪺨��l���� List
                delete	m_pDrawTraceCarList;
        try
        {
                CRemarkPlace* pRemarks;
                if(m_pRemarksList)
                {
                        for (int k = 0; k < m_pRemarksList->Count; k++)
                        {
                                pRemarks = (CRemarkPlace*) m_pRemarksList->Items[k];
                                delete pRemarks;
                        }
                        delete m_pRemarksList;
                }
        }
        catch(...)
        {}
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::Panel_StreetResize(TObject *Sender)
{
        //Draw ZoomIn Image
        ReDrawImage(Image_ZoomIn,m_dScale_ZoomInWnd);
}
//---------------------------------------------------------------------------

void __fastcall TCallCenter_Main_Form::ToolButton_RefleshClick(
      TObject *Sender)
{
	ReDrawImage(Image_Map,m_dScale);
}
//---------------------------------------------------------------------------

void __fastcall TCallCenter_Main_Form::SpeedButton_ShowSettingClick(
      TObject *Sender)
{
	Form_ViewInfo->Visible = SpeedButton_ShowSetting->Down;
        if(Form_ViewInfo->Visible==true)
	{
		ReDrawImage(Image_Map,m_dScale);
    	        UpdateAllObjPos();
        }
}
//---------------------------------------------------------------------------

void __fastcall TCallCenter_Main_Form::SpeedButton_ShowStreetClick(
      TObject *Sender)
{
	Panel_Street->Visible = SpeedButton_ShowStreet->Down;
        if(Panel_Street->Visible==true)
	{
                ReDrawImage(Image_Map,m_dScale);
    	        UpdateAllObjPos();
        }
}
//---------------------------------------------------------------------------

void __fastcall TCallCenter_Main_Form::Menu_Wnd_SettingClick(TObject *Sender)
{//
	SpeedButton_ShowSetting->Down = ! SpeedButton_ShowSetting->Down;
	SpeedButton_ShowSettingClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TCallCenter_Main_Form::Menu_Wnd_StreetClick(TObject *Sender)
{//
	SpeedButton_ShowStreet->Down = ! SpeedButton_ShowStreet->Down;
	SpeedButton_ShowStreetClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::ToolButton_ZoomInClick(
      TObject *Sender)
{
        if(m_dScale <= MAX_MAP_SCALE)
        {
                m_dScale = m_dScale*0.666;
                if(m_dScale < 1)
                {
			m_dScale = 1.0;
                }
                ToolButton_ZoomIn->Enabled = true;
                Menu_MapOP_ZoomIn->Enabled = true;
		ToolButton_ZoomOut->Enabled = true;
                Menu_MapOP_ZoomOut->Enabled = true;
        }
        if(m_dScale == 1.0 )
        {
    	        ToolButton_ZoomIn->Enabled = false;
                Menu_MapOP_ZoomIn->Enabled = false;
        }
        ReDrawImage(Image_Map,m_dScale);
   	UpdateAllObjPos();
}

//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::ToolButton_ZoomOutClick(TObject *Sender)
{
    //ZoomIn or Zoomout �H�����I����
        if(m_dScale <= MAX_MAP_SCALE)
        {
                m_dScale = m_dScale*1.5;
                if(m_dScale > MAX_MAP_SCALE)
                {
                        m_dScale = MAX_MAP_SCALE;
                }
                ToolButton_ZoomIn->Enabled = true;
                Menu_MapOP_ZoomIn->Enabled = true;
		ToolButton_ZoomOut->Enabled = true;
                Menu_MapOP_ZoomOut->Enabled = true;
        }
        if(m_dScale == MAX_MAP_SCALE )
        {
    	        ToolButton_ZoomOut->Enabled = false;
                Menu_MapOP_ZoomOut->Enabled = false;
        }
	ReDrawImage(Image_Map,m_dScale);
   	UpdateAllObjPos();
}
//---------------------------------------------------------------------------
void __fastcall	TCallCenter_Main_Form::ChangeZoom(double scale)
{
        if((scale>MAX_MAP_SCALE)||(scale<1.0))
    	        return;
        ToolButton_ZoomIn->Enabled = true;
        Menu_MapOP_ZoomIn->Enabled = true;
        ToolButton_ZoomOut->Enabled = true;
        Menu_MapOP_ZoomOut->Enabled = true;
        //ZoomIn or Zoomout �H�����I����
	m_dScale = scale;
        DrawImage(Image_Map,m_temp_X,m_temp_Y,m_dScale);
   	UpdateAllObjPos();
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::N3x1Click(TObject *Sender)
{
	double scale;
	TMenuItem* pMenu = (TMenuItem*)Sender;

    if( (pMenu==N1x1)||(pMenu==M_N1x1) )	// 1x Click
        scale=1.0;
    else if( (pMenu==N2x1)||(pMenu==M_N2x1) )
        scale=3.0;
    else if( (pMenu==N3x1)||(pMenu==M_N3x1) )
        scale=7.0;
    else if( (pMenu==N4x1)||(pMenu==M_N4x1) )
        scale=10.0;
    else if( (pMenu==N5x1)||(pMenu==M_N5x1) )
        scale=19.9;
    else if( (pMenu==N6x1)||(pMenu==M_N6x1) )
        scale=29.8;
    else if( (pMenu==N7x1)||(pMenu==M_N7x1) )
        scale=39.7;
    else if( (pMenu==N8x1)||(pMenu==M_N8x1) )
        scale=49.6;
    else if( (pMenu==N9x1)||(pMenu==M_N9x1) )
        scale=59.5;
    else if( (pMenu==N10x1)||(pMenu==M_N10x1) )
        scale=69.4;
    else if( (pMenu==N11x1)||(pMenu==M_N11x1) )
        scale=89.25;
    else if( (pMenu==N12x1)||(pMenu==M_N12x1) )
        scale=198.3;
    else if( (pMenu==N13x1)||(pMenu==M_N13x1) )
        scale=396.5;
    else if( (pMenu==MenuItem_LocalArea)||(pMenu==M_MenuItem_LocalArea) )
    {
        m_dScale = 900.0;
        DrawImage(Image_Map,114120210,22357593,m_dScale);
    }
    m_dScale = scale;
    ReDrawImage(Image_Map,m_dScale);
    UpdateAllObjPos();
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::UpdateAllObjPos()
{
	CCar *ptrCar;
        for(int i=0;i<m_pCarList->Count;i++)
        {
    	        ptrCar = (CCar*)(m_pCarList->Items[i]);
                if(ptrCar->Visible)           //Visivle��true�o�~��s!!!
                {
        	        ptrCar->Update(NULL);
                }
        }//for
        //Update All RemarkPlace
	CRemarkPlace *ptrRemarks;
        for(int i=0;i<m_pRemarksList->Count;i++)
        {
    	        ptrRemarks = (CRemarkPlace*)(m_pRemarksList->Items[i]);
    	        ptrRemarks->Update();
        }//for

}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::ChangeImagePic(TImage* pImage,char* PicFilePath)
{
        try
        {
                AnsiString FilePath = CallCenter_Main_Form->WorkingPath;
                AnsiString PicFileName = PicFilePath;
                FilePath = FilePath.SubString(1,FilePath.LastDelimiter("\\"));
                FilePath = FilePath + PicFileName.SubString(4,PicFileName.Length());
                if(pImage->Visible == true)
                {
                        pImage->Picture->LoadFromFile(FilePath.c_str());   //Gray=>Init , Blue=>Active , Red=>Locked
                }
                else
                {
                        pImage->Picture->LoadFromFile(FilePath.c_str());
                        pImage->Visible = false;
                }
        }
        catch(...)
        {
                AnsiString tmp = "..\\Data\\Remarks\\Remarks_Around.bmp";
                pImage->Picture->LoadFromFile(tmp.c_str());
        }
}             
//---------------------------------------------------------------------------
void __fastcall	TCallCenter_Main_Form::ChangeCarImgByTag(TImage* pImage,int OpType)
{//OpType=0 => Lock <-> UnLock , OpType=1 =>���`->���`�o�� , OpType=2 =>���`->���`���B�z��)
	int ImgTag = pImage->Tag;
	if(OpType !=0)
    	ImgTag = -(ImgTag);
    switch(ImgTag)
    {   // Do UnLock
    	//�p�⨮
    	case -1:ChangeImagePic(pImage,CAR_RED_IMG);	break;
        case -2:ChangeImagePic(pImage,(CAR_YELLOW_IMG));	break;
        case -3:ChangeImagePic(pImage,(CAR_GREEN_IMG)); 	break;
        case -4:ChangeImagePic(pImage,(CAR_BLUE_IMG));	break;
        case -5:ChangeImagePic(pImage,(CAR_GRAY_IMG));	break;
        //�f��
    	case -6:ChangeImagePic(pImage,(TRUCK_RED_IMG));	break;
        case -7:ChangeImagePic(pImage,(TRUCK_YELLOW_IMG));	break;
        case -8:ChangeImagePic(pImage,(TRUCK_GREEN_IMG)); 	break;
        case -9:ChangeImagePic(pImage,(TRUCK_BLUE_IMG));break;
        case -10:ChangeImagePic(pImage,(TRUCK_GRAY_IMG));break;
        //�s����
    	case -11:ChangeImagePic(pImage,(CONTAINER_RED_IMG));	break;
        case -12:ChangeImagePic(pImage,(CONTAINER_YELLOW_IMG));	break;
        case -13:ChangeImagePic(pImage,(CONTAINER_GREEN_IMG)); 	break;
        case -14:ChangeImagePic(pImage,(CONTAINER_BLUE_IMG));break;
        case -15:ChangeImagePic(pImage,(CONTAINER_GRAY_IMG));break;
        //9�H�ڤh
    	case -16:ChangeImagePic(pImage,(NINE_RED_IMG));	break;
        case -17:ChangeImagePic(pImage,(NINE_YELLOW_IMG));	break;
        case -18:ChangeImagePic(pImage,(NINE_GREEN_IMG)); 	break;
        case -19:ChangeImagePic(pImage,(NINE_BLUE_IMG));	break;
        case -20:ChangeImagePic(pImage,(NINE_GRAY_IMG));	break;
        //�ڤh
    	case -21:ChangeImagePic(pImage,(BUS_RED_IMG));	break;
        case -22:ChangeImagePic(pImage,(BUS_YELLOW_IMG));	break;
        case -23:ChangeImagePic(pImage,(BUS_GREEN_IMG)); 	break;
        case -24:ChangeImagePic(pImage,(BUS_BLUE_IMG));	break;
        case -25:ChangeImagePic(pImage,(BUS_GRAY_IMG));	break;
        //�d��2
    	case -26:ChangeImagePic(pImage,(TRUCK2_RED_IMG));	break;
        case -27:ChangeImagePic(pImage,(TRUCK2_YELLOW_IMG));	break;
        case -28:ChangeImagePic(pImage,(TRUCK2_GREEN_IMG)); 	break;
        case -29:ChangeImagePic(pImage,(TRUCK2_BLUE_IMG));	break;
        case -30:ChangeImagePic(pImage,(TRUCK2_GRAY_IMG));	break;
        //���h�ڤh
    	case -31:ChangeImagePic(pImage,(DBUS_RED_IMG));	break;
        case -32:ChangeImagePic(pImage,(DBUS_YELLOW_IMG));	break;
        case -33:ChangeImagePic(pImage,(DBUS_GREEN_IMG)); 	break;
        case -34:ChangeImagePic(pImage,(DBUS_BLUE_IMG));	break;
        case -35:ChangeImagePic(pImage,(DBUS_GRAY_IMG));	break;

        // Do Lock
        //�p�⨮
    	case 1:ChangeImagePic(pImage,(CAR_RED_LOCKED_IMG));	break;
        case 2:ChangeImagePic(pImage,(CAR_YELLOW_LOCKED_IMG));	break;
        case 3:ChangeImagePic(pImage,(CAR_GREEN_LOCKED_IMG)); 	break;
        case 4:ChangeImagePic(pImage,(CAR_BLUE_LOCKED_IMG));break;
        case 5:ChangeImagePic(pImage,(CAR_GRAY_LOCKED_IMG));break;
        //�f��
    	case 6:ChangeImagePic(pImage,(TRUCK_RED_LOCKED_IMG));	break;
        case 7:ChangeImagePic(pImage,(TRUCK_YELLOW_LOCKED_IMG));	break;
        case 8:ChangeImagePic(pImage,(TRUCK_GREEN_LOCKED_IMG)); 	break;
        case 9:ChangeImagePic(pImage,(TRUCK_BLUE_LOCKED_IMG));	break;
        case 10:ChangeImagePic(pImage,(TRUCK_GRAY_LOCKED_IMG));	break;
        //�s����
    	case 11:ChangeImagePic(pImage,(CONTAINER_RED_LOCKED_IMG));	break;
        case 12:ChangeImagePic(pImage,(CONTAINER_YELLOW_LOCKED_IMG));	break;
        case 13:ChangeImagePic(pImage,(CONTAINER_GREEN_LOCKED_IMG)); 	break;
        case 14:ChangeImagePic(pImage,(CONTAINER_BLUE_LOCKED_IMG));	break;
        case 15:ChangeImagePic(pImage,(CONTAINER_GRAY_LOCKED_IMG));	break;
        //9�H�ڤh
    	case 16:ChangeImagePic(pImage,(NINE_RED_LOCKED_IMG));	break;
        case 17:ChangeImagePic(pImage,(NINE_YELLOW_LOCKED_IMG));	break;
        case 18:ChangeImagePic(pImage,(NINE_GREEN_LOCKED_IMG)); 	break;
        case 19:ChangeImagePic(pImage,(NINE_BLUE_LOCKED_IMG));	break;
        case 20:ChangeImagePic(pImage,(NINE_GRAY_LOCKED_IMG));	break;
        //�ڤh
    	case 21:ChangeImagePic(pImage,(BUS_RED_LOCKED_IMG));	break;
        case 22:ChangeImagePic(pImage,(BUS_YELLOW_LOCKED_IMG));	break;
        case 23:ChangeImagePic(pImage,(BUS_GREEN_LOCKED_IMG)); 	break;
        case 24:ChangeImagePic(pImage,(BUS_BLUE_LOCKED_IMG));	break;
        case 25:ChangeImagePic(pImage,(BUS_GRAY_LOCKED_IMG));	break;
        //�d��2
    	case 26:ChangeImagePic(pImage,(TRUCK2_RED_LOCKED_IMG));	break;
        case 27:ChangeImagePic(pImage,(TRUCK2_YELLOW_LOCKED_IMG));	break;
        case 28:ChangeImagePic(pImage,(TRUCK2_GREEN_LOCKED_IMG)); 	break;
        case 29:ChangeImagePic(pImage,(TRUCK2_BLUE_LOCKED_IMG));	break;
        case 30:ChangeImagePic(pImage,(TRUCK2_GRAY_LOCKED_IMG));	break;
        //���h�ڤh
    	case 31:ChangeImagePic(pImage,(DBUS_RED_LOCKED_IMG));	break;
        case 32:ChangeImagePic(pImage,(DBUS_YELLOW_LOCKED_IMG));	break;
        case 33:ChangeImagePic(pImage,(DBUS_GREEN_LOCKED_IMG)); 	break;
        case 34:ChangeImagePic(pImage,(DBUS_BLUE_LOCKED_IMG));	break;
        case 35:ChangeImagePic(pImage,(DBUS_GRAY_LOCKED_IMG));	break;


        default:ShowMessage("In TCallCenter_Main_Form::ChangeCarImgByTag() Car Image Tag  �W�X�d��!");
        		return;
    }

    if(OpType==0)
    	pImage->Tag = -(pImage->Tag);
}

//---------------------------------------------------------------------------
void __fastcall	TCallCenter_Main_Form::OnCarClick(TObject *Sender)
{//for Dynamic use Event for Dynamic Car Image OnClick occure
	TImage* pCarImage = ((TImage*)Sender);
        int Origin_X ;    //�ϧΤ����I=> X
        int Origin_Y ;    //�ϧΤ����I=> Y
        //if((pCarImage->Tag ==-1)||(pCarImage->Tag == -2)||(pCarImage->Tag ==-3)||(pCarImage->Tag == -4))
        if( pCarImage->Tag < 0)	//Locked Cars
        {//Image Tag Status =>see Common.h class CCar
		//if this car is Locked => UnLock it
    	        Origin_X = pCarImage->Left+pCarImage->Width/2;    //�ϧΤ����I=> X
    	        Origin_Y = pCarImage->Top+pCarImage->Height/2;    //�ϧΤ����I=> Y
     	        // Do UnLock
                //Image Tag => see "Common.h"
                ChangeCarImgByTag(pCarImage);
    	        pCarImage->Left = Origin_X - pCarImage->Width/2;
    	        pCarImage->Top = Origin_Y - pCarImage->Height/2;
    	        //Update Monitor Status
                m_bIsLockMonitor = false;
    	        StaticText_MonitorSattus->Caption = "�ϰ�ʱ�";
    	        StaticText_MonitorSattus->Font->Color = clGreen;
    	        //Update Red Monitor Rectangle
                //DrawImage(Image_Map,m_iPos_X,m_iPos_Y,m_dScale);  //Just repaint again
		ReDrawImage(Image_Map,m_dScale);
    	        return;
        }
        //Set Car Locked => First UnLock others
	CCar *pCar;
        for(int i=0;i<m_pCarList->Count;i++)
        {
    	        pCar = (CCar*)m_pCarList->Items[i];
                if( pCar->m_pImage_Car->Tag < 0 )//Locked Cars
                {//Image Tag Status =>see Common.h class CCar
    		        Origin_X = pCar->m_pImage_Car->Left+pCar->m_pImage_Car->Width/2;    //�ϧΤ����I=> X
    		        Origin_Y = pCar->m_pImage_Car->Top+pCar->m_pImage_Car->Height/2;    //�ϧΤ����I=> Y
     		        // Do UnLock
        	        //Image Tag => see "Common.h"
        	        ChangeCarImgByTag(pCar->m_pImage_Car);
    		        pCar->m_pImage_Car->Left = Origin_X - pCarImage->Width/2;
    		        pCar->m_pImage_Car->Top = Origin_Y - pCarImage->Height/2;
                        break;	//Only one can be Locked
    	        }
        }//for
        Origin_X = pCarImage->Left+pCarImage->Width/2;    //�ϧΤ����I=> X
        Origin_Y = pCarImage->Top+pCarImage->Height/2;    //�ϧΤ����I=> Y
	// Do Lock
        //Image Tag => see "Common.h"
        ChangeCarImgByTag(pCarImage);
        pCarImage->Left = Origin_X - pCarImage->Width/2;
        pCarImage->Top = Origin_Y - pCarImage->Height/2;
	//Update Monitor Status
        m_bIsLockMonitor = true;
        StaticText_MonitorSattus->Caption = "��w�ʱ�";
	StaticText_MonitorSattus->Font->Color = clRed;
        //Update Red Monitor Rectangle and set Locked Car to Center of Image
	ReDrawImage(Image_Map,m_dScale);
        UpdateAllObjPos();
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::PopupMenu_CarPopup(TObject *Sender)
{
//���v���i��޲z
                if(N_LVCar_Cmd->Tag == 0)
                {
                        N_LVCar_Cmd->Visible = true;
                        N8->Visible = true;
                }
                else
                {
                        N_LVCar_Cmd->Visible = false;
                        N8->Visible = false;
                }
                if(N_LVCar_Watch->Tag == 0)
                {
                        N_LVCar_Watch->Visible = true;
                }
                else
                {
                        N_LVCar_Watch->Visible = false;
                }
                if(N_LVCar_Trace->Tag == 0)
                {
                        N_LVCar_Trace->Visible = true;
                }
                else
                {
                        N_LVCar_Trace->Visible = false;
                }
                if(N_LVCar_Type->Tag == 0)
                {
                        N_LVCar_Type->Visible = true;
                        MenuItem_ChgCarImg->Visible = true;
                }
                else
                {
                        N_LVCar_Type->Visible = false;
                        MenuItem_ChgCarImg->Visible = false;
                }
//�Q��Popup Menu �X�{�ɶ����l������Цs�� TImage->Tag ��
        TComponent *pComponent = PopupMenu_Car->PopupComponent;
        if (pComponent)
        {
                if(pComponent->ClassNameIs("TImage"))
                {
		        CCar *ptrCar;
    	                for(int i=0;i<m_pCarList->Count;i++)
    	                {   //��X���l����
    		                ptrCar = (CCar*)(m_pCarList->Items[i]);
        	                if(ptrCar->m_pImage_Car == (TImage *)pComponent )
        	                {//�N���l������мȦs�� Tag
        		                PopupMenu_Car->Tag = (long)ptrCar;
            	                        return;
                                }//if
    	                }//for
                }//if TImage
        }
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::MenuItem_AskReportClick(
      TObject *Sender)
{   //�郞�l�U�O -> �ߧY�^����m
	CCar *pCar = (CCar *)PopupMenu_Car->Tag; //�Q��PopupMenu_CarPopup �w��X���l�������
        if(pCar)
        {
		char strShowAlert[100];
    	        wsprintf(strShowAlert,"����: %s\n�������X: %s\n�A�T�w�n�ǰe�ߧY�^�����p�T����?",pCar->GetCarID(),pCar->GetLocatorNo());
		if (::MessageBox(Handle,strShowAlert,"�T�{�ǰe", MB_OKCANCEL|MB_ICONQUESTION) == IDOK)
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
                        memset(&Cmd,0,sizeof(sCmdMsg));        //RESET WHEN FREE
                }
        }
        //reset
        PopupMenu_Car->Tag = 0;
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::MenuItem_SetTimeReportClick(
      TObject *Sender)
{   //Show Timer Report Dialog  , free memory in TDlg_TimerReport::FormClose()
    //�Ѧ� RemoteCmdDlg.h �N������
	CCar *ptrCar = (CCar *)PopupMenu_Car->Tag; //�Q��PopupMenu_CarPopup �w��X���l�������
    if(ptrCar)
    {
		Dlg_RemoteCmd = new TDlg_RemoteCmd(this);
                Dlg_RemoteCmd->m_CarList->Add(ptrCar);
                Dlg_RemoteCmd->m_DlgCmdType = 0; //0:�w�ɦ^�� , 1:�ߧY�^�� , 2:��郞���K�X ,3:��ﱱ��߸��X , 4:��L�R�O
    	        Dlg_RemoteCmd->Show();
    }
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::MenuItem_CancelTimeReportClick(
      TObject *Sender)
{
	//�郞�l�U�O -> �����w�ɦ^��(�n�D�ɤ~�^��)
	//MenuItem_AskReportClick(Sender);
	CCar *pCar = (CCar *)PopupMenu_Car->Tag; //�Q��PopupMenu_CarPopup �w��X���l�������
        if(pCar)
        {
    	        char strShowAlert[100];
    	        wsprintf(strShowAlert,"����: %s\n�������X: %s\n�A�T�w�n�ǰe�����w�ɦ^��(�n�D�ɤ~�^��)�T����?",pCar->GetCarID(),pCar->GetLocatorNo());
		if (::MessageBox(Handle,strShowAlert,"�T�{�ǰe", MB_OKCANCEL|MB_ICONQUESTION) == IDOK)
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
                        memset(&Cmd,0,sizeof(sCmdMsg));         //RESET WHEN FREE
                }
        }
        //reset
        PopupMenu_Car->Tag = 0;
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::MenuItem_ChgLocatorPwdClick(
      TObject *Sender)
{
   //Show Timer Report Dialog  , free memory in TDlg_TimerReport::FormClose()
	CCar *ptrCar = (CCar *)PopupMenu_Car->Tag; //�Q��PopupMenu_CarPopup �w��X���l�������
    if(ptrCar)
    {
		Dlg_RemoteCmd = new TDlg_RemoteCmd(this);
        Dlg_RemoteCmd->m_CarList->Add(ptrCar);
        Dlg_RemoteCmd->m_DlgCmdType = 2;  //0:�w�ɦ^�� , 1:�ߧY�^�� , 2:��郞���K�X ,3:��ﱱ��߸��X , 4:��L�R�O
    	Dlg_RemoteCmd->Show();
    }

}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::MenuItem_RemoteCmdClick(
      TObject *Sender)
{
   //Show Timer Report Dialog  , free memory in TDlg_TimerReport::FormClose()
	CCar *ptrCar = (CCar *)PopupMenu_Car->Tag; //�Q��PopupMenu_CarPopup �w��X���l�������
    if(ptrCar)
    {
		Dlg_RemoteCmd = new TDlg_RemoteCmd(this);
        Dlg_RemoteCmd->m_CarList->Add(ptrCar);
        Dlg_RemoteCmd->m_DlgCmdType = 4;  //0:�w�ɦ^�� , 1:�ߧY�^�� , 2:��郞���K�X ,3:��ﱱ��߸��X , 4:��L�R�O
    	Dlg_RemoteCmd->Show();
    }
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::LVSendCmdByType(int DlgCmdType)
{   //-1:�����^�� , 0:�w�ɦ^�� , 1:�ߧY�^�� , 2:��郞���K�X ,3:��ﱱ��߸��X , 4:��L�R�O,5:�]�w�^���ˬd�W�v
    int selcount = Form_ViewInfo->ListView_Car->SelCount;
    if(selcount >0)
    {
        //int index = 0;
        CCar* ptrCar=NULL;
        Dlg_RemoteCmd = new TDlg_RemoteCmd(this);
        Dlg_RemoteCmd->m_DlgCmdType = DlgCmdType;
        for(int i=0; i<Form_ViewInfo->ListView_Car->Items->Count; i++)
        {
                if(Form_ViewInfo->ListView_Car->Items->Item[i]->Selected)
                {
                        AnsiString CarID = Form_ViewInfo->ListView_Car->Items->Item[i]->Caption;
                        ptrCar = GetCarByCarID(CarID);
                        if(ptrCar)
                        {	// Show Timer Report Dialog
                                Dlg_RemoteCmd->m_CarList->Add(ptrCar);
                        }
                }
        }
        Dlg_RemoteCmd->Show();
    }//if
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::MenuItem_LV_AskReportClick(
      TObject *Sender)
{
	LVSendCmdByType(1);   //-1:�����^�� , 0:�w�ɦ^�� , 1:�ߧY�^�� , 2:��郞���K�X ,3:��ﱱ��߸��X , 4:��L�R�O
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::MenuItem_LV_SetTimeReportClick(TObject *Sender)
{
	LVSendCmdByType(0);   //-1:�����^�� , 0:�w�ɦ^�� , 1:�ߧY�^�� , 2:��郞���K�X ,3:��ﱱ��߸��X , 4:��L�R�O
}
//---------------------------------------------------------------------------

void __fastcall TCallCenter_Main_Form::MenuItem_LV_CancelTimeReportClick(TObject *Sender)
{
	LVSendCmdByType(-1);   //-1:�����^�� , 0:�w�ɦ^�� , 1:�ߧY�^�� , 2:��郞���K�X ,3:��ﱱ��߸��X , 4:��L�R�O
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::MenuItem_LV_RemoteCmdClick(
      TObject *Sender)
{
	LVSendCmdByType(4);   //-1:�����^�� , 0:�w�ɦ^�� , 1:�ߧY�^�� , 2:��郞���K�X ,3:��ﱱ��߸��X , 4:��L�R�O
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::MenuItem_LV_ChgLocatorPwdClick(
      TObject *Sender)
{
	LVSendCmdByType(2);   //-1:�����^�� , 0:�w�ɦ^�� , 1:�ߧY�^�� , 2:��郞���K�X ,3:��ﱱ��߸��X , 4:��L�R�O
}
//---------------------------------------------------------------------------

void __fastcall TCallCenter_Main_Form::MenuItem_LV_ChgCenterNoClick(
      TObject *Sender)
{
	LVSendCmdByType(3);   //-1:�����^�� , 0:�w�ɦ^�� , 1:�ߧY�^�� , 2:��郞���K�X ,3:��ﱱ��߸��X , 4:��L�R�O
}
//---------------------------------------------------------------------------

void __fastcall TCallCenter_Main_Form::N3Click(TObject *Sender)
{    //����...�ìP�ʱ������t�� -> Show Form_Logo
	Form_Logo = new TForm_Logo(this);
	Form_Logo->Show();
	Form_Logo->Update();
}
//---------------------------------------------------------------------------

void __fastcall TCallCenter_Main_Form::MenuItem_AddRemarksClick(
      TObject *Sender)
{
        if(g_RemarksCount +1 > MAX_REMARKS_COUNT)
        {
    	        AnsiString str("�Ƶ��a�I�ƥضW�X�̤j�ƶq = ");
                str = str + AnsiString(MAX_REMARKS_COUNT) + "\n" + "�ЧR����L���Ƶ��a�I��A���s�ާ@ !";
        	ShowMessage(str);
                return;
        }
        Dlg_AddRemarks = new TDlg_AddRemarks(this);
	Dlg_AddRemarks->m_Remark_Owner = this;
        Dlg_AddRemarks->m_Remark_tmX = m_temp_X;
        Dlg_AddRemarks->m_Remark_tmY = m_temp_Y;
        Dlg_AddRemarks->ShowModal();
}
//---------------------------------------------------------------------------
////////  for Remarks Popup Menu Operate  /////////////////
void __fastcall TCallCenter_Main_Form::PopupMenu_RemarksPopup(
      TObject *Sender)
{ //�Q��Popup Menu �X�{�ɶ����l������Цs�� TImage->Tag ��
  TComponent *pComponent = PopupMenu_Remarks->PopupComponent;
  if (pComponent)
  {
    if (pComponent->ClassNameIs("TImage"))
    {
		CRemarkPlace *pRemarks;
    	for(int i=0;i<m_pRemarksList->Count;i++)
    	{   //��X���l����
    		pRemarks = (CRemarkPlace*)(m_pRemarksList->Items[i]);
            if( pRemarks->m_pImage_Remarks==(TImage *)pComponent )
            {
            	PopupMenu_Remarks->Tag = (long)pRemarks;  //�N���l������мȦs�� Tag
//                MI_Remarks_Change->Visible = false;
//                MI_Remarks_Content->Visible = false;
                return;
            }//if
    	}//for
    }//if TImage
  }
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::MI_Remarks_SetAsCenterClick(
      TObject *Sender)
{//�]���ù�����
	CRemarkPlace *pRemarks = (CRemarkPlace *)PopupMenu_Remarks->Tag; //�Q��PopupMenu_Remarks OnPop() �ƥ�w��X���l�������
        if(pRemarks)
        {
                DrawImage(Image_Map,pRemarks->m_X2,pRemarks->m_Y2,m_dScale);
    	        UpdateAllObjPos();
                DrawCenterLine(Image_Map);
        }
	        PopupMenu_Remarks->Tag = 0;

}
//---------------------------------------------------------------------------

void __fastcall TCallCenter_Main_Form::MI_Remarks_DeleteClick(
      TObject *Sender)
{//�R��
	CRemarkPlace *pRemarks = (CRemarkPlace *)PopupMenu_Remarks->Tag; //�Q��PopupMenu_Remarks OnPop() �ƥ�w��X���l�������
    if(pRemarks)
    {
		if (::MessageBox(Handle,"�A�T�w�n�R���o�Ӧa�I�ܶ�?","�T�{�R��", MB_OKCANCEL|MB_ICONQUESTION) == IDOK)
        {
            //remove record in ListView_Remark
	        int TotalCount = Form_ViewInfo->ListView_Remark->Items->Count;
            CRemarkPlace *ptr;
    	    for(int i=0;i<TotalCount;i++)
        	{
                //delete CRemarkPlace * ptr
                //in SubItem [7] is CRemarkPlace Pointer
                ptr = (CRemarkPlace*)atol(Form_ViewInfo->ListView_Remark->Items->Item[i]->SubItems->Strings[RMK_PTR].c_str());
                if(ptr == pRemarks )
                {
                    Form_ViewInfo->ListView_Remark->Items->Item[i]->Delete();
                	break;
                }
            }//for
            DataModule1->Delete_DB_Value("tRemarks","SerialNo",AnsiString(pRemarks->i_SerialNo));
            //remove memory
    		m_pRemarksList->Remove(pRemarks);
			delete pRemarks;

		}//if MessageBox
    }
    PopupMenu_Remarks->Tag = 0;
}
//---------------------------------------------------------------------------

void __fastcall TCallCenter_Main_Form::MI_Remarks_ChangeClick(
      TObject *Sender)
{//�ק鷺�e
        CRemarkPlace *pRemarks = (CRemarkPlace *)PopupMenu_Remarks->Tag; //�Q��PopupMenu_Remarks OnPop() �ƥ�w��X���l�������
        if(pRemarks)
        {
                Dlg_AddRemarks = new TDlg_AddRemarks(this);
                Dlg_AddRemarks->OKBtn->Caption = "�ק�";
		Dlg_AddRemarks->m_pOldRemarks = pRemarks;
                Dlg_AddRemarks->ComboBox_Company->Text = pRemarks->m_strCompanyName;
                Dlg_AddRemarks->ComboBox_Company->Enabled = false;
                Dlg_AddRemarks->ComboBox_Class->ItemIndex = pRemarks->m_RemarkType;
                Dlg_AddRemarks->m_strImagePath = pRemarks->m_strImagePath;
    	        Dlg_AddRemarks->Edit_Name->Text = pRemarks->m_strName;
    	        Dlg_AddRemarks->Edit_Phone->Text = pRemarks->m_strPhone;
    	        Dlg_AddRemarks->Edit_Addr->Text = pRemarks->m_strAddr;
    	        Dlg_AddRemarks->Edit_Email->Text = pRemarks->m_strEmail;
    	        Dlg_AddRemarks->Edit_HomePage->Text = pRemarks->m_strWebAddr;
    	        Dlg_AddRemarks->Memo_Remarks->Lines->SetText(pRemarks->m_strRemarks);
    	        Dlg_AddRemarks->ShowModal();
        }
        PopupMenu_Remarks->Tag = 0;
}
//---------------------------------------------------------------------------

void __fastcall TCallCenter_Main_Form::MI_Remarks_ContentClick(
      TObject *Sender)
{//�ԲӤ��e
        CRemarkPlace *pRemarks = (CRemarkPlace *)PopupMenu_Remarks->Tag; //�Q��PopupMenu_Remarks OnPop() �ƥ�w��X���l�������
        if(pRemarks)
        {
		Dlg_AddRemarks = new TDlg_AddRemarks(this);
                Dlg_AddRemarks->OKBtn->Caption = "�T�w";
		Dlg_AddRemarks->m_pOldRemarks = pRemarks;
                Dlg_AddRemarks->ComboBox_Company->Text = pRemarks->m_strCompanyName;
                Dlg_AddRemarks->ComboBox_Company->Enabled = false;
                Dlg_AddRemarks->ComboBox_Class->ItemIndex = pRemarks->m_RemarkType;
                Dlg_AddRemarks->ComboBox_Class->Enabled = false;
                Dlg_AddRemarks->m_strImagePath = pRemarks->m_strImagePath;
    	        Dlg_AddRemarks->Edit_Name->Text = pRemarks->m_strName;
    	        Dlg_AddRemarks->Edit_Phone->Text = pRemarks->m_strPhone;
    	        Dlg_AddRemarks->Edit_Addr->Text = pRemarks->m_strAddr;
    	        Dlg_AddRemarks->Edit_Email->Text = pRemarks->m_strEmail;
    	        Dlg_AddRemarks->Edit_HomePage->Text = pRemarks->m_strWebAddr;
    	        Dlg_AddRemarks->Memo_Remarks->Lines->SetText(pRemarks->m_strRemarks);
                Dlg_AddRemarks->SpeedButton_Image->Enabled = false;
                Dlg_AddRemarks->Edit_Name->Enabled = false;
                Dlg_AddRemarks->Edit_Phone->Enabled = false;
                Dlg_AddRemarks->Edit_Addr->Enabled = false;
                Dlg_AddRemarks->Edit_Email->ReadOnly = true;
		Dlg_AddRemarks->Edit_Email->Cursor = crHandPoint;
                Dlg_AddRemarks->Edit_Email->OnClick = Dlg_AddRemarks->OnExecShell;
                Dlg_AddRemarks->Edit_Email->TabStop = false;
                Dlg_AddRemarks->Edit_HomePage->ReadOnly = true;
                Dlg_AddRemarks->Edit_HomePage->Cursor = crHandPoint;
      	        Dlg_AddRemarks->Edit_HomePage->OnClick = Dlg_AddRemarks->OnExecShell;
                Dlg_AddRemarks->Edit_HomePage->TabStop = false;
                Dlg_AddRemarks->Memo_Remarks->Enabled = false;
                Dlg_AddRemarks->ShowModal();
        }
        PopupMenu_Remarks->Tag = 0;
}
//---------------------------------------------------------------------------
////////////  for Toggle ToolBars  //////////////
void __fastcall TCallCenter_Main_Form::Menu_ToolBar_Toggle(TCoolBand* pBandItem,TMenuItem* pMenuItem)
{
	pBandItem->Visible = !(pBandItem->Visible);
	pMenuItem->Checked = !(pMenuItem->Checked);
	ReDrawImage(Image_Map,m_dScale);
    UpdateAllObjPos();
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::Menu_MapOp_DragMapClick(
      TObject *Sender)
{//
	Menu_MapOp_DragMap->Checked = true;
    Menu_MapOp_Pointer->Checked = false;
    Menu_MapOp_SetPath->Checked = false;

    SpeedButton_DragMap->Down = true;

    m_PointerType = 1;		//0:Pointer , 1:DragMap , 2:SetPath
    Image_Map->Cursor = (TCursor)crDragHandCur;
    Image_Map->PopupMenu = PopupMenu_MainForm;

	//If ���ɥ��b�e���w���u => Clear it
	if(m_bIsBeginDrawPath==true)
	{
		m_bIsBeginDrawPath = false;
    	if(m_pDefaultSelPath)
        {
    		delete m_pDefaultSelPath;
            m_pDefaultSelPath = NULL;
        }
    }//if the First Time
    
}
//---------------------------------------------------------------------------

void __fastcall TCallCenter_Main_Form::Menu_MapOp_PointerClick(TObject *Sender)
{//
	Menu_MapOp_DragMap->Checked = false;
    Menu_MapOp_Pointer->Checked = true;
    Menu_MapOp_SetPath->Checked = false;

    SpeedButton_Pointer->Down = true;

    m_PointerType = 0;		//0:Pointer , 1:DragMap , 2:SetPath
    Image_Map->Cursor = crDefault;
    Image_Map->PopupMenu = PopupMenu_MainForm;

	//If ���ɥ��b�e���w���u => Clear it
	if(m_bIsBeginDrawPath==true)
	{
		m_bIsBeginDrawPath = false;
    	if(m_pDefaultSelPath)
        {
    		delete m_pDefaultSelPath;
            m_pDefaultSelPath = NULL;
        }
    }//if the First Time


}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::Menu_MapOp_SetPathClick(
      TObject *Sender)
{//
      Menu_MapOp_DragMap->Checked = false;
      Menu_MapOp_Pointer->Checked = false;
      Menu_MapOp_SetPath->Checked = true;

      SpeedButton_SetPath->Down = true;

      m_PointerType = 2;		//0:Pointer , 1:DragMap , 2:SetPath
      Image_Map->Cursor = crCross;
      Image_Map->PopupMenu = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TCallCenter_Main_Form::Image_MapMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if( (m_PointerType == 1) )   //0:Pointer , 1:DragMap , 2:SetPath
        {   //(Button==mbLeft)&&
    	        Image_Map->Cursor = (TCursor)crDragMoveCur;
		m_bIsBeginDragMap = true;
    	        m_Drag_Origin_sX = X;	//�ù��y��
    	        m_Drag_Origin_sY = Y;	//�ù��y��
        }
        else
    	        m_bIsBeginDragMap = false;

        //for Begin Draw Path
        if( m_PointerType == 2 )   //0:Pointer , 1:DragMap , 2:SetPath
        {
		if(Button==mbLeft)
                {
    		        m_OriginPt=Point(m_temp_X, m_temp_Y);
    		        m_MovePt=m_OriginPt;
                        //if SelectPath is exist Clear it and add new
                        if(m_bIsBeginDrawPath==false)	//�Ĥ@���}�l Draw �ɥ��M�����e�����w���u
                        {
            	                if(m_pDefaultSelPath)
                	                m_pDefaultSelPath->ClearPoint();
                                else
                	                m_pDefaultSelPath = new TCSelectPath(this);
                        }//if �Ĥ@���}�l Draw
                        m_pDefaultSelPath->AddPoint(m_temp_X,m_temp_Y);
                        m_bIsBeginDrawPath = true;
                        m_pDefaultSelPath->Radius = PathArea; //���w���u���ʱ��d��
                        ReDrawImage(Image_Map,m_dScale);
                }//if mbLeft
                else if(Button==mbRight)
                {
        	        if(m_bIsBeginDrawPath)
         		        Image_Map->PopupMenu = PopupMenu_SetPath;
                        else
                                Image_Map->PopupMenu = NULL;
                }
        }//if SetPath
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::Image_MapMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if( (m_PointerType == 0)&&(Button==mbLeft) )   //0:Pointer , 1:DragMap , 2:SetPath
        {
                DrawImage(Image_Map,X,Y,m_dScale,1); //Use �ù��y�� Draw
    	        UpdateAllObjPos();
        }
	if( (m_bIsBeginDragMap == true) )
        {   //(Button==mbLeft)&&
		m_bIsBeginDragMap = false;
                Image_Map->Cursor = (TCursor)crDragHandCur;
        }
}
//---------------------------------------------------------------------------
void __fastcall	TCallCenter_Main_Form::DrawPathLine(TCanvas* pCanvas, TPoint ptOrigin, TPoint ptNow, TPenMode AMode,TColor clColor,int Width )
{//TPoint ���HWGS�y�Ъ��
        //�ন�ù��y��
	WGS2Hdc(ptOrigin.x,ptOrigin.y,ptOrigin.x,ptOrigin.y);
	WGS2Hdc(ptNow.x,ptNow.y,ptNow.x,ptNow.y);

	TColor oldColor = pCanvas->Pen->Color;
        TPenMode oldMode = pCanvas->Pen->Mode;
        int oldPenWidth = pCanvas->Pen->Width;

        pCanvas->Pen->Color = clColor;
        pCanvas->Pen->Mode = AMode;	// pmMask pmNotXor   pmMerge
        pCanvas->Pen->Width = Width;

        //Draw Main Line
        pCanvas->MoveTo(ptOrigin.x, ptOrigin.y);
        pCanvas->LineTo(ptNow.x, ptNow.y);

        //Draw the Limited Area
        pCanvas->Pen->Color = clYellow;
        pCanvas->Pen->Width = 2*(PathArea)/((double)m_dScale*0.10089008517); //(0.000001 Deg = 0.10089008517 M) �n��25��
        pCanvas->MoveTo(ptOrigin.x, ptOrigin.y);
        pCanvas->LineTo(ptNow.x, ptNow.y);

        pCanvas->Pen->Color = oldColor;
        pCanvas->Pen->Mode = oldMode;	//pmMerge
        pCanvas->Pen->Width = oldPenWidth;
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::MenuItem_0_DefaultClick(
      TObject *Sender)
{   //�אּ�p�⨮
	CCar *pCar = (CCar *)PopupMenu_Car->Tag; //�Q��PopupMenu_CarPopup �w��X���l�������
        //��s��Ʈw���
        TQuery* pQuery_CarInfo = new TQuery(DataModule1);
        pQuery_CarInfo->DatabaseName = "DB_DCH";
        pQuery_CarInfo->SQL->Add("UPDATE tCarInfo");
        pQuery_CarInfo->SQL->Add("SET Image = 0");
        pQuery_CarInfo->SQL->Add("WHERE LocatorNo = :LocatorNo");
        pQuery_CarInfo->ParamByName("LocatorNo")->AsString = pCar->GetLocatorNo();
        pQuery_CarInfo->ExecSQL();
        delete pQuery_CarInfo;
        if(pCar)
        {   //0:�p�⨮ , 1:�f�� , 2:�s���� , 3:ĵ�� , 4:������
    	        if(pCar->m_CarType!=0)
                {
        	        pCar->m_CarType=0;
    		        if( pCar->m_pImage_Car->Tag > 0 )
    		        {
	    		        switch((pCar->m_pImage_Car->Tag)%5)
    			        {
    				        case 1:	pCar->m_pImage_Car->Tag = 1;		break;
    				        case 2:	pCar->m_pImage_Car->Tag = 2;		break;
    				        case 3:	pCar->m_pImage_Car->Tag = 3;		break;
                                        case 4: pCar->m_pImage_Car->Tag = 4;            break;
                                        case 0:	pCar->m_pImage_Car->Tag = 5;		break;
    			        }
        	        }
    		        else
    		        {
	    		        switch((pCar->m_pImage_Car->Tag)%5)
    			        {
    				        case -1:pCar->m_pImage_Car->Tag = -1;		break;
    				        case -2:pCar->m_pImage_Car->Tag = -2;		break;
                                        case -3:pCar->m_pImage_Car->Tag = -3;		break;
                                        case -4:pCar->m_pImage_Car->Tag = -4;		break;
    				        case 0:	pCar->m_pImage_Car->Tag = -5;		break;
    			        }
    		        }
                        //�󴫹ϥܥH�����I����
    		        int Origin_X = pCar->m_pImage_Car->Left+pCar->m_pImage_Car->Width/2;    //�ϧΤ����I=> X
    		        int Origin_Y = pCar->m_pImage_Car->Top+pCar->m_pImage_Car->Height/2;    //�ϧΤ����I=> Y
        	        ChangeCarImgByTag(pCar->m_pImage_Car,1);
    		        pCar->m_pImage_Car->Left = Origin_X - pCar->m_pImage_Car->Width/2;
    		        pCar->m_pImage_Car->Top = Origin_Y - pCar->m_pImage_Car->Height/2;
                }
        }
        PopupMenu_Car->Tag = 0;
}
//---------------------------------------------------------------------------

void __fastcall TCallCenter_Main_Form::MenuItem_1_TruckClick(
      TObject *Sender)
{// //�אּ�f��
	CCar *pCar = (CCar *)PopupMenu_Car->Tag; //�Q��PopupMenu_CarPopup �w��X���l�������
        //��s��Ʈw���
        TQuery* pQuery_CarInfo = new TQuery(DataModule1);
        pQuery_CarInfo->DatabaseName = "DB_DCH";
        pQuery_CarInfo->SQL->Add("UPDATE tCarInfo");
        pQuery_CarInfo->SQL->Add("SET Image = 1");
        pQuery_CarInfo->SQL->Add("WHERE LocatorNo = :LocatorNo");
        pQuery_CarInfo->ParamByName("LocatorNo")->AsString = pCar->GetLocatorNo();
        pQuery_CarInfo->ExecSQL();
        delete pQuery_CarInfo;
        if(pCar)
        {   //0:�p�⨮ , 1:�f�� , 2:�s���� , 3:ĵ�� , 4:������
    	        if(pCar->m_CarType!=1)
                {
        	        pCar->m_CarType=1;
    		        if( pCar->m_pImage_Car->Tag > 0 )
    		        {
	    		        switch((pCar->m_pImage_Car->Tag)%5)
    			        {
    				        case 1:	pCar->m_pImage_Car->Tag = 6;		break;
    				        case 2:	pCar->m_pImage_Car->Tag = 7;		break;
    				        case 3:	pCar->m_pImage_Car->Tag = 8;		break;
                                        case 4:	pCar->m_pImage_Car->Tag = 9;		break;
    				        case 0:	pCar->m_pImage_Car->Tag = 10;		break;
    			        }
        	        }
    		        else
    		        {
	    		        switch((pCar->m_pImage_Car->Tag)%5)
    			        {
    				        case -1:pCar->m_pImage_Car->Tag = -6;		break;
    				        case -2:pCar->m_pImage_Car->Tag = -7;		break;
    				        case -3:pCar->m_pImage_Car->Tag = -8;		break;
                                        case -4:pCar->m_pImage_Car->Tag = -9;		break;
    			        	case 0:	pCar->m_pImage_Car->Tag = -10;		break;
    			        }
    		        }
                        //�H�����I����
    		        int Origin_X = pCar->m_pImage_Car->Left+pCar->m_pImage_Car->Width/2;    //�ϧΤ����I=> X
    		        int Origin_Y = pCar->m_pImage_Car->Top+pCar->m_pImage_Car->Height/2;    //�ϧΤ����I=> Y
        	        ChangeCarImgByTag(pCar->m_pImage_Car,1);
    		        pCar->m_pImage_Car->Left = Origin_X - pCar->m_pImage_Car->Width/2;
    		        pCar->m_pImage_Car->Top = Origin_Y - pCar->m_pImage_Car->Height/2;
                }
        }
        PopupMenu_Car->Tag = 0;
}
//---------------------------------------------------------------------------

void __fastcall TCallCenter_Main_Form::MenuItem_2_ContainerClick(
      TObject *Sender)
{// //�אּ�s����
	CCar *pCar = (CCar *)PopupMenu_Car->Tag; //�Q��PopupMenu_CarPopup �w��X���l�������
        //��s��Ʈw���
        TQuery* pQuery_CarInfo = new TQuery(DataModule1);
        pQuery_CarInfo->DatabaseName = "DB_DCH";
        pQuery_CarInfo->SQL->Add("UPDATE tCarInfo");
        pQuery_CarInfo->SQL->Add("SET Image = 2");
        pQuery_CarInfo->SQL->Add("WHERE LocatorNo = :LocatorNo");
        pQuery_CarInfo->ParamByName("LocatorNo")->AsString = pCar->GetLocatorNo();
        pQuery_CarInfo->ExecSQL();
        delete pQuery_CarInfo;
        if(pCar)
        {   //0:�p�⨮ , 1:�f�� , 2:�s���� , 3:ĵ�� , 4:������
    	        if(pCar->m_CarType!=2)
                {
        	        pCar->m_CarType=2;
    		        if( pCar->m_pImage_Car->Tag > 0 )
    		        {
	    		        switch((pCar->m_pImage_Car->Tag)%5)
    			        {
    				        case 1:	pCar->m_pImage_Car->Tag = 11;		break;
    				        case 2:	pCar->m_pImage_Car->Tag = 12;		break;
    				        case 3:	pCar->m_pImage_Car->Tag = 13;		break;
                                        case 4:	pCar->m_pImage_Car->Tag = 14;		break;
    				        case 0:	pCar->m_pImage_Car->Tag = 15;		break;
    			        }
        	        }
    		        else
    		        {
	    		        switch((pCar->m_pImage_Car->Tag)%5)
    			        {
    				        case -1:pCar->m_pImage_Car->Tag = -11;		break;
    				        case -2:pCar->m_pImage_Car->Tag = -12;		break;
    				        case -3:pCar->m_pImage_Car->Tag = -13;		break;
                                        case -4:pCar->m_pImage_Car->Tag = -14;		break;
    				        case 0:	pCar->m_pImage_Car->Tag = -15;		break;
    			        }
    		        }
                        //�H�����I����
    		        int Origin_X = pCar->m_pImage_Car->Left+pCar->m_pImage_Car->Width/2;    //�ϧΤ����I=> X
    		        int Origin_Y = pCar->m_pImage_Car->Top+pCar->m_pImage_Car->Height/2;    //�ϧΤ����I=> Y
        	        ChangeCarImgByTag(pCar->m_pImage_Car,1);
    		        pCar->m_pImage_Car->Left = Origin_X - pCar->m_pImage_Car->Width/2;
    		        pCar->m_pImage_Car->Top = Origin_Y - pCar->m_pImage_Car->Height/2;
                }
        }
        //reset
        PopupMenu_Car->Tag = 0;
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::MenuItem_AddTraceWndClick(
      TObject *Sender)
{
        CCar *pCar = (CCar *)PopupMenu_Car->Tag; //�Q��PopupMenu_CarPopup �w��X���l�������
        if(!pCar)
    	        return;
	if(m_pSubFormList->Count > MAX_SUBFORM_COUNT -1)
        {
    	        char str[50];
    	        wsprintf(str,"�h�����ʱ��̤j�ƶq�� %d �� !",MAX_SUBFORM_COUNT);
                ShowMessage(str);
    	        return;
        }
	TCallCenter_Sub_Form *CallCenter_Sub_Form = new TCallCenter_Sub_Form(Application);
        m_pSubFormList->Add(CallCenter_Sub_Form);
        CallCenter_Sub_Form->Caption = AnsiString("�l�ܺʱ����� --- ����: ")+AnsiString(pCar->GetCarID());
        CallCenter_Sub_Form->m_pCar = pCar;
        CallCenter_Sub_Form->m_dScale = m_dScale;  	  //Image_Map Scale
        CallCenter_Sub_Form->Show();	//free Memory at OnClose()
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::Menu_CarClick(TObject *Sender)
{
	Form_ViewInfo->PageControl1->ActivePageIndex = 0;	//������T
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::Menu_RemarkClick(TObject *Sender)
{
	Form_ViewInfo->PageControl1->ActivePageIndex = 1;	//�Ƶ��a�I
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::Menu_GISClick(TObject *Sender)
{
        Form_Setting->PageControl1->ActivePage = Form_Setting->TabMap;
        if(!Form_Setting->Visible)
                Form_Setting->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::Menu_ParameterClick(TObject *Sender)
{
        Form_Setting->PageControl1->ActivePage = Form_Setting->TabPath;
        if(!Form_Setting->Visible)
                Form_Setting->Show();
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::FormClose(TObject *Sender,
      TCloseAction &Action)
{
        //Terminate Thread
        DeleteThread();
        TIniFile* ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ));
        long Center_X,Center_Y;
        Hdc2WGS((Image_Map->Width/2),(Image_Map->Height/2),Center_X,Center_Y);
        ini->WriteString("MAP","LASTSHOW_X",Center_X);
        ini->WriteString("MAP","LASTSHOW_Y",Center_Y);
        ini->WriteString("MAP","LASTSHOW_SCALE",m_dScale);
        delete ini;
        Hide();
        Form_FreeAll->Show();
        Form_FreeAll->Repaint();
        //DisConnect Database Connection
        DataModule1->Table_Cmd->Active = false;
        DataModule1->Database_DCH->Connected = false;
        if(Form_SmartQry)
    	        Form_SmartQry->Close();
        //Close Report EXE
        CloseReportEXE(ReportClass_Name,NULL);
        Sleep(300);     //��0.3��,���Ѩ�L�{�������ɶ�
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::CheckBox_DrawColorAreaClick(
      TObject *Sender)
{
	ReDrawImage(Image_Map,m_dScale);
}
//---------------------------------------------------------------------------
//////////////  ���w���u�ʱ�   ///////////////
void __fastcall TCallCenter_Main_Form::MenuItem_SetSelPathClick(
      TObject *Sender)
{
        if(!Form_CustRoute)
        {
                Form_CustRoute = new TForm_CustRoute(this);
        }
        CCar *pCar = (CCar *)PopupMenu_Car->Tag; //�Q��PopupMenu_CarPopup �w��X���l�������
        Form_CustRoute->PageControl1->ActivePage = Form_CustRoute->Tab_Assign;
        Form_CustRoute->ComboBox1->Text = pCar->GetCarID();
        Form_CustRoute->ComboBox1Change(Sender);
        Form_CustRoute->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::MenuItem_CancelSelPathClick(
      TObject *Sender)
{
        CCar *pCar = (CCar *)PopupMenu_Car->Tag; //�Q��PopupMenu_CarPopup �w��X���l�������
        if(pCar)
        {
    	        pCar->RouteNo = 0;
                pCar->m_bIsOverPath = false;
                pCar->UpdateHint();
        }
}
//---------------------------------------------------------------------------
/////////////     Set Path Popup Menu    ///////////////
void __fastcall TCallCenter_Main_Form::MenuItem_AddToPathGroupClick(
      TObject *Sender)
{//change from vincent
        m_pDefaultSelPath->AddPoint(m_temp_X,m_temp_Y);  //�N�̫�@�I�[�J
        m_pDefaultSelPath->CoID = "Common";             //�Ȯɫ��w���q�θ��u
        AddSelPathGroup();                                  //�[�J��m_pSelPathGroupList
        m_pDefaultSelPath->AddToDB(Sender);                       //�[�J��ƮwtCustAutRoute
        //�i��H�u�ק�
        if(!Form_RouteEditor)
                Form_RouteEditor = new TForm_RouteEditor(this);
        //���w���|�������
        Form_RouteEditor->pPath =  m_pDefaultSelPath;
        //���wRouteNo
        Form_RouteEditor->RouteNo = m_pDefaultSelPath->RouteNo;
        //ComboBox_Company�[�J���q�W��
        CCustomer* pCustomer;
        Form_RouteEditor->ComboBox_Company->Items->Clear();
        Form_RouteEditor->ComboBox_Company->Items->Add("�q�θ��u");       //�w�]���q�θ��u
        Form_RouteEditor->ComboBox_Company->ItemIndex = 0;
        for(int i=0; i < Form_ChooseCar->CustomerList->List->Count; i++)        //�[�J��L���qList
        {
                pCustomer = (CCustomer*)Form_ChooseCar->CustomerList->List->Items[i];
                Form_RouteEditor->ComboBox_Company->Items->Add(pCustomer->FullName);
        }
        //�[�J��L�ݩ�
        Form_RouteEditor->Edit_RouteNo->Text = Form_RouteEditor->Edit_RouteNo->Text + IntToStr(m_pDefaultSelPath->RouteNo);
        Form_RouteEditor->Edit_RouteName->Text = m_pDefaultSelPath->PathName;
        Form_RouteEditor->Edit_StartName->Text = m_pDefaultSelPath->StartPoint;
        Form_RouteEditor->Edit_EndName->Text = m_pDefaultSelPath->EndPoint;
        Form_RouteEditor->Edit_Radius->Text = m_pDefaultSelPath->Radius;
        Form_RouteEditor->BitBtnCancel->Visible = false;
        Form_RouteEditor->ShowModal();
        m_pDefaultSelPath = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::MenuItem_ClearThisPathClick(
      TObject *Sender)
{//
	//�M�������w���u
    m_bIsBeginDrawPath = false;
    if(m_pDefaultSelPath)
    {
    	delete m_pDefaultSelPath;
        m_pDefaultSelPath = NULL;
    }
	ReDrawImage(Image_Map,m_dScale);
}
////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::ToolButton_CancelAllLockClick(
      TObject *Sender)
{
	//UnLock others
	TImage* pCarImage;
    int Origin_X ;    //�ϧΤ����I=> X
    int Origin_Y ;    //�ϧΤ����I=> Y
	CCar *pCar;

	//Set Car Locked => First UnLock others
    for(int i=0;i<m_pCarList->Count;i++)
    {
    	pCar = (CCar*)m_pCarList->Items[i];
        if( pCar->m_pImage_Car->Tag < 0 )
        {//Image Tag Status =>see Common.h class CCar
        	pCarImage = pCar->m_pImage_Car;
    		Origin_X = pCar->m_pImage_Car->Left+pCar->m_pImage_Car->Width/2;    //�ϧΤ����I=> X
    		Origin_Y = pCar->m_pImage_Car->Top+pCar->m_pImage_Car->Height/2;    //�ϧΤ����I=> Y

     		// Do UnLock
        	//Image Tag => see "Common.h"
        	ChangeCarImgByTag(pCar->m_pImage_Car);

    		pCar->m_pImage_Car->Left = Origin_X - pCarImage->Width/2;
    		pCar->m_pImage_Car->Top = Origin_Y - pCarImage->Height/2;

            //Update Monitor Status
            m_bIsLockMonitor = false;
            StaticText_MonitorSattus->Caption = "�ϰ�ʱ�";
			StaticText_MonitorSattus->Font->Color = clGreen;
    		//Update Red Monitor Rectangle
			ReDrawImage(Image_Map,m_dScale);
            return;    	//Only one can be Locked
    	}
    }//for
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::MenuItem_ShowCarTraceClick(
      TObject *Sender)
{
	CCar *ptrCar = (CCar *)PopupMenu_Car->Tag; //�Q��PopupMenu_CarPopup �w��X���l�������
        if(ptrCar)
        {
    	        if(ptrCar->m_pCarTraceList==NULL)
                {//Create New
        	        ptrCar->m_pCarTraceList = new TList();
                        m_pDrawTraceCarList->Add(ptrCar);
                        //Add sSelectPoint Point in CCar::Update()
        	        //Draw Car Trace in DrawImage()
                }
        }
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::MenuItem_CancelCarTraceClick(
      TObject *Sender)
{//
	CCar *ptrCar = (CCar *)PopupMenu_Car->Tag; //�Q��PopupMenu_CarPopup �w��X���l�������
        if(ptrCar)
        {
    	        if(ptrCar->m_pCarTraceList)
                {//Free all Memory
                        for(int i=0;i<ptrCar->m_pCarTraceList->Count;i++)
            	                delete ((sSelectPoint*)ptrCar->m_pCarTraceList->Items[i]);
                        ptrCar->m_pCarTraceList->Clear();
                        delete (ptrCar->m_pCarTraceList);
        	        ptrCar->m_pCarTraceList=NULL;
                        m_pDrawTraceCarList->Remove(ptrCar);
                        ReDrawImage(Image_Map,m_dScale);
                }
	}
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::CopyAllObjToQRImg(TQRImage* pQRImage,int CopyType)
{ 	//CopyType=0 =>�u���a��, 1 =>�a��+���l , 2 =>�a��+�a�I , 3 =>�a��+���l+�a�I
    //�`�N ScaleFoctor �ե�	=> �ù��ϻP�L��Ϫ��ѪR�פ��P
    int left,top,right,bottom;
    Graphics::TBitmap*	bitmap_Map;
    bitmap_Map = new Graphics::TBitmap();
    bitmap_Map->Width = Image_Map->Width;       //Just for use TPanel change Pos
    bitmap_Map->Height = Image_Map->Height;
    bitmap_Map->Assign(Image_Map->Picture->Bitmap);

    if(CopyType==0)
    {
        pQRImage->Picture->Graphic = bitmap_Map;
    	delete bitmap_Map;
    	return;
	}

    if( (CopyType==1)||(CopyType==3) )
    {//Copy All Cars Image to Printer
		CCar *ptrCar;
    	for(int i=0;i<m_pCarList->Count;i++)
    	{
    		ptrCar = (CCar*)(m_pCarList->Items[i]);
    		if(IsInDrawRect(ptrCar->m_Status.m_iLon,ptrCar->m_Status.m_iLat))
            {
       			//�`�N�٭�ե��] ToolBar(X),CoolBsr(Y) �ް_���첾
            	left = ptrCar->m_Center_Xs - ptrCar->m_pImage_Car->Width/2;
                top =  ptrCar->m_Center_Ys - ptrCar->m_pImage_Car->Height/2;
                right = left + ptrCar->m_pImage_Car->Width;
                bottom= top + ptrCar->m_pImage_Car->Height;
        		bitmap_Map->Canvas->CopyRect(TRect(left,top,right,bottom),ptrCar->m_pImage_Car->Canvas,ptrCar->m_pImage_Car->ClientRect);
			}
    	}//for
    }//if  �a��+���l

    if( (CopyType==2)||(CopyType==3) )
    { //Copy All RemarkPlace Image to Printer
		CRemarkPlace *ptrRemarks;
    	for(int i=0;i<m_pRemarksList->Count;i++)
    	{
    		ptrRemarks = (CRemarkPlace*)(m_pRemarksList->Items[i]);
    		if(IsInDrawRect(ptrRemarks->m_X2,ptrRemarks->m_Y2))
            {
       			//�`�N�٭�ե��] ToolBar(X),CoolBsr(Y) �ް_���첾
            	left = ptrRemarks->m_Center_Xs - ptrRemarks->m_pImage_Remarks->Width/2;
                top =  ptrRemarks->m_Center_Ys - ptrRemarks->m_pImage_Remarks->Height/2;
                right = left + ptrRemarks->m_pImage_Remarks->Width;
                bottom= top + ptrRemarks->m_pImage_Remarks->Height;
        		bitmap_Map->Canvas->CopyRect(TRect(left,top,right,bottom),ptrRemarks->m_pImage_Remarks->Canvas,ptrRemarks->m_pImage_Remarks->ClientRect);
            }
    	}//for
    }//if �a��+�a�I

    pQRImage->Picture->Graphic = bitmap_Map;
    //free Memory
    delete bitmap_Map;
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::Menu_SYS_PrintAllClick(
      TObject *Sender)
{
        TForm_Report *Form_Report = new TForm_Report(this);
        CopyAllObjToQRImg(Form_Report->QRImage_Map,3);	//CopyType=0 =>�u���a��, 1 =>�a��+���l , 2 =>�a��+�a�I , 3 =>�a��+���l+�a�I
        Form_Report->QuickRep_Map->Preview();
        delete Form_Report;
        chdir(CallCenter_Main_Form->WorkingPath);
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::Menu_SYS_Print_Map_CarsClick(
      TObject *Sender)
{
	TForm_Report *Form_Report = new TForm_Report(this);
    CopyAllObjToQRImg(Form_Report->QRImage_Map,1);	//CopyType=0 =>�u���a��, 1 =>�a��+���l , 2 =>�a��+�a�I , 3 =>�a��+���l+�a�I
    Form_Report->QuickRep_Map->Preview();
    delete Form_Report;
    chdir(CallCenter_Main_Form->WorkingPath);
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::Menu_SYS_Print_Map_RemarksClick(
      TObject *Sender)
{
	TForm_Report *Form_Report = new TForm_Report(this);
    CopyAllObjToQRImg(Form_Report->QRImage_Map,2);	//CopyType=0 =>�u���a��, 1 =>�a��+���l , 2 =>�a��+�a�I , 3 =>�a��+���l+�a�I
    Form_Report->QuickRep_Map->Preview();
    delete Form_Report;
    chdir(CallCenter_Main_Form->WorkingPath);
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::Menu_SYS_Print_MapOnlyClick(
      TObject *Sender)
{
	TForm_Report *Form_Report = new TForm_Report(this);
    CopyAllObjToQRImg(Form_Report->QRImage_Map,0);	//CopyType=0 =>�u���a��, 1 =>�a��+���l , 2 =>�a��+�a�I , 3 =>�a��+���l+�a�I
    Form_Report->QuickRep_Map->Preview();
    delete Form_Report;
    chdir(CallCenter_Main_Form->WorkingPath);
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::FormResize(TObject *Sender)
{	//��������=> FormClose()->FormResize()->FormDestroy()->FormResize()
        if(!Application->Terminated)  //�קKTerminated�ɵo�ͭ��e�ӷ��
        {
		ReDrawImage(Image_Map,m_dScale);
        }
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::Menu_SYS_CloseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::MenuItem_ShowAllSelPathClick(
      TObject *Sender)
{
	Form_Setting->BitBtn_ShowAllSelPathClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::PopupMenu_ListViewPopup(
      TObject *Sender)
{   //���p�u��ܤ@�Ө��l,1.�⨮�l��CCar���Ч�X,�üȦs��Tag
    //                   2.��ܩҦ��ۮe���O
        MenuItemLV_CarHisTrace->Visible = true;
        //�S���郞�ɥ�������
        if(Form_ViewInfo->ListView_Car->Selected == NULL)
        {
                N_LVCar_Cmd->Visible = false;
                N8->Visible = false;
                N_LVCar_Watch->Visible = false;
                N_LVCar_Trace->Visible = false;
                N_LVCar_Type->Visible = false;
                MenuItem_ChgCarImg->Visible = false;
                N_CarInfo->Visible = false;
                IO2->Visible = false;
                return;
        }
        //��ܤ@�x��
        if(Form_ViewInfo->ListView_Car->SelCount == 1)
        {
        //Tag == 0���  == -1 �����,�o�O���F�v�����D
                if(N_LVCar_Cmd->Tag == 0)
                {
                        N_LVCar_Cmd->Visible = true;
                        N8->Visible = true;
                }
                else
                {
                        N_LVCar_Cmd->Visible = false;
                        N8->Visible = false;
                }
                if(N_LVCar_Watch->Tag == 0)
                {
                        N_LVCar_Watch->Visible = true;
                }
                else
                {
                        N_LVCar_Watch->Visible = false;
                }
                if(N_LVCar_Trace->Tag == 0)
                {
                        N_LVCar_Trace->Visible = true;
                }
                else
                {
                        N_LVCar_Trace->Visible = false;
                }
                if(N_LVCar_Type->Tag == 0)
                {
                        N_LVCar_Type->Visible = true;
                        MenuItem_ChgCarImg->Visible = true;
                }
                else
                {
                        N_LVCar_Type->Visible = false;
                        MenuItem_ChgCarImg->Visible = false;
                }
                N_LVCar_Trace->Visible = true;
                N_CarInfo->Visible = true;
                MenuItemLV_SetSelPath->Visible = true;
                IO2->Visible = true;
                CCar* ptrCar=NULL;
                TListItem* Item = Form_ViewInfo->ListView_Car->Selected;
                if(m_pCarList->Count == Form_ViewInfo->ListView_Car->Items->Count)
                {
                        ptrCar = (CCar*) m_pCarList->Items[Item->Index];
                        if(Item->SubItems->Strings[CAR_LOCATOR_NO] == (AnsiString)ptrCar->GetLocatorNo())
                        {
                                PopupMenu_Car->Tag = (long)ptrCar;      //�N�Q�����CCar address�ȰO��tag
                        }
                }
        }
        //���p�h����,�N�������ۮe���O����
        if(Form_ViewInfo->ListView_Car->SelCount > 1)
        {
                if(N_LVCar_Cmd->Tag == 0)
                {
                        N_LVCar_Cmd->Visible = true;
                        N8->Visible = true;
                }
                if(N_LVCar_Watch->Tag == 0)
                        N_LVCar_Watch->Visible = true;
                if(N_LVCar_Trace->Tag == 0)
                        N_LVCar_Trace->Visible = true;
                if(N_LVCar_Type->Tag == 0)
                {
                        N_LVCar_Type->Visible = true;
                        MenuItem_ChgCarImg->Visible = true;
                }
                N_LVCar_Trace->Visible = false;
                MenuItemLV_SetSelPath->Visible = false;
                N_CarInfo->Visible = false;
                IO2->Visible = true;
        }
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::Help1Click(TObject *Sender)
{
	ShellExecute(Handle, "open",HELP_FILE_PATH, NULL, NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::MenuItem_LoadHisCarTraceClick(
      TObject *Sender)
{
        CCar *ptrCar = (CCar *)PopupMenu_Car->Tag; //�Q��PopupMenu_CarPopup �w��X���l�������
        CHisTracer->LocatorNo = ptrCar->GetLocatorNo();   //��o�ұ��j�M�������X
        if(!Form_Trace)
                Form_Trace = new TForm_Trace(this);
        Form_Trace->LocatorNoBox->Text = CHisTracer->LocatorNo;
        Form_Trace->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::Menu_ReportClick(TObject *Sender)
{
        if(!ShowReportEXE(ReportClass_Name,NULL))
        {
                AnsiString parameter = Form_Login->LoginID + " " + Form_Login->LoginPwd;
    	        ShellExecute(Handle, "open","..\\Report\\DCH.exe", parameter.c_str(), "..\\Report", SW_SHOWNORMAL);
     	        //Set Top Most
		ShowReportEXE(ReportClass_Name,NULL);
        }
}
//---------------------------------------------------------------------------
bool __fastcall TCallCenter_Main_Form::ShowReportEXE(char* szClassName,char* szWndName)
{
	HWND hWnd = FindWindow(szClassName,szWndName); //Report Generator define in Common.h
        if(hWnd)
        {
                TRect rc;
                GetWindowRect(hWnd,&rc);
                SetWindowPos(hWnd,HWND_TOPMOST,0,0,rc.Width(),rc.Height(),SWP_NOMOVE|SWP_SHOWWINDOW);
                return true;
        }
        else
    	        return false;
}
//---------------------------------------------------------------------------
bool __fastcall TCallCenter_Main_Form::CloseReportEXE(char* szClassName,char* szWndName)
{
	HWND hWnd = FindWindow(szClassName,szWndName); //Report Generator define in Common.h
        if(hWnd)
        {
    	        PostMessage(hWnd,WM_CLOSE,0,0);
                return true;
        }
        else
    	        return false;
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::Image_MapMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
        Hdc2WGS(X,Y,m_temp_X, m_temp_Y);
        //if DragMap is true => Drag the Map
        //for Proformance ! if DragMap or SetPath will not draw ��D��j�� and Show RoadName
        //for Performance ! �԰ʦa�Ϧܤ� MIN_DRAG_MAP_PIXEL Pixel�ƥH�W�~�u�����ʦa�� (Image_Map,Image_ZoomIn,Image_Taiwan)
        bool bDrawMap = false;
        int plDelta_X = X-m_Drag_Origin_sX;
        plDelta_X = abs(plDelta_X);
        int plDelta_Y = Y-m_Drag_Origin_sY;
        plDelta_Y = abs(plDelta_Y);
        int drawPixelByScale = MIN_DRAG_MAP_PIXEL;
        if((plDelta_X > drawPixelByScale-1)||((plDelta_Y) > drawPixelByScale-1))
    	        bDrawMap = true;
        if(m_bIsBeginDragMap)
        {
                //(X-m_Drag_Origin_sX) =>�P��l�첾�I�ù��y�Ф��t
    	        //Drag and Draw Image_Map
                if(bDrawMap)
    	        {
        	        long Diff_X = (X-m_Drag_Origin_sX);
        	        long Diff_Y = (Y-m_Drag_Origin_sY);
                        //�ϥΤ����I�� "�ù�" �y��
			DrawImage(Image_Map,(Image_Map->Width/2)-Diff_X,(Image_Map->Height/2)-Diff_Y,m_dScale,1);
        	        UpdateAllObjPos();
    		        //DrawImage(Image_Map,m_iPos_X,m_iPos_Y,m_dScale);
    		        m_Drag_Origin_sX = X;	//�ù��y��
    		        m_Drag_Origin_sY = Y;	//�ù��y��
        	        return;  //for Proformance ! if DragMap or SetPath will not draw ��D��j�� and Show RoadName
                }
        }
        //for Draw Monitor Path
        if((m_bIsBeginDrawPath)&&(bDrawMap))
        {
                //�� DrawPathLine ���M���e�@���ҵe���|���@��  //TPoint ���HWGS�y�Ъ��
                DrawPathLine(Image_Map->Canvas,m_OriginPt,m_MovePt,pmNotXor,clRed,2);   //pmNotXor
                m_MovePt=Point(m_temp_X, m_temp_Y);
                DrawPathLine(Image_Map->Canvas,m_OriginPt,m_MovePt,pmNotXor,clRed,2);
    	        return;
        }
	//Update Status Bar  ->��Ԧa�Ϯɤ��� Update Status Bar �ε�D��
        char strXYPos[100];
        //Query IIS
        char* pStr = GetIISName(m_temp_X,m_temp_Y);
        StatusBar1->Panels->Items[0]->Text = pStr;
        //Show �y��
        char chLon,chLat;
        if(m_temp_X >0)	chLon='E';
        else		chLon='W';
        if(m_temp_Y >0)	chLat='N';
        else		chLat='S';
        sprintf(strXYPos,"�g�n�׮y��:(%c:%f , %c:%f)",chLon,(double)m_temp_X/1000000.0,chLat,(double)m_temp_Y/1000000.0);
        StatusBar1->Panels->Items[1]->Text = strXYPos;
        //Draw ZoomIn Image ��D�X�j��
        if( (Panel_Street->Visible)&& bDrawMap )
                DrawImage(Image_ZoomIn,m_temp_X,m_temp_Y,m_dScale_ZoomInWnd);
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::FormKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
        // Ctrl + Alt + F12 -> for switch Taiwan and HongKong Map
        // iMapType=0 -> TW Local Test , iMapType=1 -> HK Map
	if( (Key==(VK_CONTROL|VK_F12)) && (Shift.Contains(ssAlt)) )
        {
    	        if(g_iMapType==0)
    	        {
    		        g_iMapType = 1;
    		        InitDispDLL(g_iMapType);
        	        DrawImage(Image_Map,114175745,22315023,m_dScale);
                        UpdateAllObjPos();
    	        }
    	        else
    	        {
			g_iMapType = 0;
    		        InitDispDLL(g_iMapType);
    		        DrawImage(Image_Map,121298319,25016501,m_dScale);
                        UpdateAllObjPos();
    	        }
        }
        //the Zoom in[+] & Zoom out[-] Keboard switch
        if(Key == VK_ADD)
        {
                ToolButton_ZoomInClick(Sender);
        }
        else if(Key ==  VK_SUBTRACT)
        {
                ToolButton_ZoomOutClick(Sender);
        }
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::MenuItemLV_CarHisTraceClick(
      TObject *Sender)
{
        CHisTracer->LocatorNo = Form_ViewInfo->ListView_Car->Selected->SubItems->Strings[CAR_LOCATOR_NO];   //��o�ұ��j�M�������X
        if(!Form_Trace)
                Form_Trace = new TForm_Trace(this);
        for(int i=0; i < Form_Trace->LocatorNoBox->Items->Count; i++)
        {
                if(Form_Trace->LocatorNoBox->Items->Strings[i] == Form_ViewInfo->ListView_Car->Selected->Caption)
                {
                        Form_Trace->LocatorNoBox->ItemIndex = i;
                }
        }
        Form_Trace->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::AddSelPathGroup()
{
        if(m_pSelPathGroupList->Count  < MAX_SEL_PATH_COUNT )
        {
                //���w���u�W��begin
                sSelectPoint* pStartPoint = (sSelectPoint*)m_pDefaultSelPath->m_pSelPointList->Items[0];     //���u���Y(�Ĥ@���I)
                m_pDefaultSelPath->StartPoint = GetIISName(pStartPoint->mX2,pStartPoint->mY2);
                if(!m_pDefaultSelPath->StartPoint.IsEmpty())
                {
                        int position = m_pDefaultSelPath->StartPoint.Pos(":");
                        m_pDefaultSelPath->StartPoint.Delete(1,position);
                }
                sSelectPoint* pEndPoint = (sSelectPoint*)m_pDefaultSelPath->m_pSelPointList->Items[m_pDefaultSelPath->m_pSelPointList->Count-1];    //���u����(�̫�@���I)
                m_pDefaultSelPath->EndPoint = GetIISName(pEndPoint->mX2,pEndPoint->mY2);
                if(!m_pDefaultSelPath->EndPoint.IsEmpty())
                {
                        int position = m_pDefaultSelPath->EndPoint.Pos(":");
                        m_pDefaultSelPath->EndPoint.Delete(1,position);
                }
                AnsiString tmpstr = m_pDefaultSelPath->StartPoint + "-->" + m_pDefaultSelPath->EndPoint;
                m_pDefaultSelPath->PathName = tmpstr;
                //���w���u�W��end
                m_pDefaultSelPath->Radius = PathArea; //���w���u���ʱ��d��
                m_pSelPathGroupList->Add(m_pDefaultSelPath);        //�N�Ӹ��u�[�J�s��m_pSelPathGroupList��
                if(Form_CustRoute->Visible)     //���p���}��Form_CustRoute�h�n��s��ListView
                {
                        Form_CustRoute->Query1->Active = false;
                        Form_CustRoute->Query1->Active = true;
                }
        }
        else
        {
                AnsiString str("���w�ʱ����u�ƥضW�X�̤j�ƶq = ");
                str = str + AnsiString(MAX_SEL_PATH_COUNT) + "\n" + "�ЧR����L�����w���u��A���s�ާ@ !";
                ShowMessage(str);
        }
        //Draw This Path
        ReDrawImage(Image_Map,m_dScale);
        m_bIsBeginDrawPath = false;
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::Menu_RouteClick(TObject *Sender)
{
        Form_Setting->BitBtn_ShowAllSelPathClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::PopupMenu_LV_RemarksPopup(
      TObject *Sender)
{
        if(Form_ViewInfo->ListView_Remark->Selected == NULL)
        {
                MenuItem1->Visible = false;
                MenuItem2->Visible = false;
                MenuItem3->Visible = false;
                MenuItem4->Visible = false;
                return;
        }
        else
        {
                if(MenuItem1->Tag == 0)
                        MenuItem1->Visible = true;
                if(MenuItem2->Tag == 0)
                        MenuItem1->Visible = true;
                if(MenuItem3->Tag == 0)
                        MenuItem1->Visible = true;
                if(MenuItem4->Tag == 0)
                        MenuItem1->Visible = true;
                TListItem* pItem = Form_ViewInfo->ListView_Remark->Selected;
                CRemarkPlace *pRemarks = (CRemarkPlace*)atol(pItem->SubItems->Strings[RMK_PTR].c_str());
                PopupMenu_Remarks->Tag = (long)pRemarks;
        }
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::ToolButton_HKClick(TObject *Sender)
{
        m_dScale = 900;
        DrawImage(Image_Map,114120210,22357593,m_dScale);
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::SpeedButton_QueryClick(
      TObject *Sender)
{
   //Show Smart Query Form
        if(!Form_SmartQry)
        {
		Form_SmartQry = new TForm_SmartQry(this);
                SpeedButton_Query->Down = true;
                N_Query->Checked = true;
                Form_SmartQry->Show();	//free Memory at OnClose()
        }
        else
        {
                Form_SmartQry->Close();
        }
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::SpeedButton_HistoryClick(
      TObject *Sender)
{
        if(!Form_Trace)
        {
                Form_Trace = new TForm_Trace(this);
                Form_Trace->Visible = true;
        }
        else
        {
                Form_Trace->Close();
        }
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::Splitter1Moved(TObject *Sender)
{
        if(Splitter1->Left > 10)
        {
                DockSiteWidth = Splitter1->Left;
        }
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::InitialDraw()
{
        // iMapType=0 -> TW Local Test , iMapType=1 -> HK Map
        if(g_iMapType==0)
                DrawImage(Image_Map,121298319,25016501,m_dScale);
        else
        {
                TIniFile* ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ));
                long strx = ini->ReadFloat("MAP","LASTSHOW_X",114175745);
                long stry = ini->ReadFloat("MAP","LASTSHOW_Y",22315023);
                m_dScale = ini->ReadFloat("MAP","LASTSHOW_SCALE",m_dScale);
                delete ini;
                DrawImage(Image_Map,strx,stry,m_dScale);
        }
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::DeleteThread()
{
    ThreadDeleteing = true;
    if(Maintain_Thread)
    {
        Maintain_Thread->Terminate();
        Maintain_Thread->WaitFor();
        Maintain_Thread = NULL;
    }
    if(DataModule1->DB_Check_Thread)
    {
        DataModule1->DB_Check_Thread->Terminate();
        DataModule1->DB_Check_Thread->WaitFor();
        DataModule1->DB_Check_Thread = NULL;
    }
    ThreadDeleteing = false;
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::RebuildThread()
{
        DataModule1->bCanAccess.UnLock();          //�קK��Thread�Qdelete�ɨS���]�^UnLock�ҥH�o�̭��]��UnLock
        if(!Maintain_Thread)
        {
                Maintain_Thread = new CTMaintain(true);
        }
        if(!DataModule1->DB_Check_Thread)
        {
                DataModule1->DB_Check_Thread = new TGetTrackThread(true);
        }
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::Insert_CCar_QSort(CCar* pCar)
{
        if(pCar)
        {
                //�N�����[�JCCarList_SortByLocatorNo
                CCarList_SortByLocatorNo->Add(pCar);
                CCarList_SortByLocatorNo->Sort(Compare_CCar_By_LocatorNo);
                //�����[�JList���ñƧ�
                m_pCarList->Add(pCar);
                m_pCarList->Sort(Compare_CCar_By_CarID);
        }
}
//---------------------------------------------------------------------------
CCar* TCallCenter_Main_Form::GetCarByCarID(AnsiString CarID)
{
//�HCarID�ӧ�CCar point
//���return CCar else �䤣��return NULL
//�HBinary Search���覡�N������X
        int HeadPos = 0;
        int TailPos = m_pCarList->Count - 1;
        int Position;
        CCar* ptrCar;
        if(m_pCarList->Count == 0)                //List�S�����
        {
                return NULL;
        }
        if(m_pCarList->Count == 1)                //List�O�u���@�����
        {
                ptrCar = (CCar*)m_pCarList->Items[0];
                if(CarID == ptrCar->m_szCarID)
                {
                        return ptrCar;
                }
                else
                {
                        return NULL;
                }
        }
        for(Position = (TailPos + HeadPos) / 2; (HeadPos != Position); Position = (TailPos + HeadPos) / 2)
        {
                ptrCar = (CCar*)m_pCarList->Items[Position];
                if(CarID > ptrCar->m_szCarID)
                {
                        HeadPos = Position;
                }
                else if(CarID < ptrCar->m_szCarID)
                {
                        TailPos = Position;
                }
                else
                {
                        return ptrCar;          //���F
                }
        }
        //���Head and Tail��X�O���@��
        ptrCar = (CCar*)m_pCarList->Items[TailPos];
        if(CarID == ptrCar->m_szCarID)
        {
                return ptrCar;
        }
        ptrCar = (CCar*)m_pCarList->Items[HeadPos];
        if(CarID == ptrCar->m_szCarID)
        {
                return ptrCar;
        }
        //���S���return NULL
        return NULL;
}
//---------------------------------------------------------------------------
CCar* TCallCenter_Main_Form::GetCarByCarID_with_index(AnsiString CarID, int* index)
{
//�HCarID�ӧ�CCar point
//���return CCar else �䤣��return NULL
//�HBinary Search���覡�N������X
        int HeadPos = 0;
        int TailPos = m_pCarList->Count - 1;
        int Position;
        CCar* ptrCar;
        if(m_pCarList->Count == 0)                //List�S�����
        {
                return NULL;
        }
        if(m_pCarList->Count == 1)                //List�O�u���@�����
        {
                ptrCar = (CCar*)m_pCarList->Items[0];
                if(CarID == ptrCar->m_szCarID)
                {
                        *index = 0;
                        return ptrCar;
                }
                else
                {
                        return NULL;
                }
        }
        for(Position = (TailPos + HeadPos) / 2; (HeadPos != Position); Position = (TailPos + HeadPos) / 2)
        {
                ptrCar = (CCar*)m_pCarList->Items[Position];
                if(CarID > ptrCar->m_szCarID)
                {
                        HeadPos = Position;
                }
                else if(CarID < ptrCar->m_szCarID)
                {
                        TailPos = Position;
                }
                else
                {
                        *index = Position;
                        return ptrCar;          //���F
                }
        }
        //���Head and Tail��X�O���@��
        ptrCar = (CCar*)m_pCarList->Items[TailPos];
        if(CarID == ptrCar->m_szCarID)
        {
                *index = TailPos;
                return ptrCar;
        }
        ptrCar = (CCar*)m_pCarList->Items[HeadPos];
        if(CarID == ptrCar->m_szCarID)
        {
                *index = HeadPos;
                return ptrCar;
        }
        //���S���return NULL
        return NULL;
}
//---------------------------------------------------------------------------
CCar* TCallCenter_Main_Form::GetCarByLocatorNo(AnsiString LocatorNo)
{
//�HLocatorNo�ӧ�CCar point
//���return CCar else �䤣��return NULL
        int HeadPos = 0;
        int TailPos = CCarList_SortByLocatorNo->Count - 1;
        int Position;
        CCar* ptrCar;
        if(CCarList_SortByLocatorNo->Count == 0)                //List�S�����
        {
                return NULL;
        }
        if(CCarList_SortByLocatorNo->Count == 1)                //List�O�u���@�����
        {
                ptrCar = (CCar*)CCarList_SortByLocatorNo->Items[0];
                if(LocatorNo == (AnsiString)ptrCar->m_Status.m_szLocatorNo)
                {
                        return ptrCar;
                }
                else
                {
                        return NULL;
                }
        }
        for(Position = (TailPos + HeadPos) / 2; (HeadPos != Position); Position = (TailPos + HeadPos) / 2)
        {
                ptrCar = (CCar*)CCarList_SortByLocatorNo->Items[Position];
                if(LocatorNo > (AnsiString)ptrCar->m_Status.m_szLocatorNo)
                {
                        HeadPos = Position;
                }
                else if(LocatorNo < (AnsiString)ptrCar->m_Status.m_szLocatorNo)
                {
                        TailPos = Position;
                }
                else
                {
                        return ptrCar;          //���F
                }
        }
        //���Head and Tail��X�O���@��
        ptrCar = (CCar*)CCarList_SortByLocatorNo->Items[TailPos];
        if(LocatorNo == (AnsiString)ptrCar->m_Status.m_szLocatorNo)
        {
                return ptrCar;
        }
        ptrCar = (CCar*)CCarList_SortByLocatorNo->Items[HeadPos];
        if(LocatorNo == (AnsiString)ptrCar->m_Status.m_szLocatorNo)
        {
                return ptrCar;
        }
        //���S���return NULL
        return NULL;
}
//---------------------------------------------------------------------------
CCar* TCallCenter_Main_Form::GetCarByLocatorNo_with_index(AnsiString LocatorNo, int* index)
{
//�HLocatorNo�ӧ�CCar point
//���return CCar else �䤣��return NULL
        int HeadPos = 0;
        int TailPos = CCarList_SortByLocatorNo->Count - 1;
        int Position;
        CCar* ptrCar;
        if(CCarList_SortByLocatorNo->Count == 0)                //List�S�����
        {
                return NULL;
        }
        if(CCarList_SortByLocatorNo->Count == 1)                //List�O�u���@�����
        {
                ptrCar = (CCar*)CCarList_SortByLocatorNo->Items[0];
                if(LocatorNo == (AnsiString)ptrCar->m_Status.m_szLocatorNo)
                {
                        *index = 0;
                        return ptrCar;
                }
                else
                {
                        return NULL;
                }
        }
        for(Position = (TailPos + HeadPos) / 2; (HeadPos != Position); Position = (TailPos + HeadPos) / 2)
        {
                ptrCar = (CCar*)CCarList_SortByLocatorNo->Items[Position];
                if(LocatorNo > (AnsiString)ptrCar->m_Status.m_szLocatorNo)
                {
                        HeadPos = Position;
                }
                else if(LocatorNo < (AnsiString)ptrCar->m_Status.m_szLocatorNo)
                {
                        TailPos = Position;
                }
                else
                {
                        *index = Position;
                        return ptrCar;          //���F
                }
        }
        //���Head and Tail��X�O���@��
        ptrCar = (CCar*)CCarList_SortByLocatorNo->Items[TailPos];
        if(LocatorNo == (AnsiString)ptrCar->m_Status.m_szLocatorNo)
        {
                *index = TailPos;
                return ptrCar;
        }
        ptrCar = (CCar*)CCarList_SortByLocatorNo->Items[HeadPos];
        if(LocatorNo == (AnsiString)ptrCar->m_Status.m_szLocatorNo)
        {
                *index = HeadPos;
                return ptrCar;
        }
        //���S���return NULL
        return NULL;
}
//---------------------------------------------------------------------------
TCSelectPath* TCallCenter_Main_Form::GetRoute(int RouteNo)
{
//�HRouteNo��X�Ӹ��|,���p�S�������|,�hreturn NULL
        TCSelectPath* ptrPath;
        for(int i=0; i<CallCenter_Main_Form->m_pSelPathGroupList->Count; i++)
        {
                ptrPath = (TCSelectPath*)CallCenter_Main_Form->m_pSelPathGroupList->Items[i];
                if(ptrPath->RouteNo == RouteNo)
                        return(ptrPath);
        }
        return(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::ChangePasswordClick(TObject *Sender)
{
        Form_EditPassword->ShowModal();        
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::N_Equipment_SearchClick(
      TObject *Sender)
{
        //Show Smart Query Form
        if(!Form_SmartQry)
        {
		Form_SmartQry = new TForm_SmartQry(this);
        }
        Form_SmartQry->PageControl_Qry->ActivePage = Form_SmartQry->TabSheet_Poi;
        Form_SmartQry->Show();	//free Memory at OnClose()
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::N_Road_SearchClick(TObject *Sender)
{
        //Show Smart Query Form
        if(!Form_SmartQry)
        {
		Form_SmartQry = new TForm_SmartQry(this);
        }
        Form_SmartQry->PageControl_Qry->ActivePage = Form_SmartQry->TabSheet_Road;
        Form_SmartQry->Show();	//free Memory at OnClose()
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::N_Remarks_SearchClick(
      TObject *Sender)
{
        //Show Smart Query Form
        if(!Form_SmartQry)
        {
		Form_SmartQry = new TForm_SmartQry(this);
        }
        Form_SmartQry->PageControl_Qry->ActivePage = Form_SmartQry->TabSheet_Remark;
        Form_SmartQry->Show();	//free Memory at OnClose()
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::SpeedButton_StatManagerClick(
      TObject *Sender)
{
        if(Form_ManageState->Visible == false)
        {
                Form_ManageState->Show();
                N_StatManager->Checked = true;
                SpeedButton_StatManager->Down = true;
        }
        else
        {
                Form_ManageState->Close();
        }
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::MenuItemLV_AddTraceWndClick(
      TObject *Sender)
{
        for(int i=0; i<Form_ViewInfo->ListView_Car->Items->Count; i++)
        {
                if(Form_ViewInfo->ListView_Car->Items->Item[i]->Selected)
                {
                        AnsiString CarID = Form_ViewInfo->ListView_Car->Items->Item[i]->Caption;
                        CCar* pCar = GetCarByCarID(CarID);
                        if(!pCar)
    	                        return;
	                if(m_pSubFormList->Count > MAX_SUBFORM_COUNT -1)
                        {
    	                        char str[50];
    	                        wsprintf(str,"�h�����ʱ��̤j�ƶq�� %d �� !",MAX_SUBFORM_COUNT);
                                ShowMessage(str);
    	                        return;
                        }
	                TCallCenter_Sub_Form *CallCenter_Sub_Form = new TCallCenter_Sub_Form(Application);
                        m_pSubFormList->Add(CallCenter_Sub_Form);
                        CallCenter_Sub_Form->Caption = AnsiString("�l�ܺʱ����� --- ����: ")+AnsiString(pCar->GetCarID());
                        CallCenter_Sub_Form->m_pCar = pCar;
                        CallCenter_Sub_Form->m_dScale = m_dScale;  	  //Image_Map Scale
                        CallCenter_Sub_Form->Show();	//free Memory at OnClose()
                }
        }
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::MenuItemLV_CancelSelPathClick(
      TObject *Sender)
{
        for(int i=0; i<Form_ViewInfo->ListView_Car->Items->Count; i++)
        {
                if(Form_ViewInfo->ListView_Car->Items->Item[i]->Selected)
                {
                        AnsiString CarID = Form_ViewInfo->ListView_Car->Items->Item[i]->Caption;
                        CCar* pCar = GetCarByCarID(CarID);
                        if(pCar)
                        {
    	                        pCar->RouteNo = 0;
                                pCar->m_bIsOverPath = false;
                                pCar->UpdateHint();
                        }
                }
        }
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::MenuItemLV_0_DefaultClick(
      TObject *Sender)
{   //�אּ�p�⨮
        for(int i=0; i<Form_ViewInfo->ListView_Car->Items->Count; i++)
        {
                if(Form_ViewInfo->ListView_Car->Items->Item[i]->Selected)
                {
                        AnsiString CarID = Form_ViewInfo->ListView_Car->Items->Item[i]->Caption;
                        CCar* pCar = GetCarByCarID(CarID);
                        //��s��Ʈw���
                        TQuery* pQuery_CarInfo = new TQuery(DataModule1);
                        pQuery_CarInfo->DatabaseName = "DB_DCH";
                        pQuery_CarInfo->SQL->Add("UPDATE tCarInfo");
                        pQuery_CarInfo->SQL->Add("SET Image = 0");
                        pQuery_CarInfo->SQL->Add("WHERE LocatorNo = :LocatorNo");
                        pQuery_CarInfo->ParamByName("LocatorNo")->AsString = pCar->GetLocatorNo();
                        pQuery_CarInfo->ExecSQL();
                        delete pQuery_CarInfo;
                        if(pCar)
                        {   //0:�p�⨮ , 1:�f�� , 2:�s���� , 3:ĵ�� , 4:������
    	                        if(pCar->m_CarType!=0)
                                {
        	                        pCar->m_CarType=0;
    		                        if( pCar->m_pImage_Car->Tag > 0 )
    		                        {
	    		                        switch((pCar->m_pImage_Car->Tag)%5)
    			                        {
    				                        case 1:	pCar->m_pImage_Car->Tag = 1;		break;
    				                        case 2:	pCar->m_pImage_Car->Tag = 2;		break;
    				                        case 3:	pCar->m_pImage_Car->Tag = 3;		break;
                                                        case 4: pCar->m_pImage_Car->Tag = 4;            break;
    				                        case 0:	pCar->m_pImage_Car->Tag = 5;		break;
    			                        }//switch
        	                        }
    		                        else
    		                        {
	    		                        switch((pCar->m_pImage_Car->Tag)%5)
    			                        {
    				                        case -1:pCar->m_pImage_Car->Tag = -1;		break;
    				                        case -2:pCar->m_pImage_Car->Tag = -2;		break;
    				                        case -3:pCar->m_pImage_Car->Tag = -3;		break;
                                                        case -4:pCar->m_pImage_Car->Tag = -4;		break;
    				                        case 0:	pCar->m_pImage_Car->Tag = -5;		break;
    			                        }//switch
    		                        }//else
                                        //�󴫹ϥܥH�����I����
    		                        int Origin_X = pCar->m_pImage_Car->Left+pCar->m_pImage_Car->Width/2;    //�ϧΤ����I=> X
    		                        int Origin_Y = pCar->m_pImage_Car->Top+pCar->m_pImage_Car->Height/2;    //�ϧΤ����I=> Y
        	                        ChangeCarImgByTag(pCar->m_pImage_Car,1);
    		                        pCar->m_pImage_Car->Left = Origin_X - pCar->m_pImage_Car->Width/2;
    		                        pCar->m_pImage_Car->Top = Origin_Y - pCar->m_pImage_Car->Height/2;
                                }//if CarType
                        }//if pCar
                }
        }
        //reset
        PopupMenu_Car->Tag = 0;
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::MenuItemLV_1_TruckClick(
      TObject *Sender)
{// //�אּ�f��
        for(int i=0; i<Form_ViewInfo->ListView_Car->Items->Count; i++)
        {
                if(Form_ViewInfo->ListView_Car->Items->Item[i]->Selected)
                {
                        AnsiString CarID = Form_ViewInfo->ListView_Car->Items->Item[i]->Caption;
                        CCar* pCar = GetCarByCarID(CarID);
                        //��s��Ʈw���
                        TQuery* pQuery_CarInfo = new TQuery(DataModule1);
                        pQuery_CarInfo->DatabaseName = "DB_DCH";
                        pQuery_CarInfo->SQL->Add("UPDATE tCarInfo");
                        pQuery_CarInfo->SQL->Add("SET Image = 1");
                        pQuery_CarInfo->SQL->Add("WHERE LocatorNo = :LocatorNo");
                        pQuery_CarInfo->ParamByName("LocatorNo")->AsString = pCar->GetLocatorNo();
                        pQuery_CarInfo->ExecSQL();
                        delete pQuery_CarInfo;
                        if(pCar)
                        {   //0:�p�⨮ , 1:�f�� , 2:�s���� , 3:ĵ�� , 4:������
    	                        if(pCar->m_CarType!=1)
                                {
        	                        pCar->m_CarType=1;
    		                        if( pCar->m_pImage_Car->Tag > 0 )
    		                        {
	    		                        switch((pCar->m_pImage_Car->Tag)%5)
    			                        {
    				                        case 1:	pCar->m_pImage_Car->Tag = 6;		break;
    				                        case 2:	pCar->m_pImage_Car->Tag = 7;		break;
    				                        case 3:	pCar->m_pImage_Car->Tag = 8;		break;
                                                        case 4:	pCar->m_pImage_Car->Tag = 9;		break;
    				                        case 0:	pCar->m_pImage_Car->Tag = 10;		break;
    			                        }//switch
        	                        }
    		                        else
    		                        {
	    		                        switch((pCar->m_pImage_Car->Tag)%5)
    			                        {
    				                        case -1:pCar->m_pImage_Car->Tag = -6;		break;
    				                        case -2:pCar->m_pImage_Car->Tag = -7;		break;
    				                        case -3:pCar->m_pImage_Car->Tag = -8;		break;
                                                        case -4:pCar->m_pImage_Car->Tag = -9;		break;
    				                        case 0:	pCar->m_pImage_Car->Tag = -10;		break;
    			                        }//switch
    		                        }//else
                                        //�H�����I����
    		                        int Origin_X = pCar->m_pImage_Car->Left+pCar->m_pImage_Car->Width/2;    //�ϧΤ����I=> X
    		                        int Origin_Y = pCar->m_pImage_Car->Top+pCar->m_pImage_Car->Height/2;    //�ϧΤ����I=> Y
        	                        ChangeCarImgByTag(pCar->m_pImage_Car,1);
    		                        pCar->m_pImage_Car->Left = Origin_X - pCar->m_pImage_Car->Width/2;
    		                        pCar->m_pImage_Car->Top = Origin_Y - pCar->m_pImage_Car->Height/2;
                                }//if CarType
                        }//if pCar
                }
        }
        //reset
        PopupMenu_Car->Tag = 0;
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::MenuItemLV_2_ContainerClick(
      TObject *Sender)
{// //�אּ�s����
        for(int i=0; i<Form_ViewInfo->ListView_Car->Items->Count; i++)
        {
                if(Form_ViewInfo->ListView_Car->Items->Item[i]->Selected)
                {
                        AnsiString CarID = Form_ViewInfo->ListView_Car->Items->Item[i]->Caption;
                        CCar* pCar = GetCarByCarID(CarID);
                        //��s��Ʈw���
                        TQuery* pQuery_CarInfo = new TQuery(DataModule1);
                        pQuery_CarInfo->DatabaseName = "DB_DCH";
                        pQuery_CarInfo->SQL->Add("UPDATE tCarInfo");
                        pQuery_CarInfo->SQL->Add("SET Image = 2");
                        pQuery_CarInfo->SQL->Add("WHERE LocatorNo = :LocatorNo");
                        pQuery_CarInfo->ParamByName("LocatorNo")->AsString = pCar->GetLocatorNo();
                        pQuery_CarInfo->ExecSQL();
                        delete pQuery_CarInfo;
                        if(pCar)
                        {   //0:�p�⨮ , 1:�f�� , 2:�s���� , 3:ĵ�� , 4:������
    	                        if(pCar->m_CarType!=2)
                                {
        	                        pCar->m_CarType=2;
    		                        if( pCar->m_pImage_Car->Tag > 0 )
    		                        {
	    		                        switch((pCar->m_pImage_Car->Tag)%5)
    			                        {
    				                        case 1:	pCar->m_pImage_Car->Tag = 11;		break;
    				                        case 2:	pCar->m_pImage_Car->Tag = 12;		break;
    				                        case 3:	pCar->m_pImage_Car->Tag = 13;		break;
                                                        case 4:	pCar->m_pImage_Car->Tag = 14;		break;
    				                        case 0:	pCar->m_pImage_Car->Tag = 15;		break;
    			                        }//switch
        	                        }
    		                        else
    		                        {
	    		                        switch((pCar->m_pImage_Car->Tag)%5)
    			                        {
    				                        case -1:pCar->m_pImage_Car->Tag = -11;		break;
    				                        case -2:pCar->m_pImage_Car->Tag = -12;		break;
    				                        case -3:pCar->m_pImage_Car->Tag = -13;		break;
                                                        case -4:pCar->m_pImage_Car->Tag = -14;		break;
    				                        case 0:	pCar->m_pImage_Car->Tag = -15;		break;
    			                        }//switch
    		                        }//else
                                        //�H�����I����
    		                        int Origin_X = pCar->m_pImage_Car->Left+pCar->m_pImage_Car->Width/2;    //�ϧΤ����I=> X
    		                        int Origin_Y = pCar->m_pImage_Car->Top+pCar->m_pImage_Car->Height/2;    //�ϧΤ����I=> Y
        	                        ChangeCarImgByTag(pCar->m_pImage_Car,1);
    		                        pCar->m_pImage_Car->Left = Origin_X - pCar->m_pImage_Car->Width/2;
    		                        pCar->m_pImage_Car->Top = Origin_Y - pCar->m_pImage_Car->Height/2;
                                }//if CarType
                        }//if pCar
                }
        }
        //reset
        PopupMenu_Car->Tag = 0;
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::IO11Click(TObject *Sender)
{
        CCar *pCar = (CCar *)PopupMenu_Car->Tag; //�Q��PopupMenu_CarPopup �w��X���l�������
        if(!pCar)
        {
    	        return;
        }
        pCar->IO_In_Watch = 1;
        AnsiString CarID = pCar->GetCarID();
        TListItem* pListItem = Form_ViewInfo->GetCarItem(CarID);
        //���pListItem����
        if(pListItem)
        {
                pListItem->SubItems->Strings[CAR_IO_IN_WATCH] = 1;
        }
        //�g�J��Ʈw
        TQuery* pQ_Update = new TQuery(CallCenter_Main_Form);
        pQ_Update->DatabaseName = "DB_DCH";
        pQ_Update->SQL->Add("UPDATE tCarInfo");
        pQ_Update->SQL->Add("SET DefaultIO  = :DefaultIO");
        pQ_Update->SQL->Add("WHERE CarID = :CarID");
        pQ_Update->ParamByName("DefaultIO")->AsInteger = pCar->IO_In_Watch;
        pQ_Update->ParamByName("CarID")->AsString = CarID;
        try
        {
                pQ_Update->ExecSQL();
        }
        catch(...)
        {
                ShowMessage("�ܧ󤣦��\!");
        }
        delete pQ_Update;
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::IO21Click(TObject *Sender)
{
        CCar *pCar = (CCar *)PopupMenu_Car->Tag; //�Q��PopupMenu_CarPopup �w��X���l�������
        if(!pCar)
        {
    	        return;
        }
        pCar->IO_In_Watch = 2;
        AnsiString CarID = pCar->GetCarID();
        TListItem* pListItem = Form_ViewInfo->GetCarItem(CarID);
        if(pListItem)
        {
                pListItem->SubItems->Strings[CAR_IO_IN_WATCH] = 2;
        }
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::IO31Click(TObject *Sender)
{
        CCar *pCar = (CCar *)PopupMenu_Car->Tag; //�Q��PopupMenu_CarPopup �w��X���l�������
        if(!pCar)
        {
    	        return;
        }
        pCar->IO_In_Watch = 3;
        AnsiString CarID = pCar->GetCarID();
        TListItem* pListItem = Form_ViewInfo->GetCarItem(CarID);
        if(pListItem)
        {
                pListItem->SubItems->Strings[CAR_IO_IN_WATCH] = 3;
        }
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::IO41Click(TObject *Sender)
{
        CCar *pCar = (CCar *)PopupMenu_Car->Tag; //�Q��PopupMenu_CarPopup �w��X���l�������
        if(!pCar)
        {
    	        return;
        }
        pCar->IO_In_Watch = 4;
        AnsiString CarID = pCar->GetCarID();
        TListItem* pListItem = Form_ViewInfo->GetCarItem(CarID);
        if(pListItem)
        {
                pListItem->SubItems->Strings[CAR_IO_IN_WATCH] = 4;
        }
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::IO12Click(TObject *Sender)
{
        for(int i=0; i<Form_ViewInfo->ListView_Car->Items->Count; i++)
        {
                if(Form_ViewInfo->ListView_Car->Items->Item[i]->Selected)
                {
                        AnsiString CarID = Form_ViewInfo->ListView_Car->Items->Item[i]->Caption;
                        CCar* pCar = GetCarByCarID(CarID);
                        if(!pCar)
                        {
    	                        return;
                        }
                        pCar->IO_In_Watch = 1;          //�ܧ�pCar����
                        TListItem* pListItem = Form_ViewInfo->GetCarItem(CarID);
                        if(pListItem)
                        {
                                pListItem->SubItems->Strings[CAR_IO_IN_WATCH] = 1;      //�ܧ�ListView�W����
                        }
                        //�N�ܧ�g�JDatabase
                        TQuery* pQuery = new TQuery(CallCenter_Main_Form);
                        pQuery->DatabaseName = "DB_DCH";
                        pQuery->SQL->Add("UPDATE tCarInfo");
                        pQuery->SQL->Add("SET DefaultIO = 1");
                        pQuery->SQL->Add("WHERE CarID = :CarID");
                        pQuery->SQL->Add("AND Cancel = 0");
                        pQuery->ParamByName("CarID")->AsString = CarID;
                        pQuery->ExecSQL();
                        delete pQuery;
                }
        }
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::IO22Click(TObject *Sender)
{
        for(int i=0; i<Form_ViewInfo->ListView_Car->Items->Count; i++)
        {
                if(Form_ViewInfo->ListView_Car->Items->Item[i]->Selected)
                {
                        AnsiString CarID = Form_ViewInfo->ListView_Car->Items->Item[i]->Caption;
                        CCar* pCar = GetCarByCarID(CarID);
                        if(!pCar)
                        {
    	                        return;
                        }
                        pCar->IO_In_Watch = 2;          //�ܧ�pCar����
                        TListItem* pListItem = Form_ViewInfo->GetCarItem(CarID);
                        if(pListItem)
                        {
                                pListItem->SubItems->Strings[CAR_IO_IN_WATCH] = 2;      //�ܧ�ListView�W����
                        }
                        //�N�ܧ�g�JDatabase
                        TQuery* pQuery = new TQuery(CallCenter_Main_Form);
                        pQuery->DatabaseName = "DB_DCH";
                        pQuery->SQL->Add("UPDATE tCarInfo");
                        pQuery->SQL->Add("SET DefaultIO = 2");
                        pQuery->SQL->Add("WHERE CarID = :CarID");
                        pQuery->SQL->Add("AND Cancel = 0");
                        pQuery->ParamByName("CarID")->AsString = CarID;
                        pQuery->ExecSQL();
                        delete pQuery;
                }
        }
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::IO32Click(TObject *Sender)
{
        for(int i=0; i<Form_ViewInfo->ListView_Car->Items->Count; i++)
        {
                if(Form_ViewInfo->ListView_Car->Items->Item[i]->Selected)
                {
                        AnsiString CarID = Form_ViewInfo->ListView_Car->Items->Item[i]->Caption;
                        CCar* pCar = GetCarByCarID(CarID);
                        if(!pCar)
                        {
    	                        return;
                        }
                        pCar->IO_In_Watch = 3;          //�ܧ�pCar����
                        TListItem* pListItem = Form_ViewInfo->GetCarItem(CarID);
                        if(pListItem)
                        {
                                pListItem->SubItems->Strings[CAR_IO_IN_WATCH] = 3;      //�ܧ�ListView�W����
                        }
                        //�N�ܧ�g�JDatabase
                        TQuery* pQuery = new TQuery(CallCenter_Main_Form);
                        pQuery->DatabaseName = "DB_DCH";
                        pQuery->SQL->Add("UPDATE tCarInfo");
                        pQuery->SQL->Add("SET DefaultIO = 3");
                        pQuery->SQL->Add("WHERE CarID = :CarID");
                        pQuery->SQL->Add("AND Cancel = 0");
                        pQuery->ParamByName("CarID")->AsString = CarID;
                        pQuery->ExecSQL();
                        delete pQuery;
                }
        }
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::IO42Click(TObject *Sender)
{
        for(int i=0; i<Form_ViewInfo->ListView_Car->Items->Count; i++)
        {
                if(Form_ViewInfo->ListView_Car->Items->Item[i]->Selected)
                {
                        AnsiString CarID = Form_ViewInfo->ListView_Car->Items->Item[i]->Caption;
                        CCar* pCar = GetCarByCarID(CarID);
                        if(!pCar)
                        {
    	                        return;
                        }
                        pCar->IO_In_Watch = 4;          //�ܧ�pCar����
                        TListItem* pListItem = Form_ViewInfo->GetCarItem(CarID);
                        if(pListItem)
                        {
                                pListItem->SubItems->Strings[CAR_IO_IN_WATCH] = 4;      //�ܧ�ListView�W����
                        }
                        //�N�ܧ�g�JDatabase
                        TQuery* pQuery = new TQuery(CallCenter_Main_Form);
                        pQuery->DatabaseName = "DB_DCH";
                        pQuery->SQL->Add("UPDATE tCarInfo");
                        pQuery->SQL->Add("SET DefaultIO = 4");
                        pQuery->SQL->Add("WHERE CarID = :CarID");
                        pQuery->SQL->Add("AND Cancel = 0");
                        pQuery->ParamByName("CarID")->AsString = CarID;
                        pQuery->ExecSQL();
                        delete pQuery;
                }
        }
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::M_N1x1Click(TObject *Sender)
{
	double scale;
	TMenuItem* pMenu = (TMenuItem*)Sender;
        if( (pMenu==N1x1)||(pMenu==M_N1x1) )	// 1x Click
                scale=1.0;
        else if( (pMenu==N2x1)||(pMenu==M_N2x1) )
                scale=3.0;
        else if( (pMenu==N3x1)||(pMenu==M_N3x1) )
                scale=7.0;
        else if( (pMenu==N4x1)||(pMenu==M_N4x1) )
                scale=10.0;
        else if( (pMenu==N5x1)||(pMenu==M_N5x1) )
                scale=19.9;
        else if( (pMenu==N6x1)||(pMenu==M_N6x1) )
                scale=29.8;
        else if( (pMenu==N7x1)||(pMenu==M_N7x1) )
                scale=39.7;
        else if( (pMenu==N8x1)||(pMenu==M_N8x1) )
                scale=49.6;
        else if( (pMenu==N9x1)||(pMenu==M_N9x1) )
                scale=59.5;
        else if( (pMenu==N10x1)||(pMenu==M_N10x1) )
                scale=69.4;
        else if( (pMenu==N11x1)||(pMenu==M_N11x1) )
                scale=89.25;
        else if( (pMenu==N12x1)||(pMenu==M_N12x1) )
                scale=198.3;
        else if( (pMenu==N13x1)||(pMenu==M_N13x1) )
                scale=396.5;
        ChangeZoom(scale);
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::SpeedButton_TripClick(
      TObject *Sender)
{
        if(!Form_Trip)
        {
                Form_Trip = new TForm_Trip(this);
                Form_Trip->Visible = true;
        }
        else
        {
                Form_Trip->Close();
        }
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::MenuItemLV_CarTripClick(
      TObject *Sender)
{
        CHisTracer->LocatorNo = Form_ViewInfo->ListView_Car->Selected->SubItems->Strings[CAR_LOCATOR_NO];   //��o�ұ��j�M�������X
        if(!Form_Trip)
                Form_Trip = new TForm_Trip(this);
        for(int i=0; i < Form_Trip->LocatorNoBox->Items->Count; i++)
        {
                if(Form_Trip->LocatorNoBox->Items->Strings[i] == Form_ViewInfo->ListView_Car->Selected->Caption)
                {
                        Form_Trip->LocatorNoBox->ItemIndex = i;
                }
        }
        Form_Trip->Visible = true;        
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::MenuItem_LoadTripClick(
      TObject *Sender)
{
        CCar *ptrCar = (CCar *)PopupMenu_Car->Tag; //�Q��PopupMenu_CarPopup �w��X���l�������
        CHisTracer->LocatorNo = ptrCar->GetLocatorNo();   //��o�ұ��j�M�������X
        if(!Form_Trip)
                Form_Trip = new TForm_Trip(this);
        Form_Trip->LocatorNoBox->Text = CHisTracer->LocatorNo;
        Form_Trip->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::N_SetReportCheckClick(
      TObject *Sender)
{  //�]�w�����^���ˬd�W�v
	CCar *ptrCar = (CCar *)PopupMenu_Car->Tag; //�Q��PopupMenu_CarPopup �w��X���l�������
        if(ptrCar)
        {
		Dlg_RemoteCmd = new TDlg_RemoteCmd(this);
                Dlg_RemoteCmd->m_CarList->Add(ptrCar);
                Dlg_RemoteCmd->m_DlgCmdType = 5; //0:�w�ɦ^�� , 1:�ߧY�^�� , 2:��郞���K�X ,3:��ﱱ��߸��X , 4:��L�R�O, 5:�]�w�^���ˬd�W�v
    	        Dlg_RemoteCmd->Show();
        }
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::N_LV_SetReportCheckClick(
      TObject *Sender)
{
	LVSendCmdByType(5);   //-1:�����^�� , 0:�w�ɦ^�� , 1:�ߧY�^�� , 2:��郞���K�X ,3:��ﱱ��߸��X , 4:��L�R�O,5:�]�w�^���ˬd�W�v        
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::Timer1Timer(TObject *Sender)
{
//���pTerminate,�NTimer����
        if(Application->Terminated)
        {
                Timer1->Enabled = false;
                return;
        }
//�վ�D�������󪺦�m
        int Top = Image_Map->Top;
        int Left = Image_Map->Left + Image_Map->Width - 100;
        if(Label_Scale->Top != Top)
        {
                Label_Scale->Top = Top;
        }
        if(Label_Scale->Left != Left)
        {
                Label_Scale->Left = Left;
        }
        double Scale;
        Scale = 100 * 0.10089008517 * m_dScale;
        Label_Scale->Caption = Label_Scale->Caption.FormatFloat("0.0",Scale);
        Label_Scale->Caption = Label_Scale->Caption + "m";
//��MaintainThread->void __fastcall CTMaintain::PanelStat()�禡
    if(CallCenter_Main_Form->Visible == true)
    {
//2002.5.7 �ץ��̤p��Panel��,�n�^�_�|�]�j�p�L�k�^�_�����D
        CallCenter_Main_Form->SpeedButton_ShowSetting->Down = Form_ViewInfo->Visible;
        CallCenter_Main_Form->SpeedButton_ShowStreet->Down = CallCenter_Main_Form->Panel_Street->Visible;
        CallCenter_Main_Form->Menu_Wnd_Setting->Checked = Form_ViewInfo->Visible;
        CallCenter_Main_Form->Menu_Wnd_Street->Checked = CallCenter_Main_Form->Panel_Street->Visible;
        bool CanMinimize = true;               //�O�_�i�H�̤p��
        for(int i=0; i < CallCenter_Main_Form->Panel_DynBar->DockClientCount; i++)
        {
                TControl* pControl = CallCenter_Main_Form->Panel_DynBar->DockClients[i];
                if(pControl->Visible == true)
                {
                        CanMinimize = false;
                        break;
                }
        }
        if(CanMinimize)
        {
                if(CallCenter_Main_Form->Panel_DynBar->Width != 2)
                {
                        CallCenter_Main_Form->Panel_DynBar->Width = 2;
                        CallCenter_Main_Form->UpdateAllObjPos();
                        ReDrawImage(CallCenter_Main_Form->Image_Map,CallCenter_Main_Form->m_dScale);
                }
        }
        else if((CallCenter_Main_Form->Panel_DynBar->Width != CallCenter_Main_Form->DockSiteWidth) && (CanMinimize == false))
        {
                CallCenter_Main_Form->Panel_DynBar->Width = CallCenter_Main_Form->DockSiteWidth;
	        CallCenter_Main_Form->UpdateAllObjPos();
                ReDrawImage(CallCenter_Main_Form->Image_Map,CallCenter_Main_Form->m_dScale);
        }
        //�P�B�����Ұʫ��s���A
        if(Form_ViewInfo->PageControl1->ActivePage == Form_ViewInfo->TabSheet_Car)
        {
                CallCenter_Main_Form->Menu_Car->Checked = true;
                CallCenter_Main_Form->Menu_Remark->Checked = false;
        }
        else if(Form_ViewInfo->PageControl1->ActivePage == Form_ViewInfo->TabSheet_Remark)
        {
                CallCenter_Main_Form->Menu_Car->Checked = false;
                CallCenter_Main_Form->Menu_Remark->Checked = true;
        }
        if(!Form_SmartQry)
        {
                CallCenter_Main_Form->SpeedButton_Query->Down = false;
                CallCenter_Main_Form->N_Query->Checked = false;
        }
        else
        {
                CallCenter_Main_Form->SpeedButton_Query->Down = true;
                CallCenter_Main_Form->N_Query->Checked = true;
        }
        static bool InitialMap = false;
        if(!InitialMap)
        {
                CallCenter_Main_Form->InitialDraw();
                InitialMap = true;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::MenuItemLV_ShowCarTraceClick(
      TObject *Sender)
{
	CCar* ptrCar = GetCarByCarID(Form_ViewInfo->ListView_Car->Selected->Caption);
        if(ptrCar)
        {
    	        if(ptrCar->m_pCarTraceList==NULL)
                {//Create New
        	        ptrCar->m_pCarTraceList = new TList();
                        m_pDrawTraceCarList->Add(ptrCar);
                        //Add sSelectPoint Point in CCar::Update()
        	        //Draw Car Trace in DrawImage()
                }
        }
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::MenuItemLV_CancelCarTraceClick(
      TObject *Sender)
{
        CCar* ptrCar = GetCarByCarID(Form_ViewInfo->ListView_Car->Selected->Caption);
        if(ptrCar)
        {
    	        if(ptrCar->m_pCarTraceList)
                {//Free all Memory
                        for(int i=0;i<ptrCar->m_pCarTraceList->Count;i++)
            	                delete ((sSelectPoint*)ptrCar->m_pCarTraceList->Items[i]);
                        ptrCar->m_pCarTraceList->Clear();
                        delete (ptrCar->m_pCarTraceList);
        	        ptrCar->m_pCarTraceList=NULL;
                        m_pDrawTraceCarList->Remove(ptrCar);
                        ReDrawImage(Image_Map,m_dScale);
                }
	}
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::MenuItemLV_SetSelPathClick(
      TObject *Sender)
{
        if(!Form_CustRoute)
        {
                Form_CustRoute = new TForm_CustRoute(this);
        }
        CCar* ptrCar = GetCarByCarID(Form_ViewInfo->ListView_Car->Selected->Caption);
        Form_CustRoute->PageControl1->ActivePage = Form_CustRoute->Tab_Assign;
        Form_CustRoute->ComboBox1->Text = ptrCar->GetCarID();
        Form_CustRoute->ComboBox1Change(Sender);
        Form_CustRoute->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::N_CarInfoClick(TObject *Sender)
{
        if(!Form_CarInfo)
        {
                Form_CarInfo = new TForm_CarInfo(this);
                Form_CarInfo->ListView1->Items->Item[0]->SubItems->Add("");
                Form_CarInfo->ListView1->Items->Item[1]->SubItems->Add("");
                Form_CarInfo->ListView1->Items->Item[2]->SubItems->Add("");
                Form_CarInfo->ListView1->Items->Item[3]->SubItems->Add("");
                Form_CarInfo->ListView1->Items->Item[4]->SubItems->Add("");
                Form_CarInfo->ListView1->Items->Item[5]->SubItems->Add("");
                Form_CarInfo->ListView1->Items->Item[6]->SubItems->Add("");
        }
        Form_CarInfo->ListView1->Items->Item[0]->SubItems->Strings[0] = Form_ViewInfo->ListView_Car->Selected->Caption;
        Form_CarInfo->Show();
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::N_UserManagerClick(TObject *Sender)
{
        if(!Form_UserManager)
        {
                Form_UserManager = new TForm_UserManager(this);
        }
        Form_UserManager->Show();
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::Update_CCar_List()
{
/*
2002.5.3 vincent:�R���쥻�i��w�R�����ư������{��(tDel_Log)
2002.5.6 vincent:�ˬdUpdateTime,�ɶ����@�ˤ~��s
2002.5.7 vincent:�令���hŪ����Ʈw,��ViewInfo CarListView�H�� CCarInfo�Ө��o���,
                 �]�N�O�n����sCarInfo�A�ӳB�z�o��
*/
        try
        {
                for(int i=0; i < Form_ViewInfo->ListView_Car->Items->Count; i++)
                {
                        if(CallCenter_Main_Form->ThreadDeleteing)
                        {    //��o����
                                return;
                        }
                        TListItem* pListItem = Form_ViewInfo->ListView_Car->Items->Item[i];
                        AnsiString CarID = pListItem->Caption;
                        CCarInfo* pCarInfo = Form_ChooseCar->CustomerList->FindCarByCarID(CarID);
                        AnsiString LocatorNo = pCarInfo->LocatorNo;
                        CCar* pCar = CallCenter_Main_Form->GetCarByCarID(CarID);
                        if(pCar == NULL)  //���p�S���o�x���hnew�@�x�X��,�åB�ˬdLatestStatus�O�_���ӽ���
                        {
                                //new�@�x�X��
                                sCarStatus CarStatus;
                                memset(&CarStatus,0,sizeof(sCarStatus));
                                strcpy(CarStatus.m_szLocatorNo,LocatorNo.c_str());
                                pCar = new CCar(CarID,CarStatus, CallCenter_Main_Form, pCarInfo->Image);
                                CallCenter_Main_Form->Insert_CCar_QSort(pCar);      //�N�spCar�[�JList
                                memset(&CarStatus,0,sizeof(sCarStatus));        //RESET WHEN FREE
                        }
                        //2002.5.6 ���pDatabase UpdateTime�����ܪ���,�~�i���s
                        if(pCar->CarInfoUpdateTime != pCarInfo->UpdateTime)
                        {
                                //�i���s
                                pCar->CarInfoUpdateTime = pCarInfo->UpdateTime;
                                strcpy(pCar->m_Status.m_szLocatorNo, pCarInfo->LocatorNo.c_str());
                                CCarList_SortByLocatorNo->Sort(Compare_CCar_By_LocatorNo);
                                pCar->IO_In_Watch = pCarInfo->DefaultIO;
                                //��X�Ө�����CarType
                                CCustomer* pCustomer = (CCustomer*)((CGroup*)pCarInfo->pParent)->pParent;
                                CCodeUnit* pCodeSet = pCustomer->TypeCodes->FindCodeUnit(pCarInfo->TypeCode);
                                if(pCodeSet)
                                {
                                        pCar->CarType = pCodeSet->Explanation;
                                }
                                //��X�Ө�����CarGroup
                                pCodeSet = pCustomer->GroupCodes->FindCodeUnit(pCarInfo->GroupCode);
                                if(pCodeSet)
                                {
                                        pCar->CarGroup = pCodeSet->Explanation;
                                }
                        }
                }
        }
        catch(...)
        {
        }
}
//---------------------------------------------------------------------------


void __fastcall TCallCenter_Main_Form::MenuItemLV_3_NINEClick(
      TObject *Sender)
{
// //�אּ�E�H�y
        for(int i=0; i<Form_ViewInfo->ListView_Car->Items->Count; i++)
        {
                if(Form_ViewInfo->ListView_Car->Items->Item[i]->Selected)
                {
                        AnsiString CarID = Form_ViewInfo->ListView_Car->Items->Item[i]->Caption;
                        CCar* pCar = GetCarByCarID(CarID);
                        //��s��Ʈw���
                        TQuery* pQuery_CarInfo = new TQuery(DataModule1);
                        pQuery_CarInfo->DatabaseName = "DB_DCH";
                        pQuery_CarInfo->SQL->Add("UPDATE tCarInfo");
                        pQuery_CarInfo->SQL->Add("SET Image = 3");
                        pQuery_CarInfo->SQL->Add("WHERE LocatorNo = :LocatorNo");
                        pQuery_CarInfo->ParamByName("LocatorNo")->AsString = pCar->GetLocatorNo();
                        pQuery_CarInfo->ExecSQL();
                        delete pQuery_CarInfo;
                        if(pCar)
                        {   //0:�p�⨮ , 1:�f�� , 2:�s���� , 3:�E�H�y , 4:������
    	                        if(pCar->m_CarType!=3)
                                {
        	                        pCar->m_CarType=3;
    		                        if( pCar->m_pImage_Car->Tag > 0 )
    		                        {
	    		                        switch((pCar->m_pImage_Car->Tag)%5)
    			                        {
    				                        case 1:	pCar->m_pImage_Car->Tag = 16;		break;
    				                        case 2:	pCar->m_pImage_Car->Tag = 17;		break;
    				                        case 3:	pCar->m_pImage_Car->Tag = 18;		break;
                                                        case 4:	pCar->m_pImage_Car->Tag = 19;		break;
    				                        case 0:	pCar->m_pImage_Car->Tag = 20;		break;
    			                        }//switch
        	                        }
    		                        else
    		                        {
	    		                        switch((pCar->m_pImage_Car->Tag)%5)
    			                        {
    				                        case -1:pCar->m_pImage_Car->Tag = -16;		break;
    				                        case -2:pCar->m_pImage_Car->Tag = -17;		break;
    				                        case -3:pCar->m_pImage_Car->Tag = -18;		break;
                                                        case -4:pCar->m_pImage_Car->Tag = -19;		break;
    				                        case 0:	pCar->m_pImage_Car->Tag = -20;		break;
    			                        }//switch
    		                        }//else
                                        //�H�����I����
    		                        int Origin_X = pCar->m_pImage_Car->Left+pCar->m_pImage_Car->Width/2;    //�ϧΤ����I=> X
    		                        int Origin_Y = pCar->m_pImage_Car->Top+pCar->m_pImage_Car->Height/2;    //�ϧΤ����I=> Y
        	                        ChangeCarImgByTag(pCar->m_pImage_Car,1);
    		                        pCar->m_pImage_Car->Left = Origin_X - pCar->m_pImage_Car->Width/2;
    		                        pCar->m_pImage_Car->Top = Origin_Y - pCar->m_pImage_Car->Height/2;
                                }//if CarType
                        }//if pCar
                }
        }
        //reset
        PopupMenu_Car->Tag = 0;
}
//---------------------------------------------------------------------------

void __fastcall TCallCenter_Main_Form::MenuItem_3_NINEClick(
      TObject *Sender)
{
// //�אּ�E�H�y
	CCar *pCar = (CCar *)PopupMenu_Car->Tag; //�Q��PopupMenu_CarPopup �w��X���l�������
        //��s��Ʈw���
        TQuery* pQuery_CarInfo = new TQuery(DataModule1);
        pQuery_CarInfo->DatabaseName = "DB_DCH";
        pQuery_CarInfo->SQL->Add("UPDATE tCarInfo");
        pQuery_CarInfo->SQL->Add("SET Image = 3");
        pQuery_CarInfo->SQL->Add("WHERE LocatorNo = :LocatorNo");
        pQuery_CarInfo->ParamByName("LocatorNo")->AsString = pCar->GetLocatorNo();
        pQuery_CarInfo->ExecSQL();
        delete pQuery_CarInfo;
        if(pCar)
        {   //0:�p�⨮ , 1:�f�� , 2:�s���� , 3:ĵ�� , 4:������
    	        if(pCar->m_CarType!=3)
                {
        	        pCar->m_CarType=3;
    		        if( pCar->m_pImage_Car->Tag > 0 )
    		        {
	    		        switch((pCar->m_pImage_Car->Tag)%5)
    			        {
    				        case 1:	pCar->m_pImage_Car->Tag = 16;		break;
    				        case 2:	pCar->m_pImage_Car->Tag = 17;		break;
    				        case 3:	pCar->m_pImage_Car->Tag = 18;		break;
                                        case 4:	pCar->m_pImage_Car->Tag = 19;		break;
    				        case 0:	pCar->m_pImage_Car->Tag = 20;		break;
    			        }
        	        }
    		        else
    		        {
	    		        switch((pCar->m_pImage_Car->Tag)%5)
    			        {
    				        case -1:pCar->m_pImage_Car->Tag = -16;		break;
    				        case -2:pCar->m_pImage_Car->Tag = -17;		break;
    				        case -3:pCar->m_pImage_Car->Tag = -18;		break;
                                        case -4:pCar->m_pImage_Car->Tag = -19;		break;
    				        case 0:	pCar->m_pImage_Car->Tag = -20;		break;
    			        }
    		        }
                        //�H�����I����
    		        int Origin_X = pCar->m_pImage_Car->Left+pCar->m_pImage_Car->Width/2;    //�ϧΤ����I=> X
    		        int Origin_Y = pCar->m_pImage_Car->Top+pCar->m_pImage_Car->Height/2;    //�ϧΤ����I=> Y
        	        ChangeCarImgByTag(pCar->m_pImage_Car,1);
    		        pCar->m_pImage_Car->Left = Origin_X - pCar->m_pImage_Car->Width/2;
    		        pCar->m_pImage_Car->Top = Origin_Y - pCar->m_pImage_Car->Height/2;
                }
        }
        //reset
        PopupMenu_Car->Tag = 0;
}
//---------------------------------------------------------------------------

void __fastcall TCallCenter_Main_Form::MenuItemLV_4_BUSClick(
      TObject *Sender)
{
// //�אּ�ڤh
        for(int i=0; i<Form_ViewInfo->ListView_Car->Items->Count; i++)
        {
                if(Form_ViewInfo->ListView_Car->Items->Item[i]->Selected)
                {
                        AnsiString CarID = Form_ViewInfo->ListView_Car->Items->Item[i]->Caption;
                        CCar* pCar = GetCarByCarID(CarID);
                        //��s��Ʈw���
                        TQuery* pQuery_CarInfo = new TQuery(DataModule1);
                        pQuery_CarInfo->DatabaseName = "DB_DCH";
                        pQuery_CarInfo->SQL->Add("UPDATE tCarInfo");
                        pQuery_CarInfo->SQL->Add("SET Image = 4");
                        pQuery_CarInfo->SQL->Add("WHERE LocatorNo = :LocatorNo");
                        pQuery_CarInfo->ParamByName("LocatorNo")->AsString = pCar->GetLocatorNo();
                        pQuery_CarInfo->ExecSQL();
                        delete pQuery_CarInfo;
                        if(pCar)
                        {   //0:�p�⨮ , 1:�f�� , 2:�s���� , 3:�E�H�y , 4:���h�ڤh
    	                        if(pCar->m_CarType!=4)
                                {
        	                        pCar->m_CarType=4;
    		                        if( pCar->m_pImage_Car->Tag > 0 )
    		                        {
	    		                        switch((pCar->m_pImage_Car->Tag)%5)
    			                        {
    				                        case 1:	pCar->m_pImage_Car->Tag = 21;		break;
    				                        case 2:	pCar->m_pImage_Car->Tag = 22;		break;
    				                        case 3:	pCar->m_pImage_Car->Tag = 23;		break;
                                                        case 4:	pCar->m_pImage_Car->Tag = 24;		break;
    				                        case 0:	pCar->m_pImage_Car->Tag = 25;		break;
    			                        }//switch
        	                        }
    		                        else
    		                        {
	    		                        switch((pCar->m_pImage_Car->Tag)%5)
    			                        {
    				                        case -1:pCar->m_pImage_Car->Tag = -21;		break;
    				                        case -2:pCar->m_pImage_Car->Tag = -22;		break;
    				                        case -3:pCar->m_pImage_Car->Tag = -23;		break;
                                                        case -4:pCar->m_pImage_Car->Tag = -24;		break;
    				                        case 0:	pCar->m_pImage_Car->Tag = -25;		break;
    			                        }//switch
    		                        }//else
                                        //�H�����I����
    		                        int Origin_X = pCar->m_pImage_Car->Left+pCar->m_pImage_Car->Width/2;    //�ϧΤ����I=> X
    		                        int Origin_Y = pCar->m_pImage_Car->Top+pCar->m_pImage_Car->Height/2;    //�ϧΤ����I=> Y
        	                        ChangeCarImgByTag(pCar->m_pImage_Car,1);
    		                        pCar->m_pImage_Car->Left = Origin_X - pCar->m_pImage_Car->Width/2;
    		                        pCar->m_pImage_Car->Top = Origin_Y - pCar->m_pImage_Car->Height/2;
                                }//if CarType
                        }//if pCar
                }
        }
        //reset
        PopupMenu_Car->Tag = 0;
}
//---------------------------------------------------------------------------

void __fastcall TCallCenter_Main_Form::MenuItem_4_BUSClick(TObject *Sender)
{
// //�אּ�ڤh
	CCar *pCar = (CCar *)PopupMenu_Car->Tag; //�Q��PopupMenu_CarPopup �w��X���l�������
        //��s��Ʈw���
        TQuery* pQuery_CarInfo = new TQuery(DataModule1);
        pQuery_CarInfo->DatabaseName = "DB_DCH";
        pQuery_CarInfo->SQL->Add("UPDATE tCarInfo");
        pQuery_CarInfo->SQL->Add("SET Image = 4");
        pQuery_CarInfo->SQL->Add("WHERE LocatorNo = :LocatorNo");
        pQuery_CarInfo->ParamByName("LocatorNo")->AsString = pCar->GetLocatorNo();
        pQuery_CarInfo->ExecSQL();
        delete pQuery_CarInfo;
        if(pCar)
        {   //0:�p�⨮ , 1:�f�� , 2:�s���� , 3:ĵ�� , 4:���h�ڤh
    	        if(pCar->m_CarType!=4)
                {
        	        pCar->m_CarType=4;
    		        if( pCar->m_pImage_Car->Tag > 0 )
    		        {
	    		        switch((pCar->m_pImage_Car->Tag)%5)
    			        {
    				        case 1:	pCar->m_pImage_Car->Tag = 21;		break;
    				        case 2:	pCar->m_pImage_Car->Tag = 22;		break;
    				        case 3:	pCar->m_pImage_Car->Tag = 23;		break;
                                        case 4:	pCar->m_pImage_Car->Tag = 24;		break;
    				        case 0:	pCar->m_pImage_Car->Tag = 25;		break;
    			        }
        	        }
    		        else
    		        {
	    		        switch((pCar->m_pImage_Car->Tag)%5)
    			        {
    				        case -1:pCar->m_pImage_Car->Tag = -21;		break;
    				        case -2:pCar->m_pImage_Car->Tag = -22;		break;
    				        case -3:pCar->m_pImage_Car->Tag = -23;		break;
                                        case -4:pCar->m_pImage_Car->Tag = -24;		break;
    				        case 0:	pCar->m_pImage_Car->Tag = -25;		break;
    			        }
    		        }
                        //�H�����I����
    		        int Origin_X = pCar->m_pImage_Car->Left+pCar->m_pImage_Car->Width/2;    //�ϧΤ����I=> X
    		        int Origin_Y = pCar->m_pImage_Car->Top+pCar->m_pImage_Car->Height/2;    //�ϧΤ����I=> Y
        	        ChangeCarImgByTag(pCar->m_pImage_Car,1);
    		        pCar->m_pImage_Car->Left = Origin_X - pCar->m_pImage_Car->Width/2;
    		        pCar->m_pImage_Car->Top = Origin_Y - pCar->m_pImage_Car->Height/2;
                }
        }
        //reset
        PopupMenu_Car->Tag = 0;
}
//---------------------------------------------------------------------------

void __fastcall TCallCenter_Main_Form::MenuItemLV_5_TRUCK2Click(
      TObject *Sender)
{
// //�אּ�d��2
        for(int i=0; i<Form_ViewInfo->ListView_Car->Items->Count; i++)
        {
                if(Form_ViewInfo->ListView_Car->Items->Item[i]->Selected)
                {
                        AnsiString CarID = Form_ViewInfo->ListView_Car->Items->Item[i]->Caption;
                        CCar* pCar = GetCarByCarID(CarID);
                        //��s��Ʈw���
                        TQuery* pQuery_CarInfo = new TQuery(DataModule1);
                        pQuery_CarInfo->DatabaseName = "DB_DCH";
                        pQuery_CarInfo->SQL->Add("UPDATE tCarInfo");
                        pQuery_CarInfo->SQL->Add("SET Image = 5");
                        pQuery_CarInfo->SQL->Add("WHERE LocatorNo = :LocatorNo");
                        pQuery_CarInfo->ParamByName("LocatorNo")->AsString = pCar->GetLocatorNo();
                        pQuery_CarInfo->ExecSQL();
                        delete pQuery_CarInfo;
                        if(pCar)
                        {   //0:�p�⨮ , 1:�f�� , 2:�s���� , 3:�E�H�y , 4:���h�ڤh , 5:�d��
    	                        if(pCar->m_CarType!=5)
                                {
        	                        pCar->m_CarType=5;
    		                        if( pCar->m_pImage_Car->Tag > 0 )
    		                        {
	    		                        switch((pCar->m_pImage_Car->Tag)%5)
    			                        {
    				                        case 1:	pCar->m_pImage_Car->Tag = 26;		break;
    				                        case 2:	pCar->m_pImage_Car->Tag = 27;		break;
    				                        case 3:	pCar->m_pImage_Car->Tag = 28;		break;
                                                        case 4:	pCar->m_pImage_Car->Tag = 29;		break;
    				                        case 0:	pCar->m_pImage_Car->Tag = 30;		break;
    			                        }//switch
        	                        }
    		                        else
    		                        {
	    		                        switch((pCar->m_pImage_Car->Tag)%5)
    			                        {
    				                        case -1:pCar->m_pImage_Car->Tag = -26;		break;
    				                        case -2:pCar->m_pImage_Car->Tag = -27;		break;
    				                        case -3:pCar->m_pImage_Car->Tag = -28;		break;
                                                        case -4:pCar->m_pImage_Car->Tag = -29;		break;
    				                        case 0:	pCar->m_pImage_Car->Tag = -30;		break;
    			                        }//switch
    		                        }//else
                                        //�H�����I����
    		                        int Origin_X = pCar->m_pImage_Car->Left+pCar->m_pImage_Car->Width/2;    //�ϧΤ����I=> X
    		                        int Origin_Y = pCar->m_pImage_Car->Top+pCar->m_pImage_Car->Height/2;    //�ϧΤ����I=> Y
        	                        ChangeCarImgByTag(pCar->m_pImage_Car,1);
    		                        pCar->m_pImage_Car->Left = Origin_X - pCar->m_pImage_Car->Width/2;
    		                        pCar->m_pImage_Car->Top = Origin_Y - pCar->m_pImage_Car->Height/2;
                                }//if CarType
                        }//if pCar
                }
        }
        //reset
        PopupMenu_Car->Tag = 0;
}
//---------------------------------------------------------------------------

void __fastcall TCallCenter_Main_Form::MenuItem_5_TRUCK2Click(
      TObject *Sender)
{
// //�אּ�d��2
	CCar *pCar = (CCar *)PopupMenu_Car->Tag; //�Q��PopupMenu_CarPopup �w��X���l�������
        //��s��Ʈw���
        TQuery* pQuery_CarInfo = new TQuery(DataModule1);
        pQuery_CarInfo->DatabaseName = "DB_DCH";
        pQuery_CarInfo->SQL->Add("UPDATE tCarInfo");
        pQuery_CarInfo->SQL->Add("SET Image = 5");
        pQuery_CarInfo->SQL->Add("WHERE LocatorNo = :LocatorNo");
        pQuery_CarInfo->ParamByName("LocatorNo")->AsString = pCar->GetLocatorNo();
        pQuery_CarInfo->ExecSQL();
        delete pQuery_CarInfo;
        if(pCar)
        {   //0:�p�⨮ , 1:�f�� , 2:�s���� , 3:ĵ�� , 4:���h�ڤh ,5:�d��
    	        if(pCar->m_CarType!=5)
                {
        	        pCar->m_CarType=5;
    		        if( pCar->m_pImage_Car->Tag > 0 )
    		        {
	    		        switch((pCar->m_pImage_Car->Tag)%5)
    			        {
    				        case 1:	pCar->m_pImage_Car->Tag = 26;		break;
    				        case 2:	pCar->m_pImage_Car->Tag = 27;		break;
    				        case 3:	pCar->m_pImage_Car->Tag = 28;		break;
                                        case 4:	pCar->m_pImage_Car->Tag = 29;		break;
    				        case 0:	pCar->m_pImage_Car->Tag = 30;		break;
    			        }
        	        }
    		        else
    		        {
	    		        switch((pCar->m_pImage_Car->Tag)%5)
    			        {
    				        case -1:pCar->m_pImage_Car->Tag = -26;		break;
    				        case -2:pCar->m_pImage_Car->Tag = -27;		break;
    				        case -3:pCar->m_pImage_Car->Tag = -28;		break;
                                        case -4:pCar->m_pImage_Car->Tag = -29;		break;
    				        case 0:	pCar->m_pImage_Car->Tag = -30;		break;
    			        }
    		        }
                        //�H�����I����
    		        int Origin_X = pCar->m_pImage_Car->Left+pCar->m_pImage_Car->Width/2;    //�ϧΤ����I=> X
    		        int Origin_Y = pCar->m_pImage_Car->Top+pCar->m_pImage_Car->Height/2;    //�ϧΤ����I=> Y
        	        ChangeCarImgByTag(pCar->m_pImage_Car,1);
    		        pCar->m_pImage_Car->Left = Origin_X - pCar->m_pImage_Car->Width/2;
    		        pCar->m_pImage_Car->Top = Origin_Y - pCar->m_pImage_Car->Height/2;
                }
        }
        //reset
        PopupMenu_Car->Tag = 0;
}
//---------------------------------------------------------------------------

void __fastcall TCallCenter_Main_Form::MenuItemLV_6_DBUSClick(
      TObject *Sender)
{
// //�אּ���h�ڤh
        for(int i=0; i<Form_ViewInfo->ListView_Car->Items->Count; i++)
        {
                if(Form_ViewInfo->ListView_Car->Items->Item[i]->Selected)
                {
                        AnsiString CarID = Form_ViewInfo->ListView_Car->Items->Item[i]->Caption;
                        CCar* pCar = GetCarByCarID(CarID);
                        //��s��Ʈw���
                        TQuery* pQuery_CarInfo = new TQuery(DataModule1);
                        pQuery_CarInfo->DatabaseName = "DB_DCH";
                        pQuery_CarInfo->SQL->Add("UPDATE tCarInfo");
                        pQuery_CarInfo->SQL->Add("SET Image = 6");
                        pQuery_CarInfo->SQL->Add("WHERE LocatorNo = :LocatorNo");
                        pQuery_CarInfo->ParamByName("LocatorNo")->AsString = pCar->GetLocatorNo();
                        pQuery_CarInfo->ExecSQL();
                        delete pQuery_CarInfo;
                        if(pCar)
                        {   //0:�p�⨮ , 1:�f�� , 2:�s���� , 3:�E�H�y , 4:�ڤh , 5:�d�� , 6:���h�ڤh
    	                        if(pCar->m_CarType!=6)
                                {
        	                        pCar->m_CarType=6;
    		                        if( pCar->m_pImage_Car->Tag > 0 )
    		                        {
	    		                        switch((pCar->m_pImage_Car->Tag)%5)
    			                        {
    				                        case 1:	pCar->m_pImage_Car->Tag = 31;		break;
    				                        case 2:	pCar->m_pImage_Car->Tag = 32;		break;
    				                        case 3:	pCar->m_pImage_Car->Tag = 33;		break;
                                                        case 4:	pCar->m_pImage_Car->Tag = 34;		break;
    				                        case 0:	pCar->m_pImage_Car->Tag = 35;		break;
    			                        }//switch
        	                        }
    		                        else
    		                        {
	    		                        switch((pCar->m_pImage_Car->Tag)%5)
    			                        {
    				                        case -1:pCar->m_pImage_Car->Tag = -31;		break;
    				                        case -2:pCar->m_pImage_Car->Tag = -32;		break;
    				                        case -3:pCar->m_pImage_Car->Tag = -33;		break;
                                                        case -4:pCar->m_pImage_Car->Tag = -34;		break;
    				                        case 0:	pCar->m_pImage_Car->Tag = -35;		break;
    			                        }//switch
    		                        }//else
                                        //�H�����I����
    		                        int Origin_X = pCar->m_pImage_Car->Left+pCar->m_pImage_Car->Width/2;    //�ϧΤ����I=> X
    		                        int Origin_Y = pCar->m_pImage_Car->Top+pCar->m_pImage_Car->Height/2;    //�ϧΤ����I=> Y
        	                        ChangeCarImgByTag(pCar->m_pImage_Car,1);
    		                        pCar->m_pImage_Car->Left = Origin_X - pCar->m_pImage_Car->Width/2;
    		                        pCar->m_pImage_Car->Top = Origin_Y - pCar->m_pImage_Car->Height/2;
                                }//if CarType
                        }//if pCar
                }
        }
        //reset
        PopupMenu_Car->Tag = 0;
}
//---------------------------------------------------------------------------

void __fastcall TCallCenter_Main_Form::MenuItem_6_DBUSClick(TObject *Sender)
{
// //�אּ���h�ڤh
	CCar *pCar = (CCar *)PopupMenu_Car->Tag; //�Q��PopupMenu_CarPopup �w��X���l�������
        //��s��Ʈw���
        TQuery* pQuery_CarInfo = new TQuery(DataModule1);
        pQuery_CarInfo->DatabaseName = "DB_DCH";
        pQuery_CarInfo->SQL->Add("UPDATE tCarInfo");
        pQuery_CarInfo->SQL->Add("SET Image = 6");
        pQuery_CarInfo->SQL->Add("WHERE LocatorNo = :LocatorNo");
        pQuery_CarInfo->ParamByName("LocatorNo")->AsString = pCar->GetLocatorNo();
        pQuery_CarInfo->ExecSQL();
        delete pQuery_CarInfo;
        if(pCar)
        {   //0:�p�⨮ , 1:�f�� , 2:�s���� , 3:ĵ�� , 4:���h�ڤh ,5:�d��
    	        if(pCar->m_CarType!=6)
                {
        	        pCar->m_CarType=6;
    		        if( pCar->m_pImage_Car->Tag > 0 )
    		        {
	    		        switch((pCar->m_pImage_Car->Tag)%5)
    			        {
    				        case 1:	pCar->m_pImage_Car->Tag = 31;		break;
    				        case 2:	pCar->m_pImage_Car->Tag = 32;		break;
    				        case 3:	pCar->m_pImage_Car->Tag = 33;		break;
                                        case 4:	pCar->m_pImage_Car->Tag = 34;		break;
    				        case 0:	pCar->m_pImage_Car->Tag = 35;		break;
    			        }
        	        }
    		        else
    		        {
	    		        switch((pCar->m_pImage_Car->Tag)%5)
    			        {
    				        case -1:pCar->m_pImage_Car->Tag = -31;		break;
    				        case -2:pCar->m_pImage_Car->Tag = -32;		break;
    				        case -3:pCar->m_pImage_Car->Tag = -33;		break;
                                        case -4:pCar->m_pImage_Car->Tag = -34;		break;
    				        case 0:	pCar->m_pImage_Car->Tag = -35;		break;
    			        }
    		        }
                        //�H�����I����
    		        int Origin_X = pCar->m_pImage_Car->Left+pCar->m_pImage_Car->Width/2;    //�ϧΤ����I=> X
    		        int Origin_Y = pCar->m_pImage_Car->Top+pCar->m_pImage_Car->Height/2;    //�ϧΤ����I=> Y
        	        ChangeCarImgByTag(pCar->m_pImage_Car,1);
    		        pCar->m_pImage_Car->Left = Origin_X - pCar->m_pImage_Car->Width/2;
    		        pCar->m_pImage_Car->Top = Origin_Y - pCar->m_pImage_Car->Height/2;
                }
        }
        //reset
        PopupMenu_Car->Tag = 0;
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Main_Form::Set_CCar_List_Empty(TList* pList)
{
//�N�ҫ��w��CCar List�M��
        for(int i=0; i < pList->Count; i++)
        {
                CCar* pCar = (CCar*)pList->Items[i];
                pList->Items[i] = NULL;
                delete pCar;
        }
        pList->Clear();
}
//---------------------------------------------------------------------------


