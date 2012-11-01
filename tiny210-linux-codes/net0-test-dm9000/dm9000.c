#include "stdio.h"

#define DM_ADDR_PORT (*((volatile unsigned short *) 0x88000000)) //地址口
#define DM_DATA_PORT (*((volatile unsigned short *) 0x88000004)) //数据口

#define DM9000_ID		0x90000A46

/* although the registers are 16 bit, they are 32-bit aligned.
*/

#define DM9000_NCR             0x00
#define DM9000_NSR             0x01
#define DM9000_TCR             0x02
#define DM9000_TSR1            0x03
#define DM9000_TSR2            0x04
#define DM9000_RCR             0x05
#define DM9000_RSR             0x06
#define DM9000_ROCR            0x07
#define DM9000_BPTR            0x08
#define DM9000_FCTR            0x09
#define DM9000_FCR             0x0A
#define DM9000_EPCR            0x0B
#define DM9000_EPAR            0x0C
#define DM9000_EPDRL           0x0D
#define DM9000_EPDRH           0x0E
#define DM9000_WCR             0x0F

#define DM9000_PAR             0x10
#define DM9000_MAR             0x16

#define DM9000_GPCR	       0x1e
#define DM9000_GPR             0x1f
#define DM9000_TRPAL           0x22
#define DM9000_TRPAH           0x23
#define DM9000_RWPAL           0x24
#define DM9000_RWPAH           0x25

#define DM9000_VIDL            0x28
#define DM9000_VIDH            0x29
#define DM9000_PIDL            0x2A
#define DM9000_PIDH            0x2B

#define DM9000_CHIPR           0x2C
#define DM9000_SMCR            0x2F

#define DM9000_ETXCSR          0x30
#define DM9000_TCCR	       0x31
#define DM9000_RCSR	       0x32

#define CHIPR_DM9000A	       0x19
#define CHIPR_DM9000B	       0x1A

#define DM9000_MRCMDX          0xF0
#define DM9000_MRCMD           0xF2
#define DM9000_MRRL            0xF4
#define DM9000_MRRH            0xF5
#define DM9000_MWCMDX          0xF6
#define DM9000_MWCMD           0xF8
#define DM9000_MWRL            0xFA
#define DM9000_MWRH            0xFB
#define DM9000_TXPLL           0xFC
#define DM9000_TXPLH           0xFD
#define DM9000_ISR             0xFE
#define DM9000_IMR             0xFF

void delay(int t)
{
	int i;

	for (i = 0; i < t; i++)
		;

	return;
}

//写DM9000寄存器   
void __inline dm_reg_write(unsigned char reg, unsigned char data)   
{   
	DM_ADDR_PORT = reg;            //将寄存器地址写到地址端口   
	DM_DATA_PORT = data;            //将数据写到数据端口   
}   

//读DM9000寄存器   
unsigned char __inline dm_reg_read(unsigned char reg)   
{   
	DM_ADDR_PORT = reg;              
	return DM_DATA_PORT;             //将数据从数据端口读出   
}  

void dm_read_id(char id[])
{
	id[0] = dm_reg_read(DM9000_VIDL); 
	id[1] = dm_reg_read(DM9000_VIDH); 
	id[2] = dm_reg_read(DM9000_PIDL); 
	id[3] = dm_reg_read(DM9000_PIDH); 

	return;
}

void dm_init(void)   
{   
	dm_reg_write(DM9000_NCR,1);         //软件复位DM9000   
	delay(30);              //延时至少20μs   
	dm_reg_write(DM9000_NCR,0);         //清除复位位   

	dm_reg_write(DM9000_NCR,1);         //为了确保复位正确，再次复位   
	delay(30);   
	dm_reg_write(DM9000_NCR,0);   

	dm_reg_write(DM9000_GPCR,1);       //设置GPIO0为输出   
	dm_reg_write(DM9000_GPR,0);         //激活内部PHY   

	dm_reg_write(DM9000_NSR,0x2c);           //清TX状态   
	dm_reg_write(DM9000_ISR,0xf);                     //清中断状态   

	dm_reg_write(DM9000_RCR,0x39);           //设置RX控制   
	dm_reg_write(DM9000_TCR,0);                //设置TX控制   
	dm_reg_write(DM9000_BPTR,0x3f);            
	dm_reg_write(DM9000_FCTR,0x3a);   
	dm_reg_write(DM9000_FCR,0xff);   
	dm_reg_write(DM9000_SMCR,0x00);   

	dm_reg_write(DM9000_PAR+0,0x00);         //设置MAC地址：00-01-02-03-04-05   
	dm_reg_write(DM9000_PAR+1,0x01);           
	dm_reg_write(DM9000_PAR+2,0x02);   
	dm_reg_write(DM9000_PAR+3,0x03);   
	dm_reg_write(DM9000_PAR+4,0x04);   
	dm_reg_write(DM9000_PAR+5,0x05);   

	dm_reg_write(DM9000_NSR,0x2c);           //再次清TX状态   
	dm_reg_write(DM9000_ISR,0xf);                     //再次清中断状态   

	dm_reg_write(DM9000_IMR,0x81);           //打开接受数据中断   
}  

