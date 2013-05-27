//---------------------------------------------------------------------------

#ifndef TripFormH
#define TripFormH

#define START_TIME 0
#define END_DATE 1
#define END_TIME 2
#define START_PLACE 3
#define END_PLACE 4
#define PERIOD 5
#define MILEAGE 6
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TraceForm.h"
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm_Trip : public TForm_Trace
{
__published:	// IDE-managed Components
        void __fastcall FormShow(TObject *Sender);
        void __fastcall TraceListViewDblClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall LocatorNoBoxDropDown(TObject *Sender);
        void __fastcall SpeedButton1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm_Trip(TComponent* Owner);
        AnsiString GetPeriod(AnsiString TimeA, AnsiString TimeB);               //比較兩筆時間字串,找出其時間差距
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_Trip *Form_Trip;
//---------------------------------------------------------------------------
#endif
