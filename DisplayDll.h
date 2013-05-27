#ifndef __DISPLAY_DLL_H__
#define __DISPLAY_DLL_H__

//API_EXP: API Export Function
//DLL_EXP: DLL Export Function
//else	 : DLL import Function
#define   DLL_IMP 		//DLL_EXP	API_EXP

#ifdef	DLL_EXP
	#define		FUNC 	extern "C" __declspec(dllexport)
#else
	#ifdef	API_EXP
		#define FUNC 	inline
	#else
		#define FUNC 	extern "C" __declspec(dllimport)
	#endif
#endif
////////////////////////////////////////////////////////////////////////


//// 2001.03.21 -> New Display DLL for DCH
//////////////////////////////////////////////////////////////////////////////////////
struct s_outo { char layerid;
                char gtype;    //1_polygon 2_polyline 3_point 0_delete
                char feature;
                char dummy;
                long recid;
                long x,y;
                long nmpt;
                long sp,ps; 
              };


/*
==0718====================================================================
�W�[layersave��layeropen��format��6
->(.roy�� binary��)
==0717====================================================================
�W�ilayerdraw�t��
==0705====================================================================
�W�[server.c��asp��
==0621====================================================================
��ent.c��checkb2(),��mifmidopen�ϥ�, �ѨM�r�ꤤ���r��,���D
==0530====================================================================
�W�[�и��W,��layerdraw
==0530====================================================================
iis����k
1.�H�p�d��d(�pmouse_move) -> entiis
2.�H�j�d��d(�p??����)     -> entiis

3.�H�Y���ݩʭȬd(�p��F��) -> entiis
  3.1 ��layerfeature(�⦸)
  3.2 �Aentiis

 layerfeature( poi_layer_id, -9, "",   1)

 layerfeature( poi_layer_id,  1, "7",  2)
 layerfeature( poi_layer_id,  1, "8",  2)

 entiis      ( poi_layer_id, -2, 0,0,360000000, 2



4.�H�r��d                 -> entstriis

==0530====================================================================
*/
FUNC long __stdcall entstriis(long layer_id,char cnm[40],long maxo,long maxnm,struct s_outo *o,char *nm,long *n);
FUNC long __stdcall entstriis2(long layer_id,char cnm[40],long maxo,long maxnm,struct s_outo *o,char *nm,long *n);
/*
�H�r��d��
cnm[40]      ��蠟�r��
��l�Pentiis
*/
//==old=====================================================================
FUNC long __stdcall maplibset(long fg, char fnm[100]);
/*
fg  1  init
   -1  free
fnm    �Y"d:\\1yu\\pda2\\maplib.txt"
==========================================================================
*/
FUNC long __stdcall maplibdraw(long id);
/*
id     �ĴX��hdc,�i��0~49
==========================================================================
*/
FUNC long __stdcall hdcsave_restore(long fg,long id);
/*
fg  1    �|create_mem_dc
   -1    �|delete_mem_dc
    2    �|�N�{�bdc�����ecopy�@����mem_dc
    3    �|�Nmem_dc�����e�٭��{�b��dc
id  0~49 �{�b��hdc�s��
==========================================================================
*/
FUNC long __stdcall layeropen(long layer_id,long format, char fnm[100],
								 long ppl, long ppn,long  psb);
