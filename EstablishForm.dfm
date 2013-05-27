object Form_Establish_Connect: TForm_Establish_Connect
  Left = 188
  Top = 132
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = '建立連線'
  ClientHeight = 118
  ClientWidth = 303
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poScreenCenter
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label_Statu: TLabel
    Left = 16
    Top = 16
    Width = 273
    Height = 13
    AutoSize = False
    Caption = '請選擇連線資料庫'
  end
  object Btn_Ignore: TBitBtn
    Left = 162
    Top = 78
    Width = 90
    Height = 25
    Caption = '離開(&I)'
    TabOrder = 0
    OnClick = Btn_IgnoreClick
    Kind = bkIgnore
  end
  object Btn_Retry: TBitBtn
    Left = 40
    Top = 78
    Width = 90
    Height = 25
    Caption = '連線(&R)'
    Enabled = False
    TabOrder = 1
    OnClick = Btn_RetryClick
    Kind = bkRetry
  end
  object ComboBox_BDE: TComboBox
    Left = 16
    Top = 40
    Width = 273
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 2
    OnChange = ComboBox_BDEChange
  end
end
