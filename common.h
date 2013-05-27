#ifndef __COMMON_H__
#define __COMMON_H__

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "SelectPath.h"			//���w���u
#include "DisplayDll.h"
#include <inifiles.hpp>

extern int g_iMapType;			// iMapType=0 -> TW Local Test , iMapType=1 -> HK Map
extern  char g_DrawFlag[12];
extern int PathArea;
extern int MaxTracePoint;
//for Call Report Generator EXE
#define     ReportClass_Name     "TfrmCarInfo"
///////////  Max Limit  ///////
#define		MAX_MAP_SCALE	   	1000.0
#define		MAX_SUBFORM_COUNT   8
#define		MAX_REMARKS_COUNT	1024
#define		MAX_SEL_PATH_COUNT	512		//���w���u�ƶq
#define		MAX_ALERT_HIDE_SECOND	120  //ĵ�ܵ������ë�A���������j�ɶ�
#define		TRACE_POINT_PIXEL		4	//�Y�ɭy���I�b�|
#define		DRAW_CARID_INTERVAL		5	//�CDRAW_CARID_INTERVAL �I�e�����@��
#define		MIN_DRAG_MAP_PIXEL		4	//�԰ʦa�Ϧܤ֪�Pixel�ƥH�W�~�u�����ʦa��
///////////  Items Define  ///////////
//#define  	REAL_TIME_DATA_SIZE		100		//(include dummy)
#define PI                   3.141592653589793    //for Point and Line Calculation
#define RADIAN_PER_DEGREE    0.01745329251994     //for Point and Line Calculation

//for Custom's Cursor
#define crDragHandCur  1
#define crDragMoveCur  2

