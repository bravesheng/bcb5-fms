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
增加layersave及layeropen其format為6
->(.roy檔 binary檔)
==0717====================================================================
增進layerdraw速度
==0705====================================================================
增加server.c給asp用
==0621====================================================================
改ent.c的checkb2(),給mifmidopen使用, 解決字串中的逗號,問題
==0530====================================================================
增加標路名,詳layerdraw
==0530====================================================================
iis的方法
1.以小範圍查(如mouse_move) -> entiis
2.以大範圍查(如??公里)     -> entiis

3.以某個屬性值查(如行政區) -> entiis
  3.1 先layerfeature(兩次)
  3.2 再entiis

 layerfeature( poi_layer_id, -9, "",   1)

 layerfeature( poi_layer_id,  1, "7",  2)
 layerfeature( poi_layer_id,  1, "8",  2)

 entiis      ( poi_layer_id, -2, 0,0,360000000, 2



4.以字串查                 -> entstriis

==0530====================================================================
*/
FUNC long __stdcall entstriis(long layer_id,char cnm[40],long maxo,long maxnm,struct s_outo *o,char *nm,long *n);
FUNC long __stdcall entstriis2(long layer_id,char cnm[40],long maxo,long maxnm,struct s_outo *o,char *nm,long *n);
/*
以字串查詢
cnm[40]      比對之字串
其餘同entiis
*/
//==old=====================================================================
FUNC long __stdcall maplibset(long fg, char fnm[100]);
/*
fg  1  init
   -1  free
fnm    即"d:\\1yu\\pda2\\maplib.txt"
==========================================================================
*/
FUNC long __stdcall maplibdraw(long id);
/*
id     第幾號hdc,可由0~49
==========================================================================
*/
FUNC long __stdcall hdcsave_restore(long fg,long id);
/*
fg  1    會create_mem_dc
   -1    會delete_mem_dc
    2    會將現在dc的內容copy一份到mem_dc
    3    會將mem_dc的內容還原到現在的dc
id  0~49 現在的hdc編號
==========================================================================
*/
FUNC long __stdcall layeropen(long layer_id,long format, char fnm[100],
								 long ppl, long ppn,long  psb);
/*
開檔
layer_id  0~19的任意值,即可同時載入20個(層)地圖資料
format    4_fnm為dxf檔
          5_fnm為mid/mif檔的mif檔
          6_為roy檔(binary)
          8_fnm為nmea的文字檔
         10_fnm為bmp或rle檔
         20_開新檔,fnm為屬性欄位名稱
         -1_只設定ppl,ppn,psb
fnm
ppl      polyline用的筆號 (-1, 0~49)
pgn      polygon用的筆號  (-1, 0~49)
psb      icon用的編號   (0~30內建, 31~99外掛)
return   -1 失敗
          1 該資料為向量,且為經緯度     (.cpt檔不存在)
          2 該資料為向量,但為bmp座標系統(.cpt檔存在)
          3 該資料為向量                (.cpt檔存在)   ->座標系統unknow
         11 該資料為bmp,且為正北朝上    (.cpt檔存在)
         12 該資料為bmp,且非正北朝上    (.cpt檔存在)
         13 該資料為bmp                 (.cpt檔不存在) ->座標系統unknow

如1: layeropen(0, 5,"c:\\a\\bc.mif",0,0,0);  //...bc.mid檔也一定要exist
如2: layeropen(1,10,"c:\\a\\de.bmp",0,0,0);
        若為bmp檔,通常應有一同主檔名,附檔名為.cpt,的檔存在 (如c:\\a\\de.cpt)
        .cpt檔為控制點資料Ascii檔,有三行的文字,示意為
        -----------
        2
        x1 y1 X1 Y1
        x2 y2 X2 Y2
        -----------
        xy為控制點在bmp中的座標,並以bmp之左下方為(0,0),y向上為正
        XY為控制點的經緯度座標,如(121.123456 25.654321)
如3: 新開一備註地點圖層
     layeropen(2,20,"name,address,tel",0,0,0);
==================================================================
*/
FUNC long __stdcall layerfree(long layer_id);
/*
程式結束時要call
==================================================================
*/

FUNC long __stdcall defgpslayer(long layer_id,long format,long sec,long spots,
								long dhop,long dist,long gen,long datefg);
/*
宣告要開始存錄gps資料
layer_id 0~19_要存到那一層
           -1_只設定參數
format   2_存成polyline
         3_存成point
sec      ??秒才要(存一筆)
spots    ??個衛星才要
hdop     ??才要
dist     ??公尺才要
gen      ??公尺才要
datefg   1_使用gps時間
         2_使用Local時間
return   1_成功 -1_失敗
=================================================================
*/
FUNC long __stdcall put1gga(char memo[20],char ggarmc[150], long *ox,long *oy);
/*
送一句nmea的句子給dll
memo     備註
ggarmc   一句nmea的句子
*ox,*oy  傳回的經緯座標
=================================================================
*/

FUNC long __stdcall layersave(long layer_id,long format, char fnm[100]);
/*
存檔
layer_id
format   4_要save成dxf檔
         5_要save成mifmid
         6_為roy檔(binary)
fnm      要save的路徑及檔名
=================================================================
*/
FUNC long __stdcall layerinfo(long layer_id,long fg,long o[20], char *nm);
/*
詢問layer的資訊
input  layer_id
input  fg
output o[0] 同layeropen的return
       o[1] 該layer有幾筆圖元素
       o[2] 第一筆圖元是 1_polygon 2_polyline 3_point
       o[3] 屬性欄位數
       o[4] 佔mem數
       o[5] 屬性mem數
       o[10,11] 向量資料的最小xy  (以下資料要fg==10才有)
       o[12,13] 向量資料的最大xy
       o[12,13] bmp的width,height
       o[14,15] bmp(0,0)的經緯度
       o[16,17] bmp(w,h)的經緯度
output nm   屬性欄名,其大小至少要250個byte
            傳出資料如"name\0address\0tel\0"
=================================================================
*/

FUNC long __stdcall hdcset(long id,long hdc,long hdcw,long hdch,
							long coord,long xc,long yc,long mp);
/*
傳進hdc及設定其對映的地理座標等資訊(並不畫圖)
id     第幾號hdc,可由0~49
hdc
hdcw   hdc的width
hdch   hdc的height
coord  1,        表示座標系統要用經緯度,且放大1000000倍(為long)
       1000~1019,表示座標系統要用bmp座標系統(詳例子)
       -3,       表示座標系統要用unknow
xc,yc  hdc中心的座標
mp     每個pixel是....(詳例子)
return 1_成功 -1_失敗

如1: hdcset(picbox.hdc, picbox.scalewidth,picbox.scaleheight,
            1,
            121123456,25654321, //hdc中心之經緯座標是 -> (121.123456,25.654321)
            1234);              //hdc每個pixel之大小為-> 0.001234度
如2: layeropen(4,10,"c:\\g.bmp");
     hdcset(picbox.hdc, picbox.scalewidth,picbox.scaleheight,
            1004, //使用 4 -> g.bmp -> 這bmp座標系統
            0,0,  //hdc中心,恰是bmp的左下角
            10);  //10 -> 1:1顯示bmp  (即mp是要*10,但xc,yc則否)
                  // 5 -> 放大2倍顯示bmp
                  //20 -> 縮小1/2顯示bmp
========================================================================
*/
FUNC long __stdcall xycalc(long id,long fg,long ix,long iy,long *ox,long *oy);
/*
座標轉換
input  id   0~49 第幾號hdc
input  fg    0   hdc座標 ->經緯座標
             1   經緯座標->hdc座標      (hdc的座標是h向下為正)
             2   bmp座標 ->經緯座標     (bmp的座標是y向上為正)
             3   經緯座標->bmp座標
input  ix,iy
output *ox,*oy
========================================================================
*/
FUNC long __stdcall rgbset(long pid, long r,long g,long b,long w);
/*
設定筆之顏色及寬度
pid    0~49  筆號
r,g,b  0~255 顏色
w      pixel 筆寬
========================================================================
*/
FUNC long __stdcall layerdraw(long id,long layer_id, char fg[30]);
/*
畫圖
id                  第幾號hdc,可由0~49
layer_id  0~19      畫指定的圖層
          999       GdiFlush
          1000~1049 以0~49色清除hdc
------------------------------------------
fg[0]     '0'       dll不進行任何selectobject
          '1'       依layer
          '2'       依feature
fg[1]     '0'       不畫polygon
          '1'       要畫polygon
          '2'       把polygon當成polyline
fg[2]     '0'       不畫polyline
          '1'       要畫polyline
------------------------------------------
fg[3]     '0'       point之icon及text均不畫
          '1'       依fg[4~6]之設定,並以polyline的顏色設SetTextColor
          '2'       依fg[4~6]之設定,並不設SetTextColor
fg[4]     '0'       icon_不畫
          '1'       icon_依layer
          '2'       icon_依feature
fg[5]     '1'       icon_與text同步考量重疊問題
          '2'       icon_重疊則不畫
          '3'       icon_一定畫(不考慮重疊)
fg[6]     '0'       text_不畫
          '1'
          '2'       text_重疊則不畫
          '3'       text_一定畫(不考慮重疊)
          '4'       text_若icon不畫,則也不畫
fg[7]     '1'       text為第1個欄位(由1起算)
          '2'~
-----------------------------------------------
fg[10]    '3'       點當成polyline畫
fg[11]    '2'       標路名
fg[12]    '1'~      路名為第?個欄位(由1起算)
fg[13]    '1'       路名使用黑色
========================================================================
*/

FUNC long __stdcall layerfeature(long layer_id,long attid, char s[100], long feature);
/*
依屬性進行圖形分類
layer_id
attid    0~_第幾個屬性,由0起算
         //-9_all
s        比較值
feature  設定值 0,1~99
========================================================================
*/

FUNC long __stdcall entposet(long layer_id,long recid,long x,long y,
							long feature, char nm[500],char fg[10]);
/*
新增或修改一筆point資料
layer_id
recid    0~_起算  修改第幾筆資料
         -1_新增
x,y      新的值
feature  新的值
nm       新的值,如"xxxx,vvvvv,kkkkkk"     -> 請特別先過濾掉不該有的','
fg[0]   '9'_delete      (若recid=-1,則fg無用)
fg[1]   '1'_修改xy
fg[2]   '1'_修改feature
fg[3]   '1'_修改屬性(nm)
========================================================================
*/

FUNC long __stdcall entiis(long layer_id,long recid,long x,long y,long r,long feature,
						long maxo,long maxnm,struct s_outo *o,char *nm,long *n);
/*
查詢
input  layer_id
input  recid    0~_已知其recid(以0起算),查一筆資料
                -1_以座標查一筆
                -2_以座標查多筆
input  x,y      座標
inout  r        查詢半徑
input  feature  通常給0
input  maxo     最多查幾筆
input  maxnm    最大之屬性byte數(最少要500)
output *o       (struct s_outo[])
output *nm      屬性
output *n       查到幾筆
return -1 失敗,或沒找到
        1 成功
        2 成功,但實際筆數太多,請加大記憶体或縮小查詢範圍
========================================================================
*/

FUNC long __stdcall iconinitfree(long fg, char fnm[100]);
/*
fg    1  emf的icon的init
      2  bmp的icon的init
     -1  emf的icon的free
     -2  bmp的icon的free
fnm      詳例子

如1: iconinitfree(1, "d:\\1yu\\pda2\\data"); //程式開始時
     必需存在"d:\1yu\pda2\data\icon.def"此文字檔,示意如下:
     ---------------------
     31  50  20  31.emf
     32  20  30  32.emf
     ---------------------
     id  寬  高  檔名(檔案也是放在d:\1yu\pda2\data\)

     icon編號0~30是dll系統內建
     若要用更多的icon(31以後)則必需外加emf或bmp格式的icon

如2: iconinitfree(2, "d:\\1yu\\pda2\\data\\icon16.bmp")
     若是用bmp格式的,則所有的icon,必需全部集中畫在d:\1yu\pda2\data\icon16.bmp

如3: iconinitfree(-1,""); //程式結束時要call
     iconinitfree(-2,"");
=========================================================================
*/
//////////////////////////////////////////////////////////////////////////////////////





/*	//Old A.dll Display and PathFinding Function
////////////  路徑搜尋  /////////////
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
// startP : 起點
// endP   : 終點
// way[5] : 第 1,2,3,4,5 個經由地
// searchType : 1:高速公路(快速道路)優先  2:行車距離最短  3:行車時間最短  4:免費道路優先
//
// Output
// linkPoint :   Link點 Array 指標
// linkPointCnt: Link點數目
// crossInfo   : 交叉點資訊 Structure 指標
// crossInfoCnt: 交叉點資訊數目
// sumLength   : 路徑總長

//////////// 地圖顯示 ///////////////
DLL_IMP	long __stdcall dllsetpath(char s[200]);
//設定地圖資料目錄
//如 setpath("f:\\dat\\");
//請注意字串最後要加"\\"

DLL_IMP	long __stdcall dllfree();
//Free Resource 程式關閉時使用

DLL_IMP	long __stdcall dllsetcolor(long pi, long r, long g, long b, long ww);
//設定畫圖顏色
//ww: 筆寬度
//pi: 筆號 0底色 1~9國快省縣..道 10選擇路 11,12河 13縣界 14鄉界 15鐵路


// 0831 Release
//DLL_IMP	long __stdcall dlldrawback(long kdc, long ww, long hh);
//畫底色
//DLL_IMP	long  __stdcall dlldraw(long kdc, long xb, long yb, long ww, long hh,double cs, long bgfg,long nmfg);
//DLL_IMP	long __stdcall dlldraw(long kdc,  long xb, long yb, long ww, long hh,double cs,long id44, char fg[10]);
//id44->-1 不畫設施(blue) , 100~ 330  畫出該類設施(分類詳id44.txt/rtc設施碼與本系統碼對照)
//fg[0] '1' 要畫地名/路名(black) ,非'1'則不畫 , fg[1..9]  目前未使用
//dlldraw -> 不畫底色(即不清除)
//畫出地圖
//kdc:  HDC
//xb:   左上角的x座標,如301234
//yb:   左上角的y座標,如2771234
//ww:   HDC的width(pixel)
//hh:   HDC的height(pixel)
//cs:   cs:   Screen一個pixel代表??公尺,範圍1.0~50.0 (m/pixel) 改為1.0~ 即無上限
///////  0807 舊版參數 ////////
//bgfg: 1行政界與鐵路要畫 0不畫
//nmfg: 1地名路名要畫     0不畫


//// 0907 Release
DLL_IMP	long __stdcall dlldraw(long kdc,  long xb, long yb, long ww, long hh,double cs,char fg[10]);
//id44取消(請用dlldraw44)
//fg[0]  '0'不畫背景 '1'要畫背景(即dlldrawback)
//fg[1]  '0'不畫行政區polygon  '1'要畫                          (cs<=50)
//fg[2]  '0'不畫行政界polyline  '1'要畫縣市界  '2'要畫鄉鎮界    (cs<=50)
//fg[3]  '0'不畫地名 '1'要畫                                    (cs<=50)
//fg[4]  '0'不畫設施型狀  '1'要畫
//畫出地圖
//kdc:  HDC
//xb:   左上角的x座標,如301234
//yb:   左上角的y座標,如2771234
//ww:   HDC的width(pixel)
//hh:   HDC的height(pixel)
//cs:   cs:   Screen一個pixel代表??公尺,範圍1.0~50.0 (m/pixel) 改為1.0~ 即無上限

//DLL_IMP	long __stdcall dlldraw44(long id44);
DLL_IMP	long __stdcall dlldraw44(long id44, long doublefg);
//畫指定分類(id44)的設施
//由於沒傳hdc,xb,yb,ww,hh,cs等參數,故必需緊接dlldraw後使用
//id44若為0,則指為全部分類的設施
//doublefg   	1  不考慮"重疊"問題
//				2  會考慮"重疊"問題  (cs<=5)
//          	3  會考慮"重疊"問題  (cs<=5)  , 但不考慮與路名的重疊



DLL_IMP	long __stdcall dllquroad(long xi, long yi, double cs, char rnm[50]);

//以(xi,yi)查詢路的行政區碼及路名
//cs: 同dlldraw
//傳回的結果存於rnm
//rnm[0..2]:  行政區碼
//rnm[3]:     1~9道路等級,參見pi
//rnm[4..7]:  道路編號(同rtc)
//rnm[8..27]: 路名

DLL_IMP	long __stdcall dlldistsearch(char strName[50]);
//說明: 輸入"六福"or"福村", 要求進行設施搜尋
//output: c:\dist44.tmp

DLL_IMP	long __stdcall dlldistxy(long xi, long yi, long r, long id44);
//給定一點(xi,yi)及半徑(r), 要求列出設施(為id44類)的清單及xy
//設施類別為3碼,詳id44.txt
//output  c:\dist44.tmp

DLL_IMP	long __stdcall dlldist44(long distid,long id44);
//給行政區碼,設施碼,要求列出設施清單
//output  c:\dist44.tmp


DLL_IMP	long __stdcall dlldistroad(long distid);
//查路清單
//input: 行政區碼(distid)
//output:該區所有路名清單及xy c:\distroad.tmp (txt檔)

DLL_IMP	long __stdcall dlldistcrrd(long distid, char strRoadName[50]);
//查交叉路
//input: 行政區碼及道路名稱
//output:與該路有交叉的路的清單及xy c:\distcrrd.tmp
//return: -1=Not Found , 其餘為 index => index+1 = Total counts
*/

#endif	//__DISPLAY_DLL_H__