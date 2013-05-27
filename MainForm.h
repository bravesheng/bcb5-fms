//---------------------------------------------------------------------------
#ifndef MainFormH
#define MainFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <Graphics.hpp>
#include <jpeg.hpp>
#include <ImgList.hpp>
#include <Qrctrls.hpp>
//---------------------------------------------------------------------------
#include <Mask.hpp>
#include <ScktComp.hpp>
#include <MPlayer.hpp>

#include "DataMudule.h"
#include "GetTrackThread.h"
#include "DisplayDll.h"  		//for Test Displsy Dll
#include "RemarkPlace.h"	        //備註地點
#include "SelectPath.h"
#include "MaintainThread.h"
#include <Db.hpp>
#include <DBTables.hpp>
#include <Dialogs.hpp>
#include <NMUDP.hpp>
#include <ActnList.hpp>
#include <CheckLst.hpp>		        //指定路線

class TCallCenter_Main_Form : public TForm
{
__published:	// IDE-managed Components
        TStatusBar *StatusBar1;
	TMainMenu *MainMenu1;
	TMenuItem *Main1;
	TPopupMenu *PopupMenu_Car;
	TMenuItem *MenuItem_AskReport;
	TLabel *Label6;
	TImage *Image_Map;
	TCoolBar *CoolBar_Tools;
	TToolBar *ToolBar_MapOp;
	TToolButton *ToolButton_ZoomIn;
	TToolButton *ToolButton_ZoomOut;
        TToolBar *ToolBar_Windows;
	TImageList *ImageList_ToolBar_Main;
	TPanel *Panel_DynBar;
	TToolButton *ToolButton_Reflesh;
	TSpeedButton *SpeedButton_ShowSetting;
	TSpeedButton *SpeedButton_ShowStreet;
        TToolBar *ToolBar_Status;
	TLabel *Label1;
	TStaticText *StaticText_ActiveCarCount;
	TLabel *Label3;
	TStaticText *StaticText_CarsStatus;
	TPopupMenu *PopupMenu_Zoom;
	TMenuItem *N3x1;
	TMenuItem *N4x1;
	TMenuItem *N5x1;
	TMenuItem *N6x1;
	TMenuItem *N7x1;
	TMenuItem *N8x1;
	TMenuItem *N9x1;
	TMenuItem *N10x1;
	TMenuItem *N11x1;
	TMenuItem *N12x1;
	TMenuItem *N13x1;
	TLabel *Label4;
	TStaticText *StaticText_TotalCarCount;
	TMenuItem *MenuItem_CancelTimeReport;
	TMenuItem *MenuItem_SetTimeReport;
	TPopupMenu *PopupMenu_ListView;
	TMenuItem *MenuItem_LV_AskReport;
	TMenuItem *MenuItem_LV_SetTimeReport;
	TMenuItem *MenuItem_LV_CancelTimeReport;
	TMenuItem *N1;
	TMenuItem *Help1;
	TMenuItem *N3;
	TMenuItem *MenuItem_RemoteCmd;
	TMenuItem *MenuItem_ChgLocatorPwd;
	TMenuItem *MenuItem_LV_ChgLocatorPwd;
	TMenuItem *MenuItem_LV_RemoteCmd;
	TPopupMenu *PopupMenu_MainForm;
	TMenuItem *MenuItem_AddRemarks;
	TMenuItem *N14;
	TMenuItem *Zoom1;
	TMenuItem *M_N4x1;
	TMenuItem *M_N5x1;
	TMenuItem *M_N6x1;
	TMenuItem *M_N7x1;
	TMenuItem *M_N8x1;
	TMenuItem *M_N9x1;
	TMenuItem *M_N10x1;
	TMenuItem *M_N11x1;
	TMenuItem *M_N12x1;
	TMenuItem *M_N13x1;
	TPopupMenu *PopupMenu_Remarks;
	TMenuItem *MI_Remarks_Content;
	TMenuItem *MI_Remarks_Delete;
	TMenuItem *MI_Remarks_Change;
	TMenuItem *MI_Remarks_SetAsCenter;
	TMenuItem *N16;
	TMenuItem *Menu_Wnd_Street;
	TMenuItem *Menu_Wnd_Setting;
	TMenuItem *N20;
        TMenuItem *Menu_Car;
        TMenuItem *Menu_Parameter;
        TMenuItem *Menu_Remark;
	TMenuItem *N25;
	TMenuItem *Menu_MapOp_Pointer;
	TMenuItem *Menu_MapOp_SetPath;
	TMenuItem *Menu_MapOp_DragMap;
	TLabel *Label7;
	TStaticText *StaticText_MonitorSattus;
	TToolButton *ToolButton3;
	TToolButton *ToolButton4;
	TSpeedButton *SpeedButton_DragMap;
	TSpeedButton *SpeedButton_SetPath;
	TSpeedButton *SpeedButton_Pointer;
	TMenuItem *M_N2x1;
	TMenuItem *M_N3x1;
	TMenuItem *M_N1x1;
	TMenuItem *N2x1;
	TMenuItem *N1x1;
	TImageList *ImageList_CarsStatus;
	TMenuItem *MenuItem_ChgCarImg;
	TMenuItem *MenuItem_0_Default;
	TMenuItem *MenuItem_1_Truck;
	TMenuItem *MenuItem_2_Container;
	TMenuItem *MenuItem_AddTraceWnd;
        TMenuItem *Menu_GIS;
	TMenuItem *MenuItem_SetSelPath;
	TMenuItem *N13;
	TMenuItem *MenuItem_CancelSelPath;
	TPopupMenu *PopupMenu_SetPath;
	TMenuItem *MenuItem_AddToPathGroup;
	TMenuItem *MenuItem_ClearThisPath;
	TMenuItem *MenuItem_ContinueSetPath;
	TStaticText *StaticText_ExceptCars;
	TLabel *Label15;
	TToolButton *ToolButton_CancelAllLock;
	TMenuItem *MenuItem_ShowCarTrace;
	TMenuItem *N34;
	TMenuItem *MenuItem_CancelCarTrace;
	TMenuItem *Menu_SYS_PrintImage;
	TMenuItem *Menu_SYS_PrintAll;
	TMenuItem *Menu_SYS_Print_Map_Cars;
	TMenuItem *Menu_SYS_Print_Map_Remarks;
	TMenuItem *Menu_SYS_Print_MapOnly;
	TMenuItem *Menu_SYS_Close;
	TMenuItem *N38;
        TMenuItem *Menu_Search;
        TMenuItem *Menu_Route;
	TMenuItem *Menu_MapOP_ZoomIn;
	TMenuItem *Menu_MapOP_ZoomOut;
	TMenuItem *Menu_MapOp_Reflesh;
	TMenuItem *N37;
	TMenuItem *N39;
	TMenuItem *Menu_MapOp_CancelAllLock;
	TMenuItem *MenuItem_ShowAllSelPath;
	TMenuItem *M_MenuItem_LocalArea;
	TMenuItem *MenuItem_LocalArea;
	TMenuItem *MenuItem_LoadHisCarTrace;
	TSaveDialog *SaveDialog_Track;
	TOpenDialog *OpenDialog_Track;
	TMenuItem *Menu_Report;
        TMenuItem *N_LVCar_Cmd;
        TMenuItem *MenuItemLV_CarHisTrace;
        TMenuItem *Menu_HisTrace;
        TMenuItem *N8;
        TMenuItem *MenuItemLV_AddTraceWnd;
        TMenuItem *MenuItemLV_SetSelPath;
        TMenuItem *MenuItemLV_CancelSelPath;
        TMenuItem *N_LVCar_Watch;
        TMenuItem *MenuItemLV_ShowCarTrace;
        TMenuItem *MenuItemLV_CancelCarTrace;
        TMenuItem *N_LVCar_Trace;
        TMenuItem *N_LVCar_Type;
        TMenuItem *MenuItemLV_2_Container;
        TMenuItem *MenuItemLV_1_Truck;
        TMenuItem *MenuItemLV_0_Default;
        TSplitter *Splitter1;
        TPanel *Panel_Street;
        TImage *Image_ZoomIn;
        TMenuItem *N6;
        TMenuItem *N9;
        TPopupMenu *PopupMenu_LV_Remarks;
        TMenuItem *MenuItem1;
        TMenuItem *MenuItem2;
        TMenuItem *MenuItem3;
        TMenuItem *MenuItem4;
        TLabel *Label2;
        TToolBar *ToolBar_Connect;
        TStaticText *StaticText_ConnectStatus;
        TToolButton *ToolButton_HK;
        TMenuItem *N_Query;
        TMenuItem *N_History;
        TSpeedButton *SpeedButton_Query;
        TSpeedButton *SpeedButton_History;
        TMenuItem *N_HKMAP;
        TMenuItem *ChangePassword;
        TMenuItem *N_Equipment_Search;
        TMenuItem *N_Road_Search;
        TMenuItem *N_Remarks_Search;
        TSpeedButton *SpeedButton_StatManager;
        TMenuItem *N_StatManager;
        TMenuItem *IO1;
        TMenuItem *IO11;
        TMenuItem *IO21;
        TMenuItem *IO31;
        TMenuItem *IO41;
        TMenuItem *IO2;
        TMenuItem *IO12;
        TMenuItem *IO22;
        TMenuItem *IO32;
        TMenuItem *IO42;
        TSpeedButton *SpeedButton_Trip;
        TMenuItem *Menu_HisTrip;
        TMenuItem *N_Trip;
        TMenuItem *MenuItemLV_CarTrip;
        TMenuItem *MenuItem_LoadTrip;
        TToolButton *ToolButton1;
        TLabel *Label5;
        TToolButton *ToolButton2;
        TLabel *Label8;
        TToolButton *ToolButton5;
        TToolButton *ToolButton6;
        TMenuItem *N2;
        TMenuItem *N_SetReportCheck;
        TMenuItem *N4;
        TMenuItem *N_LV_SetReportCheck;
        TLabel *Label_Scale;
        TTimer *Timer1;
        TMenuItem *N_CarInfo;
        TMenuItem *N_UserManager;
        TToolButton *ToolButton8;
        TToolButton *ToolButton9;
        TToolButton *ToolButton10;
        TToolButton *ToolButton11;
        TMenuItem *MenuItemLV_3_NINE;
        TMenuItem *MenuItem_3_NINE;
        TMenuItem *MenuItemLV_4_BUS;
        TMenuItem *MenuItem_4_BUS;
        TMenuItem *MenuItemLV_5_TRUCK2;
        TMenuItem *MenuItem_5_TRUCK2;
        TMenuItem *MenuItemLV_6_DBUS;
        TMenuItem *MenuItem_6_DBUS;
    TProgressBar *ProgressBar1;
    TProgressBar *ProgressBar2;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ToolButton_ZoomOutClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall Panel_StreetResize(TObject *Sender);
	void __fastcall ToolButton_RefleshClick(TObject *Sender);
	void __fastcall SpeedButton_ShowSettingClick(TObject *Sender);
	void __fastcall SpeedButton_ShowStreetClick(TObject *Sender);
	void __fastcall ToolButton_ZoomInClick(TObject *Sender);
	void __fastcall N3x1Click(TObject *Sender);
	void __fastcall MenuItem_AskReportClick(TObject *Sender);
	void __fastcall PopupMenu_CarPopup(TObject *Sender);
	void __fastcall MenuItem_CancelTimeReportClick(TObject *Sender);
	void __fastcall MenuItem_LV_AskReportClick(TObject *Sender);
	void __fastcall MenuItem_SetTimeReportClick(TObject *Sender);
	void __fastcall N3Click(TObject *Sender);
	void __fastcall MenuItem_LV_SetTimeReportClick(TObject *Sender);
	void __fastcall MenuItem_RemoteCmdClick(TObject *Sender);
	void __fastcall MenuItem_ChgLocatorPwdClick(TObject *Sender);
	void __fastcall MenuItem_AddRemarksClick(TObject *Sender);
	void __fastcall MI_Remarks_SetAsCenterClick(TObject *Sender);
	void __fastcall MI_Remarks_DeleteClick(TObject *Sender);
	void __fastcall MI_Remarks_ChangeClick(TObject *Sender);
	void __fastcall MI_Remarks_ContentClick(TObject *Sender);
	void __fastcall PopupMenu_RemarksPopup(TObject *Sender);
	void __fastcall MenuItem_LV_CancelTimeReportClick(TObject *Sender);
	void __fastcall MenuItem_LV_RemoteCmdClick(TObject *Sender);
	void __fastcall MenuItem_LV_ChgLocatorPwdClick(TObject *Sender);
	void __fastcall MenuItem_LV_ChgCenterNoClick(TObject *Sender);
	void __fastcall Menu_MapOp_PointerClick(TObject *Sender);
	void __fastcall Menu_MapOp_SetPathClick(TObject *Sender);
	void __fastcall Menu_MapOp_DragMapClick(TObject *Sender);
	void __fastcall Image_MapMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall Image_MapMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall MenuItem_0_DefaultClick(TObject *Sender);
	void __fastcall MenuItem_1_TruckClick(TObject *Sender);
	void __fastcall MenuItem_2_ContainerClick(TObject *Sender);
	void __fastcall MenuItem_AddTraceWndClick(TObject *Sender);
	void __fastcall Menu_CarClick(TObject *Sender);
	void __fastcall Menu_ParameterClick(TObject *Sender);
	void __fastcall Menu_RemarkClick(TObject *Sender);
	void __fastcall Menu_GISClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall CheckBox_DrawColorAreaClick(TObject *Sender);
	void __fastcall MenuItem_SetSelPathClick(TObject *Sender);
	void __fastcall MenuItem_CancelSelPathClick(TObject *Sender);
	void __fastcall MenuItem_AddToPathGroupClick(TObject *Sender);
	void __fastcall MenuItem_ClearThisPathClick(TObject *Sender);
	void __fastcall ToolButton_CancelAllLockClick(TObject *Sender);
	void __fastcall MenuItem_ShowCarTraceClick(TObject *Sender);
	void __fastcall MenuItem_CancelCarTraceClick(TObject *Sender);
	void __fastcall Menu_SYS_PrintAllClick(TObject *Sender);
	void __fastcall Menu_SYS_Print_Map_CarsClick(TObject *Sender);
	void __fastcall Menu_SYS_Print_Map_RemarksClick(TObject *Sender);
	void __fastcall Menu_SYS_Print_MapOnlyClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall Menu_SYS_CloseClick(TObject *Sender);
	void __fastcall Menu_Wnd_SettingClick(TObject *Sender);
	void __fastcall Menu_Wnd_StreetClick(TObject *Sender);
	void __fastcall MenuItem_ShowAllSelPathClick(TObject *Sender);
	void __fastcall PopupMenu_ListViewPopup(TObject *Sender);
	void __fastcall Help1Click(TObject *Sender);
	void __fastcall MenuItem_LoadHisCarTraceClick(TObject *Sender);
	void __fastcall Menu_ReportClick(TObject *Sender);
	void __fastcall Image_MapMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall MenuItemLV_CarHisTraceClick(TObject *Sender);
        void __fastcall Menu_RouteClick(TObject *Sender);
        void __fastcall PopupMenu_LV_RemarksPopup(TObject *Sender);
        void __fastcall ToolButton_HKClick(TObject *Sender);
        void __fastcall SpeedButton_QueryClick(TObject *Sender);
        void __fastcall SpeedButton_HistoryClick(TObject *Sender);
        void __fastcall Splitter1Moved(TObject *Sender);
        void __fastcall ChangePasswordClick(TObject *Sender);
        void __fastcall N_Equipment_SearchClick(TObject *Sender);
        void __fastcall N_Road_SearchClick(TObject *Sender);
        void __fastcall N_Remarks_SearchClick(TObject *Sender);
        void __fastcall SpeedButton_StatManagerClick(TObject *Sender);
        void __fastcall MenuItemLV_AddTraceWndClick(TObject *Sender);
        void __fastcall MenuItemLV_CancelSelPathClick(TObject *Sender);
        void __fastcall MenuItemLV_0_DefaultClick(TObject *Sender);
        void __fastcall MenuItemLV_1_TruckClick(TObject *Sender);
        void __fastcall MenuItemLV_2_ContainerClick(TObject *Sender);
        void __fastcall IO11Click(TObject *Sender);
        void __fastcall IO21Click(TObject *Sender);
        void __fastcall IO31Click(TObject *Sender);
        void __fastcall IO41Click(TObject *Sender);
        void __fastcall IO12Click(TObject *Sender);
        void __fastcall IO22Click(TObject *Sender);
        void __fastcall IO32Click(TObject *Sender);
        void __fastcall IO42Click(TObject *Sender);
        void __fastcall M_N1x1Click(TObject *Sender);
        void __fastcall SpeedButton_TripClick(TObject *Sender);
        void __fastcall MenuItemLV_CarTripClick(TObject *Sender);
        void __fastcall MenuItem_LoadTripClick(TObject *Sender);
        void __fastcall N_SetReportCheckClick(TObject *Sender);
        void __fastcall N_LV_SetReportCheckClick(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall MenuItemLV_ShowCarTraceClick(TObject *Sender);
        void __fastcall MenuItemLV_CancelCarTraceClick(TObject *Sender);
        void __fastcall MenuItemLV_SetSelPathClick(TObject *Sender);
        void __fastcall N_CarInfoClick(TObject *Sender);
        void __fastcall N_UserManagerClick(TObject *Sender);
        void __fastcall MenuItemLV_3_NINEClick(TObject *Sender);
        void __fastcall MenuItem_3_NINEClick(TObject *Sender);
        void __fastcall MenuItemLV_4_BUSClick(TObject *Sender);
        void __fastcall MenuItem_4_BUSClick(TObject *Sender);
        void __fastcall MenuItemLV_5_TRUCK2Click(TObject *Sender);
        void __fastcall MenuItem_5_TRUCK2Click(TObject *Sender);
        void __fastcall MenuItemLV_6_DBUSClick(TObject *Sender);
        void __fastcall MenuItem_6_DBUSClick(TObject *Sender);

private:	// User declarations
        char m_strDemoAlert[150];
public:		// User declarations
        Graphics::TBitmap* dc_bitmap_Map;
        bool ThreadDeleteing;
        char WorkingPath[512];                  //工作目錄
        float LateReportRate;                   //最晚回報時間百分比
        int ACC_OFF_Delay_Time;
        int DockSiteWidth;                      //Docksite的寬度
        CTMaintain* Maintain_Thread;            //即時維護主視窗狀態
        TList* m_pCarList;
        TList* CCarList_SortByLocatorNo;    //以LocatorNo排序
	TList* m_pRemarksList;
        TList* m_pSubFormList;
        TList* m_pSelPathGroupList;
        TCSelectPath* m_pDefaultSelPath;
        TList* m_pDrawTraceCarList;   //需要畫即時軌跡的車子指標 List
        long m_temp_X;     //游標中心點 X 座標 (經緯度座標)(MouseMove Temp)
        long m_temp_Y;     //游標中心點 Y 座標 (經緯度座標)(MouseMove Temp)
        double m_dScale;  //Image_Map Scale
        double m_dScale_ZoomInWnd;	//Image_ZoomIn Scale
        bool m_bIsLockMonitor;
        int m_iLockedAreaPixels;
        int m_PointerType;		//0:Pointer , 1:DragMap , 2:SetPath
        bool m_bInitialed;           //Define if the application is initialed at startup
        bool m_bIsBeginDragMap;
	bool m_bIsBeginDrawPath;
	bool m_bIsBeginDragMap_Taiwan;
        int m_Drag_Origin_sX;			//螢幕座標
        int m_Drag_Origin_sY;			//螢幕座標
        int m_Drag_Origin_sX_Taiwan;  	//螢幕座標
        int m_Drag_Origin_sY_Taiwan;  	//螢幕座標
        TPoint m_OriginPt,m_MovePt; 	    //TPoint 均以WGS座標表示 , 紀錄指定監控路線之前後點
        __fastcall TCallCenter_Main_Form(TComponent* Owner);
        void __fastcall UpdateAllObjPos();
        void __fastcall	ChangeImagePic(TImage* pImage,char* PicFilePath);
        void __fastcall	ChangeCarImgByTag(TImage* pImage,int OpType=0);	//OpType=0 => Lock <-> UnLock , OpType=1 =>正常->異常發生																//OpType=2 =>異常->異常正處理中
	void __fastcall	OnCarClick(TObject *Sender);  //Left Click the car
        void __fastcall	ChangeZoom(double scale);
        void __fastcall LVSendCmdByType(int DlgCmdType);
        void __fastcall Menu_ToolBar_Toggle(TCoolBand* pBandItem,TMenuItem* pMenuItem);
        void __fastcall DrawPathLine(TCanvas* pCanvas, TPoint ptOrigin, TPoint ptNow, TPenMode AMode,TColor clColor,int Width );
        void __fastcall CopyAllObjToQRImg(TQRImage* pQRImage,int CopyType);	//CopyType=0 =>只有地圖, 1 =>地圖+車子 , 2 =>地圖+地點 , 3 =>地圖+車子+地點
        bool __fastcall ShowReportEXE(char* szClassName,char* szWndName);
        bool __fastcall CloseReportEXE(char* szClassName,char* szWndName);
        void __fastcall AddSelPathGroup();      //vincent:Add list into self path group
        void __fastcall InitialDraw();
        void __fastcall DeleteThread();         //刪掉所有Thread,並讓影響的Timer暫停
        void __fastcall RebuildThread();        //重件所有Thread
        void __fastcall Update_CCar_List();             //更新CCar List
        CCar *GetCarByCarID(AnsiString CarID);               //Find the CCar point using the CarID
        CCar *GetCarByCarID_with_index(AnsiString CarID, int* index);
        CCar *GetCarByLocatorNo_with_index(AnsiString LocatorNo, int* index);
        CCar *GetCarByLocatorNo(AnsiString LocatorNo);       //以LocatorNumber找CCar point
        void __fastcall Insert_CCar_QSort(CCar* pCar);      //新加入CCar List
        TCSelectPath *GetRoute(int);            //Find the route point using the RouteNo
        void __fastcall Set_CCar_List_Empty(TList* pList);
};
//---------------------------------------------------------------------------
extern PACKAGE TCallCenter_Main_Form *CallCenter_Main_Form;
//---------------------------------------------------------------------------


#endif