//2001.03.21 New Display Test for DCH
//Resource Path
#define 	REMARK_PLACE_PATH		"Remarks.mrk"
#define		NET_MONITOR_AVI			"..\\Data\\sample.avi"
#define		ALERT_SOUND_PATH		"..\\Data\\Alert.wav"
#define     HELP_FILE_PATH          "..\\Data\\Help.htm"
//Car Image
//============    �p�⨮   ================================================================================================================================================//
#define		CAR_GRAY_IMG				"..\\Data\\Cars Image\\Saloon\\CAR_Gray.bmp"
#define		CAR_BLUE_IMG				"..\\Data\\Cars Image\\Saloon\\CAR_Blue.bmp"
#define		CAR_RED_IMG				"..\\Data\\Cars Image\\Saloon\\CAR_Red.bmp"
#define		CAR_YELLOW_IMG				"..\\Data\\Cars Image\\Saloon\\CAR_Yellow.bmp"
#define         CAR_GREEN_IMG                           "..\\Data\\Cars Image\\Saloon\\CAR_Green.bmp"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
#define		CAR_GRAY_LOCKED_IMG			"..\\Data\\Cars Image\\Saloon\\CAR_Gray_Lock.bmp"
#define		CAR_BLUE_LOCKED_IMG			"..\\Data\\Cars Image\\Saloon\\CAR_Blue_Lock.bmp"
#define		CAR_RED_LOCKED_IMG			"..\\Data\\Cars Image\\Saloon\\CAR_Red_Lock.bmp"
#define		CAR_YELLOW_LOCKED_IMG		        "..\\Data\\Cars Image\\Saloon\\CAR_Yellow_Lock.bmp"
#define         CAR_GREEN_LOCKED_IMG                    "..\\Data\\Cars Image\\Saloon\\CAR_Green_Lock.bmp"
//========================================================================================================================================================================//
//============    �f��    ================================================================================================================================================//
#define		TRUCK_GRAY_IMG				"..\\Data\\Cars Image\\Truck\\Truck_Gray.bmp"
#define		TRUCK_BLUE_IMG				"..\\Data\\Cars Image\\Truck\\Truck_Blue.bmp"
#define		TRUCK_RED_IMG				"..\\Data\\Cars Image\\Truck\\Truck_Red.bmp"
#define		TRUCK_YELLOW_IMG			"..\\Data\\Cars Image\\Truck\\Truck_Yellow.bmp"
#define		TRUCK_GREEN_IMG			        "..\\Data\\Cars Image\\Truck\\Truck_Green.bmp"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
#define		TRUCK_GRAY_LOCKED_IMG		"..\\Data\\Cars Image\\Truck\\Truck_Gray_Lock.bmp"
#define		TRUCK_BLUE_LOCKED_IMG		"..\\Data\\Cars Image\\Truck\\Truck_Blue_Lock.bmp"
#define		TRUCK_RED_LOCKED_IMG		"..\\Data\\Cars Image\\Truck\\Truck_Red_Lock.bmp"
#define		TRUCK_YELLOW_LOCKED_IMG		"..\\Data\\Cars Image\\Truck\\Truck_Yellow_Lock.bmp"
#define		TRUCK_GREEN_LOCKED_IMG		"..\\Data\\Cars Image\\Truck\\Truck_Green_Lock.bmp"
//========================================================================================================================================================================//
//============    �s����   ================================================================================================================================================//
#define		CONTAINER_GRAY_IMG			"..\\Data\\Cars Image\\Container\\Container_Gray.bmp"
#define		CONTAINER_BLUE_IMG			"..\\Data\\Cars Image\\Container\\Container_Blue.bmp"
#define		CONTAINER_RED_IMG			"..\\Data\\Cars Image\\Container\\Container_Red.bmp"
#define		CONTAINER_YELLOW_IMG		        "..\\Data\\Cars Image\\Container\\Container_Yellow.bmp"
#define		CONTAINER_GREEN_IMG		        "..\\Data\\Cars Image\\Container\\Container_Green.bmp"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
#define		CONTAINER_GRAY_LOCKED_IMG	"..\\Data\\Cars Image\\Container\\Container_Gray_Lock.bmp"
#define		CONTAINER_BLUE_LOCKED_IMG	"..\\Data\\Cars Image\\Container\\Container_Blue_Lock.bmp"
#define		CONTAINER_RED_LOCKED_IMG	"..\\Data\\Cars Image\\Container\\Container_Red_Lock.bmp"
#define		CONTAINER_YELLOW_LOCKED_IMG	"..\\Data\\Cars Image\\Container\\Container_Yellow_Lock.bmp"
#define		CONTAINER_GREEN_LOCKED_IMG	"..\\Data\\Cars Image\\Container\\Container_Green_Lock.bmp"
//========================================================================================================================================================================//
//============    9�H�p�ڤh   ================================================================================================================================================//
#define	        NINE_GRAY_IMG                    "..\\Data\\Cars Image\\9\\9_Gray.bmp"
#define		NINE_BLUE_IMG			"..\\Data\\Cars Image\\9\\9_Blue.bmp"
#define		NINE_RED_IMG			"..\\Data\\Cars Image\\9\\9_Red.bmp"
#define		NINE_YELLOW_IMG		        "..\\Data\\Cars Image\\9\\9_Yellow.bmp"
#define		NINE_GREEN_IMG		        "..\\Data\\Cars Image\\9\\9_Green.bmp"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
#define		NINE_GRAY_LOCKED_IMG	"..\\Data\\Cars Image\\9\\9_Gray_Lock.bmp"
#define		NINE_BLUE_LOCKED_IMG	"..\\Data\\Cars Image\\9\\9_Blue_Lock.bmp"
#define		NINE_RED_LOCKED_IMG	"..\\Data\\Cars Image\\9\\9_Red_Lock.bmp"
#define		NINE_YELLOW_LOCKED_IMG	"..\\Data\\Cars Image\\9\\9_Yellow_Lock.bmp"
#define		NINE_GREEN_LOCKED_IMG	"..\\Data\\Cars Image\\9\\9_Green_Lock.bmp"
//========================================================================================================================================================================//
//============    �ڤh   ================================================================================================================================================//
#define	        BUS_GRAY_IMG                    "..\\Data\\Cars Image\\BUS\\BUS_Gray.bmp"
#define		BUS_BLUE_IMG			"..\\Data\\Cars Image\\BUS\\BUS_Blue.bmp"
#define		BUS_RED_IMG			"..\\Data\\Cars Image\\BUS\\BUS_Red.bmp"
#define		BUS_YELLOW_IMG		        "..\\Data\\Cars Image\\BUS\\BUS_Yellow.bmp"
#define		BUS_GREEN_IMG		        "..\\Data\\Cars Image\\BUS\\BUS_Green.bmp"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
#define		BUS_GRAY_LOCKED_IMG	"..\\Data\\Cars Image\\BUS\\BUS_Gray_Lock.bmp"
#define		BUS_BLUE_LOCKED_IMG	"..\\Data\\Cars Image\\BUS\\BUS_Blue_Lock.bmp"
#define		BUS_RED_LOCKED_IMG	"..\\Data\\Cars Image\\BUS\\BUS_Red_Lock.bmp"
#define		BUS_YELLOW_LOCKED_IMG	"..\\Data\\Cars Image\\BUS\\BUS_Yellow_Lock.bmp"
#define		BUS_GREEN_LOCKED_IMG	"..\\Data\\Cars Image\\BUS\\BUS_Green_Lock.bmp"
//========================================================================================================================================================================//
//============    �f��2    ================================================================================================================================================//
#define		TRUCK2_GRAY_IMG				"..\\Data\\Cars Image\\Truck2\\Truck2_Gray.bmp"
#define		TRUCK2_BLUE_IMG				"..\\Data\\Cars Image\\Truck2\\Truck2_Blue.bmp"
#define		TRUCK2_RED_IMG				"..\\Data\\Cars Image\\Truck2\\Truck2_Red.bmp"
#define		TRUCK2_YELLOW_IMG			"..\\Data\\Cars Image\\Truck2\\Truck2_Yellow.bmp"
#define		TRUCK2_GREEN_IMG			        "..\\Data\\Cars Image\\Truck2\\Truck2_Green.bmp"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
#define		TRUCK2_GRAY_LOCKED_IMG		"..\\Data\\Cars Image\\Truck2\\Truck2_Gray_Lock.bmp"
#define		TRUCK2_BLUE_LOCKED_IMG		"..\\Data\\Cars Image\\Truck2\\Truck2_Blue_Lock.bmp"
#define		TRUCK2_RED_LOCKED_IMG		"..\\Data\\Cars Image\\Truck2\\Truck2_Red_Lock.bmp"
#define		TRUCK2_YELLOW_LOCKED_IMG	"..\\Data\\Cars Image2\\Truck2\\Truck2_Yellow_Lock.bmp"
#define		TRUCK2_GREEN_LOCKED_IMG		"..\\Data\\Cars Image\\Truck2\\Truck2_Green_Lock.bmp"
//========================================================================================================================================================================//
//============    ���h�ڤh    ================================================================================================================================================//
#define		DBUS_GRAY_IMG				"..\\Data\\Cars Image\\DBUS\\DBUS_Gray.bmp"
#define		DBUS_BLUE_IMG				"..\\Data\\Cars Image\\DBUS\\DBUS_Blue.bmp"
#define		DBUS_RED_IMG				"..\\Data\\Cars Image\\DBUS\\DBUS_Red.bmp"
#define		DBUS_YELLOW_IMG			"..\\Data\\Cars Image\\DBUS\\DBUS_Yellow.bmp"
#define		DBUS_GREEN_IMG			        "..\\Data\\Cars Image\\DBUS\\DBUS_Green.bmp"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
#define		DBUS_GRAY_LOCKED_IMG		"..\\Data\\Cars Image\\DBUS\\DBUS_Gray_Lock.bmp"
#define		DBUS_BLUE_LOCKED_IMG		"..\\Data\\Cars Image\\DBUS\\DBUS_Blue_Lock.bmp"
#define		DBUS_RED_LOCKED_IMG		"..\\Data\\Cars Image\\DBUS\\DBUS_Red_Lock.bmp"
#define		DBUS_YELLOW_LOCKED_IMG	        "..\\Data\\Cars Image2\\DBUS\\DBUS_Yellow_Lock.bmp"
#define		DBUS_GREEN_LOCKED_IMG		"..\\Data\\Cars Image\\DBUS\\DBUS_Green_Lock.bmp"
//========================================================================================================================================================================//

