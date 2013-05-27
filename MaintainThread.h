//---------------------------------------------------------------------------

#ifndef MaintainThreadH
#define MaintainThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "common.h"
//---------------------------------------------------------------------------
class CTMaintain : public TThread
{
private:
        void __fastcall PanelStat();
        void __fastcall Update_ListView_CarInfo(CCar* pCar,TListItem* pListItem);
        void __fastcall	UpdateCarStatusOnForm();
        void __fastcall UpdateCarStatusOnSubForm();
protected:
        void __fastcall Execute();
public:
        __fastcall CTMaintain(bool CreateSuspended);
        bool bCanTerminate;      //¬O§_¥i¥H°±¤îthread
};
//---------------------------------------------------------------------------
#endif
