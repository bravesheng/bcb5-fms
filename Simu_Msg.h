  // File: Simu_Msg.h
//

#ifndef __SIMU_MSG_H__
#define __SIMU_MSG_H__

//for Send Simulated Data to Other Wnd
struct sSimuMsg
{
	char szPhone[20];
	char szData[160];
};

//---------------------------------------------------------------------------
struct sSimuCarData
{
   HANDLE m_hPlayFile_SimuCar;
   char   m_CarID_SimuCar[10];
   char   m_LocatorNo_SimuCar[11];
   int    m_iCurrent_SimuCar;
   int    m_iItemCount_SimuCar;
   int    m_iDelayTime_SimuCar;    // 1/1000 秒
   bool   m_bAutoRePlay_SimuCar;
   int    m_iPlayStatus_SimuCar;   //0:暫停 , 1:播放中 , -1:播畢 -2:未播放
   char   m_FilePath_SimuCar[256];
   void*  m_ptrThread;
   int    m_MsgType;               //0:正常 , 1:被竊 , 4:緊急事件
};

/*

///////////  for send Msg to Other Wnd  ///////////////
//Net Module Monitor => ReceiverID = 0
static  LPCTSTR    g_Recv_Wnd_Name_Nonitor        = "CallCenter 網路監控程式";
static  LPCTSTR    g_Recv_Wnd_ClassName_Monitor   = "#32770"; //Center_Demo

//Net Module Simulator => ReceiverID = 1
static  LPCTSTR    g_Recv_Wnd_Name_Simulator      = "Net Module 測試模擬器";
static  LPCTSTR    g_Recv_Wnd_ClassName_Simulator = "TSimulator_Form";

//CallCenter Demo => ReceiverID = 2
static  LPCTSTR    g_Recv_Wnd_Name_CallCenterDemo      = "CallCenter Demo 程式";
static  LPCTSTR    g_Recv_Wnd_ClassName_CallCenterDemo = "TCallCenter_Demo_Form";

inline void SendDataToWnd (void* p,DWORD size, int ReceiverID=0)
{
    COPYDATASTRUCT cd;
    HWND hWnd ;
	if(ReceiverID == 0)
		hWnd = ::FindWindow (g_Recv_Wnd_ClassName_Monitor, g_Recv_Wnd_Name_Nonitor);
	else if(ReceiverID == 1)
		hWnd = ::FindWindow (g_Recv_Wnd_ClassName_Simulator, g_Recv_Wnd_Name_Simulator);
	else if(ReceiverID == 2)
		hWnd = ::FindWindow (g_Recv_Wnd_ClassName_CallCenterDemo, g_Recv_Wnd_Name_CallCenterDemo);
	else
	{
		::MessageBox(NULL,"Receiver ID Error !! (0,1,2)","CallCenter SendDataToWnd()",MB_ICONSTOP|MB_OK);
		return;
	}

    if (hWnd)
    {
        cd.dwData = 0;
        cd.cbData = size;
        cd.lpData = (void *)p;
        ::SendMessageTimeout (hWnd, WM_COPYDATA, 0, (LPARAM)&cd, SMTO_NORMAL, 0, 0);
    }
}

*/


#endif//  __SIMU_MSG_H__
