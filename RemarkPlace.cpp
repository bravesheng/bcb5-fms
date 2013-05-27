// RemarkPlace.cpp

#include "RemarkPlace.h"
#include "MainForm.h"

///////////  for CRemarkPlace class   ////////////// �Ƶ��a�I

CRemarkPlace::CRemarkPlace(TWinControl* Owner,long tmX,long tmY,char* ImagePath)
{
        m_RemarkType = 0;	       
        m_X2 = tmX;  			//WGS�y��
        m_Y2 = tmY;             //WGS�y��

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

        ////�ϧΤ����I X,Y �ù��y��
        WGS2Hdc(m_X2,m_Y2,m_Center_Xs,m_Center_Ys);

        //�`�N�ե��] ToolBar(X),CoolBsr(Y) �ް_���첾
        //(�]��m_pImage_Car ��Owner and Parent �� Main_Form �ӫD Image_Map)
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
    ////�ϧΤ����I X,Y �ù��y��
    WGS2Hdc(m_X2,m_Y2,m_Center_Xs,m_Center_Ys);

    if(IsInDrawRect(m_X2,m_Y2))
    {
    	if(m_pImage_Remarks->Visible == false)
			m_pImage_Remarks->Visible = true;

       	//�`�N�ե��] ToolBar(X),CoolBsr(Y) �ް_���첾
        //(�]��m_pImage_Car ��Owner and Parent �� Main_Form �ӫD Image_Map)
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
    {//���]Hint
    	m_pImage_Remarks->ShowHint = false;
        return;
    }

    AnsiString strHint("�W��: ");
    strHint += m_strName;
    strHint = strHint + "\n" + "���O: " + GetClassName();
    if(strlen(m_strPhone)>0)
    	strHint = strHint + "\n" + "�q��: " + m_strPhone;
    if(strlen(m_strAddr)>0)
    	strHint = strHint + "\n" + "�a�}: " + m_strAddr;
    if(strlen(m_strEmail)>0)
    	strHint = strHint + "\n" + "E-mail: " + m_strEmail;
    if(strlen(m_strWebAddr)>0)
    	strHint = strHint + "\n" + "���}: " + m_strWebAddr;
    if(strlen(m_strRemarks)>0)
    	strHint = strHint + "\n" + "����: " + m_strRemarks;

  	m_pImage_Remarks->Hint = strHint;
    m_pImage_Remarks->ShowHint = true;

}

AnsiString __fastcall CRemarkPlace::GetClassName()
{
        AnsiString ClassArry[10];
        ClassArry[0] = "�����q";
        ClassArry[1] = "��F�ϡB�a�W";
        ClassArry[2] = "�������B���B���B�ڤh���B�X�Y";
        ClassArry[3] = "����B�����I";
        ClassArry[4] = "�ؿv���B�D�n�a��";
        ClassArry[5] = "�Ȧ�";
        ClassArry[6] = "������";
        ClassArry[7] = "�q�v�|";
        ClassArry[8] = "����";
        ClassArry[9] = "�ʳf�B�ө�";
        return ClassArry[m_RemarkType];
}

AnsiString __fastcall GetClassName(int TypeID)
{
        AnsiString ClassArry[10];
        ClassArry[0] = "�����q";
        ClassArry[1] = "��F�ϡB�a�W";
        ClassArry[2] = "�������B���B���B�ڤh���B�X�Y";
        ClassArry[3] = "����B�����I";
        ClassArry[4] = "�ؿv���B�D�n�a��";
        ClassArry[5] = "�Ȧ�";
        ClassArry[6] = "������";
        ClassArry[7] = "�q�v�|";
        ClassArry[8] = "����";
        ClassArry[9] = "�ʳf�B�ө�";
        return ClassArry[TypeID];
}



