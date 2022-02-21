

typedef struct COMMAND_STRUCT
  {
  char Cmd1;
  char Cmd2;
/*  void ( *Exec)(); No more function pointers, to enable overlaying */
  };


extern UC   MySlaveAddress;
extern UL   Parameter[3];
extern char CmdDelimiter;
extern char Cmd[2];
extern char String[10]; /* used to hold string, if sent by master */


void send(UC gen, UI, UC*, int);
void receive_byte(UC gen, UI, UC*);
UC   spi_transfer (UC, UC);
void CharHandler(void);
void WriteString(UC *);
void WriteVal(UL);
void WriteChar(char);
bit  TestInputBuffer(void);
void RS232break(void);
void RS232Init(void);

