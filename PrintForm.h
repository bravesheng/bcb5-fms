//---------------------------------------------------------------------------

#ifndef PrintFormH
#define PrintFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TForm_Print : public TForm
{
__published:	// IDE-managed Components
        TImage *Image1;
        TMainMenu *MainMenu1;
        TMenuItem *N1;
        TMenuItem *N2;
        TPrinterSetupDialog *PrinterSetupDialog1;
        TMenuItem *N3;
        TMenuItem *N4;
        TMenuItem *N5;
        TMenuItem *N6;
        TMenuItem *N7;
private:	// User declarations
public:		// User declarations
        __fastcall TForm_Print(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_Print *Form_Print;
//---------------------------------------------------------------------------
#endif
