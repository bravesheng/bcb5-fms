//---------------------------------------------------------------------------

#ifndef LogoFormH
#define LogoFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include <MPlayer.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TForm_Logo : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TImage *Image_Logo;
        TMemo *Memo_VersionInfo;
        void __fastcall Image_LogoClick(TObject *Sender);
        void __fastcall Memo_VersionInfoClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm_Logo(TComponent* Owner);
        __fastcall InitApp();   //Initial Program in startup
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_Logo *Form_Logo;
//---------------------------------------------------------------------------
#endif
