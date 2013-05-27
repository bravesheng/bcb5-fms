//---------------------------------------------------------------------------

#ifndef GetTrackThreadH
#define GetTrackThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
#include "common.h"

class TGetTrackThread : public TThread
{
private:
	void __fastcall GetLatestStatus();
        void __fastcall Determine_Car_Image_Color();    //�M�w�����C��
        void __fastcall DiagnoseConnect();
        void __fastcall OpenDBDetecotr();
        void __fastcall CarStatDetect();        //���������O�_���H�W����
        bool __fastcall ShowReportEXE(char* szClassName,char* szWndName);
        bool __fastcall CloseReportEXE(char* szClassName,char* szWndName);
        void __fastcall Check_Acknowledgement();
        void __fastcall Check_If_Update_From_DB();         //�ˬd�O�_�ݭn�Ѹ�Ʈw��s�ثe���
        void __fastcall Sync_Route_List();              //���@���w���uList���,�Ϥ��PDatabase�P�B
        void __fastcall ProcessSound();         //�B�zĵ�i�n���o�ͮɶ�
protected:
	void __fastcall Execute();
public:
        bool bCanTerminate;      //�O�_�i�H����thread
        __fastcall TGetTrackThread(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
