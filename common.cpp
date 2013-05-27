/*
2002.5.23 刪除變色球的class程式
*/
//Common.cpp

#include <vcl.h>
#include <mbstring.h>       // for _mbstok()
							//use sys include header file before #pragma hdrstop
							//for pre-Header compile Technology
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#pragma hdrstop

#include "MainForm.h"
#include "common.h"
#include "StatManager.h"
#include "LoginForm.h"
#include "StatManager.h"
#include "ChooseCarForm.h"
/////////  Global Common Function   /////////////
int g_iMapType = 1;			// iMapType=0 -> TW Local Test , iMapType=1 -> HK Map
char g_DrawFlag[12] = "011111100";
int g_RemarksCount=0;
int PathArea = DEF_PATH_WIDTH;     //監控路線監控半徑
int MaxTracePoint = 200;           //最大即時軌跡數目

unsigned short G_GetCmdID()
{
        static int nCmdID = 0;
        nCmdID++;
        nCmdID %= 100;
        return nCmdID;
}

char* __fastcall   GetIISName(long WGS_X,long WGS_Y)
{
        static char szIISName[160];
        char szString[500]="";
        struct s_outo IISData;
        long count=0;
        //Link LayerID=4 , 查詢半徑 40
        entiis(4,-1,WGS_X,WGS_Y,40,0,1,500,&IISData,szString,&count);
        if(count < 1)
        {//再查 Building Name , LayerID = 3
                entiis(3,-1,WGS_X,WGS_Y,40,0,1,500,&IISData,szString,&count);
                if(count < 1)
                {//再查 LandMark Name , LayerID = 7
                        entiis(7,-1,WGS_X,WGS_Y,40,0,1,500,&IISData,szString,&count);
                        if(count < 1)
                        {//再查 行政區名稱 , LayerID = 1
                                entiis(1,-1,WGS_X,WGS_Y,40,0,1,500,&IISData,szString,&count);
                                if(count < 1)
                                {
                                        strcpy(szIISName,"");
                                        return szIISName;
                                }
                                else //取第二欄英文名稱
                                {
                                        char* pStr2 = szString+(strlen(szString)+1);
                                        sprintf(szIISName,"District Name : %s ",pStr2);
                                }
                        }
                        else //LandMark Name Found   //只取前一欄顯示(因為其以 '\0')
                                sprintf(szIISName,"LandMark Name : %s",szString);
                }//if
                else //Building Name Found   //只取前一欄顯示(因為其以 '\0')
                        sprintf(szIISName,"Building Name : %s",szString);
        }//if
        else //Road Name Found
        {//只取第十欄顯示(因為其以 '\0')
                sprintf(szIISName,"Road Name : %s",szString);
                // iMapType=0 -> TW Local Test , iMapType=1 -> HK Map
                if(g_iMapType==0)
                {
                        char* pStr2 = szString+(strlen(szString)+1);
                        char* pStr3 = pStr2+(strlen(pStr2)+1);
                        char* pStr4 = pStr3+(strlen(pStr3)+1);
                        char* pStr5 = pStr4+(strlen(pStr4)+1);
                        char* pStr6 = pStr5+(strlen(pStr5)+1);
                        char* pStr7 = pStr6+(strlen(pStr6)+1);
                        char* pStr8 = pStr7+(strlen(pStr7)+1);
                        char* pStr9 = pStr8+(strlen(pStr8)+1);
                        char* pStr10 = pStr9+(strlen(pStr9)+1);
                        sprintf(szIISName,"Road Name : %s",pStr10);
                }
        }
        return szIISName;
}

void 	__fastcall  Dlldraw(long HdcID,char fg[12])
{//fg[8] == 1 means 考慮重疊不畫
        layerdraw(HdcID,1001,"");    //012345678901234567
        layerdraw(HdcID,1, "11111132111111");   //distpg
        if(fg[2]=='1')
        {
                layerdraw(HdcID,2, "11111111111111");   //park
        }
        if(fg[5]=='1')
        {
                layerdraw(HdcID,5, "11111331111111");   //st_block
        }
        if(fg[3]=='1')
        {
                layeropen(  3,-1,"", 5,5,0);
                layerdraw(HdcID,3, "11111111111111");   //build
        }
        if(fg[6]=='1')
        {
                layerdraw(HdcID,6, "11111111111111");   //rail
        }
        if(fg[7]=='1')
        {
                layerdraw(HdcID,7, "11111111111111");   //landmark
        }
        if(fg[1]=='1')
        {
                layerdraw(HdcID,9, "21112221111111");    //行政區名稱標示
        }
        if(fg[4]=='1')
        {
                if(g_iMapType==0)	//TW Map
                {
             	        layerdraw(HdcID,4, "211111111112:11111");   //road_name
                }
                else //HK Map
                {
                        if(fg[0] == '1')
                                layerdraw(HdcID,4, "00000000000210");   //road_name考慮重疊
                        else
                                layerdraw(HdcID,4, "00000033000210");   //road_name不考慮重疊
                }
        }
        if(fg[8]=='1')
        {
                if(g_iMapType==0)	//TW Map
                {
                        layerdraw(HdcID,8, "111111141111111111");   //poi
                }
                else //HK Map
                {
                        if(fg[0] == '1')
                                layerdraw(HdcID,8, "11111111111111");   //poi考慮重疊
                        else
                                layerdraw(HdcID,8, "11111331111111");   //poi不考慮重疊
                }
        }
        if(fg[9]=='1')
        {
                layerdraw(HdcID,9, "11111111111111");
        }
        if(fg[10]=='1')
        {
                layerdraw(HdcID,10, "11111111111111");
        }
        if(fg[11]=='1')
        {
                layerdraw(HdcID,11, "11111111111111");
        }
}

//Global Function

