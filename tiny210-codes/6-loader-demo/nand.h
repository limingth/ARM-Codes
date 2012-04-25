
void nand_init(void);

void nand_read_id(char id[]);

void nand_read_page(int addr, char buf[]);

void nand_read(int nand_addr, char * sdram_addr, int size);