//RemarkPlace Image
#define		REMARKS_HOME_IMG		"..\\Data\\Remarks\\Remarks_Home.bmp"
#define		REMARKS_DEFAULT_IMG		"..\\Data\\Remarks\\Remarks_Building1.bmp"

#define  REMARKS_SEARCH_START_IMG		"..\\Data\\Remarks\\Remarks_Start2.bmp"
#define	 REMARKS_SEARCH_END_IMG			"..\\Data\\Remarks\\Remarks_End2.bmp"
#define	 REMARKS_SEARCH_ROUTE_1_IMG		"..\\Data\\Remarks\\Remarks_Way1.bmp"
#define	 REMARKS_SEARCH_ROUTE_2_IMG		"..\\Data\\Remarks\\Remarks_Way2.bmp"
#define	 REMARKS_SEARCH_ROUTE_3_IMG		"..\\Data\\Remarks\\Remarks_Way3.bmp"
#define	 REMARKS_SEARCH_ROUTE_4_IMG		"..\\Data\\Remarks\\Remarks_Way4.bmp"
#define	 REMARKS_SEARCH_ROUTE_5_IMG		"..\\Data\\Remarks\\Remarks_Way5.bmp"

//////////////////  Common Class or Structure  /////////////////

unsigned short G_GetCmdID();
//Global Draw Function