//Free Display DLL
void __fastcall FreeDispDLL()
{
        //2001.03.21 New Display Test for DCH
        for(int LayerID=0;LayerID<50;LayerID++)
                layerfree(LayerID);
}
int HexToInt(AnsiString str)
{
        char *ch = str.c_str();
        switch (*ch)
        {
        case '0': return 0;
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
        case 'A': return 10;
        case 'B': return 11;
        case 'C': return 12;
        case 'D': return 13;
        case 'E': return 14;
        case 'F': return 15;
        }
        return NULL;
}
//Map Display DLL Initialize
void __fastcall	InitDispDLL(int iMapType)
{// iMapType=0 -> TW Local Test , iMapType=1 -> HK Map
	FreeDispDLL();
        TIniFile* ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ));
        ////// Just for Taiwan Map Test ////////
        if(iMapType==0)	//Taiwan Map Test
        {
                for(int i=1; ini->ReadString("TAIWAN_RGBSET",IntToStr(i),"0") != "0"; i++)
                {
                        AnsiString RGBStr = ini->ReadString("TAIWAN_RGBSET",IntToStr(i),"0");
                        int RGBR = HexToInt(RGBStr.SubString(3,1))*16;
                        RGBR = RGBR + HexToInt(RGBStr.SubString(4,1));
                        int RGBG = HexToInt(RGBStr.SubString(5,1))*16;
                        RGBG = RGBG + HexToInt(RGBStr.SubString(6,1));
                        int RGBB = HexToInt(RGBStr.SubString(7,1))*16;
                        RGBB = RGBB + HexToInt(RGBStr.SubString(8,1));
                        int LineWidth  = RGBStr.SubString(9,1).ToInt();
                        rgbset(i, RGBR,RGBG,RGBB,LineWidth);
                }
  		layeropen(1,5,"..\\Map\\276027\\district.mif",    3, 2, 0);
  		layeropen(2,5,"..\\Map\\276027\\landuse.mif",      4, 4, 0);
  		layeropen(3,5,"..\\Map\\276027\\water_pg.mif",     5, 5, 0);
  		layeropen(4,5,"..\\Map\\276027\\link.mif",      1, 0, 0);
    	        layerfeature(4, -9, "",   9);
                layerfeature(4,  2, "1",  7);
    	        layerfeature(4,  2, "2",  7);
    	        layerfeature(4,  2, "3",  8);
  		layeropen(6,5,"..\\Map\\276027\\rail.mif",     12, 0, 0);
  		layeropen(8,5,"..\\Map\\276027\\Point.mif",  11, 0, 3);
        }
        else
        {
                for(int i=1; ini->ReadString("HK_RGBSET",IntToStr(i),"0") != "0"; i++)
                {
                        AnsiString RGBStr = ini->ReadString("HK_RGBSET",IntToStr(i),"0");
                        int RGBR = HexToInt(RGBStr.SubString(3,1))*16;
                        RGBR = RGBR + HexToInt(RGBStr.SubString(4,1));
                        int RGBG = HexToInt(RGBStr.SubString(5,1))*16;
                        RGBG = RGBG + HexToInt(RGBStr.SubString(6,1));
                        int RGBB = HexToInt(RGBStr.SubString(7,1))*16;
                        RGBB = RGBB + HexToInt(RGBStr.SubString(8,1));
                        int LineWidth  = RGBStr.SubString(9,1).ToInt();
                        rgbset(i, RGBR,RGBG,RGBB,LineWidth);
                }
                layeropen(1,6,"..\\Map\\distpg.roy",    3, 2, 0);
                layeropen(2,6,"..\\Map\\park.roy",      4, 4, 0);
                layeropen(3,6,"..\\Map\\build.roy",     5, 5, 0);
                layeropen(4,6,"..\\Map\\link.roy",      1, 0, 0);
    	        layerfeature(4, -9, "",   9);
    	        layerfeature(4,  2, "1",  7);
    	        layerfeature(4,  2, "2",  7);
    	        layerfeature(4,  2, "3",  8);
                layeropen(5,6,"..\\Map\\st_block.roy", 10, 0, 0);
                layeropen(6,6,"..\\Map\\rail.roy",     12, 0, 0);
                layeropen(7,6,"..\\Map\\landmark.roy", 11, 0, 3);
                layeropen(8,6,"..\\Map\\poi_map.roy",  11, 0, 3);
                layeropen(9,6,"..\\Map\\poi_map.roy",  11, 0, 3);
                layerfeature(9,-9,"",-1);
                layerfeature(9,2,"1",10);         //設施->行政區層
//                layerfeature(9,0,"",10);
//                layerfeature(9,1,"",10);
	}
        delete ini;
}

void __fastcall	Hdc2WGS(long Hdc_X,long Hdc_Y,long& WGS_X,long& WGS_Y)
{
        xycalc(0,0,Hdc_X,Hdc_Y,&WGS_X,&WGS_Y);
}

void __fastcall	WGS2Hdc(long WGS_X,long WGS_Y,long& Hdc_X,long& Hdc_Y)
{//Just for test to shift Taiwan to HongKong Test Lon=114173585 , Lat=22299504  , Taoyuan: Lon=121298319 , Lat=25016501
        xycalc(0,1,WGS_X,WGS_Y,&Hdc_X,&Hdc_Y);
}

//---------------------------------------------------------------------------

void __fastcall	ReDrawImage(TImage* pImage,double Scale)
{
        long Center_X,Center_Y ;
        Hdc2WGS((pImage->Width/2),(pImage->Height/2),Center_X,Center_Y);
        DrawImage(pImage,Center_X,Center_Y,Scale);
}

