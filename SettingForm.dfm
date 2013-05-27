object Form_Setting: TForm_Setting
  Left = 285
  Top = 216
  Width = 359
  Height = 361
  Caption = '設定'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poMainFormCenter
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 351
    Height = 289
    ActivePage = TabMap
    Align = alTop
    TabOrder = 0
    object TabMap: TTabSheet
      Caption = '圖層'
      object GroupBox1: TGroupBox
        Left = 0
        Top = 0
        Width = 343
        Height = 261
        Align = alClient
        Caption = '圖層設定'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        object LayerBox: TCheckListBox
          Left = 12
          Top = 48
          Width = 213
          Height = 201
          OnClickCheck = LayerBoxClickCheck
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 16
          Items.Strings = (
            '文字重疊排除'
            '1 行政區名稱標示'
            '2 公園'
            '3 建築物'
            '4 道路名'
            '5 街道輪廓'
            '6 鐵路地鐵'
            '7 地標'
            '8 各項設施')
          ParentFont = False
          TabOrder = 0
        end
        object ComboBox_Layer: TComboBox
          Left = 12
          Top = 24
          Width = 169
          Height = 24
          ItemHeight = 16
          TabOrder = 1
          Text = '道路名稱顯示模式'
          OnChange = ComboBox_LayerChange
          Items.Strings = (
            '道路名稱顯示模式'
            '設施名稱顯示模式'
            '使用者自訂模式')
        end
        object Button1: TButton
          Left = 232
          Top = 224
          Width = 89
          Height = 25
          Caption = '地理資訊查詢'
          TabOrder = 2
          OnClick = Button1Click
        end
      end
    end
    object TabPath: TTabSheet
      Caption = '其他'
      ImageIndex = 1
      object GroupBox_MonitorArea: TGroupBox
        Left = 0
        Top = 0
        Width = 343
        Height = 137
        Align = alTop
        Caption = '監控路線參數設定'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        object Label_LockedAreaPixels: TLabel
          Left = 6
          Top = 18
          Width = 139
          Height = 16
          AutoSize = False
          Caption = '設定監控模式範圍：'
        end
        object Label8: TLabel
          Left = 6
          Top = 42
          Width = 139
          Height = 16
          AutoSize = False
          Caption = '指定路線監控半徑：'
        end
        object Label10: TLabel
          Left = 175
          Top = 18
          Width = 34
          Height = 23
          AutoSize = False
          Caption = 'pixel'
        end
        object Label9: TLabel
          Left = 183
          Top = 42
          Width = 28
          Height = 23
          AutoSize = False
          Caption = '公尺'
        end
        object Label13: TLabel
          Left = 6
          Top = 107
          Width = 131
          Height = 16
          AutoSize = False
          Caption = '顯示指定監控路線：'
        end
        object Label16: TLabel
          Left = 6
          Top = 72
          Width = 139
          Height = 16
          AutoSize = False
          Caption = '最大即時軌跡數目：'
        end
        object Label17: TLabel
          Left = 183
          Top = 72
          Width = 28
          Height = 25
          AutoSize = False
          Caption = '點'
        end
        object MaskEdit_LockedAreaPixels: TMaskEdit
          Left = 119
          Top = 14
          Width = 40
          Height = 24
          Hint = '監控範圍為以螢幕為中心向外延伸的螢幕點數'
          BiDiMode = bdLeftToRight
          EditMask = '099;1;_'
          MaxLength = 3
          ParentBiDiMode = False
          TabOrder = 0
          Text = '400'
          OnChange = MaskEdit_LockedAreaPixelsChange
        end
        object UpDown_LockedAreaPixels: TUpDown
          Left = 159
          Top = 14
          Width = 15
          Height = 24
          Associate = MaskEdit_LockedAreaPixels
          Min = 1
          Max = 999
          Position = 400
          TabOrder = 1
          Wrap = False
          OnChanging = UpDown_LockedAreaPixelsChanging
        end
        object BitBtn_ShowAllSelPath: TBitBtn
          Left = 119
          Top = 101
          Width = 106
          Height = 25
          Caption = '顯示路線'
          TabOrder = 2
          OnClick = BitBtn_ShowAllSelPathClick
          Glyph.Data = {
            0E060000424D0E06000000000000360000002800000016000000160000000100
            180000000000D8050000C40E0000C40E00000000000000000000FFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF5F5F59A9A9A8B8B8B8B8B
            8B8B8B8B9A9A9AA6A6A6A9A9A9A9A9A9AAAAAAE2E2E2FFFFFFFFFFFF0000FFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7E7E7E1D19155D30035A
            2D00452D00045528403003160D052B2B2B424242424242575757FFFFFFFFFFFF
            0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBBBBBB3523114B2600633100
            6331007831004947160176311657264E340352280033200F3C3C3CD7D7D7FFFF
            FFFFFFFF0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC6C6C647290D6331006331
            006731009231009C31006B471604965600923107642A4531006331004B2908B3
            B3B3FFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFFFFFFCDCDCD2E170063310066
            31008D31009D3100AA31009C31003E552303985F009C55009C3F00903F194C1A
            9C3100531A00C8C8C8FFFFFF0000FFFFFFFFFFFFFFFFFFFFFFFF353535633100
            6331008F31009C3100C13100CE31004E552407A35B00C06300C06300C063009C
            63009159634E1DA931001F1F1FFFFFFF0000FFFFFFFFFFFFFFFFFFACACAC552D
            056631008F31009E3100BE3100CE3100D33100009C5A04D2682DFB9722F08B09
            D76D00CB6306BD5C645221CE3100982E029797970000FFFFFFFFFFFFFFFFFF5F
            53485C2E007231009C3100BF3100CE3100ED3400F751001F783116E48B31FF9F
            31FFA717E5842C9652AF6017ED4A00D93100C42F006B4C3D0000FFFFFFFFFFFF
            FFFFFF3624136331009C31009F3100CF3100EF3400FD630083681F17C45A44F6
            A644F6C014B35EB07039FF6810FF6431FF6329FF4602CE31006D260F0000FFFF
            FFFFFFFFFFFFFF2311006331009C3100C03100D63600B75A116B841F56983BFF
            8236FF9C4CFF9C4CF09D4CFF9C4CFF9C31FF8931FF6331FF6322DC36022A300C
            0000424242979797353535212121212121212121212121D03A0800873100D268
            8BC97CFF9C6300CE63E7CE63FFCE63FFCE63FFC063FF9C3EFF9131FF6331E76C
            08005D2D00000000001010102C43582D475E2D475E2D475E2D475E1D3E2A00D2
            69FF6604FF904E8B9C4CFFCE6EFFCE99FFCE9CFFCE82FFCE63FFA963FF9631FF
            6C11009E4A085B38000000000028405590C4F497C9FB97C9FB97C9FB2F41510A
            0D0C32BF7C3FC055179A3100F692FFB563FFCE9CFFEAB8FFCE9CFFCE63FFCB63
            FF9231E2651100E88D113F280000000000465D739CCEFF9CCEFF9CCEFF9CCEFF
            7397BB516B841FA26331FFCE5AEBAF31FF9C53B046F6D58BFFF8C7FFD3A2FFCE
            63FFC163F07D2E80731F01C77C9090900000000000465D739CCEFF9CCEFF9CCE
            FF9CCEFF87B3DD6889AA273F343F645B454C3D3F645288CA632DA338FFB959FF
            CE6DFFCE63FFA945B0692F237028132D24EDEDED0000000000465D739CCEFF9C
            CEFF9CCEFF9CCEFF3E576E1B2B381B2B381B2B381B2B381B2B381A774600CE63
            889C31FFB959FFB655FF913ADA63262952286C6C6CFFFFFF0000000000334C63
            73AADB9CCEFF9CCEFF9CCEFF91C4F58CC0F28CC0F28CC0F28CC0F28CC0F2005D
            2CA8CE63FFD795FFDDA7FF7331E54C172E771F76807AFFFFFFFFFFFF00002E2E
            2E2E2E2E2E2E2E618EB68EC0EF799FC6080A0D0A0A0A000B06001C0A05170D09
            1209FFD28BFFD7A0FFFCCBFFF6C597722E04A14A545454FFFFFFFFFFFFFFFFFF
            0000FFFFFFFFFFFFFFFFFFA2A2A23A4B5A81B6E87397BB1B1B1B1D3328003E16
            62915EB4C695FFF5C4FFFFCEFFD9A77A624B3B6448ABABABFFFFFFFFFFFFFFFF
            FFFFFFFF0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF171717476D8E7FA8D0A2A2
            A2ECECEC353535171717171717171717353535DADADAFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFAAAAAA53
            6578D5D5D5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0000}
        end
        object Edit_PathArea: TEdit
          Left = 120
          Top = 40
          Width = 57
          Height = 24
          MaxLength = 3
          TabOrder = 3
          Text = '100'
          OnExit = Edit_PathAreaExit
          OnKeyDown = Edit_PathAreaKeyDown
        end
        object Edit_MaxTrace: TEdit
          Left = 120
          Top = 69
          Width = 57
          Height = 24
          MaxLength = 3
          TabOrder = 4
          Text = '200'
          OnExit = Edit_MaxTraceExit
          OnKeyDown = Edit_MaxTraceKeyDown
        end
      end
    end
  end
  object BitBtn1: TBitBtn
    Left = 264
    Top = 296
    Width = 75
    Height = 25
    TabOrder = 1
    OnClick = BitBtn1Click
    Kind = bkOK
  end
end
