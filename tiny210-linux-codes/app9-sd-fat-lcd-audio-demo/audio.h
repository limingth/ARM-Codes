
void IIS_init(void);

void IIC_write(int slave_addr, int addr, int data);

void IIC_init(void);

void WM8960_init(void);

int audio_play_wav(int file_addr, int file_size);
