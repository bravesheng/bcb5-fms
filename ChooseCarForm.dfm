object Form_ChooseCar: TForm_ChooseCar
  Left = 465
  Top = 124
  BorderStyle = bsDialog
  Caption = '監控車輛管理'
  ClientHeight = 425
  ClientWidth = 532
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter2: TSplitter
    Left = 0
    Top = 161
    Width = 532
    Height = 3
    Cursor = crVSplit
    Align = alTop
    Beveled = True
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 532
    Height = 161
    Align = alTop
    TabOrder = 0
    object Splitter1: TSplitter
      Left = 337
      Top = 1
      Width = 3
      Height = 159
      Cursor = crHSplit
      Beveled = True
    end
    object Panel1: TPanel
      Left = 1
      Top = 1
      Width = 160
      Height = 159
      Align = alLeft
      TabOrder = 0
      object Label1: TLabel
        Left = 8
        Top = 8
        Width = 145
        Height = 13
        AutoSize = False
        Caption = '選擇公司( Company )'
      end
      object ComboBox_Company: TComboBox
        Left = 8
        Top = 24
        Width = 145
        Height = 21
        Hint = '指定公司'
        Style = csDropDownList
        ImeName = '中文 (繁體) - 新注音'
        ItemHeight = 13
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
        OnChange = ComboBox_CompanyChange
        OnDropDown = ComboBox_CompanyDropDown
      end
    end
    object ListView_Type: TListView
      Left = 340
      Top = 1
      Width = 191
      Height = 159
      Hint = 'Type篩選'
      Align = alClient
      BorderStyle = bsNone
      Checkboxes = True
      Columns = <
        item
          Caption = 'Type'
        end
        item
          Caption = '說明'
          Width = 100
        end
        item
          Caption = 'SerialNo'
          Width = 0
        end>
      GridLines = True
      ReadOnly = True
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
      ViewStyle = vsReport
      OnMouseDown = ListView_TypeMouseDown
    end
    object ListView_Group: TListView
      Left = 161
      Top = 1
      Width = 176
      Height = 159
      Hint = 'Group篩選'
      Align = alLeft
      BorderStyle = bsNone
      Checkboxes = True
      Columns = <
        item
          Caption = 'Group'
        end
        item
          Caption = '說明'
          Width = 100
        end
        item
          Caption = 'SerialNo'
          Width = 0
        end>
      GridLines = True
      ReadOnly = True
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
      ViewStyle = vsReport
      OnMouseDown = ListView_GroupMouseDown
    end
  end
  object ListView_Selected: TListView
    Left = 297
    Top = 164
    Width = 235
    Height = 261
    Hint = '已選擇監控車輛'
    Align = alClient
    BorderStyle = bsNone
    Columns = <
      item
        Caption = '已選擇車輛'
        Width = 130
      end
      item
        Caption = 'LocatorNo'
        Width = 80
      end
      item
        Caption = 'GroupID'
        Width = 0
      end
      item
        Caption = 'TypeID'
        Width = 0
      end>
    MultiSelect = True
    ReadOnly = True
    RowSelect = True
    ParentShowHint = False
    ShowHint = True
    TabOrder = 1
    ViewStyle = vsReport
    OnDblClick = ListView_SelectedDblClick
  end
  object ListView_NoSelect: TListView
    Left = 0
    Top = 164
    Width = 241
    Height = 261
    Hint = '未被選擇監控車輛'
    Align = alLeft
    BorderStyle = bsNone
    Columns = <
      item
        Caption = '可選擇車輛'
        Width = 130
      end
      item
        Caption = 'LocatorNo'
        Width = 80
      end
      item
        Caption = 'GroupID'
        Width = 0
      end
      item
        Caption = 'TypeID'
        Width = 0
      end>
    MultiSelect = True
    ReadOnly = True
    RowSelect = True
    ParentShowHint = False
    ShowHint = True
    TabOrder = 2
    ViewStyle = vsReport
    OnDblClick = ListView_NoSelectDblClick
  end
  object Panel3: TPanel
    Left = 241
    Top = 164
    Width = 56
    Height = 261
    Align = alLeft
    TabOrder = 3
    object SpeedButton_SingleGo: TSpeedButton
      Left = 8
      Top = 51
      Width = 41
      Height = 22
      Hint = '選擇'
      Caption = '--->'
      ParentShowHint = False
      ShowHint = True
      OnClick = SpeedButton_SingleGoClick
    end
    object SpeedButton_AllGo: TSpeedButton
      Left = 8
      Top = 123
      Width = 41
      Height = 22
      Hint = '全部選擇'
      Caption = 'ALL--->'
      ParentShowHint = False
      ShowHint = True
      OnClick = SpeedButton_AllGoClick
    end
    object SpeedButton_SingleBack: TSpeedButton
      Left = 8
      Top = 75
      Width = 41
      Height = 22
      Hint = '取消'
      Caption = '<---'
      ParentShowHint = False
      ShowHint = True
      OnClick = SpeedButton_SingleBackClick
    end
    object SpeedButton_AllBack: TSpeedButton
      Left = 8
      Top = 147
      Width = 41
      Height = 22
      Hint = '全部取消'
      Caption = '<---ALL'
      ParentShowHint = False
      ShowHint = True
      OnClick = SpeedButton_AllBackClick
    end
    object SpeedButton_OK: TSpeedButton
      Left = 8
      Top = 195
      Width = 41
      Height = 22
      Hint = '完成確定'
      Caption = 'OK'
      ParentShowHint = False
      ShowHint = True
      OnClick = SpeedButton_OKClick
    end
  end
  object Query_CarType: TQuery
    DatabaseName = 'DB_DCH'
    SQL.Strings = (
      'SELECT         *'
      'FROM             tCarType'
      'WHERE         (CSerialNo = :Csn)')
    Left = 48
    Top = 88
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Csn'
        ParamType = ptUnknown
      end>
  end
  object Query_CarGroup: TQuery
    DatabaseName = 'DB_DCH'
    SQL.Strings = (
      'SELECT         *'
      'FROM             tCarGroup'
      'WHERE         (CSerialNo = :Csn)')
    Left = 16
    Top = 88
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Csn'
        ParamType = ptUnknown
      end>
  end
end
