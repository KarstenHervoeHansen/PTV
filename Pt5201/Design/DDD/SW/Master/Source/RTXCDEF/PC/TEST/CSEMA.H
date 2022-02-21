/* csema.h - RTXC Semaphore include file */

#define NSEMAS 43

#define KEY_T    1 /* On_Keyboard_Timeout  */
#define DISP_T   2 /* On_Display_Timeout   */
#define STDI_QNE 3 /* On_StdInp_NotEmpty   */
#define CBUGSEMA 4 /* RTXCBUG semafore     */
#define FRONT_T  5 /* Front timeout        */
#define MODE_C   6 /* On_Mode_Change       */
#define LOCK_C   7 /* On_Lock_Change       */
#define ALLOCK_C 8 /* On_AllLock_Change    */
#define SCAN_T   9 /* On_Scanning_Timeout  */
#define TPS_T    10 /* On_TPS_Timeout       */
#define AGC_T    11 /* On_Agc_Timeout       */
#define INPUT_C  12 /* On_Input_Change      */
#define FREQ_C   13 /* On_Frequency_Change  */
#define ALARM1_C 14 /* On_Alarm1_Change     */
#define ALARM2_C 15 /* On_Alarm2_Change     */
#define ALARM3_C 16 /* On_Alarm3_Change     */
#define ALARM_T  17 /* On_Alarm_Timeout     */
#define BERLIM_C 18 /* On_BerLimit_Change   */
#define LIMBER_C 19 /* On_LimitBer_Change   */
#define BER_T    20 /* On_Ber_Timeout       */
#define LEVLIM_C 21 /* On_LevelLimit_Change */
#define LIMLEV_C 22 /* On_LimitLevel_Change */
#define LEV_T    23 /* On_Level_Timeout     */
#define EVLOG_F  24 /* On_EventLog_Full     */
#define NOILIM_C 25 /* On_Noise_Limit_Chang */
#define LIMNOI_C 26 /* On_LimitNoise_Change */
#define NOISE_T  27 /* On_Noise_Timeout     */
#define WDOG_T   28 /* On_WatchDog_Timeout  */
#define REM_ISEM 29 /* On_RemoteIn_Event    */
#define REM_OSEM 30 /* On_RemoteOut_Event   */
#define KEY_SEM  31 /* On_Keyboard_Event    */
#define LED_SEM  32 /* On_Led_Event         */
#define HW_SEM   33 /* On_Hardware_Event    */
#define PAR_SEM  34 /* On_Parallel_Event    */
#define DIS_SEM  35 /* On_Display_Event     */
#define REAL_SEM 36 /* On_RealTimeClk_Event */
#define NV_SEM   37 /* On_NVRam_Event       */
#define FEC1_SEM 38 /* On_Fec1_Event        */
#define FEC2_SEM 39 /* On_Fec2_Event        */
#define COF_SEM  40 /* On_LsiCofdm_Event    */
#define ALRM_SEM 41 /* On_Alarm_Event       */
#define TUNI_SEM 42 /* On_TunerInput_Event  */
#define TUNF_SEM 43 /* On_TunerFreq_Event   */

/* End of file - csema.h */
