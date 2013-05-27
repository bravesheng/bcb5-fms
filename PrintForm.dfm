object Form_Print: TForm_Print
  Left = 182
  Top = 233
  Width = 591
  Height = 396
  Caption = '圖形列印'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Image1: TImage
    Left = 0
    Top = 0
    Width = 583
    Height = 350
    Align = alClient
  end
  object MainMenu1: TMainMenu
    Left = 240
    Top = 88
    object N3: TMenuItem
      Caption = '樣式'
      object N4: TMenuItem
        Caption = '全部列印'
      end
      object N5: TMenuItem
        Caption = '列印地圖與車輛'
      end
      object N6: TMenuItem
        Caption = '列印地圖與備註地點'
      end
      object N7: TMenuItem
        Caption = '只列印地圖'
      end
    end
    object N2: TMenuItem
      Caption = '設定印表機'
    end
    object N1: TMenuItem
      Caption = '列印'
    end
  end
  object PrinterSetupDialog1: TPrinterSetupDialog
    Left = 472
    Top = 16
  end
end
