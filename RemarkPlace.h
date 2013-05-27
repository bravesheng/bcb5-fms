// RemarkPlace.h
#ifndef __REMARKPLACE_H__
#define __REMARKPLACE_H__

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>

///////////   for CRemarkPlace class   ///////// 備註地點

class CRemarkPlace
{
public:
    int                 i_SerialNo;             //流水號,與資料庫識別用
    TImage*		m_pImage_Remarks;
    long		m_X2;  				//圖形中心點 X 經緯度座標
    long		m_Y2;               //圖形中心點 Y 經緯度座標
    long 		m_Center_Xs;		//圖形中心點 X 螢幕座標
    long 		m_Center_Ys;		//圖形中心點 Y 螢幕座標
    int			m_RemarkType;		//0:一般備註地點 , 1:路徑搜尋標示地 , 2: 整合資訊設施
    char		m_szCoID[11];            //vincent guess:公司編號
    char                m_strCompanyName[80];      //vincent add 公司名稱
    char  		m_strImagePath[256];    //標示圖檔路徑
    char  		m_strName[31];       	//限制 30
    char		m_strPhone[21];      	//限制 20
    char		m_strAddr[51];       	//限制 50
    char		m_strEmail[31];      	//限制 30
    char  		m_strWebAddr[31];    	//限制 30
    char		m_strRemarks[61];      	//限制 60
    AnsiString  __fastcall GetClassName();
    CRemarkPlace(TWinControl* Owner,long tmX,long tmY,char* ImagePath);
    ~CRemarkPlace();

    //inline bool __fastcall InDrawRect();
    void __fastcall Update();
    void __fastcall SetHint();

};
AnsiString __fastcall GetClassName(int TypeID);

#endif	//__REMARKPLACE_H__ 
