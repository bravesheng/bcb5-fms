object Form_Login: TForm_Login
  Left = 362
  Top = 394
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = '使用者登錄'
  ClientHeight = 158
  ClientWidth = 308
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 16
    Top = 56
    Width = 89
    Height = 16
    AutoSize = False
    Caption = '使用者名稱'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 17
    Top = 90
    Width = 88
    Height = 16
    AutoSize = False
    Caption = '密碼'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 24
    Top = 16
    Width = 249
    Height = 24
    Caption = 'Fleet Management System'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Edit_UserName: TEdit
    Left = 112
    Top = 56
    Width = 177
    Height = 24
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ImeMode = imClose
    ImeName = '中文 (繁體) - 新注音'
    ParentFont = False
    TabOrder = 0
  end
  object Button_Ok: TButton
    Left = 56
    Top = 120
    Width = 75
    Height = 25
    Caption = '確定'
    TabOrder = 2
    OnClick = Button_OkClick
  end
  object Button_Cancel: TButton
    Left = 160
    Top = 120
    Width = 75
    Height = 25
    Caption = '取消'
    TabOrder = 3
    OnClick = Button_CancelClick
  end
  object Edit_Password: TEdit
    Left = 112
    Top = 88
    Width = 177
    Height = 24
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ImeMode = imClose
    ImeName = '中文 (繁體) - 新注音'
    ParentFont = False
    PasswordChar = '*'
    TabOrder = 1
    OnKeyDown = Edit_PasswordKeyDown
  end
end
