//---------------------------------------------------------------------------

#ifndef CarinfoFormH
#define CarinfoFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TForm_CarInfo : public TForm
{
__published:	// IDE-managed Components
        TListView *ListView1;
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm_CarInfo(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_CarInfo *Form_CarInfo;
//---------------------------------------------------------------------------
#endif
