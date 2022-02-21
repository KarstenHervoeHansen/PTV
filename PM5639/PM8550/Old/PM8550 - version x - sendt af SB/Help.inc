{Include file: Help.inc}


PROCEDURE Help(Id : STR_30);

VAR RetKey  : _KeySeq;
    RetCRef : _HelpIDStr;

BEGIN
__DispHlp(Id,0,_ChainRef_hlp,nil,nil,RetCRef,RetKey);



END;
