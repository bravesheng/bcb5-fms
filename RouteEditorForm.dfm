object Form_RouteEditor: TForm_RouteEditor
  Left = 215
  Top = 232
  BorderStyle = bsSingle
  Caption = '設定路線'
  ClientHeight = 196
  ClientWidth = 268
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object BitBtnOk: TBitBtn
    Left = 56
    Top = 160
    Width = 75
    Height = 25
    TabOrder = 5
    OnClick = BitBtnOkClick
    Kind = bkOK
  end
  object BitBtnCancel: TBitBtn
    Left = 136
    Top = 160
    Width = 75
    Height = 25
    TabOrder = 7
    OnClick = BitBtnCancelClick
    Kind = bkCancel
  end
  object Edit_RouteNo: TEdit
    Left = 8
    Top = 8
    Width = 249
    Height = 21
    BorderStyle = bsNone
    Color = clActiveCaption
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clCaptionText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ImeName = '中文 (繁體) - 新注音'
    ParentFont = False
    ReadOnly = True
    TabOrder = 6
    Text = ' 路線編號：'
  end
  object Edit_StartName: TEdit
    Left = 88
    Top = 80
    Width = 169
    Height = 21
    BorderStyle = bsNone
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBtnText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ImeName = '中文 (繁體) - 新注音'
    ParentFont = False
    TabOrder = 2
    OnChange = Edit_StartNameChange
  end
  object Edit3: TEdit
    Left = 8
    Top = 80
    Width = 80
    Height = 21
    BorderStyle = bsNone
    Color = clActiveCaption
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clCaptionText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ImeName = '中文 (繁體) - 新注音'
    ParentFont = False
    ReadOnly = True
    TabOrder = 8
    Text = ' 起點'
  end
  object Edit4: TEdit
    Left = 8
    Top = 104
    Width = 80
    Height = 21
    BorderStyle = bsNone
    Color = clActiveCaption
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clCaptionText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ImeName = '中文 (繁體) - 新注音'
    ParentFont = False
    ReadOnly = True
    TabOrder = 9
    Text = ' 終點'
  end
  object Edit_EndName: TEdit
    Left = 88
    Top = 104
    Width = 169
    Height = 21
    BorderStyle = bsNone
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBtnText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ImeName = '中文 (繁體) - 新注音'
    ParentFont = False
    TabOrder = 3
    OnChange = Edit_EndNameChange
  end
  object Edit_RouteName: TEdit
    Left = 88
    Top = 56
    Width = 169
    Height = 21
    BorderStyle = bsNone
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBtnText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ImeName = '中文 (繁體) - 新注音'
    ParentFont = False
    TabOrder = 1
    OnChange = Edit_RouteNameChange
  end
  object Edit7: TEdit
    Left = 8
    Top = 56
    Width = 80
    Height = 21
    BorderStyle = bsNone
    Color = clActiveCaption
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clCaptionText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ImeName = '中文 (繁體) - 新注音'
    ParentFont = False
    ReadOnly = True
    TabOrder = 10
    Text = ' 路線名稱'
  end
  object Edit8: TEdit
    Left = 8
    Top = 128
    Width = 80
    Height = 21
    BorderStyle = bsNone
    Color = clActiveCaption
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clCaptionText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ImeName = '中文 (繁體) - 新注音'
    ParentFont = False
    ReadOnly = True
    TabOrder = 11
    Text = ' 有效半徑'
  end
  object Edit_Radius: TMaskEdit
    Left = 88
    Top = 128
    Width = 169
    Height = 21
    BorderStyle = bsNone
    EditMask = '099;1; '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ImeName = '中文 (繁體) - 新注音'
    MaxLength = 3
    ParentFont = False
    TabOrder = 4
    Text = '   '
    OnChange = Edit_RadiusChange
  end
  object Edit1: TEdit
    Left = 8
    Top = 32
    Width = 80
    Height = 21
    BorderStyle = bsNone
    Color = clActiveCaption
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clCaptionText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ImeName = '中文 (繁體) - 新注音'
    ParentFont = False
    ReadOnly = True
    TabOrder = 12
    Text = '所屬群組'
  end
  object ComboBox_Company: TComboBox
    Left = 88
    Top = 32
    Width = 169
    Height = 21
    Style = csDropDownList
    ImeName = '中文 (繁體) - 新注音'
    ItemHeight = 13
    TabOrder = 0
    OnChange = ComboBox_CompanyChange
  end
end
