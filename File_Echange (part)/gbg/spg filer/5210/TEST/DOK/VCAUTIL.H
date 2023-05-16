#define ON 1
#define OFF 0

#define FAST 0
#define NORM 1
#define SLOW 2

#define WAVEF 0
#define CURSOR 1

#define SERIAL_A 2
#define SERIAL_B 3

#define FRAME4 0
#define FRAME1 1
#define LINE 2

#define X1 1
#define X5 5
#define X10 10

#define Y1 1
#define Y5 5
#define Y10 10

#define A1 1
#define A10 10
#define A50 50
#define A100 100
#define A200 200

#define X5Div 5
#define X10Div 10
#define X30Div 30
#define X50Div 50
#define X100Div 100

#define Y2Div 2
#define Y5Div 5
#define Y10Div 10

#define F1K 1
#define F100K 100


// general variables

extern char *VCA_ID;


// variables for returnloss measurements
extern int 		RL_data_length,
    			RL_data_mode;
    	
extern float 	RL_freq_start,
	  			RL_freq_stop,
	  			RL_freq_delta;

extern float RL_freq[300];
extern float RL_level[300];



// variables for amplitude spectrum measurements
extern int 		AS_data_length,
    			AS_data_mode;
    	
extern float 	AS_freq_start,
	  			AS_freq_stop,
	  			AS_freq_delta;

extern float AS_freq[300];
extern float AS_level[300];
		



// variables for jitter time measurements
extern int 	JT_data_length,
    		JT_data_mode;
    	
extern float 	JT_time_start,
	  			JT_time_stop,
	  			JT_time_delta;

extern float   JT_peakpeak_ui;

extern float JT_freq[300];
extern float JT_level[300];




// variables for signal delay measurements
extern float	SD_total;		 // delay in seconds
extern int		SD_frames,		 // no of franes
    			SD_lines,		 // no of lines
    			SD_samples;		 // no of samples




// -------------------- VCA selection ----------------------------------

extern void vca_reset(void);
extern void vca_readid(void);
extern void vca_selectinput(int input);		   		   // 2.19
extern void vca_selectsystem(int system);			   // 2.19

// -------------------- return loss ------------------------------------
extern void vca_returnloss(void);					   // 2.21
extern void vca_returnloss_rescale(void);			   // 2.28
extern void vca_returnloss_norm(int on_off);		   // 2.27
extern void vca_returnloss_Ygain(int Ygain);
extern void vca_returnloss_Xgain(int Xgain);
extern void vca_returnloss_sweep(int sweep);
extern void vca_returnloss_cursorfreq(int cursorfreq);
extern void vca_returnloss_variation(int variation);
extern int vca_returnloss_readdata(void);
extern void vca_returnloss_readscale(void);

// -------------------- amplitude spectrum -----------------------------
extern void vca_amplspec(void);						   // 2.21
extern void vca_amplspec_rescale(void);				   // 2.26
extern void vca_amplspec_sweep(int sweep);			   // 2.26
extern void vca_amplspec_norm(int on_off);			   
extern void vca_amplspec_Ygain(int Ygain);
extern void vca_amplspec_Xgain(int Xgain);
extern void vca_amplspec_cursorfreq(int cursorfreq);
extern int vca_amplspec_readdata(void);
extern void vca_amplspec_readscale(void);


// -------------------- jitter time ------------------------------------
extern void vca_jittertime(void);					   // p. 2.29
extern void vca_jittertime_average(int aver);	// LINE  FRAME1  FRAME4	p. 2.36
extern void vca_jittertime_lpfilter(int lp);   	   // OFF  F1K  K100K
extern void vca_jittertime_meastime(int meastime); // 1  5  10  100 200
extern int vca_jittertime_readscale(void);
extern int vca_jittertime_readpeakpeak(float *ppUI);
extern void vca_jittertime_Xgain(int Xgain);	// X1  X5  X10
extern void vca_jittertime_Ygain(int Ygain);	// Y1  Y5  Y10


// -------------------- signal delay -----------------------------------
extern void vca_signaldelay(void);					   // 2.36
extern int vca_signaldelay_readdelay(float *delaytotal);
