//---------------------------------------------------------------------------

#ifndef SubForm1H
#define SubForm1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "MainForm.h"
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
#include <jpeg.hpp>
#include <Mask.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
//---------------------------------------------------------------------------
//class TCallCenter_Sub_Form : public TCallCenter_Main_Form
class TCallCenter_Sub_Form : public TForm
{
__published:	// IDE-managed Components
        TImage *Image_Map_Sub;
	TLabel *Label_CarStatus;
	TPopupMenu *PopupMenu_SubForm;
	TMenuItem *Zoom_SubForm;
	TMenuItem *N13x1;
	TMenuItem *N12x1;
	TMenuItem *N11x1;
	TMenuItem *N10x1;
	TMenuItem *N9x1;
	TMenuItem *N8x1;
	TMenuItem *N7x1;
	TMenuItem *N6x1;
	TMenuItem *N5x1;
	TMenuItem *N4x1;
	TMenuItem *N3x1;
	TMenuItem *N2x1;
	TMenuItem *N1x1;
	TMenuItem *ZoomIn_SubForm;
	TMenuItem *ZoomOut_SubForm;
	TMenuItem *N2;
	TMenuItem *ReDraw_SubForm;
	TMenuItem *Menu_ShowCarStatus;
        TTimer *Timer1;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall N13x1Click(TObject *Sender);
	void __fastcall ZoomIn_SubFormClick(TObject *Sender);
	void __fastcall ZoomOut_SubFormClick(TObject *Sender);
	void __fastcall ReDraw_SubFormClick(TObject *Sender);
	void __fastcall Menu_ShowCarStatusClick(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
private:	// User declarations
    void	__fastcall	ReDraw();
public:		// User declarations
	CCar*	m_pCar;
    TImage*	m_pImage_Car;
    long	m_OldCar_X;
	long	m_OldCar_Y;
	double	m_dScale;  	  //Image_Map Scale
    
	void	__fastcall	DrawSubFmImage(TImage* pImage,long WGS_X,long WGS_Y,double Scale);
    bool	__fastcall	IsCarMove_UpdateCarImg();
    void 	__fastcall 	OnCarDbClick(TObject *Sender);
    void	__fastcall	ChangeZoom(double scale);
	__fastcall TCallCenter_Sub_Form(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TCallCenter_Sub_Form *CallCenter_Sub_Form;
//---------------------------------------------------------------------------
#endif