void dm_tran_packet(unsigned char *datas, int length)   
{   
	int i;   

	dm_reg_write(DM9000_IMR, 0x80);          //在发送数据过程中禁止网卡中断   

	dm_reg_write(DM9000_TXPLH, (length>>8) & 0x0ff);           //设置发送数据长度   
	dm_reg_write(DM9000_TXPLL, length & 0x0ff);   

	DM_ADDR_PORT = DM9000_MWCMD;                 //发送数据缓存赋予数据端口   

	//发送数据   
	for(i=0;i<length;i+=2)   
	{   
		delay(50);   
		DM_DATA_PORT = datas[i]|(datas[i+1]<<8);            //8位数据转换为16位数据输出   
	}       

	dm_reg_write(DM9000_TCR, 0x01);          //把数据发送到以太网上   

	while((dm_reg_read(DM9000_NSR) & 0x0c) == 0)   
		;                           //等待数据发送完成   

	delay(50);   

	dm_reg_write(DM9000_NSR, 0x2c);          //清除TX状态   
	dm_reg_write(DM9000_IMR, 0x81);          //打开DM9000接收数据中断   
}  

int dm_rec_packet(unsigned char *datas)   
{   
	unsigned char int_status;   
	unsigned char rx_ready;   
	unsigned short rx_status;   
	unsigned short rx_length;   
	unsigned short temp;   
	int i;   

	int_status = dm_reg_read(DM9000_ISR);           //读取ISR   
	if(int_status & 0x1)                     //判断是否有数据要接受   
	{   
		rx_ready = dm_reg_read(DM9000_MRCMDX);         //先读取一个无效的数据   
		rx_ready = (unsigned char)DM_DATA_PORT;            //真正读取到的数据包首字节   

		if(rx_ready == 1)                 //判读首字节是否为1或0   
		{   
			DM_ADDR_PORT = DM9000_MRCMD;           //连续读取数据包内容   

			rx_status = DM_DATA_PORT;                           //状态字节   

			rx_length = DM_DATA_PORT;                          //数据长度   

			if(!(rx_status & 0xbf00) && (rx_length < 10000))     //判读数据是否符合要求   
			{   
				for(i=0; i<rx_length; i+=2)          //16位数据转换为8位数据存储   
				{   
					delay(50);   
					temp = DM_DATA_PORT;   
					datas[i] = temp & 0x0ff;   
					datas[i + 1] = (temp >> 8) & 0x0ff;   
				}   
			}   
		}   
		else if(rx_ready !=0)      //停止设备   
		{   
			//dm_reg_write(DM9000_IMR,0x80);  //停止中断   
			//dm_reg_write(DM9000_ISR,0x0F);   //清中断状态   
			//dm_reg_write(DM9000_RCR,0x0);    //停止接收   
			//还需要复位系统，这里暂时没有处理   
		}   
	}   
	dm_reg_write(DM9000_ISR, 0x1);             //清中断   
	return rx_length;   
}  

unsigned char arpsendbuf[42]={   

	0xff,0xff,0xff,0xff,0xff,0xff,                     //以太网目标地址，全1表示为广播地址   
	0x00,0x01,0x02,0x03,0x04,0x05,        //以太网源地址   
	0x08,0x06,                                        //帧类型：ARP帧   

	0x00,0x01,                                        //硬件类型：以太网   
	0x08,0x00,                                        //协议类型：IP协议   
	0x06,                                                //硬件地址长度：6字节   
	0x04,                                                //协议地址长度：4字节   
	0x00,0x01,                                        //操作码：ARP请求   

	0x00,0x01,0x02,0x03,0x04,0x05,        //发送端以太网硬件地址   
	192, 168, 1, 50,                                 //发送端IP协议地址   
	0x00,0x00,0x00,0x00,0x00,0x00,        //接收端以太网硬件地址   
	192, 168, 1, 120                                 //接收端IP协议地址   
};  

void dm_send_arp(void)
{
	dm_tran_packet(arpsendbuf, 42 );   

	return;
}

unsigned char buffer[1024];

void putchar_hex(char c);

void dm_recv_arp(void)
{
	int packet_len;

	packet_len = dm_rec_packet(buffer);                  //接收网卡数据   

//	if((buffer[12]==0x08)&&(buffer[13]==0x06))          //是ARP协议   
	if (packet_len == 64)
	{              
		int i;

		printf("packet len = %d, type = <%d %d> \n", packet_len, buffer[12], buffer[13]);
		puts("------------------------------\n");
		for(i=0;i<packet_len;i++)   
		{   
		//	printf("%x\n", buffer[i]);
			
			putchar_hex(buffer[i]);
			putchar(' ');
		}   

		putchar('\n');
		puts("------------------------------\n");
	}

	return;
}