/*
�}��
layer_id  0~19�����N��,�Y�i�P�ɸ��J20��(�h)�a�ϸ��
format    4_fnm��dxf��
          5_fnm��mid/mif�ɪ�mif��
          6_��roy��(binary)
          8_fnm��nmea����r��
         10_fnm��bmp��rle��
         20_�}�s��,fnm���ݩ����W��
         -1_�u�]�wppl,ppn,psb
fnm
ppl      polyline�Ϊ����� (-1, 0~49)
pgn      polygon�Ϊ�����  (-1, 0~49)
psb      icon�Ϊ��s��   (0~30����, 31~99�~��)
return   -1 ����
          1 �Ӹ�Ƭ��V�q,�B���g�n��     (.cpt�ɤ��s�b)
          2 �Ӹ�Ƭ��V�q,����bmp�y�Шt��(.cpt�ɦs�b)
          3 �Ӹ�Ƭ��V�q                (.cpt�ɦs�b)   ->�y�Шt��unknow
         11 �Ӹ�Ƭ�bmp,�B�����_�¤W    (.cpt�ɦs�b)
         12 �Ӹ�Ƭ�bmp,�B�D���_�¤W    (.cpt�ɦs�b)
         13 �Ӹ�Ƭ�bmp                 (.cpt�ɤ��s�b) ->�y�Шt��unknow

�p1: layeropen(0, 5,"c:\\a\\bc.mif",0,0,0);  //...bc.mid�ɤ]�@�w�nexist
�p2: layeropen(1,10,"c:\\a\\de.bmp",0,0,0);
        �Y��bmp��,�q�`�����@�P�D�ɦW,���ɦW��.cpt,���ɦs�b (�pc:\\a\\de.cpt)
        .cpt�ɬ������I���Ascii��,���T�檺��r,�ܷN��
        -----------
        2
        x1 y1 X1 Y1
        x2 y2 X2 Y2
        -----------
        xy�������I�bbmp�����y��,�åHbmp�����U�謰(0,0),y�V�W����
        XY�������I���g�n�׮y��,�p(121.123456 25.654321)
�p3: �s�}�@�Ƶ��a�I�ϼh
     layeropen(2,20,"name,address,tel",0,0,0);
==================================================================
*/
FUNC long __stdcall layerfree(long layer_id);
/*
�{�������ɭncall
==================================================================
*/

FUNC long __stdcall defgpslayer(long layer_id,long format,long sec,long spots,
								long dhop,long dist,long gen,long datefg);
/*
�ŧi�n�}�l�s��gps���
layer_id 0~19_�n�s�쨺�@�h
           -1_�u�]�w�Ѽ�
format   2_�s��polyline
         3_�s��point
sec      ??��~�n(�s�@��)
spots    ??�ӽìP�~�n
hdop     ??�~�n
dist     ??���ؤ~�n
gen      ??���ؤ~�n
datefg   1_�ϥ�gps�ɶ�
         2_�ϥ�Local�ɶ�
return   1_���\ -1_����
=================================================================
*/
FUNC long __stdcall put1gga(char memo[20],char ggarmc[150], long *ox,long *oy);
/*
�e�@�ynmea���y�l��dll
memo     �Ƶ�
ggarmc   �@�ynmea���y�l
*ox,*oy  �Ǧ^���g�n�y��
=================================================================
*/

FUNC long __stdcall layersave(long layer_id,long format, char fnm[100]);
/*
�s��
layer_id
format   4_�nsave��dxf��
         5_�nsave��mifmid
         6_��roy��(binary)
fnm      �nsave�����|���ɦW
=================================================================
*/
FUNC long __stdcall layerinfo(long layer_id,long fg,long o[20], char *nm);
/*
�߰�layer����T
input  layer_id
input  fg
output o[0] �Playeropen��return
       o[1] ��layer���X���Ϥ���
       o[2] �Ĥ@���Ϥ��O 1_polygon 2_polyline 3_point
       o[3] �ݩ�����
       o[4] ��mem��
       o[5] �ݩ�mem��
       o[10,11] �V�q��ƪ��̤pxy  (�H�U��ƭnfg==10�~��)
       o[12,13] �V�q��ƪ��̤jxy
       o[12,13] bmp��width,height
       o[14,15] bmp(0,0)���g�n��
       o[16,17] bmp(w,h)���g�n��
output nm   �ݩ���W,��j�p�ܤ֭n250��byte
            �ǥX��Ʀp"name\0address\0tel\0"
=================================================================
*/

FUNC long __stdcall hdcset(long id,long hdc,long hdcw,long hdch,
							long coord,long xc,long yc,long mp);
