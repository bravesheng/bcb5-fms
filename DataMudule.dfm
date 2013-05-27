object DataModule1: TDataModule1
  OldCreateOrder = False
  Left = 154
  Top = 188
  Height = 454
  Width = 773
  object Database_DCH: TDatabase
    AliasName = 'DCH'
    DatabaseName = 'DB_DCH'
    LoginPrompt = False
    Params.Strings = (
      'user name=DCH'
      'password=dch')
    SessionName = 'Default'
    Left = 24
    Top = 16
  end
  object Query_Update_tCustAutRoute: TQuery
    DatabaseName = 'DB_DCH'
    SessionName = 'Default'
    SQL.Strings = (
      'UPDATE        tCustAutRoute'
      
        'SET                  Note = :sNote, Radius = :iRadius, StartPoin' +
        't = :sStartPoint, EndPoint = :sEndPoint, CoID = :sCoID'
      'WHERE         (RouteNo = :iRouteNo) AND (PtSerialNo = 0)')
    Left = 24
    Top = 152
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'sNote'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'iRadius'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'sStartPoint'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'sEndPoint'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'sCoID'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'iRouteNo'
        ParamType = ptUnknown
      end>
  end
  object Query_Route: TQuery
    DatabaseName = 'DB_DCH'
    SessionName = 'Default'
    SQL.Strings = (
      'SELECT *'
      'FROM tCustAutRoute'
      'ORDER BY  RouteNo, PtSerialNo')
    Left = 24
    Top = 224
  end
  object Table_Cmd: TTable
    DatabaseName = 'DB_DCH'
    SessionName = 'Default'
    StoreDefs = True
    TableName = 'tCmdQueue'
    Left = 96
    Top = 224
  end
  object Query_DB: TQuery
    DatabaseName = 'DB_DCH'
    SessionName = 'Default'
    Left = 208
    Top = 16
  end
  object Table_Remarks: TTable
    DatabaseName = 'DB_DCH'
    Filtered = True
    SessionName = 'Default'
    FieldDefs = <
      item
        Name = 'SerialNo'
        DataType = ftInteger
      end
      item
        Name = 'CoID'
        DataType = ftString
        Size = 10
      end
      item
        Name = 'Lon'
        DataType = ftInteger
      end
      item
        Name = 'Lat'
        DataType = ftInteger
      end
      item
        Name = 'RemarkType'
        DataType = ftInteger
      end
      item
        Name = 'Name'
        DataType = ftString
        Size = 30
      end
      item
        Name = 'Phone'
        DataType = ftString
        Size = 20
      end
      item
        Name = 'Address'
        DataType = ftString
        Size = 50
      end
      item
        Name = 'Email'
        DataType = ftString
        Size = 30
      end
      item
        Name = 'WebAddress'
        DataType = ftString
        Size = 30
      end
      item
        Name = 'ImagePath'
        DataType = ftString
        Size = 255
      end
      item
        Name = 'Memo'
        DataType = ftString
        Size = 60
      end>
    StoreDefs = True
    TableName = 'tRemarks'
    Left = 208
    Top = 80
  end
  object Query_Update: TQuery
    DatabaseName = 'DB_DCH'
    Left = 24
    Top = 80
  end
  object Query_tCustInfo: TQuery
    DatabaseName = 'DB_DCH'
    SQL.Strings = (
      'SELECT         SerialNo, CoID, FullName'
      'FROM             tCustInfo')
    Left = 208
    Top = 144
  end
  object Query_tCustGroup: TQuery
    DatabaseName = 'DB_DCH'
    SQL.Strings = (
      'SELECT         SerialNo, CSerialNo, GrID, FullName'
      'FROM             tCustGroup')
    Left = 48
    Top = 288
  end
  object Query_tCarInfo: TQuery
    DatabaseName = 'DB_DCH'
    SessionName = 'Default'
    SQL.Strings = (
      'SELECT         CarID, LocatorNo'
      'FROM             tCarInfo'
      'WHERE         (Cancel = 0)'
      '')
    Left = 272
    Top = 96
  end
  object Timer_Check_If_Establish_Connect: TTimer
    OnTimer = Timer_Check_If_Establish_ConnectTimer
    Left = 224
    Top = 280
  end
  object Database_TEMP: TDatabase
    AliasName = 'DCH'
    DatabaseName = 'DB_TEMP'
    LoginPrompt = False
    Params.Strings = (
      'user name=DCH'
      'password=dch')
    SessionName = 'Default'
    Left = 104
    Top = 16
  end
end
