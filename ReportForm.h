//---------------------------------------------------------------------------

#ifndef ReportFormH
#define ReportFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
#include <QuickRpt.hpp>
#include <Qrctrls.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <QRPrntr.hpp>
#include <Graphics.hpp>
#include <Chart.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
//---------------------------------------------------------------------------
class TForm_Report : public TForm
{
__published:	// IDE-managed Components
	TQuickRep *QuickRep_Map;
	TQRBand *QRBand6;
	TQRImage *QRImage_Map;
        TQRBand *PageFooterBand1;
        TQRBand *PageHeaderBand1;

private:	// User declarations
public:		// User declarations
	__fastcall TForm_Report(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_Report *Form_Report;
//---------------------------------------------------------------------------
#endif