/*
�Ƕihdc�γ]�w���M���a�z�y�е���T(�ä��e��)
id     �ĴX��hdc,�i��0~49
hdc
hdcw   hdc��width
hdch   hdc��height
coord  1,        ��ܮy�Шt�έn�θg�n��,�B��j1000000��(��long)
       1000~1019,��ܮy�Шt�έn��bmp�y�Шt��(�ԨҤl)
       -3,       ��ܮy�Шt�έn��unknow
xc,yc  hdc���ߪ��y��
mp     �C��pixel�O....(�ԨҤl)
return 1_���\ -1_����

�p1: hdcset(picbox.hdc, picbox.scalewidth,picbox.scaleheight,
            1,
            121123456,25654321, //hdc���ߤ��g�n�y�ЬO -> (121.123456,25.654321)
            1234);              //hdc�C��pixel���j�p��-> 0.001234��
�p2: layeropen(4,10,"c:\\g.bmp");
     hdcset(picbox.hdc, picbox.scalewidth,picbox.scaleheight,
            1004, //�ϥ� 4 -> g.bmp -> �obmp�y�Шt��
            0,0,  //hdc����,��Obmp�����U��
            10);  //10 -> 1:1���bmp  (�Ymp�O�n*10,��xc,yc�h�_)
                  // 5 -> ��j2�����bmp
                  //20 -> �Y�p1/2���bmp
========================================================================
*/
FUNC long __stdcall xycalc(long id,long fg,long ix,long iy,long *ox,long *oy);
/*
�y���ഫ
input  id   0~49 �ĴX��hdc
input  fg    0   hdc�y�� ->�g�n�y��
             1   �g�n�y��->hdc�y��      (hdc���y�ЬOh�V�U����)
             2   bmp�y�� ->�g�n�y��     (bmp���y�ЬOy�V�W����)
             3   �g�n�y��->bmp�y��
input  ix,iy
output *ox,*oy
========================================================================
*/
FUNC long __stdcall rgbset(long pid, long r,long g,long b,long w);
/*
�]�w�����C��μe��
pid    0~49  ����
r,g,b  0~255 �C��
w      pixel ���e
========================================================================
*/
FUNC long __stdcall layerdraw(long id,long layer_id, char fg[30]);
/*
�e��
id                  �ĴX��hdc,�i��0~49
layer_id  0~19      �e���w���ϼh
          999       GdiFlush
          1000~1049 �H0~49��M��hdc
------------------------------------------
fg[0]     '0'       dll���i�����selectobject
          '1'       ��layer
          '2'       ��feature
fg[1]     '0'       ���epolygon
          '1'       �n�epolygon
          '2'       ��polygon��polyline
fg[2]     '0'       ���epolyline
          '1'       �n�epolyline
------------------------------------------
fg[3]     '0'       point��icon��text�����e
          '1'       ��fg[4~6]���]�w,�åHpolyline���C��]SetTextColor
          '2'       ��fg[4~6]���]�w,�ä��]SetTextColor
fg[4]     '0'       icon_���e
          '1'       icon_��layer
          '2'       icon_��feature
fg[5]     '1'       icon_�Ptext�P�B�Ҷq���|���D
          '2'       icon_���|�h���e
          '3'       icon_�@�w�e(���Ҽ{���|)
fg[6]     '0'       text_���e
          '1'
          '2'       text_���|�h���e
          '3'       text_�@�w�e(���Ҽ{���|)
          '4'       text_�Yicon���e,�h�]���e
fg[7]     '1'       text����1�����(��1�_��)
          '2'~
-----------------------------------------------
fg[10]    '3'       �I��polyline�e
fg[11]    '2'       �и��W
fg[12]    '1'~      ���W����?�����(��1�_��)
fg[13]    '1'       ���W�ϥζ¦�
========================================================================
*/

FUNC long __stdcall layerfeature(long layer_id,long attid, char s[100], long feature);
/*
���ݩʶi��ϧΤ���
layer_id
attid    0~_�ĴX���ݩ�,��0�_��
         //-9_all
s        �����
feature  �]�w�� 0,1~99
========================================================================
*/

FUNC long __stdcall entposet(long layer_id,long recid,long x,long y,
							long feature, char nm[500],char fg[10]);
/*
�s�W�έק�@��point���
layer_id
recid    0~_�_��  �ק�ĴX�����
         -1_�s�W
x,y      �s����
feature  �s����
nm       �s����,�p"xxxx,vvvvv,kkkkkk"     -> �ЯS�O���L�o�����Ӧ���','
fg[0]   '9'_delete      (�Yrecid=-1,�hfg�L��)
fg[1]   '1'_�ק�xy
fg[2]   '1'_�ק�feature
fg[3]   '1'_�ק��ݩ�(nm)
========================================================================
*/

