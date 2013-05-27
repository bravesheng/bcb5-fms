//---------------------------------------------------------------------------

#ifndef SmartQryFormH
#define SmartQryFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include <ImgList.hpp>
#include <ExtCtrls.hpp>
#include <CheckLst.hpp>
//---------------------------------------------------------------------------
struct sPoint		//指定路線的點
{
   long mX2;    //指定路線的點 - X 座標(WGS)
   long mY2;    //指定路線的點 - Y 座標(WGS)
};
class TForm_SmartQry : public TForm
{
__published:	// IDE-managed Components
	TImageList *ImageList_Facility;
	TPageControl *PageControl_Qry;
        TTabSheet *TabSheet_Poi;
	TListView *ListView_Results;
	TPanel *Panel1;
        TPanel *Panel2;
        TLabel *Label1;
        TComboBox *ComboBox_InputName;
        TBitBtn *BitBtn_Query;
        TTabSheet *TabSheet_Road;
        TStatusBar *StatusBar_Poi;
        TPanel *Panel3;
        TPanel *Panel4;
        TLabel *Label2;
        TComboBox *ComboBox_Road;
        TBitBtn *BitBtn_QueryRoad;
        TListView *ListView_Roads;
        TStatusBar *StatusBar_Road;
        TListBox *ListBox_DistPG;
        TEdit *Edit_DistPG;
        TLabel *Label3;
        TTrackBar *TrackBar1;
        TLabel *Label4;
        TTabSheet *TabSheet_Remark;
        TPanel *Panel5;
        TPanel *Panel6;
        TLabel *Label5;
        TComboBox *ComboBox_Remark;
        TBitBtn *BitBtn_QueryRemark;
        TCheckListBox *CheckListBox1;
        TListView *ListView_Remark;
        TCheckListBox *CheckListBox2;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall BitBtn_QueryClick(TObject *Sender);
	void __fastcall ListView_ResultsDblClick(TObject *Sender);
	void __fastcall ComboBox_InputNameKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall ListView_ResultsColumnClick(TObject *Sender,
          TListColumn *Column);
	void __fastcall ListView_ResultsCompare(TObject *Sender, TListItem *Item1,
          TListItem *Item2, int Data, int &Compare);
	void __fastcall ListView_AllResultsColumnClick(TObject *Sender,
          TListColumn *Column);
	void __fastcall ListView_AllResultsCompare(TObject *Sender,
          TListItem *Item1, TListItem *Item2, int Data, int &Compare);
	void __fastcall ListView_QryRoadColumnClick(TObject *Sender,
          TListColumn *Column);
	void __fastcall ListView_QryRoadCompare(TObject *Sender, TListItem *Item1,
          TListItem *Item2, int Data, int &Compare);
        void __fastcall CheckListBox1ClickCheck(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall BitBtn_QueryRoadClick(TObject *Sender);
        void __fastcall ListView_RoadsColumnClick(TObject *Sender,
          TListColumn *Column);
        void __fastcall ListView_RoadsCompare(TObject *Sender,
          TListItem *Item1, TListItem *Item2, int Data, int &Compare);
        void __fastcall ListView_RoadsDblClick(TObject *Sender);
        void __fastcall ComboBox_RoadKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall Edit_DistPGChange(TObject *Sender);
        void __fastcall TrackBar1Change(TObject *Sender);
        void __fastcall ListBox_DistPGClick(TObject *Sender);
        void __fastcall BitBtn_QueryRemarkClick(TObject *Sender);
        void __fastcall ListView_RemarkColumnClick(TObject *Sender,
          TListColumn *Column);
        void __fastcall ListView_RemarkCompare(TObject *Sender,
          TListItem *Item1, TListItem *Item2, int Data, int &Compare);
        void __fastcall ListView_RemarkDblClick(TObject *Sender);
        void __fastcall ComboBox_RemarkKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations

    int 	m_ColumnToSort_inListView_Results ;	//for Sort
    int 	m_ColumnToSort_inListView_AllResults ;	//for Sort
    int 	m_ColumnToSort_inListView_QryRoad ;	//for Sort
    int         m_ColumnToSort_inListView_QryRemark;
    Set <int,0,20> ObjSet;        //篩選條件用集合
    AnsiString PoiStr[20];        //設施種類名稱對應
    AnsiString RoadTypeStr[6];    //道路類別名稱對應      
public:		// User declarations
        sPoint DistPG_Point[1000];
	__fastcall TForm_SmartQry(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_SmartQry *Form_SmartQry;
//---------------------------------------------------------------------------
#endif