void __fastcall WriteLog(AnsiString Str);         //�ΥH����log
AnsiString __fastcall IntToBinary(int source);
void 	__fastcall  Dlldraw(long HdcID,char fg[12]);
void 	__fastcall 	FreeDispDLL();
void 	__fastcall	InitDispDLL(int iMapType);   //iMapType=0 -> TW Local Test , iMapType=1 -> HK Map
void  	__fastcall	Hdc2WGS(long Hdc_X,long Hdc_Y,long& WGS_X,long& WGS_Y);
void  	__fastcall	WGS2Hdc(long WGS_X,long WGS_Y,long& Hdc_X,long& Hdc_Y);
void	__fastcall	ReDrawImage(TImage* pImage,double Scale);
void	__fastcall	DrawImage(TImage* pImage,long X,long Y,double Scale,int CoordType=0);//CoordType Defalult is WGS �y��
void 	__fastcall	DrawLockArea(TCanvas* pCanvas,TImage* pImage,int halfLockedArea);
void	__fastcall	DrawCenterLine(TImage* pImage,TColor cl=clBlue,bool bDrawCircular=false);
void 	__fastcall	DrawSelectPath(TCanvas* pCanvas,TCSelectPath* pPath,double Scale);
void 	__fastcall	DrawCarTrace(TCanvas* pCanvas,void* pCar,double Scale,int DrawType=0);//0: �e�Y�ɭy�� , 1: �e���v�y��
bool	__fastcall  IsInDrawRect(long WGS_X,long WGS_Y);	//WGS�y��
bool	__fastcall  AnyOneInDrawRect(TList* pPointList);
//for Point and Line calculate
short 	__fastcall  CalLineAngle(long xs, long ys, long xe, long ye);
float 	__fastcall	CalDist( long xs, long ys, long xe, long ye );
float	__fastcall 	MinLen_Point2Line( double Cx, double Cy, double Ax, double Ay, double Bx, double By );
bool 	__fastcall 	IsOverSelPath(long mX2,long mY2,TCSelectPath* pPath);
//Global GetPlace Info Function
char*   __fastcall  GetIISName(long WGS_X,long WGS_Y);
void    __fastcall  GetNowDateTime(char* out_szDate,char* out_szTime);
    int __fastcall 	GetImgIndexFromTag(int Tag);

///////////////////////////////////////////////////////////////

struct sCarStatus
{
        AnsiString DistName;      //���ݦ�F�ϦW��
        bool GPSLock;
        char m_szLocatorNo[21];
        char m_szRecvDate[10];
        char m_szRecvTime[10];
        char m_szSendDate[10];
        char m_szSendTime[10];
        int m_iStatus;
        int m_iStatusHandle;
        int m_iLon;
        int m_iLat;
        short m_sSpeed;
        AnsiString m_szPlaceName;
        int m_iDistID;
        double m_dMileage;          //��줽��
        int ReportRate;          //Vincent add:�^���W�v
};
struct sCmdMsg
{
    char   m_szLocatorNo[21];
    char   m_szDate[10];
    char   m_szTime[10];
    int    m_iCommand;
    char   m_szOptions[21];
    char   m_szPassword[10];
    int    m_iCmdID;
    char   m_szSender[21];
};

