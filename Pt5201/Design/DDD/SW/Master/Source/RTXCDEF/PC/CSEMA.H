/* csema.h - RTXC Semaphore include file */

#define NSEMAS 51
#define DNSEMAS 128

#define KEY_T    1 /* On_Keyboard_Timeout  */
#define DISP_T   2 /* On_Display_Timeout   */
#define STDI_QNE 3 /* On_StdInp_NotEmpty   */
#define CBUGSEMA 4 /* RTXCBUG semafore     */
#define FRONT_T  5 /* Front timeout        */
#define AUTO_C   6 /* On_AutoScan_Change   */
#define FFT_C    7 /* On_FFT_Change        */
#define POL_C    8 /* On_Polarity_Change   */
#define GUARD_C  9 /* On_Guard_Change      */
#define CONST_C  10 /* On_Constel_Change    */
#define TIM_C    11 /* On_TIM_Change        */
#define AFC_C    12 /* On_AFC_Change        */
#define TPS_C    13 /* On_TPS_Change        */
#define FRAME_C  14 /* On_Frame_Change      */
#define FEC_C    15 /* On_FEC_Change        */
#define ALLOCK_C 16 /* On_AllLock_Change    */
#define SCAN_T   17 /* On_Scanning_Timeout  */
#define TPS_T    18 /* On_TPS_Timeout       */
#define AGC_T    19 /* On_Agc_Timeout       */
#define INPUT_C  20 /* On_Input_Change      */
#define FREQ_C   21 /* On_Frequency_Change  */
#define ALARM1_C 22 /* On_Alarm1_Change     */
#define ALARM2_C 23 /* On_Alarm2_Change     */
#define ALARM3_C 24 /* On_Alarm3_Change     */
#define ALARM_T  25 /* On_Alarm_Timeout     */
#define BERLIM_C 26 /* On_BerLimit_Change   */
#define LIMBER_C 27 /* On_LimitBer_Change   */
#define BER_T    28 /* On_Ber_Timeout       */
#define LEVLIM_C 29 /* On_LevelLimit_Change */
#define LIMLEV_C 30 /* On_LimitLevel_Change */
#define LEV_T    31 /* On_Level_Timeout     */
#define EVLOG_F  32 /* On_EventLog_Full     */
#define NOILIM_C 33 /* On_Noise_Limit_Chang */
#define LIMNOI_C 34 /* On_LimitNoise_Change */
#define NOISE_T  35 /* On_Noise_Timeout     */
#define WDOG_T   36 /* On_WatchDog_Timeout  */
#define REM_ISEM 37 /* On_RemoteIn_Event    */
#define REM_OSEM 38 /* On_RemoteOut_Event   */
#define KEY_SEM  39 /* On_Keyboard_Event    */
#define LED_SEM  40 /* On_Led_Event         */
#define HW_SEM   41 /* On_Hardware_Event    */
#define PAR_SEM  42 /* On_Parallel_Event    */
#define DIS_SEM  43 /* On_Display_Event     */
#define REAL_SEM 44 /* On_RealTimeClk_Event */
#define NV_SEM   45 /* On_NVRam_Event       */
#define FEC1_SEM 46 /* On_Fec1_Event        */
#define FEC2_SEM 47 /* On_Fec2_Event        */
#define COF_SEM  48 /* On_LsiCofdm_Event    */
#define ALRM_SEM 49 /* On_Alarm_Event       */
#define TUNI_SEM 50 /* On_TunerInput_Event  */
#define TUNF_SEM 51 /* On_TunerFreq_Event   */

/* End of file - csema.h */