FUNC long __stdcall entiis(long layer_id,long recid,long x,long y,long r,long feature,
						long maxo,long maxnm,struct s_outo *o,char *nm,long *n);
/*
�d��
input  layer_id
input  recid    0~_�w����recid(�H0�_��),�d�@�����
                -1_�H�y�Ьd�@��
                -2_�H�y�Ьd�h��
input  x,y      �y��
inout  r        �d�ߥb�|
input  feature  �q�`��0
input  maxo     �̦h�d�X��
input  maxnm    �̤j���ݩ�byte��(�̤֭n500)
output *o       (struct s_outo[])
output *nm      �ݩ�
output *n       �d��X��
return -1 ����,�ΨS���
        1 ���\
        2 ���\,����ڵ��ƤӦh,�Х[�j�O���^���Y�p�d�߽d��
========================================================================
*/

FUNC long __stdcall iconinitfree(long fg, char fnm[100]);
/*
fg    1  emf��icon��init
      2  bmp��icon��init
     -1  emf��icon��free
     -2  bmp��icon��free
fnm      �ԨҤl

�p1: iconinitfree(1, "d:\\1yu\\pda2\\data"); //�{���}�l��
     ���ݦs�b"d:\1yu\pda2\data\icon.def"����r��,�ܷN�p�U:
     ---------------------
     31  50  20  31.emf
     32  20  30  32.emf
     ---------------------
     id  �e  ��  �ɦW(�ɮפ]�O��bd:\1yu\pda2\data\)

     icon�s��0~30�Odll�t�Τ���
     �Y�n�Χ�h��icon(31�H��)�h���ݥ~�[emf��bmp�榡��icon

�p2: iconinitfree(2, "d:\\1yu\\pda2\\data\\icon16.bmp")
     �Y�O��bmp�榡��,�h�Ҧ���icon,���ݥ��������e�bd:\1yu\pda2\data\icon16.bmp

�p3: iconinitfree(-1,""); //�{�������ɭncall
     iconinitfree(-2,"");
=========================================================================
*/
//////////////////////////////////////////////////////////////////////////////////////





