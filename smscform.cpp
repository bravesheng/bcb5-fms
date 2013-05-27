//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "smscform.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_SMSC *Form_SMSC;
//---------------------------------------------------------------------------
__fastcall TForm_SMSC::TForm_SMSC(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm_SMSC::BitBtn1Click(TObject *Sender)
{
   DataSource1->Enabled = false;
   Table1->Active = false;
   Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_SMSC::FormShow(TObject *Sender)
{
        Table1->Active = false;
        Table1->Active = true;
        DataSource1->Enabled = false;
        DataSource1->Enabled = true;
}
//---------------------------------------------------------------------------


void __fastcall TForm_SMSC::FormClose(TObject *Sender,
      TCloseAction &Action)
{
   DataSource1->Enabled = false;
   Table1->Active = false;
   Action = caFree;
   Form_SMSC = NULL;
}
//---------------------------------------------------------------------------

