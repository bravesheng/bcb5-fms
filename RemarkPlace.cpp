// RemarkPlace.cpp

#include "RemarkPlace.h"
#include "MainForm.h"

///////////  for CRemarkPlace class   ////////////// 備註地點

CRemarkPlace::CRemarkPlace(TWinControl* Owner,long tmX,long tmY,char* ImagePath)
{
        m_RemarkType = 0;	       
        m_X2 = tmX;  			//WGS座標
        m_Y2 = tmY;             //WGS座標

        m_pImage_Remarks = new TImage(Owner);
        m_pImage_Remarks->Parent = Owner;

        CallCenter_Main_Form->ChangeImagePic(m_pImage_Remarks,ImagePath);
	//m_strImagePath = ImagePath;
        strcpy(m_strImagePath,ImagePath);
        strcpy(m_strName,"");
        strcpy(m_strPhone,"");
        strcpy(m_strAddr,"");
        strcpy(m_strEmail,"");
        strcpy(m_strWebAddr,"");
        strcpy(m_strRemarks,"");
        m_pImage_Remarks->Cursor = crHandPoint;
	m_pImage_Remarks->AutoSize = true;
	m_pImage_Remarks->Transparent = true;

        ////圖形中心點 X,Y 螢幕座標
        WGS2Hdc(m_X2,m_Y2,m_Center_Xs,m_Center_Ys);

        //注意校正因 ToolBar(X),CoolBsr(Y) 引起之位移
        //(因為m_pImage_Car 的Owner and Parent 為 Main_Form 而非 Image_Map)
        m_pImage_Remarks->Left = (CallCenter_Main_Form->Image_Map->Left) + (m_Center_Xs - m_pImage_Remarks->Width/2);
        m_pImage_Remarks->Top = (CallCenter_Main_Form->Image_Map->Top) + (m_Center_Ys - m_pImage_Remarks->Height/2);

        //Use Main Form's PopupMenu
        m_pImage_Remarks->PopupMenu = CallCenter_Main_Form->PopupMenu_Remarks;
        //Use OnClick
        //m_pImage_Remarks->OnClick = CallCenter_Main_Form->OnCarClick;

}

CRemarkPlace::~CRemarkPlace()
{
	delete  m_pImage_Remarks;
}

void __fastcall CRemarkPlace::Update()
{
    ////圖形中心點 X,Y 螢幕座標
    WGS2Hdc(m_X2,m_Y2,m_Center_Xs,m_Center_Ys);

    if(IsInDrawRect(m_X2,m_Y2))
    {
    	if(m_pImage_Remarks->Visible == false)
			m_pImage_Remarks->Visible = true;

       	//注意校正因 ToolBar(X),CoolBsr(Y) 引起之位移
        //(因為m_pImage_Car 的Owner and Parent 為 Main_Form 而非 Image_Map)
    	m_pImage_Remarks->Left = (CallCenter_Main_Form->Image_Map->Left) + (m_Center_Xs - m_pImage_Remarks->Width/2);
    	m_pImage_Remarks->Top = (CallCenter_Main_Form->Image_Map->Top) + (m_Center_Ys - m_pImage_Remarks->Height/2);
    }
    else
    	m_pImage_Remarks->Visible = false;

}

void __fastcall CRemarkPlace::SetHint()
{
    //Use Hint
	if( strlen(m_strName)==0  )
    {//不設Hint
    	m_pImage_Remarks->ShowHint = false;
        return;
    }

    AnsiString strHint("名稱: ");
    strHint += m_strName;
    strHint = strHint + "\n" + "類別: " + GetClassName();
    if(strlen(m_strPhone)>0)
    	strHint = strHint + "\n" + "電話: " + m_strPhone;
    if(strlen(m_strAddr)>0)
    	strHint = strHint + "\n" + "地址: " + m_strAddr;
    if(strlen(m_strEmail)>0)
    	strHint = strHint + "\n" + "E-mail: " + m_strEmail;
    if(strlen(m_strWebAddr)>0)
    	strHint = strHint + "\n" + "網址: " + m_strWebAddr;
    if(strlen(m_strRemarks)>0)
    	strHint = strHint + "\n" + "說明: " + m_strRemarks;

  	m_pImage_Remarks->Hint = strHint;
    m_pImage_Remarks->ShowHint = true;

}

AnsiString __fastcall CRemarkPlace::GetClassName()
{
        AnsiString ClassArry[10];
        ClassArry[0] = "分公司";
        ClassArry[1] = "行政區、地名";
        ClassArry[2] = "火車站、捷運站、巴士站、碼頭";
        ClassArry[3] = "公園、風景點";
        ClassArry[4] = "建築物、主要地標";
        ClassArry[5] = "銀行";
        ClassArry[6] = "停車場";
        ClassArry[7] = "電影院";
        ClassArry[8] = "飯店";
        ClassArry[9] = "百貨、商店";
        return ClassArry[m_RemarkType];
}

AnsiString __fastcall GetClassName(int TypeID)
{
        AnsiString ClassArry[10];
        ClassArry[0] = "分公司";
        ClassArry[1] = "行政區、地名";
        ClassArry[2] = "火車站、捷運站、巴士站、碼頭";
        ClassArry[3] = "公園、風景點";
        ClassArry[4] = "建築物、主要地標";
        ClassArry[5] = "銀行";
        ClassArry[6] = "停車場";
        ClassArry[7] = "電影院";
        ClassArry[8] = "飯店";
        ClassArry[9] = "百貨、商店";
        return ClassArry[TypeID];
}