/*	//Old A.dll Display and PathFinding Function
////////////  ���|�j�M  /////////////
#pragma pack(1)
struct CROSS_INFO
{
    int       iMeshId;
    WORD      wNX;
    WORD      wNY;
    char      cCrossInfo[100];
    BYTE      bType; //0:start,1:end,2~6:wayPt,255:noSpecial
};
#pragma pack()

DLL_IMP bool __stdcall DllPathSearch( POINT startP, POINT endP, POINT way[5], int searchType, POINT* linkPoint, int& linkPointCnt, CROSS_INFO* crossInfo, int& crossInfoCnt, int& sumLength );
// Input
// startP : �_�I
// endP   : ���I
// way[5] : �� 1,2,3,4,5 �Ӹg�Ѧa
// searchType : 1:���t����(�ֳt�D��)�u��  2:�樮�Z���̵u  3:�樮�ɶ��̵u  4:�K�O�D���u��
//
// Output
// linkPoint :   Link�I Array ����
// linkPointCnt: Link�I�ƥ�
// crossInfo   : ��e�I��T Structure ����
// crossInfoCnt: ��e�I��T�ƥ�
// sumLength   : ���|�`��

//////////// �a����� ///////////////
DLL_IMP	long __stdcall dllsetpath(char s[200]);
//�]�w�a�ϸ�ƥؿ�
//�p setpath("f:\\dat\\");
//�Ъ`�N�r��̫�n�["\\"

DLL_IMP	long __stdcall dllfree();
//Free Resource �{�������ɨϥ�

DLL_IMP	long __stdcall dllsetcolor(long pi, long r, long g, long b, long ww);
//�]�w�e���C��
//ww: ���e��
//pi: ���� 0���� 1~9��֬ٿ�..�D 10��ܸ� 11,12�e 13���� 14�m�� 15�K��


// 0831 Release
//DLL_IMP	long __stdcall dlldrawback(long kdc, long ww, long hh);
//�e����
//DLL_IMP	long  __stdcall dlldraw(long kdc, long xb, long yb, long ww, long hh,double cs, long bgfg,long nmfg);
//DLL_IMP	long __stdcall dlldraw(long kdc,  long xb, long yb, long ww, long hh,double cs,long id44, char fg[10]);
//id44->-1 ���e�]�I(blue) , 100~ 330  �e�X�����]�I(������id44.txt/rtc�]�I�X�P���t�νX���)
//fg[0] '1' �n�e�a�W/���W(black) ,�D'1'�h���e , fg[1..9]  �ثe���ϥ�
//dlldraw -> ���e����(�Y���M��)
//�e�X�a��
//kdc:  HDC
//xb:   ���W����x�y��,�p301234
//yb:   ���W����y�y��,�p2771234
//ww:   HDC��width(pixel)
//hh:   HDC��height(pixel)
//cs:   cs:   Screen�@��pixel�N��??����,�d��1.0~50.0 (m/pixel) �אּ1.0~ �Y�L�W��
///////  0807 �ª��Ѽ� ////////
//bgfg: 1��F�ɻP�K���n�e 0���e
//nmfg: 1�a�W���W�n�e     0���e


//// 0907 Release
DLL_IMP	long __stdcall dlldraw(long kdc,  long xb, long yb, long ww, long hh,double cs,char fg[10]);
//id44����(�Х�dlldraw44)
//fg[0]  '0'���e�I�� '1'�n�e�I��(�Ydlldrawback)
//fg[1]  '0'���e��F��polygon  '1'�n�e                          (cs<=50)
//fg[2]  '0'���e��F��polyline  '1'�n�e������  '2'�n�e�m���    (cs<=50)
//fg[3]  '0'���e�a�W '1'�n�e                                    (cs<=50)
//fg[4]  '0'���e�]�I����  '1'�n�e
//�e�X�a��
//kdc:  HDC
//xb:   ���W����x�y��,�p301234
//yb:   ���W����y�y��,�p2771234
//ww:   HDC��width(pixel)
//hh:   HDC��height(pixel)
//cs:   cs:   Screen�@��pixel�N��??����,�d��1.0~50.0 (m/pixel) �אּ1.0~ �Y�L�W��

//DLL_IMP	long __stdcall dlldraw44(long id44);
DLL_IMP	long __stdcall dlldraw44(long id44, long doublefg);
//�e���w����(id44)���]�I
//�ѩ�S��hdc,xb,yb,ww,hh,cs���Ѽ�,�G���ݺ�dlldraw��ϥ�
//id44�Y��0,�h���������������]�I
//doublefg   	1  ���Ҽ{"���|"���D
//				2  �|�Ҽ{"���|"���D  (cs<=5)
//          	3  �|�Ҽ{"���|"���D  (cs<=5)  , �����Ҽ{�P���W�����|



DLL_IMP	long __stdcall dllquroad(long xi, long yi, double cs, char rnm[50]);

//�H(xi,yi)�d�߸�����F�ϽX�θ��W
//cs: �Pdlldraw
//�Ǧ^�����G�s��rnm
//rnm[0..2]:  ��F�ϽX
//rnm[3]:     1~9�D������,�Ѩ�pi
//rnm[4..7]:  �D���s��(�Prtc)
//rnm[8..27]: ���W

DLL_IMP	long __stdcall dlldistsearch(char strName[50]);
//����: ��J"����"or"�֧�", �n�D�i��]�I�j�M
//output: c:\dist44.tmp

DLL_IMP	long __stdcall dlldistxy(long xi, long yi, long r, long id44);
//���w�@�I(xi,yi)�Υb�|(r), �n�D�C�X�]�I(��id44��)���M���xy
//�]�I���O��3�X,��id44.txt
//output  c:\dist44.tmp

DLL_IMP	long __stdcall dlldist44(long distid,long id44);
//����F�ϽX,�]�I�X,�n�D�C�X�]�I�M��
//output  c:\dist44.tmp


DLL_IMP	long __stdcall dlldistroad(long distid);
//�d���M��
//input: ��F�ϽX(distid)
//output:�ӰϩҦ����W�M���xy c:\distroad.tmp (txt��)

DLL_IMP	long __stdcall dlldistcrrd(long distid, char strRoadName[50]);
//�d��e��
//input: ��F�ϽX�ιD���W��
//output:�P�Ӹ�����e�������M���xy c:\distcrrd.tmp
//return: -1=Not Found , ��l�� index => index+1 = Total counts
*/

#endif	//__DISPLAY_DLL_H__