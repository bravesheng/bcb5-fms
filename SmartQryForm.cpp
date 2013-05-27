//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "SmartQryForm.h"
#include "Common.h"
#include "DisplayDll.h"  		//for Test Displsy Dll
#include "MainForm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_SmartQry *Form_SmartQry = NULL;
//---------------------------------------------------------------------------
__fastcall TForm_SmartQry::TForm_SmartQry(TComponent* Owner)
	: TForm(Owner)
{

    m_ColumnToSort_inListView_AllResults = 0;
    m_ColumnToSort_inListView_Results = 0;
    m_ColumnToSort_inListView_QryRoad = 0;

}
//---------------------------------------------------------------------------
void __fastcall TForm_SmartQry::FormClose(TObject *Sender,
      TCloseAction &Action)
{
        CallCenter_Main_Form->SpeedButton_Query->Down = false;
        CallCenter_Main_Form->N_Query->Checked = false;
	Hide();
	Action = caFree;
    Form_SmartQry = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TForm_SmartQry::ListView_ResultsDblClick(TObject *Sender)
{
	TListView* pListView = (TListView*)Sender;
	if(pListView->SelCount == 1 )
    {
    	//delete Simu_Car_Thread
        TListItem* pItem = pListView->Selected;
        //in SubItem [4],[5] is WGS X,Y 座標
        long mX2 = atol(pItem->SubItems->Strings[4].c_str());
        long mY2 = atol(pItem->SubItems->Strings[5].c_str());
        DrawImage(CallCenter_Main_Form->Image_Map,mX2,mY2,CallCenter_Main_Form->m_dScale);
        CallCenter_Main_Form->UpdateAllObjPos();
        //Set Center Line
        DrawCenterLine(CallCenter_Main_Form->Image_Map);
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm_SmartQry::ComboBox_InputNameKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
	if(Key==VK_RETURN)
    	BitBtn_QueryClick(Sender);	
}
//---------------------------------------------------------------------------
/////////  for Sort ListView_Remarks ////////////

void __fastcall TForm_SmartQry::ListView_ResultsColumnClick(
      TObject *Sender, TListColumn *Column)
{
	m_ColumnToSort_inListView_Results = Column->Index;
  	((TCustomListView *)Sender)->AlphaSort();
}
//---------------------------------------------------------------------------

void __fastcall TForm_SmartQry::ListView_ResultsCompare(TObject *Sender,
      TListItem *Item1, TListItem *Item2, int Data, int &Compare)
{
	if (m_ColumnToSort_inListView_Results == 0)
    	Compare = CompareText(Item1->Caption,Item2->Caption);
  	else
        Compare = CompareText(Item1->SubItems->Strings[m_ColumnToSort_inListView_Results-1],Item2->SubItems->Strings[m_ColumnToSort_inListView_Results-1]);

}
//---------------------------------------------------------------------------

void __fastcall TForm_SmartQry::ListView_AllResultsColumnClick(
      TObject *Sender, TListColumn *Column)
{
	m_ColumnToSort_inListView_AllResults = Column->Index;
  	((TCustomListView *)Sender)->AlphaSort();
}
//---------------------------------------------------------------------------

void __fastcall TForm_SmartQry::ListView_AllResultsCompare(TObject *Sender,
      TListItem *Item1, TListItem *Item2, int Data, int &Compare)
{
	if (m_ColumnToSort_inListView_AllResults == 0)
    	Compare = CompareText(Item1->Caption,Item2->Caption);
  	else
        Compare = CompareText(Item1->SubItems->Strings[m_ColumnToSort_inListView_AllResults-1],Item2->SubItems->Strings[m_ColumnToSort_inListView_AllResults-1]);
}
//---------------------------------------------------------------------------


void __fastcall TForm_SmartQry::ListView_QryRoadColumnClick(
      TObject *Sender, TListColumn *Column)
{
	m_ColumnToSort_inListView_QryRoad = Column->Index;
  	((TCustomListView *)Sender)->AlphaSort();
}
//---------------------------------------------------------------------------

void __fastcall TForm_SmartQry::ListView_QryRoadCompare(TObject *Sender,
      TListItem *Item1, TListItem *Item2, int Data, int &Compare)
{
	if (m_ColumnToSort_inListView_QryRoad == 0)
    	Compare = CompareText(Item1->Caption,Item2->Caption);
  	else
        Compare = CompareText(Item1->SubItems->Strings[m_ColumnToSort_inListView_QryRoad-1],Item2->SubItems->Strings[m_ColumnToSort_inListView_QryRoad-1]);
	
}
//---------------------------------------------------------------------------




void __fastcall TForm_SmartQry::CheckListBox1ClickCheck(TObject *Sender)
{
        ObjSet.Clear();
        if(CheckListBox1->Checked[0])
                ObjSet << 1;
        if(CheckListBox1->Checked[1])
        {
                ObjSet << 2;
                ObjSet << 14;
                ObjSet << 13;
        }
        if(CheckListBox1->Checked[2])
        {
                ObjSet << 3;
                ObjSet << 18;
        }
        if(CheckListBox1->Checked[3])
        {
                ObjSet << 4;
                ObjSet << 5;
        }
        if(CheckListBox1->Checked[4])
                ObjSet << 12;
        if(CheckListBox1->Checked[5])
                ObjSet << 15;
        if(CheckListBox1->Checked[6])
                ObjSet << 16;
        if(CheckListBox1->Checked[7])
                ObjSet << 17;
        if(CheckListBox1->Checked[8])
        {
                ObjSet << 11;
                ObjSet << 19;
        }
}
//---------------------------------------------------------------------------


void __fastcall TForm_SmartQry::FormCreate(TObject *Sender)
{
        for(int i=0; i<9; i++)
        {
                CheckListBox1->State[i] = cbChecked;
                CheckListBox2->State[i] = cbChecked;
        }
        CheckListBox2->State[9] = cbChecked;
        CheckListBox1ClickCheck(this);
        PoiStr[1] = "地名";
        PoiStr[2] = "車站";
        PoiStr[3] = "公園";
        PoiStr[5] = "建築物";
        PoiStr[4] = "地標";
        PoiStr[11] = "Welcome";
        PoiStr[12] = "銀行";
        PoiStr[13] = "碼頭";
        PoiStr[14] = "巴士站";
        PoiStr[15] = "停車場";
        PoiStr[16] = "電影院";
        PoiStr[17] = "飯店";
        PoiStr[18] = "風景點";
        PoiStr[19] = "百貨公司";
        RoadTypeStr[1] = "快速路";
        RoadTypeStr[2] = "閘道";
        RoadTypeStr[3] = "大路";
        RoadTypeStr[4] = "中路";
        RoadTypeStr[5] = "小路";
}
//---------------------------------------------------------------------------
void __fastcall TForm_SmartQry::BitBtn_QueryClick(TObject *Sender)
{
        if( ComboBox_InputName->Text.Length() > 0 )
        {
                ComboBox_InputName->Items->Add(ComboBox_InputName->Text);
                if(ListView_Results->Items->Count >0)
                {
                        m_ColumnToSort_inListView_Results = 0;
                        ListView_Results->Items->Clear();                       // 清除先前查詢的結果
                }
                //2001.03.21 New Display Test for DCH
                char szQuery[40];
                strcpy(szQuery,ComboBox_InputName->Text.c_str());
                for(int i=0; i<40; i++)                 //將空白字元置換成底線字元
                {
                        if((((szQuery[0] > 'A')&&(szQuery[0] < 'Z'))||((szQuery[0] > 'a')&&(szQuery[0] < 'z'))))
                        {
                                if(szQuery[i] == ' ')
                                {
                                        szQuery[i] = '_';
                                }
                        }
                }
                char szString[40960]="";
                memset(szString,0,40960);
                struct s_outo IISData[500];
                long count=0;
                char *pStr,*pStr2;
                AnsiString Str, Str2;
                long WGS_X,WGS_Y;
                TListItem *pItem;
                AnsiString StrName, StrType, StrAddress, StrPhone, StrX, StrY;
                //layerID = 8 -> POI  , 第二欄表名稱
                int SearchResult = entstriis2(8,szQuery,500,40960,IISData,szString,&count);
                if(SearchResult == 2)
                {
                        ShowMessage("輸入條件符合過多！\n建議輸入更明確搜尋條件！");
                }
                StatusBar_Poi->Panels->Items[0]->Text = "找到" + IntToStr(count) + "筆資料";
                if(count > 0)
                {
                        pStr = szString;                                        //第一次第1欄
                        int j=1;
                        for(int i=0;i<count;i++)
                        {
                                AnsiString field[10];
                                if(pStr == szString)
                                {
                                        field[0] = pStr;
                                }
                                else
                                {
                                        pStr = pStr + (strlen(pStr)+1);
                                        field[0] = pStr;
                                }
                                for(int k=1; k<7; k++)
                                {
                                        pStr = pStr + (strlen(pStr)+1);
                                        field[k] = pStr;
                                }
                                if(field[0] != "")
                                {
                                        StrName = field[0];
                                        if(field[1] != "")
                                        {
                                                while(field[1].Pos("_")!=0)          //將底線置換成空白字元
                                                {
                                                        int i = field[1].Pos("_");
                                                        field[1].Delete(i,1);
                                                        field[1].Insert(" ",i);
                                                }
                                                StrName = StrName + " ( " + field[1] + " ) ";
                                        }
                                }
                                else
                                {
                                        StrName = field[1];
                                }
                                StrType = PoiStr[field[2].ToInt()];
                                if(field[3] != "")
                                {
                                        StrAddress = field[3];
                                        if(field[4] != "")
                                        {
                                                while(field[4].Pos("_")!=0)          //將底線置換成空白字元
                                                {
                                                        int i = field[4].Pos("_");
                                                        field[4].Delete(i,1);
                                                        field[4].Insert(" ",i);
                                                }
                                                StrAddress = StrAddress + " ( " + field[4] + " ) ";
                                        }
                                }
                                else
                                {
                                        StrAddress = field[4];
                                }
                                StrPhone  = field[5];
                                StrX = IISData[i].x;
                                StrY = IISData[i].y;
                                if(ObjSet.Contains(field[2].ToInt()))
                                {
                                        pItem = ListView_Results->Items->Add();
                                        pItem->Caption = j;
                                        while(pItem->Caption.Length() < 3)
                                        {
                                                pItem->Caption.Insert("0",1);
                                        }
                                        j++;
                                        pItem->SubItems->Add(StrName);
                                        pItem->SubItems->Add(StrType);
                                        pItem->SubItems->Add(StrAddress);
                                        pItem->SubItems->Add(StrPhone);
                                        pItem->SubItems->Add(StrX);
                                        pItem->SubItems->Add(StrY);
                                }
                        }//for
                }//if
                else
                {
                        ShowMessage("找不到符合此條件的資料 !\n請再重新查詢 !");
                }
                memset(szString,0,40960);       //RESET WHEN FREE
        }//if
        else
        {
    	        ShowMessage("請輸入欲查詢的設施名稱或地名 !\n再重新查詢 !");
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm_SmartQry::BitBtn_QueryRoadClick(TObject *Sender)
{
        if( ComboBox_Road->Text.Length() > 0 )
        {
                ComboBox_Road->Items->Add(ComboBox_Road->Text);
                if(ListView_Roads->Items->Count >0)
                {
                        m_ColumnToSort_inListView_Results = 0;
                        ListView_Roads->Items->Clear();                       // 清除先前查詢的結果
                }
                //2001.03.21 New Display Test for DCH
                char szQuery[40];
                strcpy(szQuery,ComboBox_Road->Text.c_str());
                for(int i=0; i<40; i++)                 //將空白字元置換成底線字元
                {
                        if((((szQuery[0] > 'A')&&(szQuery[0] < 'Z'))||((szQuery[0] > 'a')&&(szQuery[0] < 'z'))))
                        {
                                if(szQuery[i] == ' ')
                                {
                                        szQuery[i] = '_';
                                }
                        }
                }
                char szString[102400]="";
                memset(szString,0,102400);
                struct s_outo IISData[1000];
                long count=0;
                char *pStr;
                long WGS_X,WGS_Y;
                TListItem *pItem;
                AnsiString StrName, StrNumber, StrType, StrX, StrY, PreStrName;
                //layerID = 4 -> 道路
                int SearchResult = entstriis2(4,szQuery,1000,102400,IISData,szString,&count);
                if(SearchResult == 2)
                {
                        ShowMessage("輸入條件符合過多！\n建議輸入更明確搜尋條件！");
                }
                StatusBar_Road->Panels->Items[0]->Text = "找到" + IntToStr(count) + "筆資料";
                if(count > 0)
                {
                        pStr = szString;                                        //第一次第1欄
                        int j=1;
                        for(int i=0;i<count;i++)
                        {
                                AnsiString field[10];
                                if(pStr == szString)
                                {
                                        field[0] = pStr;
                                }
                                else
                                {
                                        pStr = pStr + (strlen(pStr)+1);
                                        field[0] = pStr;
                                }
                                for(int k=1; k<10; k++)
                                {
                                        pStr = pStr + (strlen(pStr)+1);
                                        field[k] = pStr;
                                }
                                if(field[0] != "")
                                {
                                        StrName = field[0];
                                        if(field[1] != "")
                                        {
                                                while(field[1].Pos("_")!=0)          //將底線置換成空白字元
                                                {
                                                        int i = field[1].Pos("_");
                                                        field[1].Delete(i,1);
                                                        field[1].Insert(" ",i);
                                                }
                                                StrName = StrName + " ( " + field[1] + " ) ";
                                        }
                                }
                                else
                                {
                                        StrName = field[1];
                                }
                                StrType = RoadTypeStr[field[2].ToInt()];
                                StrX = IISData[i].x;
                                StrY = IISData[i].y;
                                if(StrName != PreStrName)
                                {
                                        pItem = ListView_Roads->Items->Add();
                                        pItem->Caption = j;
                                        while(pItem->Caption.Length() < 3)
                                        {
                                                pItem->Caption.Insert("0",1);
                                        }
                                        j++;
                                        pItem->SubItems->Add(StrName);
                                        pItem->SubItems->Add(StrType);
                                        pItem->SubItems->Add(StrX);
                                        pItem->SubItems->Add(StrY);
                                        PreStrName = StrName;
                                }
                        }//for
                }//if
                else
                {
                        ShowMessage("找不到符合此條件的資料 !\n請再重新查詢 !");
                }
                memset(szString,0,102400);              //RESET WHEN FREE
        }//if
        else
        {
    	        ShowMessage("請輸入欲查詢的設施名稱或地名 !\n再重新查詢 !");
        }
}
//---------------------------------------------------------------------------


void __fastcall TForm_SmartQry::ListView_RoadsColumnClick(TObject *Sender,
      TListColumn *Column)
{
	m_ColumnToSort_inListView_Results = Column->Index;
  	((TCustomListView *)Sender)->AlphaSort();        
}
//---------------------------------------------------------------------------

void __fastcall TForm_SmartQry::ListView_RoadsCompare(TObject *Sender,
      TListItem *Item1, TListItem *Item2, int Data, int &Compare)
{
	if (m_ColumnToSort_inListView_Results == 0)
    	Compare = CompareText(Item1->Caption,Item2->Caption);
  	else
        Compare = CompareText(Item1->SubItems->Strings[m_ColumnToSort_inListView_Results-1],Item2->SubItems->Strings[m_ColumnToSort_inListView_Results-1]);

}
//---------------------------------------------------------------------------

void __fastcall TForm_SmartQry::ListView_RoadsDblClick(TObject *Sender)
{
	TListView* pListView = (TListView*)Sender;
	if(pListView->SelCount == 1 )
    {
    	//delete Simu_Car_Thread
        TListItem* pItem = pListView->Selected;
        //in SubItem [4],[5] is WGS X,Y 座標
        long mX2 = atol(pItem->SubItems->Strings[2].c_str());
        long mY2 = atol(pItem->SubItems->Strings[3].c_str());
        DrawImage(CallCenter_Main_Form->Image_Map,mX2,mY2,CallCenter_Main_Form->m_dScale);
        CallCenter_Main_Form->UpdateAllObjPos();
        //Set Center Line
        DrawCenterLine(CallCenter_Main_Form->Image_Map);
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm_SmartQry::ComboBox_RoadKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
	if(Key==VK_RETURN)
                BitBtn_QueryRoadClick(this);

}
//---------------------------------------------------------------------------

void __fastcall TForm_SmartQry::Edit_DistPGChange(TObject *Sender)
{
        if( Edit_DistPG->Text.Length() > 0 )
        {
                char szQuery[40];
                strcpy(szQuery,Edit_DistPG->Text.c_str());
                for(int i=0; i<40; i++)                 //將空白字元置換成底線字元
                {
                        if(szQuery[i] == 0)
                        {
                                break;
                        }
                        if((((szQuery[0] > 'A')&&(szQuery[0] < 'Z'))||((szQuery[0] > 'a')&&(szQuery[0] < 'z'))))
                        {
                                if(szQuery[i] == ' ')
                                {
                                        szQuery[i] = '_';
                                }
                        }
                }
                char szString[102400]="";
                memset(szString,0,102400);
                struct s_outo IISData[1000];
                long count=0;
                char *pStr;
                long WGS_X,WGS_Y;
                TListItem *pItem;
                AnsiString StrName, StrNumber, StrType, StrX, StrY, PreStrName;
                //layerID = 4 -> 道路
                entstriis2(1,szQuery,1000,102400,IISData,szString,&count);
                if(count > 0)
                {
                        ListBox_DistPG->Items->Clear();
                        for(int i=0; i<count; i++)
                        {
                                char *pStr;
                                char *pStrEng;
                                pStr = szString + IISData[i].nmpt;
                                AnsiString StrName = pStr;
                                pStrEng = pStr + StrName.Length() + 1;
                                for(int i=0;pStrEng[i]!=0;i++)          //將底線置換成空白字元
                                {
                                        if(pStrEng[i] == 0)
                                        {
                                                break;
                                        }
                                        if(pStrEng[i] == ' ')
                                        {
                                                szQuery[i] = '_';
                                        }
                                }
                                StrName = StrName + " " + pStrEng;
                                ListBox_DistPG->Items->Add(StrName);
                                DistPG_Point[i].mX2 = IISData[i].x;
                                DistPG_Point[i].mY2 = IISData[i].y;
                                AnsiString StrNameEng = pStrEng;
                        }
                }
                memset(szString,0,102400);      //RESET WHEN FREE
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm_SmartQry::TrackBar1Change(TObject *Sender)
{
        Label4->Caption = "查詢半徑" + IntToStr(TrackBar1->Position) + "KM";
}
//---------------------------------------------------------------------------

void __fastcall TForm_SmartQry::ListBox_DistPGClick(TObject *Sender)
{
        char szString[102400]="";
        memset(szString,0,102400);
        struct s_outo IISData[1000];
        long count=0;
        sPoint Point = DistPG_Point[ListBox_DistPG->ItemIndex];
        TListItem *pItem;
        AnsiString StrName, StrNumber, StrType, StrX, StrY, PreStrName;
        entiis(4,-2,Point.mX2,Point.mY2,(TrackBar1->Position*1000),0,1000,102400,IISData,szString,&count);
        StatusBar_Road->Panels->Items[0]->Text = "找到" + IntToStr(count) + "筆資料";
        if(ListView_Roads->Items->Count >0)
        {
                m_ColumnToSort_inListView_Results = 0;
                ListView_Roads->Items->Clear();                       // 清除先前查詢的結果
        }
        if(count > 0)
        {
                char *pStr;
                pStr = szString;                                        //第一次第1欄
                int j=1;
                for(int i=0;i<count;i++)
                {
                        AnsiString field[10];
                        if(pStr == szString)
                        {
                                field[0] = pStr;
                        }
                        else
                        {
                                pStr = pStr + (strlen(pStr)+1);
                                field[0] = pStr;
                        }
                        for(int k=1; k<10; k++)
                        {
                                pStr = pStr + (strlen(pStr)+1);
                                field[k] = pStr;
                        }
                        if(field[0] != "")
                        {
                                StrName = field[0];
                                if(field[1] != "")
                                {
                                        StrName = StrName + " ( " + field[1] + " ) ";
                                }
                        }
                        else
                        {
                                StrName = field[1];
                        }
                        StrType = RoadTypeStr[field[2].ToInt()];
                        StrX = IISData[i].x;
                        StrY = IISData[i].y;
                        if(StrName != PreStrName)
                        {
                                pItem = ListView_Roads->Items->Add();
                                pItem->Caption = j;
                                while(pItem->Caption.Length() < 3)
                                {
                                        pItem->Caption.Insert("0",1);
                                }
                                j++;
                                pItem->SubItems->Add(StrName);
                                pItem->SubItems->Add(StrType);
                                pItem->SubItems->Add(StrX);
                                pItem->SubItems->Add(StrY);
                                PreStrName = StrName;
                        }
                }//for
        }//if
        memset(szString,0,102400);      //RESET WHEN FREE
}
//---------------------------------------------------------------------------
void __fastcall TForm_SmartQry::BitBtn_QueryRemarkClick(TObject *Sender)
{
        ListView_Remark->Items->Clear();
        bool havechecked = false;
        for(int i=0; i<10; i++)
        {
                if(CheckListBox2->State[i] == cbChecked)
                        havechecked = true;
        }
        if(havechecked == false)    //假如沒有選擇任何類別就跳出
        {
                return;
        }
        TQuery* pQ_Remarks = new TQuery(DataModule1);
        pQ_Remarks->DatabaseName = "DB_DCH";
        pQ_Remarks->SQL->Add("SELECT * FROM tRemarks");
        pQ_Remarks->SQL->Add("WHERE ((Name LIKE :Str) OR (Address LIKE :Str))");
        pQ_Remarks->SQL->Add("AND RemarkType IN (");                            //將未選取的類別篩選掉
        for(int i=0; i<10; i++)
        {
                if(CheckListBox2->State[i] == cbChecked)
                {
                        if((pQ_Remarks->SQL->Strings[2][pQ_Remarks->SQL->Strings[2].Length()] != ',')&&(pQ_Remarks->SQL->Strings[2][pQ_Remarks->SQL->Strings[2].Length()] != '('))
                        {
                                pQ_Remarks->SQL->Strings[2] = pQ_Remarks->SQL->Strings[2] + ",";
                        }
                        pQ_Remarks->SQL->Strings[2] = pQ_Remarks->SQL->Strings[2] + IntToStr(i);
                }
        }
        pQ_Remarks->SQL->Strings[2] = pQ_Remarks->SQL->Strings[2] + ")";
        pQ_Remarks->ParamByName("Str")->AsString = "%"+ComboBox_Remark->Text+"%";
        try
        {
                pQ_Remarks->Active = false;
                pQ_Remarks->Active = true;
                for(pQ_Remarks->First();!pQ_Remarks->Eof;pQ_Remarks->Next())
                {
                        TListItem* pListItem = ListView_Remark->Items->Add();
                        AnsiString CompanyName = "*通用備註地點*";
                        AnsiString CoID = pQ_Remarks->FieldByName("CoID")->AsString;
                        if(CoID != "Common")
                        {
                                CompanyName = DataModule1->Get_DB_Value("tCustInfo","FullName","CoID",CoID);
                        }
                        pListItem->Caption = CompanyName;
                        int RemarkType = pQ_Remarks->FieldValues["RemarkType"];
                        pListItem->SubItems->Add(GetClassName(RemarkType));
                        pListItem->SubItems->Add(pQ_Remarks->FieldValues["Name"]);
                        pListItem->SubItems->Add(pQ_Remarks->FieldValues["Phone"]);
                        pListItem->SubItems->Add(pQ_Remarks->FieldValues["Address"]);
                        pListItem->SubItems->Add(pQ_Remarks->FieldValues["Email"]);
                        pListItem->SubItems->Add(pQ_Remarks->FieldValues["WebAddress"]);
                        pListItem->SubItems->Add(pQ_Remarks->FieldValues["Memo"]);
                        pListItem->SubItems->Add(pQ_Remarks->FieldValues["Lon"]);
                        pListItem->SubItems->Add(pQ_Remarks->FieldValues["Lat"]);
                }
        }
        catch(...)
        {
        }

}
//---------------------------------------------------------------------------

void __fastcall TForm_SmartQry::ListView_RemarkColumnClick(TObject *Sender,
      TListColumn *Column)
{
	m_ColumnToSort_inListView_QryRemark = Column->Index;
  	((TCustomListView *)Sender)->AlphaSort();
}
//---------------------------------------------------------------------------

void __fastcall TForm_SmartQry::ListView_RemarkCompare(TObject *Sender,
      TListItem *Item1, TListItem *Item2, int Data, int &Compare)
{
	if (m_ColumnToSort_inListView_QryRemark == 0)
    	Compare = CompareText(Item1->Caption,Item2->Caption);
  	else
        Compare = CompareText(Item1->SubItems->Strings[m_ColumnToSort_inListView_QryRemark-1],Item2->SubItems->Strings[m_ColumnToSort_inListView_QryRemark-1]);
}
//---------------------------------------------------------------------------

void __fastcall TForm_SmartQry::ListView_RemarkDblClick(TObject *Sender)
{
	TListView* pListView = (TListView*)Sender;
	if(pListView->SelCount == 1 )
    {
    	//delete Simu_Car_Thread
        TListItem* pItem = pListView->Selected;
        //in SubItem [7],[8] is WGS X,Y 座標
        long mX2 = atol(pItem->SubItems->Strings[7].c_str());
        long mY2 = atol(pItem->SubItems->Strings[8].c_str());
        DrawImage(CallCenter_Main_Form->Image_Map,mX2,mY2,CallCenter_Main_Form->m_dScale);
        CallCenter_Main_Form->UpdateAllObjPos();
        //Set Center Line
        DrawCenterLine(CallCenter_Main_Form->Image_Map);
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm_SmartQry::ComboBox_RemarkKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
	if(Key==VK_RETURN)
                BitBtn_QueryRemarkClick(this);
}
//---------------------------------------------------------------------------