void __fastcall	DrawImage(TImage* pImage,long X,long Y,double Scale,int CoordType)
{
        Graphics::TBitmap* DC_BITMAP = CallCenter_Main_Form->dc_bitmap_Map;
        //Just for ReDraw Performance 先在 Bitmap 上畫好再貼入 Image 內
        DC_BITMAP->Width = pImage->ClientWidth;
        DC_BITMAP->Height= pImage->ClientHeight;
        if(CoordType==1)  //使用中心點的 "螢幕" 座標
                Hdc2WGS(X,Y,X,Y);
        //else //使用中心點的 WGS 座標
	long HdcID = 0; //Main Image Map
        if(pImage->Name == "Image_ZoomIn")
        {
                HdcID = 1;
        }
        else if(pImage->Name == "Image_Map_Sub")
        {
                HdcID = 2;
        }
        SetBkMode(DC_BITMAP->Canvas->Handle,TRANSPARENT );	//for Text can Transparent
        hdcset(HdcID,(long)DC_BITMAP->Canvas->Handle,pImage->ClientWidth,pImage->ClientHeight,1,X,Y,Scale);
        Dlldraw(HdcID,g_DrawFlag);  //fg=1,2,3
        if(pImage->Name=="Image_Map")
        {
    	        if( CallCenter_Main_Form->m_bIsLockMonitor )
    	        {	//Draw Red Locked Area Rectangle (m_iLockedAreaPixels) in Image
    		        int halfLockedArea = CallCenter_Main_Form->m_iLockedAreaPixels / 2;
                        DrawLockArea(DC_BITMAP->Canvas,pImage,halfLockedArea);
    	        }
                //此時若正在畫指定監控路線時
                if(CallCenter_Main_Form->m_pDefaultSelPath && CallCenter_Main_Form->m_bIsBeginDrawPath)
        	        DrawSelectPath(DC_BITMAP->Canvas,CallCenter_Main_Form->m_pDefaultSelPath,Scale);
                // Draw 已完成的指定監控路線
                TCSelectPath* pPath;
                for(int i=0;i<CallCenter_Main_Form->m_pSelPathGroupList->Count;i++)
                {
        	        pPath = (TCSelectPath*)CallCenter_Main_Form->m_pSelPathGroupList->Items[i];
                        DrawSelectPath(DC_BITMAP->Canvas,pPath,Scale);
                }
                //畫車子的即時軌跡
                CCar* pCar;
                if(CallCenter_Main_Form->m_pDrawTraceCarList->Count > 0)
                {
                        for(int i=0;i<CallCenter_Main_Form->m_pDrawTraceCarList->Count;i++)
                        {
            	                pCar = (CCar*)CallCenter_Main_Form->m_pDrawTraceCarList->Items[i];
                                if(pCar->m_pCarTraceList)
                                        DrawCarTrace(DC_BITMAP->Canvas,pCar,Scale,0);//0:DrawType
                        }
                }
                //畫車子的歷史軌跡
                for(int i=0;i<CallCenter_Main_Form->m_pCarList->Count;i++)
                {
        	        pCar = (CCar*)CallCenter_Main_Form->m_pCarList->Items[i];
                        if(pCar->m_pHisCarTraceList)
                                DrawCarTrace(DC_BITMAP->Canvas,pCar,Scale,1);//DrawType
                }
        }//if Image_Map
        pImage->Picture->Graphic = CallCenter_Main_Form->dc_bitmap_Map;
        pImage->Repaint();
}
//---------------------------------------------------------------------------
inline void __fastcall	DrawLockArea(TCanvas* pCanvas,TImage* pImage,int halfLockedArea)
{
       	TColor oldColor = pCanvas->Pen->Color;
        TColor oldColor2 = pCanvas->Brush->Color;
    	TPenStyle oldStyle = pCanvas->Pen->Style;
        TBrushStyle oldBrushStyle = pCanvas->Brush->Style;
    	TPenMode oldMode = pCanvas->Pen->Mode;
        int oldPenWidth = pCanvas->Pen->Width;
    	pCanvas->Pen->Color = clRed;
        pCanvas->Brush->Color = clSilver; //clRed
        pCanvas->Pen->Style = psDot;	//  psDashDotDot
        pCanvas->Brush->Style = bsBDiagonal;
    	pCanvas->Pen->Mode = pmMask;//pmMerge  pmMask
        pCanvas->Pen->Width = 3;
    	pCanvas->Rectangle(pImage->Width/2 -halfLockedArea,pImage->Height/2 -halfLockedArea,
        pImage->Width/2 +halfLockedArea, pImage->Height/2 +halfLockedArea);
    	pCanvas->Pen->Color = oldColor;
        pCanvas->Brush->Color = oldColor2;
    	pCanvas->Pen->Style = oldStyle;
        pCanvas->Brush->Style = oldBrushStyle;
    	pCanvas->Pen->Mode = oldMode;	//pmMerge
        pCanvas->Pen->Width = oldPenWidth;
}
//---------------------------------------------------------------------------
void __fastcall	DrawCenterLine(TImage* pImage,TColor cl,bool bDrawCircular)
{
       	TColor oldColor = pImage->Canvas->Pen->Color;
        TColor oldColor2 = pImage->Canvas->Brush->Color;
    	TPenStyle oldStyle = pImage->Canvas->Pen->Style;
        TBrushStyle oldBrushStyle = pImage->Canvas->Brush->Style;
    	TPenMode oldMode = pImage->Canvas->Pen->Mode;
        int oldPenWidth = pImage->Canvas->Pen->Width;
    	pImage->Canvas->Pen->Color = cl;	//clBlue
        pImage->Canvas->Brush->Color = clRed; // clSilver
        pImage->Canvas->Pen->Style = psDot;	//  psDashDotDot
        pImage->Canvas->Brush->Style = bsBDiagonal;
    	pImage->Canvas->Pen->Mode = pmMask;//pmMerge  pmMask
        pImage->Canvas->Pen->Width = 1;
        //Draw Line
        pImage->Canvas->MoveTo(0,pImage->Height/2);
        pImage->Canvas->LineTo(pImage->Width,pImage->Height/2);
        pImage->Canvas->MoveTo(pImage->Width/2,0);
        pImage->Canvas->LineTo(pImage->Width/2,pImage->Height);
        //Draw Circular
        if(bDrawCircular)
                pImage->Canvas->Ellipse(pImage->Width/2-25,pImage->Height/2-25,pImage->Width/2+25,pImage->Height/2+25);
        //Draw Centeral Point
        pImage->Canvas->Brush->Style = bsSolid;
        pImage->Canvas->Brush->Color = cl;
        pImage->Canvas->Ellipse(pImage->Width/2-4,pImage->Height/2-4,pImage->Width/2+4,pImage->Height/2+4);
    	pImage->Canvas->Pen->Color = oldColor;
        pImage->Canvas->Brush->Color = oldColor2;
    	pImage->Canvas->Pen->Style = oldStyle;
       	pImage->Canvas->Brush->Style = oldBrushStyle;
    	pImage->Canvas->Pen->Mode = oldMode;	//pmMerge
        pImage->Canvas->Pen->Width = oldPenWidth;
}
//---------------------------------------------------------------------------
void __fastcall	DrawSelectPath(TCanvas* pCanvas,TCSelectPath* pPath,double Scale)
{
        TColor oldColor = pCanvas->Pen->Color;
        TPenMode oldMode = pCanvas->Pen->Mode;
        int oldPenWidth = pCanvas->Pen->Width;
        pCanvas->Pen->Color = clRed;
        pCanvas->Pen->Mode = pmMask;	// pmMask pmNotXor   pmMerge
        pCanvas->Pen->Width = 2;
        //轉成螢幕座標
        long S_x , S_y , i;
        for(i=0;i<pPath->m_pSelPointList->Count;i++)
        {
                WGS2Hdc(((sSelectPoint*)pPath->m_pSelPointList->Items[i])->mX2,((sSelectPoint*)pPath->m_pSelPointList->Items[i])->mY2,S_x,S_y);
                if(i==0)
    		        pCanvas->MoveTo(S_x,S_y);
                else
    		        pCanvas->LineTo(S_x,S_y);
        }
        //Draw the Limited Area , (Yellow)
        pCanvas->Pen->Color = clYellow;
        for(i=0;i<pPath->m_pSelPointList->Count;i++)
        {
                //double dy = (0.000001/360.0)*2*PI*6378160.0;
	        //double dx = (0.000001/360.0)*2*PI*6378160.0*cos(25*PI/180.0);
    	        pCanvas->Pen->Width = 2*(pPath->Radius)/((double)Scale*0.10089008517); //(0.000001 Deg = 0.10089008517 M) 緯度25度
		WGS2Hdc(((sSelectPoint*)pPath->m_pSelPointList->Items[i])->mX2,((sSelectPoint*)pPath->m_pSelPointList->Items[i])->mY2,S_x,S_y);
                if(i==0)
    		        pCanvas->MoveTo(S_x,S_y);
                else
    		        pCanvas->LineTo(S_x,S_y);
        }
        pCanvas->Pen->Color = oldColor;
        pCanvas->Pen->Mode = oldMode;	//pmMerge
        pCanvas->Pen->Width = oldPenWidth;
}
//---------------------------------------------------------------------------
void __fastcall	DrawCarTrace(TCanvas* pCanvas,void* pCar,double Scale,int DrawType)
{
	CCar* ptrCar = (CCar*)pCar;
	TColor oldColor = pCanvas->Pen->Color;
        TPenStyle oldStyle = pCanvas->Pen->Style;
        TPenMode oldMode = pCanvas->Pen->Mode;
        int oldPenWidth = pCanvas->Pen->Width;
        TColor oldColor2 = pCanvas->Brush->Color;
        TBrushStyle oldBrushStyle = pCanvas->Brush->Style;
        pCanvas->Pen->Color = clBlue;  //clGreen
        pCanvas->Pen->Style = psDot;
        pCanvas->Pen->Mode = pmMask;	// pmMask pmNotXor   pmMerge
        pCanvas->Pen->Width = 1;
        pCanvas->Brush->Color = clBlue;
        pCanvas->Brush->Style = bsSolid;
        //轉成螢幕座標
        TList* pList;
        if(DrawType==0)
                pList = ptrCar->m_pCarTraceList;
        else
                pList = ptrCar->m_pHisCarTraceList;
        long S_x , S_y , i;
        for(i=0;i<pList->Count;i++)
        {
                {//轉成螢幕座標
   		        WGS2Hdc(((sSelectPoint*)pList->Items[i])->mX2,((sSelectPoint*)pList->Items[i])->mY2,S_x,S_y);
                        pCanvas->Brush->Color = oldColor2;
        	        if(i==0)
    			        pCanvas->MoveTo(S_x,S_y);
        	        else
    			        pCanvas->LineTo(S_x,S_y);
                        pCanvas->Brush->Color = clBlue;
        	        pCanvas->Ellipse(S_x-TRACE_POINT_PIXEL,S_y-TRACE_POINT_PIXEL,S_x+TRACE_POINT_PIXEL,S_y+TRACE_POINT_PIXEL);
                        if( (i%DRAW_CARID_INTERVAL)==0 )  //每DRAW_CARID_INTERVAL 點畫車號一次
                        {
            	                pCanvas->Brush->Color = oldColor2;
                                SetBkMode(pCanvas->Handle,TRANSPARENT );	//for Text can Transparent
            	                pCanvas->TextOut(S_x+TRACE_POINT_PIXEL,S_y+TRACE_POINT_PIXEL,AnsiString(ptrCar->GetCarID()));
                                pCanvas->MoveTo(S_x,S_y);
                        }
                }
        }
        pCanvas->Pen->Color = oldColor;
        pCanvas->Pen->Style = oldStyle;
        pCanvas->Pen->Mode = oldMode;	//pmMerge
        pCanvas->Pen->Width = oldPenWidth;
        pCanvas->Brush->Color = oldColor2;
        pCanvas->Brush->Style = oldBrushStyle;
}
//---------------------------------------------------------------------------
bool __fastcall IsInDrawRect(long WGS_X,long WGS_Y)
{    //以車子中心點作為判斷是否超出標準
        long Left,Top,Right,Bottom;    //Image Show Area WGS 座標
        //Get Show Area WGS 座標
        Hdc2WGS(0,0,Left,Top);
        Hdc2WGS((CallCenter_Main_Form->Image_Map->Width),(CallCenter_Main_Form->Image_Map->Height),Right,Bottom);
        //注意 ! WGS 向下為較小者
	if(WGS_X < Left)
                return false;
	if(WGS_X > Right)
                return false;
	if(WGS_Y > Top)
                return false;
	if(WGS_Y < Bottom)
                return false;
        return true;
}
//---------------------------------------------------------------------------
bool __fastcall AnyOneInDrawRect(TList* pPointList)
{       //只要有一點在範圍內即畫出此路線 m_pSelPointList
        sSelectPoint* ptr;
        for (int j = 0; j < pPointList->Count; j++)
        {
    	        ptr = ((sSelectPoint*) pPointList->Items[j]);
		long Left,Top,Right,Bottom;    //Image Show Area WGS 座標
    	        //Get Show Area WGS 座標
	        Hdc2WGS(0,0,Left,Top);
	        Hdc2WGS((CallCenter_Main_Form->Image_Map->Width),(CallCenter_Main_Form->Image_Map->Height),Right,Bottom);
                //注意 ! WGS 向下為較小者
		if( (ptr->mX2 > Left)&&(ptr->mX2 < Right)&&(ptr->mY2 < Top)&&(ptr->mY2 > Bottom))
        	        return true;
        }//for
        return false;
}
//---------------------------------------------------------------------------
short __fastcall  CalLineAngle(long xs, long ys, long xe, long ye)
{
        long   dx, dy;
        float rad;

        dx = xe - xs;
        dy = ye - ys;
        if (dx==0 && dy==0)
        {
                rad = 0.0;
        }
        else
        {
                rad = (float)(PI/2.0 - atan2(dy, dx));
                if (dx <0 && dy >=0)
                {
                        rad = (float)(rad + 2*PI);
                }
        }
        return (short)(rad*180.0/PI);
}
//---------------------------------------------------------------------------
float __fastcall  CalDist( long xs, long ys, long xe, long ye )
{
        return sqrt((xe-xs)*(xe-xs)+(ye-ys)*(ye-ys));
}
//---------------------------------------------------------------------------
float __fastcall MinLen_Point2Line( double Cx, double Cy, double Ax, double Ay, double Bx, double By )
{//// 求出Point(Cx,Cy) 與 線段Line(Ax,Ay)-(Bx,By) 之最短距離
        //若為鈍角, 則為 C 至 A or B 的距離
        double   L_AB, L;
        double   Px,Py,ratio;

        L_AB = sqrt((Bx-Ax)*(Bx-Ax)+(By-Ay)*(By-Ay));
        if (L_AB == 0)
        {
                ratio = 0;
        }
        else
        {
                ratio = ((Ay-Cy)*(Ay-By)-(Ax-Cx)*(Bx-Ax)) / (L_AB*L_AB);
        }
        if ( ratio < 0.0 )
        {
                L = sqrt((Cx-Ax)*(Cx-Ax)+(Cy-Ay)*(Cy-Ay));
        }
        else if ( ratio > 1.0 )
        {
                L = sqrt((Cx-Bx)*(Cx-Bx)+(Cy-By)*(Cy-By));
        }
        else
        {
                Px  = Ax + (ratio) * (Bx-Ax);
                Py  = Ay + (ratio) * (By-Ay);
                L    = sqrt((Cx-Px)*(Cx-Px)+(Cy-Py)*(Cy-Py));
        }
        return (float)L;
}
//---------------------------------------------------------------------------
bool __fastcall IsOverSelPath(long mX2,long mY2,TCSelectPath* pPath)
{
        sSelectPoint* pPoA,*pPoB;
        float MinLen = 9999999;
        float len;
        for(int i = 0; i < pPath->m_pSelPointList->Count;i++)
        {
    	        if(i+1 >= pPath->m_pSelPointList->Count)
        	        break;
    	        pPoA = (sSelectPoint*) pPath->m_pSelPointList->Items[i];
    	        pPoB = (sSelectPoint*) pPath->m_pSelPointList->Items[i+1];
                len = MinLen_Point2Line(mX2,mY2,pPoA->mX2,pPoA->mY2,pPoB->mX2,pPoB->mY2);
                len *= 0.10089008517;          //(0.000001 Deg = 0.10089008517 M) 緯度25度
                if(len < MinLen)
        	        MinLen = len;
        }
        if(MinLen > pPath->Radius) //監控半徑
    	        return true;
        else
    	        return false;
}
//---------------------------------------------------------------------------
CCar::CCar(AnsiString CarID,sCarStatus &Status,TWinControl* Owner, int Image)
{
        CarInfoUpdateTime = NULL;
        LatestStatusUpdateTime = NULL;
	memcpy(&m_Status,&Status,sizeof(sCarStatus));
        m_szCarID = CarID;
        Visible = true;
        ImageColor = 0;
        IO_In_Watch = 1;
        RouteNo = 0;
        GPSLock = 0;
        RouteNo = 0;
        ReportCheckRate = 0;
        m_bIsOverPath = false;
        m_pCarTraceList = NULL;
        m_pHisCarTraceList = NULL;
        m_pImage_Car = new TImage(Owner);
        m_pImage_Car->Parent = Owner;
        m_pImage_Car->Cursor = crHandPoint;
        m_pImage_Car->Visible = false;
	m_pImage_Car->AutoSize = true;
	m_pImage_Car->Transparent = true;
        m_CarType = Image;          //0:小轎車 , 1:貨車 , 2:連結車 , 3:警車 , 4:消防車
        //Use Main Form's PopupMenu
        m_pImage_Car->PopupMenu = CallCenter_Main_Form->PopupMenu_Car;
        //Use OnClick
        m_pImage_Car->OnClick = CallCenter_Main_Form->OnCarClick;
        //Use Hint
  	//m_pImage_Car->Hint = "";
        m_pImage_Car->ShowHint = true;
        //Image Status
        if(m_CarType == 0)
        {
                m_pImage_Car->Tag = 5;
                CallCenter_Main_Form->ChangeImagePic(m_pImage_Car,(CAR_GRAY_IMG));
        }
        else if(m_CarType == 1)
        {
                m_pImage_Car->Tag = 10;
                CallCenter_Main_Form->ChangeImagePic(m_pImage_Car,(TRUCK_GRAY_IMG));
        }
        else if(m_CarType == 2)
        {
                m_pImage_Car->Tag = 15;
                CallCenter_Main_Form->ChangeImagePic(m_pImage_Car,(CONTAINER_GRAY_IMG));
        }
        else if(m_CarType == 3)
        {
                m_pImage_Car->Tag = 20;
                CallCenter_Main_Form->ChangeImagePic(m_pImage_Car,(NINE_GRAY_IMG));
        }
        else if(m_CarType == 4)
        {
                m_pImage_Car->Tag = 25;
                CallCenter_Main_Form->ChangeImagePic(m_pImage_Car,(BUS_GRAY_IMG));
        }
        else if(m_CarType == 5)
        {
                m_pImage_Car->Tag = 30;
                CallCenter_Main_Form->ChangeImagePic(m_pImage_Car,(TRUCK2_GRAY_IMG));
        }
        else if(m_CarType == 6)
        {
                m_pImage_Car->Tag = 35;
                CallCenter_Main_Form->ChangeImagePic(m_pImage_Car,(DBUS_GRAY_IMG));
        }
        ReportQuality = 0;
}
//---------------------------------------------------------------------------
CCar::~CCar()
{
        if(m_pImage_Car)
	        delete  m_pImage_Car;
        //Free 即時軌跡資料
        if(m_pCarTraceList)
        {//Free all Memory
    	        for(int i=0;i<m_pCarTraceList->Count;i++)
        	        delete ((sSelectPoint*)m_pCarTraceList->Items[i]);
                m_pCarTraceList->Clear();
                delete m_pCarTraceList;
                m_pCarTraceList=NULL;
        }
        //Free 歷史軌跡資料
        if(m_pHisCarTraceList)
        {//Free all Memory
    	        for(int i=0;i<m_pHisCarTraceList->Count;i++)
        	        delete ((sSelectPoint*)m_pHisCarTraceList->Items[i]);
                m_pHisCarTraceList->Clear();
                delete m_pHisCarTraceList;
                m_pHisCarTraceList=NULL;
        }
}
//---------------------------------------------------------------------------
void __fastcall CCar::Update(sCarStatus* pStatus)           //資料庫tLatestStatus偵測到改變時即執行
{
	if(pStatus)
        {
    	        memcpy(&m_Status,pStatus,sizeof(sCarStatus));
                CallCenter_Main_Form->ChangeCarImgByTag(m_pImage_Car,1); //OpType=1 =>正常->異常發生
                //add Car Trace and Show in DrawCarTrace() ->in DrawImage()
                if(m_pCarTraceList)
                {
        	        sSelectPoint* pPoint = new sSelectPoint;
                        pPoint->mX2 = m_Status.m_iLon;
                        pPoint->mY2 = m_Status.m_iLat;
                        m_pCarTraceList->Add(pPoint);
                        if( m_pCarTraceList->Count >MaxTracePoint )
                        {   //Free Memory
            	                int DeleteCount = m_pCarTraceList->Count - MaxTracePoint;
                                for(int i=0;i<DeleteCount;i++)
        			        delete ((sSelectPoint*)m_pCarTraceList->Items[i]);
		    	        for(int j=0;j<DeleteCount;j++)
        			        m_pCarTraceList->Delete(0);
                        }
                }
        }//if
        ////車子中心點 X,Y 螢幕座標
        WGS2Hdc(m_Status.m_iLon,m_Status.m_iLat,m_Center_Xs,m_Center_Ys);
        if( m_pImage_Car->Tag < 0)  //Locked  //調整車子至螢幕中心點 400 Pixels 範圍內
        {//Image Tag Status =>see Common.h class CCar
    	        if(!InLockedRect())//車子超出 Locked Area
                {
        	        //Move Locked Car to center of Screen (Image_Map)
        	        DrawImage(CallCenter_Main_Form->Image_Map,m_Status.m_iLon,m_Status.m_iLat,CallCenter_Main_Form->m_dScale);
                        //將車子座標設為螢幕中心點 , 否則會發生 Stack OverFlow
    		        Hdc2WGS((CallCenter_Main_Form->Image_Map->Width/2),(CallCenter_Main_Form->Image_Map->Height/2),(long)m_Status.m_iLon,(long)m_Status.m_iLat);
        	        CallCenter_Main_Form->UpdateAllObjPos();
                }
        }
        if(IsInDrawRect(m_Status.m_iLon,m_Status.m_iLat))
        {
    	        if(m_pImage_Car->Visible == false)
                {
                        if(Visible == true)
                                m_pImage_Car->Visible = true;
                }
       	        //注意校正因 ToolBar(X),CoolBsr(Y) 引起之位移
                //(因為m_pImage_Car 的Owner and Parent 為 Main_Form 而非 Image_Map)
    	        m_pImage_Car->Left = (CallCenter_Main_Form->Image_Map->Left) + (m_Center_Xs - m_pImage_Car->Width/2);
    	        m_pImage_Car->Top = (CallCenter_Main_Form->Image_Map->Top) + (m_Center_Ys - m_pImage_Car->Height/2);
                //Show Car Trace
                if(m_pCarTraceList)
			ReDrawImage(CallCenter_Main_Form->Image_Map,CallCenter_Main_Form->m_dScale);
   	 	//Update Hint
    	        UpdateHint();
        }
        else
        m_pImage_Car->Visible = false;
}              
//---------------------------------------------------------------------------
bool __fastcall CCar::InLockedRect()
{   //以車子中心點作為判斷是否超出標準
        long Left = (CallCenter_Main_Form->Image_Map->Width/2 - CallCenter_Main_Form->m_iLockedAreaPixels / 2);
        long Top = (CallCenter_Main_Form->Image_Map->Height/2 - CallCenter_Main_Form->m_iLockedAreaPixels / 2);
        long Right = (CallCenter_Main_Form->Image_Map->Width/2 + CallCenter_Main_Form->m_iLockedAreaPixels / 2);
        long Bottom = (CallCenter_Main_Form->Image_Map->Height/2 + CallCenter_Main_Form->m_iLockedAreaPixels / 2);
        //Get Locked Area WGS
        Hdc2WGS(Left,Top,Left,Top);
	Hdc2WGS(Right,Bottom,Right,Bottom);
	if(m_Status.m_iLon < Left)
                return false;
	if(m_Status.m_iLon > Right)
                return false;
	if(m_Status.m_iLat > Top)
                return false;
	if(m_Status.m_iLat < Bottom)
                return false;
        return true;
}
//---------------------------------------------------------------------------
char* __fastcall CCar::GetCarID()
{
        return m_szCarID.c_str();
}
//---------------------------------------------------------------------------
long __fastcall CCar::GetCarSpeed()
{
	return m_Status.m_sSpeed;
}
//---------------------------------------------------------------------------
char* __fastcall CCar::GetPlaceInfo()
{
        static char strPlace[100];
        strcpy(strPlace,m_Status.m_szPlaceName.c_str());
        return strPlace;
}
//---------------------------------------------------------------------------
char* __fastcall CCar::GetDateTime()
{
	static char szDateTime[60];
        wsprintf(szDateTime,"%s/%s",m_Status.m_szRecvDate,m_Status.m_szRecvTime);
        return szDateTime;
}
//---------------------------------------------------------------------------
char* __fastcall CCar::SendDateTime()
{
        static char szDateTime[60];
        wsprintf(szDateTime,"%s/%s",m_Status.m_szSendDate,m_Status.m_szSendTime);
        return szDateTime;
}
//---------------------------------------------------------------------------
long __fastcall CCar::GetCar_X2()
{
	return m_Status.m_iLon;
}
//---------------------------------------------------------------------------
long __fastcall CCar::GetCar_Y2()
{
	return m_Status.m_iLat;
}
//---------------------------------------------------------------------------
char* __fastcall CCar::GetLocatorNo()
{
        static char strLocatorNo[21];
        strcpy(strLocatorNo,m_Status.m_szLocatorNo);
        return strLocatorNo;
}
//---------------------------------------------------------------------------
unsigned long __fastcall CCar::Get_ULong_Status()
{
	return m_Status.m_iStatus;
}
//---------------------------------------------------------------------------
char* __fastcall CCar::GetCarStatus()
{   //Image Tag Status =>see Common.h class CCar
        static char strCarStatus[21];
        if( m_pImage_Car->Tag > 0 )
        {
	        switch((m_pImage_Car->Tag)%4)
    	        {
    		        case 1:	strcpy(strCarStatus,"上次狀態");
                                break;
    		        case 2:	strcpy(strCarStatus,"活動中");
                                break;
    		        case 3:	strcpy(strCarStatus,"異常發生");
                                break;
    		        case 0:	strcpy(strCarStatus,"異常處理中");
                                break;
                }//switch
        }
        else
        {
                switch((m_pImage_Car->Tag)%4)
    	        {
    		        case -1:strcpy(strCarStatus,"鎖定監控+上次狀態");
                                break;
    		        case -2:strcpy(strCarStatus,"鎖定監控+活動中");
                                break;
    		        case -3:strcpy(strCarStatus,"鎖定監控+異常發生");
                                break;
    		        case 0:	strcpy(strCarStatus,"鎖定監控+異常處理中");
                                break;
    	        }//switch
        }
        return strCarStatus;
}
//---------------------------------------------------------------------------
long __fastcall CCar::GetLocatorVer(char* textLocatorVer)
{
	return 2; //RSL-1100-101021
}
//---------------------------------------------------------------------------
int __fastcall CCar::GetEventStatus(char* textEventStatus)
{   //Image Tag Status =>see Common.h class CCar
/*
return 0        正常
return 1        異常未處理
return 2        異常正在處理
*/
        bool bAlarmIO = 0;
        if(m_Status.m_iStatus & 1 == 1)
        {
                bAlarmIO = 1;
        }
        if(!bAlarmIO)
        {
                if(m_Status.m_iStatusHandle != 0)
                {
                        DataModule1->Update_DB_Value("tLatestStatus","StatusHandle",0,"LocatorNo",m_Status.m_szLocatorNo);
                }
                if(textEventStatus)
                {
                        if(m_Status.m_iLon == 0)
                        {
                                strcpy(textEventStatus,"未起始");
                        }
                        else
                        {
                                strcpy(textEventStatus,"正常");
                        }
                }
                return 0;
        }
        else if((bAlarmIO)&&(m_Status.m_iStatusHandle != 2))
        {
                if(textEventStatus)
                        strcpy(textEventStatus,"有異常發生未處理");
                return 1;
        }
        else if(m_Status.m_iStatusHandle == 2)
        {
                if(textEventStatus)
                        strcpy(textEventStatus,"有異常且正處理中");
                return 2;
        }
        else return 0;
}
//---------------------------------------------------------------------------
void __fastcall CCar::UpdateImgTag(int EventStatus)
{
        if(m_pImage_Car->Tag > 0)
        {
    	        switch((m_pImage_Car->Tag)%4)
                {
                        case 1: if(EventStatus==0)              //正常
                                        m_pImage_Car->Tag += 1;
            		        else if(EventStatus==1)         //異常發生
                    	                m_pImage_Car->Tag += 2;
                                else                            //異常處理中
                                        m_pImage_Car->Tag += 3;
                                        break;
                        case 2:	if(EventStatus==1) //異常發生
                    	                m_pImage_Car->Tag += 1;
                                else if(EventStatus==2)   //異常處理中
                                        m_pImage_Car->Tag += 2;
                                        break;
                        case 3:	if(EventStatus==0) //正常
                                        m_pImage_Car->Tag -= 1;
            		        else if(EventStatus==2) //異常處理中
                    	                m_pImage_Car->Tag += 1;
                                        break;
                        case 0: if(EventStatus==0) //正常
                                        m_pImage_Car->Tag -= 2;
            		        else if(EventStatus==1) //異常發生
                    	                m_pImage_Car->Tag -= 1;
                                        break;
                }
        }//if
        else
        {
    	        switch((m_pImage_Car->Tag)%4)
                {
        	        case -1:if(EventStatus==0) //正常
                                        m_pImage_Car->Tag -= 1;
            		        else if(EventStatus==1) //異常發生
                    	                m_pImage_Car->Tag -= 2;
                                else   //異常處理中
                                        m_pImage_Car->Tag -= 3;
                                        break;
                        case -2:if(EventStatus==1) //異常發生
                                        m_pImage_Car->Tag -= 1;
                                else if(EventStatus==2) //異常處理中
                                        m_pImage_Car->Tag -= 2;
                                        break;
                        case -3:if(EventStatus==0) //正常
                                        m_pImage_Car->Tag += 1;
            		        else if(EventStatus==2) //異常處理中
                    	                m_pImage_Car->Tag -= 1;
                                        break;
                        case 0: if(EventStatus==0) //正常
                                        m_pImage_Car->Tag += 2;
            		        else if(EventStatus==1) //異常發生
                    	                m_pImage_Car->Tag += 1;
                                        break;
                }
        }//else
}
//---------------------------------------------------------------------------
void __fastcall CCar::UpdateHint()
{
        //for Dynamic Hint
        AnsiString strEventName;
        AnsiString strSelPathName;
        TCSelectPath* pRoute;
        AnsiString PathName;
        AnsiString GPS_Stat;
        if(GPSLock == 1)
        {
                GPS_Stat = "衛星已鎖定";
        }
        else
        {
                GPS_Stat = "衛星尚未鎖定";
        }
        strEventName = IsEventOccr(Get_ULong_Status(),GetLocatorVer(NULL));
        //Show 指定路線
        if(RouteNo != 0)
        {
                pRoute = CallCenter_Main_Form->GetRoute(RouteNo);
                PathName = pRoute->PathName;
        }
        if( (m_pImage_Car->Tag == 1)||(m_pImage_Car->Tag == -1) )	//Image Tag Status =>see Common.h class CCar
        {
                m_pImage_Car->Hint = "上次最後狀態:";
                m_pImage_Car->Hint = m_pImage_Car->Hint
                                + "\n車號:" + GetCarID()
                                + "\n車機:" + GetLocatorNo()
                                + "\n速度:" + IntToStr(GetCarSpeed()) + "km/h"
                                + "\n位置:" + GetPlaceInfo()
                                + "\n車況:" + strEventName
                                + "\n指定路線:" + PathName
                                + "\nIO STAT:" + IntToBinary(m_Status.m_iStatus)
                                + "\nGPS狀態:" + GPS_Stat;
        }
        else
        {
    	        m_pImage_Car->Hint = "目前最新狀態:";
                m_pImage_Car->Hint = m_pImage_Car->Hint
                                + "\n車號:" + GetCarID()
                                + "\n車機:" + GetLocatorNo()
                                + "\n速度:" + IntToStr(GetCarSpeed()) + "km/h"
                                + "\n位置:" + GetPlaceInfo()
                                + "\n車況:" + strEventName
                                + "\n指定路線:" + PathName
                                + "\nIO STAT:" + IntToBinary(m_Status.m_iStatus)
                                + "\nGPS狀態:" + GPS_Stat;
        }
}
//---------------------------------------------------------------------------
AnsiString __fastcall CCar::IsEventOccr(unsigned long CarStatus,long LocatorVer)
{
//找看看有沒有車輛的狀況
        AnsiString EventStr;
        for(int i=0; i < Form_ManageState->ListView1->Items->Count; i++)
        {
                AnsiString CarID = GetCarID();
                TListItem* pListItem = Form_ManageState->ListView1->Items->Item[i];
                if(pListItem->Caption == CarID)
                {
                        if(pListItem->SubItems->Strings[EVENTTYPE] == "1")
                        {
                                EventStr = EventStr + "Alarm On";
                        }
                        else if(pListItem->SubItems->Strings[EVENTTYPE] == "2")
                        {
                                EventStr = EventStr + "超出路線";
                        }
                        else if(pListItem->SubItems->Strings[EVENTTYPE] == "4")
                        {
                                EventStr = EventStr + "超出時間";
                        }
                        else if(pListItem->SubItems->Strings[EVENTTYPE] == "8")
                        {
                                EventStr = EventStr + "回報不正常";
                        }
                        if(pListItem->SubItems->Strings[HANDLER] != "未處理")
                        {
                                EventStr = EventStr + "-處理中";
                        }
                }
        }
        return EventStr;
}
//---------------------------------------------------------------------------
///////// Get CarInfo From Database /////////////
char*	__fastcall CCar::GetLocatorPwd()
{
        static char strLocatorPwd[21];
        strcpy(strLocatorPwd,"11111111");	//Default Password
	try
        {
    	        DataModule1->ChkDatabase(DataModule1->Database_DCH);
    	        TQuery* pQuery = DataModule1->Query_DB;
    	        pQuery->Close();
    	        pQuery->SQL->Clear();
    	        pQuery->SQL->Add("SELECT tCarInfo.LocatorPwd AS out_Pwd FROM tCarInfo");
    	        pQuery->SQL->Add("where tCarInfo.LocatorNo= :ParaLocatorNo");
    	        pQuery->ParamByName("ParaLocatorNo")->AsString = GetLocatorNo();
    	        if(! pQuery->Prepared)
    	        {   //Just for Performance
                        pQuery->UniDirectional = true;
    		        pQuery->Prepare();
    	        }
    	        pQuery->Open();
    	        //export Data Result
    	        if(pQuery->RecordCount > 0)
                {
    		        AnsiString strPwd = pQuery->FieldValues["out_Pwd"];
        	        strcpy(strLocatorPwd,strPwd.c_str());
    	        }
                pQuery->Close();
        }//try
        catch(EDBEngineError &E)
        {
                DataModule1->EstablishConnect(1);
        }
        catch(...)
        {}
        return strLocatorPwd;
}
//---------------------------------------------------------------------------
void __fastcall GetNowDateTime(char* out_szDate,char* out_szTime)
{
	char szYear[5]="1999";
        char szMM[3]="10";
	char szDD[3]="10";
        char szhh[3]="11";
	char szmm[3]="22";
        char szss[3]="33";
	SYSTEMTIME LocalTime;
	GetLocalTime(&LocalTime);
	ltoa(LocalTime.wYear,szYear,10);
	if(LocalTime.wMonth<10)
	{
                szMM[0]='0';
                ltoa(LocalTime.wMonth,szMM+1,10);
        }
	else
        {
		ltoa(LocalTime.wMonth,szMM,10);
        }
	if(LocalTime.wDay<10)
        {
                szDD[0]='0';
                ltoa(LocalTime.wDay,szDD+1,10);
        }
	else
        {
		ltoa(LocalTime.wDay,szDD,10);
        }
	if(LocalTime.wHour<10)
	{
                szhh[0]='0';
                ltoa(LocalTime.wHour,szhh+1,10);
        }
	else
		ltoa(LocalTime.wHour,szhh,10);
	if(LocalTime.wMinute<10)
	{
                szmm[0]='0';
                ltoa(LocalTime.wMinute,szmm+1,10);
        }
	else
        {
		ltoa(LocalTime.wMinute,szmm,10);
        }
	if(LocalTime.wSecond<10)
	{
                szss[0]='0';
                ltoa(LocalTime.wSecond,szss+1,10);
        }
	else
        {
		ltoa(LocalTime.wSecond,szss,10);
        }
        strcpy(out_szDate,szYear);
        strcat(out_szDate,szMM);
        strcat(out_szDate,szDD);
        strcpy(out_szTime,szhh);
        strcat(out_szTime,szmm);
        strcat(out_szTime,szss);
}
//---------------------------------------------------------------------------
int  __fastcall GetImgIndexFromTag(int Tag)
{                    //     小轎車             //         貨車                 //           連結車                  //
//Image Tag value Map=>1 2 3 4 5 -1 -2 -3 -4 -5| 6  7  8  9  10 -6 -7 -8 -9 -10| 11 12  13 14 15 -11 -12 -13 -14 -15|
//ImageList index Map=>1 2 3 4 5  6  7  8  9 10|11 12 13 14  15 16 17 18 19  20| 21 22  23 24 25  26  27  28  29  30|
//-----------------------------------------------------------------------------------------------//


	if(Tag < 0)
        {
    	        Tag = -(Tag);
    	        return (Tag+(int)((Tag-1)/5)*5+5);
        }
        else
    	        return (Tag+(int)((Tag-1)/5)*5);
}
//---------------------------------------------------------------------------
AnsiString __fastcall IntToBinary(int source)
{
        char distinct[5] = {'0','0','0','0','\0'};
        if(source >= 32)
        {
                distinct[3] = '1';
                source = source % 32;
        }
        if(source >= 16)
        {
                distinct[2] = '1';
                source = source % 16;
        }
        if(source >= 8)
        {
                distinct[1] = '1';
                source = source % 8;
        }
        if(source >= 4)
        {
                distinct[0] = '1';
                source = source % 4;
        }
        if(source >= 2)
        {
//                distinct[1] = '1';
                source = source % 2;
        }
        if(source >= 1)
        {
//                distinct[0] = '1';
        }
        return (AnsiString)distinct;
}
//---------------------------------------------------------------------------
void __fastcall WriteLog(AnsiString Str)
{
        TDateTime* now = new TDateTime();               //free in the end
        *now = now->CurrentDateTime();
        Str = now->DateTimeString() + " " + Str;
        TStringList *TempList = new TStringList;
        TempList->LoadFromFile("runfunction.log");
        TempList->Add(Str);
        TempList->SaveToFile("runfunction.log");
        delete TempList;
        delete now;
}
//---------------------------------------------------------------------------
int __fastcall Compare_CCar_By_LocatorNo(void * Item1, void * Item2)
{
        CCar* CCar1 = (CCar*)Item1;
        CCar* CCar2 = (CCar*)Item2;
        return strcmp(CCar1->m_Status.m_szLocatorNo, CCar2->m_Status.m_szLocatorNo);
}
//---------------------------------------------------------------------------
int __fastcall Compare_CCar_By_CarID(void * Item1, void * Item2)
{
        CCar* CCar1 = (CCar*)Item1;
        CCar* CCar2 = (CCar*)Item2;
        return strcmp(CCar1->m_szCarID.c_str(), CCar2->m_szCarID.c_str());
}
