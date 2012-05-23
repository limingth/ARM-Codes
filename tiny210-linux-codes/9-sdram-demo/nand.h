
void nand_init(void);

void nand_read_id(char id[]);

void nand_read_page(int addr, char buf[]);

void nand_read(int sdram_addr, int nand__addr, int size);

void nand_write(int sdram_addr, int nand_addr, int size);
