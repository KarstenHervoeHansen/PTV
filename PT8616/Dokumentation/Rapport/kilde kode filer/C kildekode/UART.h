extern char update_flag;

void setup_UART();
void putch(unsigned char c);
void serint0(void);
void read_buf(char * out_buffer);