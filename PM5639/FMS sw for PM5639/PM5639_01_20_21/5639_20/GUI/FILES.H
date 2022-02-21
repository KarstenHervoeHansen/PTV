/* Size of print window: doPrintReport(); */

#define PrtLeft	13
#define PrtTop		85
#define PrtRight	427
#define PrtBottom	265
#define PrtWidth	( PrtRight - PrtLeft)

/* Size of printprint window: printPrintFile(); */

#define PrtPrtLeft	(( PrtRight+PrtLeft)/2)
#define PrtPrtTop		( PrtTop + WINDOWBARDEEP + 5)
#define PrtPrtRight	( PrtPrtLeft + 210)
#define PrtPrtBottom	( PrtPrtTop + 100)
#define PrtPrtWidth	( PrtRight - PrtLeft)

/* Size of Save/Restore window: chooseFile(); */

#define	FileLeft		150
#define	FileTop		( WINDOWBARDEEP + 60)
#define	FileRight	490
#define	FileBottom	290

#define FILELINESIZE		14		/* size of one file line  */

enum WinType { DisplayType=0, ProgramType, MeasurementType, FilePrintType};

/* External variables used in FILES.C: */

extern const int HlpSaveDisplay;
extern const int HlpSaveProgram;
extern const int HlpSaveMeasurement;

extern const int HlpRestoreDisplay;
extern const int HlpRestoreProgram;
extern const int HlpRestoreMeasurement;

extern const int HlpEditReport;
extern const int HlpEditReportPrint;
extern const int HlpEditReportFile;

extern ERRORINFO errTxt[];

extern WININFO Win[MAXWINDOW];
extern WININFO* WinPtr[MAXWINDOW];

extern int NoWin;
extern int FASTmode;

extern int CIECrossSize[MAXDISPLAY+1];
extern void* CIECrossImage[MAXDISPLAY+1];

extern const char* LumText[];
extern const char* ColorErrText[];
