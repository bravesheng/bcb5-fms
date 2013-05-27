inherited Form_Trip: TForm_Trip
  Left = 294
  Top = 247
  Width = 498
  Height = 344
  Caption = 'TRIP路徑'
  OldCreateOrder = True
  PixelsPerInch = 96
  TextHeight = 13
  inherited Bevel1: TBevel
    Width = 490
  end
  inherited TraceListView: TListView
    Width = 490
    Height = 238
    Columns = <
      item
        Caption = '開始日期'
        Width = 70
      end
      item
        Caption = '開始時間'
        Width = 60
      end
      item
        Caption = '結束日期'
        Width = 70
      end
      item
        Caption = '結束時間'
        Width = 60
      end
      item
        Caption = '起點位置'
        Width = 110
      end
      item
        Caption = '終點位置'
        Width = 110
      end
      item
        Caption = 'Period'
      end
      item
        Caption = '總里程數'
        Width = 60
      end>
  end
  inherited StatusBar1: TStatusBar
    Top = 298
    Width = 490
  end
end
