//---------------------------------------------------------------------------

#ifndef TraceFormH
#define TraceFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Menus.hpp>
#include "CCALENDR.h"
#include <Grids.hpp>
#include "CSPIN.h"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm_Trace : public TForm
{
__published:	// IDE-managed Components
        TDateTimePicker *StartDatePicker;
        TDateTimePicker *StartTimePicker;
        TDateTimePicker *EndDatePicker;
        TDateTimePicker *EndTimePicker;
        TComboBox *LocatorNoBox;
        TLabel *Label2;
        TListView *TraceListView;
        TStatusBar *StatusBar1;
        TLabel *Label1;
        TLabel *Label3;
        TBevel *Bevel1;
        TSpeedButton *SpeedButton1;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall TraceListViewDblClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall LocatorNoBoxDropDown(TObject *Sender);
        void __fastcall SpeedButton1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm_Trace(TComponent* Owner);
        void __fastcall ClearAllTrace_InMemory();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_Trace *Form_Trace;
//---------------------------------------------------------------------------
#endif
