// RemarkPlace.h
#ifndef __REMARKPLACE_H__
#define __REMARKPLACE_H__

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>

///////////   for CRemarkPlace class   ///////// �Ƶ��a�I

class CRemarkPlace
{
public:
    int                 i_SerialNo;             //�y����,�P��Ʈw�ѧO��
    TImage*		m_pImage_Remarks;
    long		m_X2;  				//�ϧΤ����I X �g�n�׮y��
    long		m_Y2;               //�ϧΤ����I Y �g�n�׮y��
    long 		m_Center_Xs;		//�ϧΤ����I X �ù��y��
    long 		m_Center_Ys;		//�ϧΤ����I Y �ù��y��
    int			m_RemarkType;		//0:�@��Ƶ��a�I , 1:���|�j�M�Хܦa , 2: ��X��T�]�I
    char		m_szCoID[11];            //vincent guess:���q�s��
    char                m_strCompanyName[80];      //vincent add ���q�W��
    char  		m_strImagePath[256];    //�Хܹ��ɸ��|
    char  		m_strName[31];       	//���� 30
    char		m_strPhone[21];      	//���� 20
    char		m_strAddr[51];       	//���� 50
    char		m_strEmail[31];      	//���� 30
    char  		m_strWebAddr[31];    	//���� 30
    char		m_strRemarks[61];      	//���� 60
    AnsiString  __fastcall GetClassName();
    CRemarkPlace(TWinControl* Owner,long tmX,long tmY,char* ImagePath);
    ~CRemarkPlace();

    //inline bool __fastcall InDrawRect();
    void __fastcall Update();
    void __fastcall SetHint();

};
AnsiString __fastcall GetClassName(int TypeID);

#endif	//__REMARKPLACE_H__ 
