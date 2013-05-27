//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainForm.h"
#include "SubForm1.h"
#include "Common.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "MainForm"
#pragma resource "*.dfm"
TCallCenter_Sub_Form *CallCenter_Sub_Form;

//---------------------------------------------------------------------------
//__fastcall TCallCenter_Sub_Form::TCallCenter_Sub_Form(TComponent* Owner)
//	: TCallCenter_Main_Form(Owner)
__fastcall TCallCenter_Sub_Form::TCallCenter_Sub_Form(TComponent* Owner)
	: TForm(Owner)
{
        m_pCar = NULL;
        m_OldCar_X = 0;
	m_OldCar_Y = 0;
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Sub_Form::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	Hide();
        //Free Image Memory
   	delete  m_pImage_Car;
        //Delete ptr in TList
        for(int i=0;i<CallCenter_Main_Form->m_pSubFormList->Count;i++)
        {
    	        if(CallCenter_Main_Form->m_pSubFormList->Items[i]==this)
                {
		        CallCenter_Main_Form->m_pSubFormList->Delete(i);
                        break;
                }
        }
	//free Bitmap used in Image_SubMap
	Action = caFree;
}
//---------------------------------------------------------------------------

void __fastcall TCallCenter_Sub_Form::FormCreate(TObject *Sender)
{
        //Create Car Image
        m_pImage_Car = new TImage(this);
        m_pImage_Car->Parent = this;
        m_pImage_Car->Cursor = crHandPoint;
	m_pImage_Car->AutoSize = true;
	m_pImage_Car->Transparent = true;
        //Use Main Form's PopupMenu
        //m_pImage_Car->PopupMenu = CallCenter_Main_Form->PopupMenu_Car;
        //Use OnClick
        m_pImage_Car->OnDblClick = OnCarDbClick;
        //Use Hint
  	//m_pImage_Car->Hint = "";
        m_pImage_Car->ShowHint = true;
        m_pImage_Car->Tag = 0;	//Just Init Value
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Sub_Form::FormShow(TObject *Sender)
{
        if( m_pCar==NULL )
    	        return;
        ReDraw();
        //Image Status
	if(m_pImage_Car->Tag != m_pCar->m_pImage_Car->Tag)   //Change Car Image
        {
                m_pImage_Car->Tag = m_pCar->m_pImage_Car->Tag;
    	        CallCenter_Main_Form->ChangeCarImgByTag(m_pImage_Car,1);
    	        m_pImage_Car->Left = (Image_Map_Sub->Left) + (Image_Map_Sub->ClientWidth/2 - m_pImage_Car->Width/2);
    	        m_pImage_Car->Top = (Image_Map_Sub->Top) + (Image_Map_Sub->ClientHeight/2 - m_pImage_Car->Height/2);
        }
        Image_Map_Sub->ShowHint = true;
        Image_Map_Sub->Hint = AnsiString("追蹤監控視窗\n車號: ")+AnsiString(m_pCar->GetCarID());;
}
//---------------------------------------------------------------------------

void __fastcall	TCallCenter_Sub_Form::ReDraw()
{
        //以中心點為準
        if(m_pCar)
        {
                DrawSubFmImage(Image_Map_Sub,m_pCar->m_Status.m_iLon,m_pCar->m_Status.m_iLat,m_dScale);
                m_OldCar_X = m_pCar->m_Status.m_iLon;
	        m_OldCar_Y = m_pCar->m_Status.m_iLat;
                Label_CarStatus->Caption = m_pCar->m_pImage_Car->Hint;
        }
        else
        {
                Close();
        }
}
//---------------------------------------------------------------------------
bool __fastcall	TCallCenter_Sub_Form::IsCarMove_UpdateCarImg()
{
                if(m_pImage_Car->Tag != m_pCar->m_pImage_Car->Tag)   //Change Car Image
                {
                        m_pImage_Car->Tag = m_pCar->m_pImage_Car->Tag;
    	                CallCenter_Main_Form->ChangeCarImgByTag(m_pImage_Car,1);
    	                m_pImage_Car->Left = (Image_Map_Sub->Left) + (Image_Map_Sub->ClientWidth/2 - m_pImage_Car->Width/2);
    	                m_pImage_Car->Top = (Image_Map_Sub->Top) + (Image_Map_Sub->ClientHeight/2 - m_pImage_Car->Height/2);
                }
                if( (m_OldCar_X==m_pCar->m_Status.m_iLon)&&(m_OldCar_Y==m_pCar->m_Status.m_iLat) )
    	                return false;
                else
    	                return true;
}
//---------------------------------------------------------------------------

inline void __fastcall TCallCenter_Sub_Form::DrawSubFmImage(TImage* pImage,long WGS_X,long WGS_Y,double Scale)
{
	DrawImage(pImage,WGS_X,WGS_Y,Scale);
        DrawCenterLine(pImage,clRed,true);
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Sub_Form::FormResize(TObject *Sender)
{
	ReDraw();
    m_pImage_Car->Left = (Image_Map_Sub->Left) + (Image_Map_Sub->ClientWidth/2 - m_pImage_Car->Width/2);
    m_pImage_Car->Top = (Image_Map_Sub->Top) + (Image_Map_Sub->ClientHeight/2 - m_pImage_Car->Height/2);
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Sub_Form::OnCarDbClick(TObject *Sender)
{
    //Move Locked Car to center of Screen (Image_Map)
    DrawImage(CallCenter_Main_Form->Image_Map,m_pCar->m_Status.m_iLon,m_pCar->m_Status.m_iLat,CallCenter_Main_Form->m_dScale);
    CallCenter_Main_Form->UpdateAllObjPos();
    //set Center Line
    DrawCenterLine(CallCenter_Main_Form->Image_Map);
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Sub_Form::N13x1Click(TObject *Sender)
{
	double scale;
	TMenuItem* pMenu = (TMenuItem*)Sender;

    if( pMenu==N1x1 )	// 1x Click
        scale=1.0;
    else if( pMenu==N2x1 )
        scale=3.0;
    else if( pMenu==N3x1 )
        scale=7.0;
    else if( pMenu==N4x1 )
        scale=12.0;
    else if( pMenu==N5x1 )
        scale=20.0;
    else if( pMenu==N6x1 )
        scale=30.0;
    else if( pMenu==N7x1 )
        scale=25.0;
    else if( pMenu==N8x1 )
        scale=40.0;
    else if( pMenu==N9x1 )
        scale=50.0;
    else if( pMenu==N10x1 )
        scale=60.0;
    else if( pMenu==N11x1 )
        scale=70.0;
    else if( pMenu==N12x1 )
        scale=90.0;
    else
        scale=200.0;

	ChangeZoom(scale);
}
//---------------------------------------------------------------------------
void	__fastcall	TCallCenter_Sub_Form::ChangeZoom(double scale)
{
	if((scale>MAX_MAP_SCALE)||(scale<1.0))
    	return;
        ZoomIn_SubForm->Enabled = true;
        ZoomOut_SubForm->Enabled = true;
        //ZoomIn or Zoomout 以中心點為準
	m_dScale = scale;
        DrawSubFmImage(Image_Map_Sub,m_pCar->m_Status.m_iLon,m_pCar->m_Status.m_iLat,m_dScale);
}
//---------------------------------------------------------------------------
void __fastcall TCallCenter_Sub_Form::ZoomIn_SubFormClick(TObject *Sender)
{
        //ZoomIn or Zoomout 以中心點為準
	if(m_dScale-10.0 < 1.0 )
        {
    	        m_dScale = 1.0;
    	        ZoomIn_SubForm->Enabled = false;
        }
        else if(m_dScale-10.0 <= MAX_MAP_SCALE)
        {
    	        if(m_dScale <= 500.0)
			m_dScale -= 10.0;
                else
			m_dScale -= 5000.0;
                ZoomIn_SubForm->Enabled = true;
		ZoomOut_SubForm->Enabled = true;
        }
        else
		ZoomOut_SubForm->Enabled = false;
        DrawSubFmImage(Image_Map_Sub,m_pCar->m_Status.m_iLon,m_pCar->m_Status.m_iLat,m_dScale);
}
//---------------------------------------------------------------------------

void __fastcall TCallCenter_Sub_Form::ZoomOut_SubFormClick(TObject *Sender)
{
    //ZoomIn or Zoomout 以中心點為準
	if(m_dScale+10.0 > MAX_MAP_SCALE )
    {
	    m_dScale = MAX_MAP_SCALE;
    	ZoomIn_SubForm->Enabled = false;
    }
    else if(m_dScale+10.0 <= MAX_MAP_SCALE)
    {
    	if(m_dScale >= 500.0 )
			m_dScale += 5000.0;
        else
			m_dScale += 10.0;

        ZoomIn_SubForm->Enabled = true;
		ZoomOut_SubForm->Enabled = true;
    }
    else
		ZoomOut_SubForm->Enabled = false;

    DrawSubFmImage(Image_Map_Sub,m_pCar->m_Status.m_iLon,m_pCar->m_Status.m_iLat,m_dScale);
}
//---------------------------------------------------------------------------

void __fastcall TCallCenter_Sub_Form::ReDraw_SubFormClick(TObject *Sender)
{
	ReDraw();
}
//---------------------------------------------------------------------------

void __fastcall TCallCenter_Sub_Form::Menu_ShowCarStatusClick(TObject *Sender)
{
	Menu_ShowCarStatus->Checked = !(Menu_ShowCarStatus->Checked);
	Label_CarStatus->Visible = Menu_ShowCarStatus->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TCallCenter_Sub_Form::Timer1Timer(TObject *Sender)
{
//假如Terminate,將Timer停掉
        if(Application->Terminated)
        {
                Timer1->Enabled = false;
                return;
        }
        ReDraw();
}

