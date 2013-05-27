//---------------------------------------------------------------------------

#ifndef SettingFormH
#define SettingFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <CheckLst.hpp>
#include <ComCtrls.hpp>
#include <Mask.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm_Setting : public TForm
{
__published:	// IDE-managed Components
        TPageControl *PageControl1;
        TTabSheet *TabMap;
        TTabSheet *TabPath;
        TGroupBox *GroupBox1;
        TCheckListBox *LayerBox;
        TComboBox *ComboBox_Layer;
        TGroupBox *GroupBox_MonitorArea;
        TLabel *Label_LockedAreaPixels;
        TLabel *Label8;
        TLabel *Label10;
        TLabel *Label9;
        TLabel *Label13;
        TLabel *Label16;
        TLabel *Label17;
        TMaskEdit *MaskEdit_LockedAreaPixels;
        TUpDown *UpDown_LockedAreaPixels;
        TBitBtn *BitBtn_ShowAllSelPath;
        TBitBtn *BitBtn1;
        TEdit *Edit_PathArea;
        TEdit *Edit_MaxTrace;
        TButton *Button1;
        void __fastcall ComboBox_LayerChange(TObject *Sender);
        void __fastcall LayerBoxClickCheck(TObject *Sender);
        void __fastcall MaskEdit_LockedAreaPixelsChange(TObject *Sender);
        void __fastcall UpDown_LockedAreaPixelsChanging(TObject *Sender,
          bool &AllowChange);
        void __fastcall BitBtn_ShowAllSelPathClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall Edit_PathAreaExit(TObject *Sender);
        void __fastcall Edit_PathAreaKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall Edit_MaxTraceExit(TObject *Sender);
        void __fastcall Edit_MaxTraceKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm_Setting(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_Setting *Form_Setting;
//---------------------------------------------------------------------------
#endif
