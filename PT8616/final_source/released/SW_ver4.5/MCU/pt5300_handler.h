extern unsigned char FPGA_system_control;
extern char EpromWritePending;


void pt5300_in_handle();
void pt5300_out_handle();

extern void WriteEEpromData(void);
extern void ReadEEpromData(void);