class CAuthorizeTime
{
public:
        AnsiString AutStartTime;                //���v�}�l�ɶ�hhmmss
        AnsiString AutEndTime;                  //���v�����ɶ�hhmmss
        AnsiString AutDate;                     //���v���--"00000000"��ܨS�����w,�Y�C�Ѱ���
};

/////////  for CCar Class  /////////////////
class CCar
{
public:
        TDateTime       CarInfoUpdateTime;
        TDateTime       LatestStatusUpdateTime;
        int             ReportQuality;  //�^�O�~�� 0:�^�����` 1:�^������T�{�� 2:�^������ 3:��L����
        int             GPSLock;        //gps�O�_lock
        unsigned long   ReportCheckRate;        //�^���ˬd�W�v
        sCarStatus      m_Status;
        int             RouteNo;	   //���w�ʱ����u���X
        bool            m_bIsOverPath;		//�O�_�W�X���w���u
        CAuthorizeTime  m_AuthorizeTime;      //���v��p�ɶ�
        int             IO_In_Watch;            //�ثe�ʱ�����I/O
        bool Vertifyed;         //�ΥH�M�w���x���O�_�q�L����,�T��s�b���Ʈw
        bool Visible;           //�����O�_�i��
        TImage*	        m_pImage_Car;
/***********************************************************************
        Image Tag =>  4=>UnLocked -> Emergency Handling(Yellow)
    //			=>  3=>UnLocked -> Emergency Occring(Red)
    //			=>  2=>UnLocked -> Active(Blue)
    //			=>  1=>UnLocked	-> Init(Gray)
    //Image Tag => -4=>Locked 	-> Emergency Handling(Yellow)
    //			=> -3=>Locked 	-> Emergency Occring(Red)
    //			=> -2=>Locked 	-> Active(Blue)
    //			=> -1=>Locked	-> Init(Gray)
***********************************************************************/
        TList*          m_pCarTraceList;        //�Y�ɭy���I�� List , NULL=>Not Show
	TList*          m_pHisCarTraceList;     //���v�y���I�� List , NULL=>Not Show
        AnsiString 	m_szCarID;
        int	        m_CarType;              //0:�p�⨮ , 1:�f�� , 2:�s���� , 3:ĵ�� , 4:������
        int             ImageColor;           //�Ϯת��C��
        long 	        m_Center_Xs;		//���l�����I X �ù��y��
        long 	        m_Center_Ys;		//���l�����I Y �ù��y��
        AnsiString CarType;             //����tCarType
        AnsiString CarGroup;            //����tCarGroup     

	CCar(AnsiString CarID,sCarStatus &Status,TWinControl* Owner, int Image);
        ~CCar();
        void __fastcall Update(sCarStatus* pStatus);	//if pStatus=NULL => Just Update Position (for DrawImage())
        inline bool __fastcall InLockedRect();
        inline void __fastcall UpdateHint();
        //Get Car Member
	char* __fastcall GetCarID();
	char* __fastcall GetPlaceInfo();
	long __fastcall GetCarSpeed();
        unsigned long __fastcall Get_ULong_Status();
	char*  	__fastcall GetCarStatus();
        char*   __fastcall GetDateTime();
        char*   __fastcall SendDateTime();
	long  	__fastcall GetCar_X2();
	long  	__fastcall GetCar_Y2();
        char*	__fastcall GetLocatorNo();
        long 	__fastcall GetLocatorVer(char* textLocatorVer);
        int 	__fastcall GetEventStatus(char* textEventStatus);      //To find if have some alarm occured
        AnsiString __fastcall IsEventOccr(unsigned long CarStatus,long LocatorVer);
        void 	__fastcall UpdateImgTag(int EventStatus);
        //Get CarInfo From Database
        char*	__fastcall GetLocatorPwd();
};
int __fastcall Compare_CCar_By_LocatorNo(void * CCar1, void * CCar2);
int __fastcall Compare_CCar_By_CarID(void * CCar1, void * CCar2);
#endif	//__COMMON_H__
