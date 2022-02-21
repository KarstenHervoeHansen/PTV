/* mnuinter.h */



void do_AnlBlkConfig(BBObject *, UC, UC);
void do_GenlockConfig(GenlockObject *, UC);
void do_SdiConfig(CBObject *, UC);
void do_SdiText(UC);

void do_AnalogConfig(AudioObject *, UC);
void do_AesebuConfig(AudioObject *, UC);
void do_RecallPreset(UC, UC*);
void do_StorePreset(UC);
void do_NamePreset(UC, char *);
void do_Calibration(UC);
void do_calibration_save(void);
void do_Audio_Kind(UC);
void do_FactoryReset(UC);
//void put_command(char *);
void init_configs(UC *);
void do_cal_store(void);
void do_cal_enable_off(void);
void do_cal_enable_on(void);
void send_down(void);
void send_up(void);
void send_max(void);
void send_min(void);

