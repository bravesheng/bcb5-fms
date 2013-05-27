//---------------------------------------------------------------------------

#ifndef FreeAllFormH
#define FreeAllFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm_FreeAll : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label_Statu;
private:	// User declarations
public:		// User declarations
        __fastcall TForm_FreeAll(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_FreeAll *Form_FreeAll;
//---------------------------------------------------------------------------
#endif
