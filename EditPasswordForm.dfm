object Form_EditPassword: TForm_EditPassword
  Left = 532
  Top = 254
  BorderStyle = bsDialog
  Caption = '�]�w�K�X'
  ClientHeight = 167
  ClientWidth = 299
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 32
    Top = 24
    Width = 105
    Height = 16
    AutoSize = False
    Caption = '�±K�X'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 32
    Top = 56
    Width = 105
    Height = 16
    AutoSize = False
    Caption = '�s�K�X'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 32
    Top = 88
    Width = 105
    Height = 16
    AutoSize = False
    Caption = '�T�{�s�K�X'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Edit_OldPassword: TEdit
    Left = 144
    Top = 22
    Width = 121
    Height = 24
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ImeName = '���� (�c��) - �s�`��'
    ParentFont = False
    PasswordChar = '*'
    TabOrder = 0
  end
  object Edit_NewPassword: TEdit
    Left = 144
    Top = 54
    Width = 121
    Height = 24
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ImeName = '���� (�c��) - �s�`��'
    ParentFont = False
    PasswordChar = '*'
    TabOrder = 1
  end
  object Edit_NewPasRetry: TEdit
    Left = 144
    Top = 86
    Width = 121
    Height = 24
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ImeName = '���� (�c��) - �s�`��'
    ParentFont = False
    PasswordChar = '*'
    TabOrder = 2
  end
  object Button_ok: TButton
    Left = 56
    Top = 128
    Width = 75
    Height = 25
    Caption = '�T�w'
    TabOrder = 3
    OnClick = Button_okClick
  end
  object Button_Cancel: TButton
    Left = 152
    Top = 128
    Width = 75
    Height = 25
    Caption = '����'
    TabOrder = 4
    OnClick = Button_CancelClick
  end
end
