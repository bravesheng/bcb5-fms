//---------------------------------------------------------------------------

#ifndef SelectPathH
#define SelectPathH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#define DEF_PATH_WIDTH 100      //�w�]�����w���u�ʱ����u�b�|
//---------------------------------------------------------------------------
struct sSelectPoint		//���w���u���I
{
   long mX2;    //���w���u���I - X �y��(WGS)
   long mY2;    //���w���u���I - Y �y��(WGS)
};
//���w���u
class TCSelectPath : public TDataModule
{
__published:	// IDE-managed Components
        TQuery *Query_Add;
        TQuery *Query1;
        TQuery *Query_Delete;
        void __fastcall DataModuleDestroy(TObject *Sender);
        void __fastcall DataModuleCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TCSelectPath(TComponent* Owner);
        int RouteNo;            //�����u���ѧO�n���X,�ߤ@���|����
        AnsiString CoID;        //�����u����Company ID
        AnsiString PathName;          //���u�W�ٵ��O
        int Radius;  		//���w���u���ʱ��d��(�ʱ��b�|)
        TList* m_pSelPointList;         //for List of  sSelectPoint
        AnsiString StartPoint;
        AnsiString EndPoint;
        bool Vertify;   //�аO�O�_���ҹL
        void __fastcall AddPoint(long mX2,long mY2);	        //�[�JWGS�y���I
        void __fastcall ClearPoint();
        void __fastcall AddToDB(TObject *Sender);               //�N�u���[�J��ƮwtCusAutRoute
        void __fastcall DeleteFromDB(TObject *Sender);          //�N�u���q��Ʈw�R��
        void __fastcall QuestData(int RouteNo);        //Ū��tAutRoute��@Route�����,�æ^��TCSelectPath


};
//---------------------------------------------------------------------------
extern PACKAGE TCSelectPath *CSelectPath;
//---------------------------------------------------------------------------
#endif
