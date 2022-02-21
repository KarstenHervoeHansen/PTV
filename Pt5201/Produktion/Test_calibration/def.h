#define DISABLED 1
#define ENABLED  0

#if 1
#define IEEEBUS 1    //IEEE interface 
#define AT232   1    //RS232 interface (com5,6,7)
#define DEBUG   0    //Debug flag
#define HP34970 1    //Enable/Disable HP34970-instrument
#else
#define IEEEBUS 0    //IEEE interface 
#define AT232   0    //RS232 interface (com5,6,7)
#define DEBUG   0    //Debug flag
#define HP34970 0    //Enable/Disable HP34970-instrument
#endif

#define YES    1
#define NO     0
#define TRUE   1
#define FALSE  0
#define LF    10
#define CR    13
#define WAIT   1
#define NOWAIT 0
#define ON     1
#define OFF    0

#define SPGNOTRESET 2

#define LOCALLOG    0   // log cal data on C-drive
#define LANLOG      1   // log cal data on LAN (M-drive)
                    // see 'LogDestination'
            
#define VITSEXTDATA  0    // VITS control for PM5640
#define VITSPASS     1

#define LEFTCHANNEL 1
#define RIGHTCHANNEL 2
#define LEFTCH 1
#define RIGHTCH 2

#define PM5640GIEEEADDR 16    // IEEE addr for PM5640
#define PM5640MIEEEADDR 17

#define REARTERM34401    0    // HP34401 multimeter
#define FRONTTERM34401   1

#define PAL    1
#define NTSC   2
#define NTSCJP 3 // JK***

#define L625 625
#define L525 525

#define ALLUNITS  0    // apparat config
#define MAIN    1
#define AUDIOGEN  2
#define BLACKBURST  3
#define SPG     4
#define COLORBAR  5

#define GENL_PAL    0  // genlock mux styring
#define GENL_NTSC   1
#define GENL_443_HIGH 2
#define GENL_443_LOW  3
#define GENL_358_HIGH 4
#define GENL_358_LOW  5
#define GENL_10M_HIGH 6
#define GENL_10M_LOW  7
#define GENL_OFF       10

#define DUT_LOCKED 0x09    // genlock status register i SPG
#define DUT_INTERN 0x04

#define GAIN_DAC     0     // BB gain kalaibrering
#define OFFSET_DAC   1    // BB level og offset kalibrering
#define SYNC_DAC     2    
                  
#define OFFSET_CAL   1
#define SYNC_CAL     2

#define PT5201 5201

#define VCO6144 1
#define VCO56448  2

#define FREQ27  27    // 27.0MHz X-TAL
#define FREQ13  13    // 13.0MHz TCXO

#define NR_OF_TESTS 22

