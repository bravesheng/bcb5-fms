object CSelectPath: TCSelectPath
  OldCreateOrder = False
  OnCreate = DataModuleCreate
  OnDestroy = DataModuleDestroy
  Left = 646
  Top = 161
  Height = 479
  Width = 741
  object Query_Add: TQuery
    DatabaseName = 'DB_DCH'
    SQL.Strings = (
      'INSERT INTO tCustAutRoute'
      
        '(RouteNo,PtSerialNo,Note,Lon,Lat,Radius,StartPoint,EndPoint,CoID' +
        ')'
      
        'VALUES  (:RouteNo,:PtSerialNo,:Note,:Lon,:Lat,:Radius,:StartPoin' +
        't,:EndPoint,:CoID)'
      ' '
      ' ')
    Left = 32
    Top = 16
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'RouteNo'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'PtSerialNo'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Note'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Lon'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Lat'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'Radius'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'StartPoint'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'EndPoint'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'CoID'
        ParamType = ptUnknown
      end>
  end
  object Query1: TQuery
    DatabaseName = 'DB_DCH'
    Left = 32
    Top = 80
  end
  object Query_Delete: TQuery
    DatabaseName = 'DB_DCH'
    SQL.Strings = (
      'DELETE FROM tCustAutRoute'
      'WHERE         (RouteNo = :RouteNo)')
    Left = 40
    Top = 152
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'RouteNo'
        ParamType = ptUnknown
      end>
  end
end
