object Form_CarInfo: TForm_CarInfo
  Left = 226
  Top = 48
  BorderIcons = [biSystemMenu]
  BorderStyle = bsToolWindow
  Caption = '車機參數'
  ClientHeight = 165
  ClientWidth = 259
  Color = clBtnFace
  DragKind = dkDock
  DragMode = dmAutomatic
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDefault
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object ListView1: TListView
    Left = 0
    Top = 0
    Width = 259
    Height = 165
    Align = alClient
    Columns = <
      item
        Caption = '屬性'
        Width = 80
      end
      item
        Caption = '值'
        Width = 160
      end>
    Items.Data = {
      DF0000000700000000000000FFFFFFFFFFFFFFFF000000000000000004A8AEB8
      B900000000FFFFFFFFFFFFFFFF000000000000000008A8AEBEF7B8B9BD580000
      0000FFFFFFFFFFFFFFFF000000000000000008AED5A5BFB0D1BCC600000000FF
      FFFFFFFFFFFFFF00000000000000000CB1B1A8EEA4A4A4DFB8B9BD5800000000
      FFFFFFFFFFFFFFFF00000000000000000CA977AEC9A65EB3F8C057B276000000
      00FFFFFFFFFFFFFFFF00000000000000000CB0D1BCC6A65EB3F8A4E9B4C10000
      0000FFFFFFFFFFFFFFFF00000000000000000CB0D1BCC6A65EB3F8AEC9B6A1}
    TabOrder = 0
    ViewStyle = vsReport
  end
end
