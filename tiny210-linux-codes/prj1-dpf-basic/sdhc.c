#include "sdhc.h"
#include "uart.h"
#include "stdio.h"

//#define	debug		printf

static int debug(const char * fmt, ...)
{
	return 0;
}

typedef enum _SDHC_REGS {
	SDHC_SYS_ADDR						= 0x00,
	SDHC_BLK_SIZE						= 0x04, //16-bit SFR
	SDHC_BLK_COUNT					= 0x06, //16-bit SFR
	SDHC_ARG							= 0x08,
	SDHC_TRANS_MODE						= 0x0C, //16-bit SFR
	SDHC_COMMAND						= 0x0E, //16-bit SFR
	SDHC_RSP0							= 0x10,
	SDHC_RSP1							= 0x14,
	SDHC_RSP2							= 0x18,
	SDHC_RSP3							= 0x1C,
	SDHC_BUF_DAT_PORT				= 0x20,
	SDHC_PRESENT_STAT				= 0x24,
	SDHC_HOST_CTRL					= 0x28, // 8-bit SFR
	SDHC_PWR_CTRL						= 0x29, // 8-bit SFR
	SDHC_BLOCKGAP_CTRL				= 0x2A, // 8-bit SFR
	SDHC_WAKEUP_CTRL					= 0x2B, // 8-bit SFR
	SDHC_CLK_CTRL						= 0x2C, //16-bit SFR
	SDHC_TIMEOUT_CTRL					= 0x2E, // 8-bit SFR
	SDHC_SOFTWARE_RESET				= 0x2F, // 8-bit SFR
	SDHC_NORMAL_INT_STAT				= 0x30, //16-bit SFR
	SDHC_ERROR_INT_STAT				= 0x32, //16-bit SFR
	SDHC_NORMAL_INT_STAT_ENABLE		= 0x34, //16-bit SFR
	SDHC_ERROR_INT_STAT_ENABLE		= 0x36, //16-bit SFR
	SDHC_NORMAL_INT_SIGNAL_ENABLE	= 0x38, //16-bit SFR
	SDHC_ERROR_INT_SIGNAL_ENABLE	= 0x3A, //16-bit SFR
	SDHC_AUTO_CMD12_ERR_STAT		= 0x3C, //16-bit SFR
	SDHC_CAPA							= 0x40,
	SDHC_MAX_CURRENT_CAPA			= 0x48,
//	SDHC_MONITOR						= 0x4C,
	SDHC_CONTROL2						= 0x80,
		// [15] Feedback clock used for Tx Data/Command logic.
		// [14] Feedback clock used for Rx Data/Command logic.
		// [13] Select card detection signal. 0=nSDCD, 1=DAT[3].
		// [11] CE-ATA I/F mode. 1=Enable, 0=Disable.
		// [8]  SDCLK Hold enable. 
	SDHC_CONTROL3						= 0x84,
	SDHC_DEBUG						= 0x88,
	SDHC_CONTROL4						= 0x8C,
	SDHC_FORCE_EVENT_CMD12_ERROR	= 0x50,	// 16-bit SFR
	SDHC_FORCE_EVENT_NOR_ERROR		= 0x52,	// 16-bit SFR
	SDHC_ADMA_ERROR					= 0x54,	// 32-bit SFR
	SDHC_ADMA_SYSTEM_ADDRESS		= 0x58,	// 32-bit SFR
	SDHC_HOST_CONTROLLER_VERSION	= 0xFE  //16-bit SFR
}SDHC_REGS;


// SDHC Transfer Mode.
typedef enum _SDHC_operation{
	SDHC_POLLING_MODE = 0,
	SDHC_INTERRUPT_MODE,
	SDHC_SDMA_MODE,
	SDHC_ADMA2_MODE
} SDHC_operation;


// SDHC Channel Number.
typedef enum _SDHC_channel{
	SDHC_CHANNEL_0 = 0,
	SDHC_CHANNEL_1,
	SDHC_CHANNEL_2,
	SDHC_CHANNEL_CNT
} SDHC_channel;

// SDHC Card Type.
typedef enum _SDHC_card_type{
	SDHC_SD_CARD=0,
	SDHC_MMC_CARD,
	SDHC_CE_ATA_CARD,
	SDHC_SDIO_CARD
} SDHC_card_type;

// Clock Source
typedef enum _SDHC_clockSource{
	SDHC_HCLK=1,
	SDHC_EPLL=2,
	SDHC_EXTCLK=3
} SDHC_clockSource;

// SD/MMC Speed Mode
typedef enum _SDHC_SpeedMode {
	SDHC_NORMAL_SPEED = 0,
	SDHC_HIGH_SPEED = 1
} SDHC_SpeedMode;

// Command Type
typedef enum _SDHC_CommandType {
	SDHC_CMD_BC_TYPE,		// broadcast commands (bc), no response
	SDHC_CMD_BCR_TYPE,		// broadcast commands with response (bcr)
	SDHC_CMD_AC_TYPE,		// addressed (point-to-point) commands (ac), no data transfer on DAT lines
	SDHC_CMD_ADTC_TYPE		// addressed (point-to-point) data transfer commands (adtc), data transfer on DAT lines
} SDHC_CommandType;

// Command Response Type.
typedef enum _SDHC_ResponseType {
	SDHC_RES_NO_TYPE=0,
	SDHC_RES_R1_TYPE,
	SDHC_RES_R1B_TYPE,
	SDHC_RES_R2_TYPE,
	SDHC_RES_R3_TYPE,
	SDHC_RES_R4_TYPE,
	SDHC_RES_R5_TYPE,
	SDHC_RES_R6_TYPE,
	SDHC_RES_R7_TYPE
} SDHC_ResponseType;

//Transfer mode;
typedef enum {
	SDHC_BYTE_MODE=0,
	SDHC_BLOCK_MODE
} SDHC_transfer_mode;

// Common SD/MMC Structure.
typedef struct {
	U8* m_uBaseAddr;		//œµÕ≥µÿ÷∑
	SDHC_operation  m_eOpMode;
	SDHC_card_type m_eCardType;
	SDHC_transfer_mode m_eTransMode;
	SDHC_channel m_eChannel;
	SDHC_clockSource m_eClockSource;
	U32  m_uClockDivision;
	U16  m_uRca;
	U16	 m_usTransMode;
	U16  m_usClkCtrlReg;
	U16 m_uRemainBlock;
	U8	m_uCCSResponse;		// CCS signal for CE-ATA
	U8   m_ucSpecVer;
	U8   m_ucHostCtrlReg;
	U8   m_ucBandwidth;
	U32 * m_uBufferPtr;
	// -- Card Information
	U32 m_uStartBlockPos;		// startBlock Position. - for Test Case usage.
	U16 m_uOneBlockSize;		// multi block count.	- fot Test Case usage.
	U16 m_sReadBlockLen;		// One Block Size = (1<<READ_BL_LEN)
	U16 m_sReadBlockPartial;
	U16 m_sCSize;
	U16 m_sCSizeMult;			// CardSize = (1<<m_sReadBlockLen)*(m_sCSize+1)*(1<<(m_sCSizeMult+2))/1048576
	U16 m_sNormalStatus;		// normal status
	U16 m_sErrorStatus;			// error status.
	//
	U8 m_ucIntChannelNum;		// VIC Interrupt Number.
	void (*m_fIntFn)(void);	// DMA Interrupt Handler Pointer.
} SDHC;

#define Assert(a)	{if(!a) while(1);}	
#define TRUE	1
#define FALSE	0

//Normal Interrupt Signal Enable
#define	SDHC_ERROR_INTERRUPT_EN				(1<<15)
#define	SDHC_SD_ADDRESS_INT3_EN				(1<<14)
#define	SDHC_SD_ADDRESS_INT2_EN				(1<<13)
#define	SDHC_SD_ADDRESS_INT1_EN				(1<<12)
#define	SDHC_SD_ADDRESS_INT0_EN				(1<<11)
#define	SDHC_READWAIT_SIG_INT_EN			(1<<10)
#define	SDHC_CCS_INTERRUPT_STATUS_EN		(1<<9)
#define	SDHC_CARD_SIG_INT_EN					(1<<8)
#define	SDHC_CARD_REMOVAL_SIG_INT_EN		(1<<7)
#define	SDHC_CARD_INSERT_SIG_INT_EN			(1<<6)
#define	SDHC_BUFFER_READREADY_SIG_INT_EN	(1<<5)
#define	SDHC_BUFFER_WRITEREADY_SIG_INT_EN	(1<<4)
#define	SDHC_DMA_SIG_INT_EN					(1<<3)
#define	SDHC_BLOCKGAP_EVENT_SIG_INT_EN		(1<<2)
#define	SDHC_TRANSFERCOMPLETE_SIG_INT_EN	(1<<1)
#define	SDHC_COMMANDCOMPLETE_SIG_INT_EN	(1<<0)

// Error Interrupt Signal Enable.
#define	SDHC_ADMA2_ERROR						(1<<9)
#define	SDHC_AUTO_CMD12_ERROR				(1<<8)
#define	SDHC_CURRENT_LIMIT_ERROR				(1<<7)
#define	SDHC_DATA_END_BIT_ERROR				(1<<6)
#define	SDHC_DATA_CRC_ERROR					(1<<5)
#define	SDHC_DATA_TIME_OUT_ERROR			(1<<4)
#define	SDHC_CMD_INDEX_ERROR					(1<<3)
#define	SDHC_CMD_END_BIT_ERROR				(1<<2)
#define	SDHC_CMD_CRC_ERROR					(1<<1)
#define	SDHC_CMD_TIMEOUT_ERROR				(1<<0)

// ADMA2 Error status register.
#define	SDHC_ADMA_FINAL_BLOCK_TRANSFERRED	(1<<10)
#define	SDHC_ADMA_CONTINUE_REQUEST			(1<<9)
#define	SDHC_ADMA_INTERRUPT_STATUS			(1<<8)
#define	SDHC_ADMA_LENGTH_MISMATCH_ERR		(1<<2)
#define	SDHC_ADMA_ERROR_STATUS				(1<<0)

#define SDOutp32(addr,data)		*((volatile unsigned int*)(addr))=data
#define SDOutp16(addr,data)		*((volatile unsigned short*)(addr))=data
#define SDOutp8(addr,data)		*((volatile unsigned char*)(addr))=data
#define SDInp32(addr)			*((volatile unsigned int*)(addr))
#define SDInp16(addr)			*((volatile unsigned short*)(addr))
#define SDInp8(addr)			*((volatile unsigned char*)(addr))
#define SDHC_MMC_HIGH_SPEED_CLOCK 20000000
#define SDHC_SD_HIGH_SPEED_CLOCK 25000000


SDHC SDHC_descriptor;
//////////
// File Name : SDHC_SetBlockCountReg (Inline Macro)
// File Description : This function set block count register.
// Input : SDHC, block count 
// Output : NONE.
#define SDHC_SetBlockCountReg( sCh, uBlkCnt) \
	SDOutp16( (sCh)->m_uBaseAddr + SDHC_BLK_COUNT, (uBlkCnt) );

//////////
// File Name : SDHC_SetSystemAddressReg (Inline Macro)
// File Description : This function set DMA start address.
// Input : SDHC, start address.
// Output : NONE.
#define SDHC_SetSystemAddressReg( sCh, SysAddr) \
	SDOutp32( (sCh)->m_uBaseAddr + SDHC_SYS_ADDR, (SysAddr) );

//////////
// File Name : SDHC_SetBlockSizeReg (Inline Macro)
// File Description : This function set block size and buffer size.
// Input : SDHC, DMA buffer boundary, One block size.
// Output : NONE.
#define SDHC_SetBlockSizeReg( sCh, uDmaBufBoundary, uBlkSize ) \
	SDOutp16( (sCh)->m_uBaseAddr + SDHC_BLK_SIZE, (((uDmaBufBoundary)<<12)|(uBlkSize)) );



//////////
// File Name : SDHC_INT_WAIT_CLEAR (Inline Macro)
// File Description : Interrupt wait and clear.
// Input : SDHC, interrupt bit, timeout loop count 
// Output : NONE.	// 0x7F000000 youngbo.song
#define SDHC_INT_WAIT_CLEAR(sCh,bit,loop) \
	loop=0x7F000000; \
	while ( !(SDInp16( (sCh)->m_uBaseAddr + SDHC_NORMAL_INT_STAT ) & (1<<bit) ) ) { \
		if ( --loop == 0 ) { \
			/*CONSOL_Printf( "***********Time out Error : bit : %d, Line:%d \n", bit, __LINE__ ); */\
			break;	} } \
	do { SDOutp16( (sCh)->m_uBaseAddr + SDHC_NORMAL_INT_STAT, (1<<bit) ); \
	} while( SDInp16( (sCh)->m_uBaseAddr + SDHC_NORMAL_INT_STAT ) & (1<<bit) );



//////////
// File Name : SDHC_INT_CLEAR (Inline Macro)
// File Description : Interrupt clear.
// Input : SDHC, interrupt bit, timeout loop count 
// Output : NONE.	// 0x7F000000 youngbo.song
#define SDHC_ERROR_INT_CLEAR(sCh,bit) \
	SDOutp16( (sCh)->m_uBaseAddr + SDHC_ERROR_INT_STAT, (1<<bit) ); \
	while( SDInp16( (sCh)->m_uBaseAddr + SDHC_ERROR_INT_STAT ) & (1<<bit) );


//////////
// File Name : SDHC_INT_CLEAR (Inline Macro)
// File Description : Interrupt clear.
// Input : SDHC, interrupt bit, timeout loop count 
// Output : NONE.	// 0x7F000000 youngbo.song
#define SDHC_NORMAL_INT_CLEAR(sCh,bit) \
	SDOutp16( (sCh)->m_uBaseAddr + SDHC_NORMAL_INT_STAT, (1<<bit) ); \
	while( SDInp16( (sCh)->m_uBaseAddr + SDHC_NORMAL_INT_STAT ) & (1<<bit) );

//
// [7:6] Command Type
// [5]  Data Present Select
// [4] Command Index Check Enable
// [3] CRC Check Enable
// [1:0] Response Type Select
const unsigned char SDHC_cmd_sfr_data[] = {
	(unsigned char)((0<<4)|(0<<3)|(0<<0)),	// RES_NO_TYPE
	(unsigned char)((1<<4)|(1<<3)|(2<<0)),	// RES_R1_TYPE,
	(unsigned char)((1<<4)|(1<<3)|(3<<0)),	// RES_R1B_TYPE,
	(unsigned char)((0<<4)|(1<<3)|(1<<0)),	// RES_R2_TYPE,
	(unsigned char)((0<<4)|(0<<3)|(2<<0)),	// RES_R3_TYPE,
	(unsigned char)((0<<4)|(0<<3)|(2<<0)),	// RES_R4_TYPE,
	(unsigned char)((1<<4)|(1<<3)|(2<<0)),	// RES_R5_TYPE,
	(unsigned char)((1<<4)|(1<<3)|(2<<0)),	// RES_R6_TYPE,	// check need.
	(unsigned char)((1<<4)|(1<<3)|(2<<0)),	// RES_R7_TYPE,	// check need.
};

SDHC* SDHC_curr_card[SDHC_CHANNEL_CNT];
unsigned int SDHC_global_card_size;




U8 SDHC_Init(void);
void SDHC_CloseMedia(void);
static void SDHC_WriteOneBlock(U32 uBufAddr);
static void SDHC_ReadOneBlock(U32 uBufAddr);
U8 SDHC_WriteBlocks(U32 uStBlock, U16 uBlocks, U32 uBufAddr);
U8 SDHC_ReadBlocks(U32 uStBlock, U16 uBlocks, U32 uBufAddr);
static U8 SDHC_IdentifyCard(SDHC* sCh);
static void SDHC_ResetController(SDHC* sCh);
static void SDHC_SetSdClock(SDHC* sCh, SDHC_SpeedMode speed);
static U8 SDHC_IssueCommand( SDHC* sCh, U16 uCmd, U32 uArg, SDHC_CommandType cType, SDHC_ResponseType rType );
static U8 SDHC_GetSdScr(SDHC* sCh);
static U8 SDHC_SetSDOCR(SDHC* sCh);
static U8 SDHC_WaitForCard2TransferState(SDHC* sCh);
static void SDHC_ClearErrInterruptStatus(SDHC* sCh);
static void SDHC_SetTransferModeReg(U32 MultiBlk, U32 DataDirection, U32 AutoCmd12En, U32 BlockCntEn, U32 DmaEn, SDHC* sCh);
static void SDHC_SetSdhcInterruptEnable(U16 uNormalIntStatusEn, U16 uErrorIntStatusEn, U16 uNormalIntSigEn, U16 uErrorIntSigEn, SDHC* sCh);
static void SDHC_SetHostCtrlSpeedMode(SDHC_SpeedMode eSpeedMode, SDHC* sCh);
static void SDHC_SetSdhcCardIntEnable(U8 ucTemp, SDHC* sCh);
static void SDHC_SetSdClockOnOff(U8 uOnOff, SDHC* sCh);
static U8 SDHC_SetDataTransferWidth(SDHC* sCh);
static U8 SDHC_SetSdCardSpeedMode(SDHC_SpeedMode eSpeedMode, SDHC* sCh);
static void SDHC_DisplayCardInformation(SDHC * sCh);
static void SDHC_Set_InitClock( SDHC* sCh );



static void SDHC_InterruptHandler(SDHC * sCh);
static void SDHC_ErrorInterruptHandler(SDHC* sCh);

//void OS_interrupt_install( sCh->m_ucIntChannelNum, sCh->m_fIntFn );
//void OS_interrupt_umask( sCh->m_ucIntChannelNum );



//////////
// File Name : SDHC_DMADone
// File Description : Interrupt handler for channel 0
// Input : NONE.
// Output : NONE.
void SDHC_ISR0(void) {
	SDHC_InterruptHandler( SDHC_curr_card[SDHC_CHANNEL_0]);
	//INTC_ClearVectAddr();
}

/**
 * act2  |  act1  | symbol
 *   0    |    0    | NOP
 *   0    |    1    | reserved
 *   1    |    0    | transfer data
 *   1    |    1    | Link
 * valid : valid = 1 -> Indicates this line of descriptor is effective. If valid=0 generate ADMA Error Interrupt and stop ADMA to prevent runaway.
 * end : end = 1 -> Indicates to end of descriptor. The transfer Complete Interrupt is generated when the operation of the descriptor line is completed.
 * Int : Interrupt = 1 -> generates DMA Interrupt When the Operation of the descriptor line is completed.
 */

void SDHC_SetDriveStrength(SDHC*sCh, U8 cStrength ) {
	if(cStrength>3) {
		cStrength=3;
	}
	if(sCh->m_eChannel == SDHC_CHANNEL_0 || sCh->m_eChannel == SDHC_CHANNEL_1 ) {
		SDOutp32( 0x7F0081A0, (SDInp32(0x7F0081A0 ) & ~(0x3<<26)) | (cStrength<<26) );	// Data Line
		SDOutp32( sCh->m_uBaseAddr+SDHC_CONTROL4, 0x3<<16);				// Clock
	}
}


void SDHC_ErrorInterruptHandler(SDHC* sCh) {
	unsigned short status;

	status = sCh->m_sErrorStatus = SDInp16(sCh->m_uBaseAddr+SDHC_ERROR_INT_STAT);

	if( status & SDHC_ADMA2_ERROR ) {
		//CONSOL_Printf("SDHC_ADMA2_ERROR[%x]\n", SDInp32(sCh->m_uBaseAddr+SDHC_ADMA_ERROR));
		SDHC_ERROR_INT_CLEAR(sCh, 9);
	}
	if( status & SDHC_AUTO_CMD12_ERROR ) {
		//CONSOL_Printf("SDHC_AUTO_CMD12_ERROR\n");
		SDHC_ERROR_INT_CLEAR(sCh, 8);
	}
	if( status & SDHC_CURRENT_LIMIT_ERROR ) {
		//CONSOL_Printf("SDHC_CURRENT_LIMIT_ERROR\n");
		SDHC_ERROR_INT_CLEAR(sCh, 7);
	}
	if( status & SDHC_DATA_END_BIT_ERROR ) {
		//CONSOL_Printf("SDHC_DATA_END_BIT_ERROR\n");
		SDHC_ERROR_INT_CLEAR(sCh, 6);
	}
	if( status & SDHC_DATA_CRC_ERROR ) {
		//CONSOL_Printf("SDHC_DATA_CRC_ERROR\n");
		SDHC_ERROR_INT_CLEAR(sCh, 5);
	}
	if( status & SDHC_DATA_TIME_OUT_ERROR ) {
		//CONSOL_Printf("SDHC_DATA_TIME_OUT_ERROR\n");
		SDHC_ERROR_INT_CLEAR(sCh, 4);
	}
	if( status & SDHC_CMD_INDEX_ERROR ) {
		//CONSOL_Printf("SDHC_CMD_INDEX_ERROR\n");
		SDHC_ERROR_INT_CLEAR(sCh, 3);
	}
	if( status & SDHC_CMD_END_BIT_ERROR ) {
		//CONSOL_Printf("SDHC_CMD_END_BIT_ERROR\n");
		SDHC_ERROR_INT_CLEAR(sCh, 2);
	}
	if( status & SDHC_CMD_CRC_ERROR ) {
		//CONSOL_Printf("SDHC_CMD_CRC_ERROR\n");
		SDHC_ERROR_INT_CLEAR(sCh, 1);
	}
	if( status & SDHC_CMD_TIMEOUT_ERROR ) {
		//CONSOL_Printf("SDHC_CMD_TIMEOUT_ERROR\n");
		SDHC_ERROR_INT_CLEAR(sCh, 0);
	}
}

//////////
// File Name : SDHC_DMAIntHandler
// File Description : This function is interrupt service routine for Common DMA Handling.
// Input : NONE.
// Output : NONE.
void SDHC_InterruptHandler(SDHC * sCh) {
	unsigned short status;

	sCh->m_sNormalStatus |= status  = SDInp16(sCh->m_uBaseAddr+SDHC_NORMAL_INT_STAT);

//	CONSOL_Printf("normal status : %x\n", status);
	puts("SDHC_InterruptHandler\n");
	
	if( status & SDHC_ERROR_INTERRUPT_EN ) {
		SDHC_ErrorInterruptHandler(sCh);
		SDHC_NORMAL_INT_CLEAR(sCh, 15);
	}
	if( status & SDHC_SD_ADDRESS_INT3_EN ) {
		SDHC_NORMAL_INT_CLEAR(sCh, 14 );
	}
	if( status & SDHC_SD_ADDRESS_INT2_EN ) {
		SDHC_NORMAL_INT_CLEAR(sCh, 13 );
	}
	if( status & SDHC_SD_ADDRESS_INT1_EN ) {
		SDHC_NORMAL_INT_CLEAR(sCh, 12 );
	}
	if( status & SDHC_SD_ADDRESS_INT0_EN ) {
		SDHC_NORMAL_INT_CLEAR(sCh, 11 );
	}
	if( status & SDHC_READWAIT_SIG_INT_EN ) {
		SDHC_NORMAL_INT_CLEAR(sCh, 10 );
	}
	if( status & SDHC_CCS_INTERRUPT_STATUS_EN ) {
		SDHC_NORMAL_INT_CLEAR(sCh, 9 );
		sCh->m_uCCSResponse=0;
	}
	if( status & SDHC_CARD_SIG_INT_EN ) {	// SDIO Interrupt.
		//CONSOL_Printf("Card Interrupt\n");
		SDHC_NORMAL_INT_CLEAR(sCh, 8 );
	}
	if( status & SDHC_CARD_REMOVAL_SIG_INT_EN ) {
		//CONSOL_Printf("Card Removed\n");
		SDHC_NORMAL_INT_CLEAR(sCh, 7 );
	}
	if( status & SDHC_CARD_INSERT_SIG_INT_EN ) {
		//CONSOL_Printf("Card Insered\n");
		SDHC_NORMAL_INT_CLEAR(sCh, 6 );
	}
	if( status & SDHC_BUFFER_READREADY_SIG_INT_EN ) {
		SDHC_NORMAL_INT_CLEAR(sCh, 5 );
		SDHC_ReadOneBlock( (U32)sCh->m_uBufferPtr );
	}
	if( status & SDHC_BUFFER_WRITEREADY_SIG_INT_EN ) {
		SDHC_NORMAL_INT_CLEAR(sCh, 4 );
		SDHC_WriteOneBlock( (U32)sCh->m_uBufferPtr );
	}
	if( status & SDHC_DMA_SIG_INT_EN ) {
		SDHC_NORMAL_INT_CLEAR(sCh, 3 );
		SDHC_SetSystemAddressReg(sCh, SDInp32(sCh->m_uBaseAddr+SDHC_SYS_ADDR) );
	}
	if( status & SDHC_BLOCKGAP_EVENT_SIG_INT_EN ) {
		//CONSOL_Printf("BlockGap\n");
		SDHC_NORMAL_INT_CLEAR(sCh, 2 );
	}
	if( status & SDHC_TRANSFERCOMPLETE_SIG_INT_EN ) {
		sCh->m_uRemainBlock=0;
		SDHC_NORMAL_INT_CLEAR(sCh, 1 );
	}
	if( status & SDHC_COMMANDCOMPLETE_SIG_INT_EN ) {
		// Command Complete.
		SDHC_NORMAL_INT_CLEAR(sCh, 0 );
	}
}




//////////
// File Name : SDHC_SetSdhcInterruptEnable
// File Description : Setting normal and error interrupt.
// Input : Normal interrupt, Error interrupt, SDHC
// Output : NONE.
void SDHC_SetSdhcInterruptEnable(U16 uNormalIntStatusEn, U16 uErrorIntStatusEn, U16 uNormalIntSigEn, U16 uErrorIntSigEn, SDHC* sCh)
{
	SDHC_ClearErrInterruptStatus(sCh);
	SDOutp16( sCh->m_uBaseAddr+SDHC_NORMAL_INT_STAT_ENABLE, uNormalIntStatusEn);
	SDOutp16( sCh->m_uBaseAddr+SDHC_ERROR_INT_STAT_ENABLE, uErrorIntStatusEn);
	SDOutp16( sCh->m_uBaseAddr+SDHC_NORMAL_INT_SIGNAL_ENABLE, uNormalIntSigEn);
	SDOutp16( sCh->m_uBaseAddr+SDHC_ERROR_INT_SIGNAL_ENABLE, uErrorIntSigEn);
}

//////////
// File Name : SDHC_OpenMedia
// File Description : Initialize channel information.
// Input : SDHC Clock Source, SDHC pointer.
// Output : Success or Failure.
U8 SDHC_Init(void)
{
	SDHC_SpeedMode speed;
	U32 uOperFreq = 0;
	U32 cnt = 0;
	SDHC* sCh = &SDHC_descriptor;
	sCh->m_eChannel = SDHC_CHANNEL_0;
	sCh->m_eClockSource = SDHC_HCLK;
	sCh->m_eOpMode = SDHC_POLLING_MODE;//SDHC_SDMA_MODE;
	sCh->m_uStartBlockPos =1000;// start Block address.
	sCh->m_ucBandwidth = 4;	// bandwidth.
	sCh->m_uClockDivision = 2;	// clock division
	sCh->m_uOneBlockSize = 2048;	// block size.
	sCh->m_ucHostCtrlReg = 0;
	sCh->m_usClkCtrlReg = 0;
	sCh->m_ucBandwidth = 4;		// bit width.
	SDHC_curr_card[sCh->m_eChannel]=sCh;	// Pointer...
	sCh->m_uBaseAddr = (U8*)ELFIN_HSMMC_0_BASE;
	sCh->m_fIntFn = SDHC_ISR0;
	sCh->m_ucIntChannelNum = 56/*IRQ_HSMMC0*/;
	// GPIO Setting.
   	//SDHC_SetGPIO(sCh->m_eChannel, sCh->m_ucBandwidth);
	rGPGCON =(rGPGCON & 0xf0000000);
	rGPGCON |= 0x02222222;
	rGPGPUD = rGPGPUD & 0xfffff000;
	
//	printf("WorkingFreq = 0x%xMHz\n", (133000000 / (sCh->m_uClockDivision*2))/1000000 );
	
	SDHC_ResetController(sCh);

	//SDHC_SetDriveStrength(sCh, 3);
	
	debug("begin to SDHC_IdentifyCard\n");
	while(!SDHC_IdentifyCard(sCh))
	{
		if(++cnt>4)
		return FALSE;
	}
	debug("SDHC_IdentifyCard ok!\n");

	// card Selection
	if ( !SDHC_IssueCommand( sCh, 7, (U32)(sCh->m_uRca<<16), SDHC_CMD_AC_TYPE, SDHC_RES_R1B_TYPE ) )
		return FALSE;
	
	 if ( sCh->m_eCardType == SDHC_SD_CARD ) {
		SDHC_GetSdScr(sCh);
		if (sCh->m_ucSpecVer==1||sCh->m_ucSpecVer==2) {
			speed = ( uOperFreq > SDHC_SD_HIGH_SPEED_CLOCK) ? (SDHC_HIGH_SPEED) : (SDHC_NORMAL_SPEED);
			if (!SDHC_SetSdCardSpeedMode(speed, sCh)) {
				puts("init SDHC_SetSdCardSpeedMode fail\n");
				return FALSE;
			}
		}
		else
			Assert(uOperFreq<=SDHC_SD_HIGH_SPEED_CLOCK); // Error! Old version SD card can not support working frequency higher than 25MHz");
	}
	// host controller speed setting.
	speed = ( uOperFreq > SDHC_MMC_HIGH_SPEED_CLOCK) ? (SDHC_HIGH_SPEED) : (SDHC_NORMAL_SPEED);
	SDHC_SetHostCtrlSpeedMode( speed, sCh );

	SDHC_SetSdClockOnOff(0, sCh); // If the sd clock is to be changed, you need to stop sd-clock.
	SDHC_SetSdClock(sCh, speed);

	// After a card was selected, then the bus width can be changed.
	if (!SDHC_SetDataTransferWidth( sCh))
		return FALSE;

	if (!SDHC_WaitForCard2TransferState(sCh))
		return FALSE;

	// CMD16 can not be set while card is in programming state.
	if (!SDHC_IssueCommand(sCh, 16, 512, SDHC_CMD_AC_TYPE, SDHC_RES_R1_TYPE ) ) // Set the block size
		return FALSE;

	// youngbo.song
	SDOutp32( sCh->m_uBaseAddr+SDHC_CONTROL2, SDInp32(sCh->m_uBaseAddr+SDHC_CONTROL2)|(1<<8)|(2<<9)|(1<<28));
	return TRUE;
}

//////////
// File Name : SDHC_SetSDOCR
// File Description : Get SD OCR Register from SD Card.
// Input : SDHC channel
// Output : success or failure.
U8 SDHC_SetSDOCR(SDHC* sCh)
{
	U32 i, OCR;

	// Place all cards in the idle state.
	if (!SDHC_IssueCommand( sCh, 0, 0, SDHC_CMD_BC_TYPE, SDHC_RES_NO_TYPE ) )
		return FALSE;


	for(i=0; i<500; i++)
	{
		// CMD55 (For ACMD)
		SDHC_IssueCommand( sCh, 55, 0, SDHC_CMD_AC_TYPE, SDHC_RES_R1_TYPE );
		// (Ocr:2.7V~3.6V)
		SDHC_IssueCommand( sCh, 41, 0x40ff8000, SDHC_CMD_BCR_TYPE, SDHC_RES_R3_TYPE );

		if (SDInp32(sCh->m_uBaseAddr+SDHC_RSP0)&(unsigned int)((unsigned)0x1<<31))
		{
			OCR = SDInp32( sCh->m_uBaseAddr+SDHC_RSP0);
			//CONSOL_Printf("\nrHM_RSPREG0=%x",rHM_RSPREG0);
			//if(OCR & (1<<7));
				//CONSOL_Printf("Voltage range : 1.65V ~ 1.95V\n");
			//if(OCR & (1<<21))
				//CONSOL_Printf("Voltage range: 2.7V ~ 3.4V\n");
			//else if(OCR & (1<<20))
				//CONSOL_Printf("Voltage range: 2.7V ~ 3.3V\n");
		//	else if(OCR & (1<<23))
		//		CONSOL_Printf("Voltage range: 2.7V ~ 3.6V\n");

			if(OCR&(0x1<<30)) {
				sCh->m_eTransMode = SDHC_BLOCK_MODE;
				//CONSOL_Printf("High Capacity Card\n");
			}
			else{
				sCh->m_eTransMode = SDHC_BYTE_MODE;
				//CONSOL_Printf("Byte mode\n");
			}
			// Normal SD has Command Response Error.
			SDHC_ClearErrInterruptStatus(sCh);

			sCh->m_eCardType = SDHC_SD_CARD;
			return TRUE;
		}
	}
	// The current card is MMC card, then there's time out error, need to be cleared.
	SDHC_ClearErrInterruptStatus(sCh);
	return FALSE;
}


//////////
// File Name : SDHC_GetSdSCR
// File Description : Setting sd card SCR infomation.
// Input : SDHC, temp buffer for gather sd infomation up to 512 byte.
// Output : success of failure.
U8 SDHC_GetSdScr(SDHC* sCh) {
	U32 buffer[2];
	U32 ignore = 0;

	if(!SDHC_IssueCommand( sCh, 16, 8, SDHC_CMD_AC_TYPE, SDHC_RES_R1_TYPE ) )
		return FALSE;

	SDHC_SetBlockSizeReg( sCh, 7, 8);
	SDHC_SetBlockCountReg( sCh, 1);
	SDHC_SetTransferModeReg(0, 1, 0, 0, 0, sCh);
	sCh->m_uRemainBlock = 1;
	sCh->m_uBufferPtr = buffer;

	// CMD55 (For ACMD)
	if (!SDHC_IssueCommand( sCh, 55, sCh->m_uRca<<16, SDHC_CMD_AC_TYPE, SDHC_RES_R1_TYPE ) )
		return FALSE;

	// Acmd51 - Send SCR
	if(!SDHC_IssueCommand( sCh, 51, 0, SDHC_CMD_ADTC_TYPE, SDHC_RES_R1_TYPE ) )
		return FALSE;
	SDHC_ReadOneBlock( (U32)sCh->m_uBufferPtr );
	SDHC_INT_WAIT_CLEAR( sCh, 1, ignore );	// SDHC_TRANSFERCOMPLETE_STS_INT_EN

// Transfer mode is determined by capacity register at OCR setting.
//	sCh->m_eTransMode = SDHC_BYTE_MODE;
	if ((*buffer&0xf) == 0x0)
		sCh->m_ucSpecVer = 0; // Version 1.0 ~ 1.01
	else if ((*buffer&0xf) == 0x1)
		sCh->m_ucSpecVer = 1; // Version 1.10, support cmd6
	else if((*buffer&0xf) == 0x2) {
		sCh->m_ucSpecVer = 2; // Version 2.0 support cmd6 and cmd8
//		sCh->m_eTransMode = SDHC_BLOCK_MODE;
	}
	else {
		sCh->m_ucSpecVer = 0; // Error... Deda
	}

	//CONSOL_Printf("SDSpecVer=%d\n", sCh->m_ucSpecVer);
	return TRUE;
}


//////////
// File Name : SDHC_ResetController
// File Description : Reset SDHC Controller.
// Input : SDHC channel
// Output : NONE.
void SDHC_ResetController(SDHC* sCh) {
	SDOutp8( sCh->m_uBaseAddr+SDHC_SOFTWARE_RESET, 0x3);
}


//////////
// File Name : SDHC_SetSdhcCardIntEnable
// File Description : Setting normal and error interrupt.
// Input : SDHC, Normal interrupt, Error interrupt.
// Output : NONE.
void SDHC_SetSdhcCardIntEnable(U8 ucTemp, SDHC* sCh)
{
	U16 usSfr;

	usSfr = SDInp16( sCh->m_uBaseAddr+SDHC_NORMAL_INT_STAT_ENABLE);
	usSfr = usSfr & 0xFEFF;
	usSfr |= (ucTemp<<8);
	SDOutp16( sCh->m_uBaseAddr+SDHC_NORMAL_INT_STAT_ENABLE, usSfr );
}



/////////
// File Name : SDHC_ClearErrInterruptStatus
// File Description : Clear error interrupt status register.
// Input : SDHC channel
// Output : NONE.
void SDHC_ClearErrInterruptStatus(SDHC* sCh) {
	U16 usSfr, usSfr1;
	usSfr = SDInp16(sCh->m_uBaseAddr+SDHC_NORMAL_INT_STAT);
	while (usSfr&(0x1<<15)) {
		usSfr1 = SDInp16(sCh->m_uBaseAddr+SDHC_ERROR_INT_STAT);
		SDOutp16(sCh->m_uBaseAddr+SDHC_ERROR_INT_STAT, usSfr1);
		usSfr = SDInp16(sCh->m_uBaseAddr+SDHC_NORMAL_INT_STAT);
	}
}


//////////
// File Name : SDHC_IssueCommand
// File Description : This function issue sd/mmc command.
// Input : SDHC, command Index, Argument, Command Type, Response Type
// Output : Command Result.
U8 SDHC_IssueCommand( SDHC* sCh, U16 uCmd, U32 uArg, SDHC_CommandType cType, SDHC_ResponseType rType ) {
	U16 sfrData;
	U32 Loop;

	while( SDInp32( sCh->m_uBaseAddr+SDHC_PRESENT_STAT ) & 0x1 );	// Check CommandInhibit_CMD
	 
	sfrData = (uCmd<<8) | SDHC_cmd_sfr_data[ rType ];
	if ( cType == SDHC_CMD_ADTC_TYPE ) {
		sfrData |= (1<<5);
	}
	if ( uCmd == 12 ) { // check abort bit when stop command.
		sfrData |= (3<<6);
	}
	// argument setting.
	SDOutp32( sCh->m_uBaseAddr+SDHC_ARG, uArg);
	
	SDOutp16( sCh->m_uBaseAddr+SDHC_COMMAND, sfrData);

	// Command Complete. - SDHC_COMMANDCOMPLETE_STS_INT_EN
	SDHC_INT_WAIT_CLEAR( sCh, 0, Loop);

	// Error Status Check - reduce too much error message.
	if ( (SDInp16( sCh->m_uBaseAddr+SDHC_NORMAL_INT_STAT ) & (1<<15)) && !(uCmd==1||uCmd==55||uCmd==41) ) {
		//CONSOL_Printf("Command = %d, Error Stat = %x\n", SDInp16( sCh->m_uBaseAddr+SDHC_COMMAND )>>8,
			//SDInp16( sCh->m_uBaseAddr+SDHC_ERROR_INT_STAT ) );
		SDOutp16( sCh->m_uBaseAddr+SDHC_ERROR_INT_STAT, SDInp16( sCh->m_uBaseAddr+SDHC_ERROR_INT_STAT ) );
		return FALSE;
	}

	// Busy Check.
	if ( rType == SDHC_RES_R1B_TYPE ) {
		while( SDInp32( sCh->m_uBaseAddr+SDHC_PRESENT_STAT ) & (1<<1) );// Check CommandInhibit_DAT
	}

	return TRUE;
}

//////////
// File Name : SDHC_WriteOneBlock
// File Description : This function writes one block data by CPU transmission mode.
// Input : SDHC( assert buffer pointer and remain data length.)
// Output : NONE.
void SDHC_WriteOneBlock(U32 uBufAddr) {
	SDHC* sCh = &SDHC_descriptor;
	U32* source_Ptr = (U32 *)uBufAddr;
	int block_size;
	int i;
	block_size = SDInp16( (sCh)->m_uBaseAddr + SDHC_BLK_SIZE ) & 0xFFF;
	
	// Wait for buffer write ready... - SDHC_BUFFER_WRITEREADY_STS_INT_EN
	SDHC_INT_WAIT_CLEAR( sCh, 4, i );
	//if ( 100000 - i > 1 )
		//CONSOL_Printf( "100000 to time:%d\n", 100000-i);
	
	block_size = (block_size+3) >> 2;	// block_size = (block_size+3) / 4;
	for(i=block_size; i>0; i--)	//512/4
	{
		SDOutp32( sCh->m_uBaseAddr+SDHC_BUF_DAT_PORT, *source_Ptr++);
	}
	sCh->m_uRemainBlock--;
	sCh->m_uBufferPtr = source_Ptr;
}

//////////
// File Name : SDHC_ReadBlocks
// File Description : This function reads user-data common usage.
// Input : start block, block count, target buffer address, SDHC channel
// Output : Success or Failure
U8 SDHC_ReadBlocks(U32 uStBlock, U16 uBlocks, U32 uBufAddr)
{
	U32 ignore;
	SDHC* sCh = &SDHC_descriptor;

	debug("<SDHC_ReadBlocks> start=%d, size=%d\n", uStBlock, uBlocks);
	
#if 0
	puts("SDHC_ReadBlocks : ");
	putx(uStBlock);
	puts("   ");
	putx(uBlocks);
	puts("\n");
#endif
	
	if(sCh->m_eTransMode == SDHC_BYTE_MODE)
		uStBlock = uStBlock<<9;//*512;
		
	sCh->m_uRemainBlock = uBlocks;
	sCh->m_uBufferPtr=(U32*)uBufAddr;

	if ( !SDHC_WaitForCard2TransferState( sCh ) )
		return 3;

	SDHC_SetBlockSizeReg(sCh, 7, 512); // Maximum DMA Buffer Size, Block Size
	SDHC_SetBlockCountReg(sCh, uBlocks); // Block Numbers to Write

	if ( sCh->m_eOpMode == SDHC_SDMA_MODE ) {
		SDHC_SetSystemAddressReg(sCh, uBufAddr);// AHB System Address For Write
		SDHC_SetTransferModeReg((uBlocks==1)?(0):(1), 1, (uBlocks==1)?(0):(1), 1, 1, sCh ); //Transfer mode setting
	}
	else {
		SDHC_SetTransferModeReg((uBlocks==1)?(0):(1), 1, (uBlocks==1)?(0):(1), 1, 0, sCh ); //Transfer mode setting
	}

	if (uBlocks == 1) { // CMD17: Single-Read
		if ( !SDHC_IssueCommand( sCh, 17, uStBlock, SDHC_CMD_ADTC_TYPE, SDHC_RES_R1_TYPE )) {
			return 4;
		}
	}
	else { // CMD18: Multi-Read
		if( !SDHC_IssueCommand( sCh, 18, uStBlock, SDHC_CMD_ADTC_TYPE, SDHC_RES_R1_TYPE )) {
			return 5;
		}
	}

	if( sCh->m_eOpMode == SDHC_SDMA_MODE || sCh->m_eOpMode == SDHC_ADMA2_MODE ) {
	}
	else if( sCh->m_eOpMode == SDHC_POLLING_MODE ) {
		while(sCh->m_uRemainBlock != 0 ) {
			SDHC_ReadOneBlock( (U32)sCh->m_uBufferPtr );
		}
	}
	else if( sCh->m_eOpMode == SDHC_INTERRUPT_MODE ) {
		while(sCh->m_uRemainBlock != 0);
	}
	else {
		Assert("Not support mode");
	}
	// wait for transfer complete.
	SDHC_INT_WAIT_CLEAR( sCh, 1, ignore );
	sCh->m_uRemainBlock = 0;

	debug("<SDHC_ReadBlocks> ok! return 1\n");
	return 1;	// block_cnt * 512
}


//////////
// File Name : SDHC_WriteBlocks
// File Description : This function writes user-data common usage.
// Input : start block, block count, source buffer address, SDHC channel
// Output : Success or Failure
U8 SDHC_WriteBlocks(U32 uStBlock, U16 uBlocks, U32 uBufAddr) {
	U32 ignore;
	SDHC* sCh = &SDHC_descriptor;
	if(sCh->m_eTransMode == SDHC_BYTE_MODE)
		uStBlock = uStBlock<<9;	//	 uStBlock * 512;

	sCh->m_uRemainBlock = uBlocks;	// number of blocks.
	sCh->m_uBufferPtr = (U32*)uBufAddr;

	if ( !SDHC_WaitForCard2TransferState( sCh ) )
		return 3;
	SDHC_SetBlockSizeReg(sCh, 7, 512); // Maximum DMA Buffer Size, Block Size
	SDHC_SetBlockCountReg(sCh, uBlocks); // Block Numbers to Write 
	if( sCh->m_eOpMode == SDHC_SDMA_MODE ) {
		SDHC_SetSystemAddressReg(sCh, uBufAddr);// AHB System Address For Write
		SDHC_SetTransferModeReg( (uBlocks==1)?(0):(1), 0, (uBlocks==1)?(0):(1), 1, 1, sCh ); // transfer mode 
	}
	else {
		// Interrupt or Polling mode.
		SDHC_SetTransferModeReg( (uBlocks==1)?(0):(1), 0, (uBlocks==1)?(0):(1), 1, 0, sCh ); // transfer mode 
	}

	if (uBlocks == 1) {
		if ( !SDHC_IssueCommand( sCh, 24, uStBlock, SDHC_CMD_ADTC_TYPE, SDHC_RES_R1_TYPE )) {
			return 4;
		}
	}
	else {
		if ( !SDHC_IssueCommand( sCh, 25, uStBlock, SDHC_CMD_ADTC_TYPE, SDHC_RES_R1_TYPE )) {
			return 5;
		}
	}

	if( sCh->m_eOpMode == SDHC_SDMA_MODE || sCh->m_eOpMode == SDHC_ADMA2_MODE ) {


	}
	else if( sCh->m_eOpMode == SDHC_POLLING_MODE ) {
		while(sCh->m_uRemainBlock != 0 ) {
			SDHC_WriteOneBlock( (U32)sCh->m_uBufferPtr );
		}
	}
	else if( sCh->m_eOpMode == SDHC_INTERRUPT_MODE ) {
		while(sCh->m_uRemainBlock != 0);
	}
	else {
		Assert("Not support mode");
	}
	// wait for transfer complete.
	SDHC_INT_WAIT_CLEAR( sCh, 1, ignore );
	sCh->m_uRemainBlock = 0;

	return 0;
}
//////////
// File Name : SDHC_CloseMedia
// File Description : This function close media session.
// Input : SDHC
// Output : NONE.
void SDHC_CloseMedia(void)
{
	//puts("over");
	
	SDHC* sCh = &SDHC_descriptor;
	SDHC_SetSdClockOnOff(FALSE, sCh); // SDCLK Disable

}

U8 SDHC_IdentifyCard(SDHC* sCh)
{
	SDHC_Set_InitClock( sCh );	// initial Clock Setting.

//	SDOutp8( sCh->m_uBaseAddr+SDHC_TIMEOUT_CTRL, 
//		(SDInp8(sCh->m_uBaseAddr+SDHC_TIMEOUT_CTRL)&(0xF<<4))|0xe);	// Timeout setting.
	SDOutp8( sCh->m_uBaseAddr+SDHC_TIMEOUT_CTRL, 
		(SDInp8(sCh->m_uBaseAddr+SDHC_TIMEOUT_CTRL)&(0xF<<4))|0xE);	// Timeout setting.
	SDOutp8( sCh->m_uBaseAddr+SDHC_HOST_CTRL, 0);					// NORMAL Speed mode. and SDMA selected.
	SDHC_SetHostCtrlSpeedMode(SDHC_NORMAL_SPEED, sCh);
	//OS_interrupt_install( sCh->m_ucIntChannelNum, sCh->m_fIntFn );
	//OS_interrupt_umask( sCh->m_ucIntChannelNum );

	SDHC_SetSdhcInterruptEnable(0x3F7, 0xFF, 0x0, 0x0, sCh);	// except DMA interrupt
	// Check card OCR(Operation Condition Register)
	if (SDHC_SetSDOCR(sCh))
		sCh->m_eCardType = SDHC_SD_CARD;
	else
		return FALSE;

	// Check the attached card and place the card in the IDENT state rHM_RSPREG0
	SDHC_IssueCommand( sCh, 2, 0, SDHC_CMD_BCR_TYPE, SDHC_RES_R2_TYPE );
   
	printf("Product Name : %c%c%c%c%c%c\n",((SDInp32(sCh->m_uBaseAddr+SDHC_RSP2)>>24)&0xFF),
		((SDInp32(sCh->m_uBaseAddr+SDHC_RSP2)>>16)&0xFF),
		((SDInp32(sCh->m_uBaseAddr+SDHC_RSP2)>>8)&0xFF),
		(SDInp32(sCh->m_uBaseAddr+SDHC_RSP2)&0xFF),
		((SDInp32(sCh->m_uBaseAddr+SDHC_RSP1)>>24)&0xFF),
		((SDInp32(sCh->m_uBaseAddr+SDHC_RSP1)>>16)&0xFF));
	
	// Send RCA(Relative Card Address). It places the card in the STBY state
	sCh->m_uRca= (sCh->m_eCardType==SDHC_MMC_CARD) ? (1): (0);
	SDHC_IssueCommand( sCh, 3, sCh->m_uRca<<16, SDHC_CMD_AC_TYPE, SDHC_RES_R1_TYPE );
	if( sCh->m_eCardType == SDHC_SD_CARD)
	{
		sCh->m_uRca = (SDInp32(sCh->m_uBaseAddr+SDHC_RSP0)>>16)&0xFFFF;
		//CONSOL_Printf("RCA=0x%x\n", sCh->m_uRca);
	}
									
	//Send CSD
	SDHC_IssueCommand( sCh, 9, sCh->m_uRca<<16, SDHC_CMD_AC_TYPE, SDHC_RES_R2_TYPE );
	SDHC_DisplayCardInformation(sCh);

	return TRUE;
}

//////////
// File Name : SDHC_SetSdCardSpeedMode
// File Description : Setting speed mode inside SD card.
// Input : SDHC, SDHC_SpeedMode, temp buffer for operating up to 512 byte.
// Output : NONE.
U8 SDHC_SetSdCardSpeedMode(SDHC_SpeedMode eSpeedMode, SDHC* sCh)
{
	U32 buffer[16];
	U32 uArg = 0;
	U32 ignore;
	
	puts("SDHC_SetSdCardSpeedMode\n");

	// CMD16
	if(!SDHC_IssueCommand( sCh, 16, 64, SDHC_CMD_AC_TYPE, SDHC_RES_R1_TYPE ) ) {
		//CONSOL_Printf("CMD16 fail\n");
		return FALSE;
	}

	SDHC_SetBlockSizeReg(sCh, 7, 64);
	SDHC_SetBlockCountReg(sCh, 1);
	SDHC_SetTransferModeReg(0, 1, 0, 0, 0,sCh);
	sCh->m_uRemainBlock = 1;
	sCh->m_uBufferPtr = buffer;

	// High Speed = 1, Normal Speed = 0
	uArg = ((unsigned)0x1<<31)|(0xffff<<8)|(eSpeedMode);

	if( !SDHC_IssueCommand( sCh, 6, uArg, SDHC_CMD_ADTC_TYPE, SDHC_RES_R1_TYPE ) ) {
		//CONSOL_Printf("CMD6 fail\n");
		puts("CMD6 fail\n");
		return FALSE;
	}
	SDHC_ReadOneBlock( (U32)sCh->m_uBufferPtr );
	SDHC_INT_WAIT_CLEAR( sCh, 1, ignore ); // SDHC_TRANSFERCOMPLETE_STS_INT_EN
	
	if ( buffer[3] & (1<<9) ) { // Function Group 1 <- access mode.
		//CONSOL_Printf( "This Media support high speed mode.\n" );
		printf( "This Media support high speed mode.\n" );
	}
	else {
		//CONSOL_Printf( "This Media can't support high speed mode.\n" );
		printf( "This Media can't support high speed mode.\n" );
	}
	
	return TRUE;
}


//////////
// File Name : SDHC_SetHostCtrlSpeedMode
// File Description : Set SD/MMC Host Speed
// Input : Speed mode, SDHC Channel
// Output : NONE
void SDHC_SetHostCtrlSpeedMode(SDHC_SpeedMode eSpeedMode, SDHC* sCh)
{

		SDOutp8( sCh->m_uBaseAddr+SDHC_HOST_CTRL,
			SDInp8(sCh->m_uBaseAddr+SDHC_HOST_CTRL)&~(1<<2) );	// Normal Speed mode.
}

//////////
// File Name : SDHC_SetTransferModeReg
// File Description : Set host controller transfer mode.
// Input : multi block, read or write, auto command 12 on/off, block count enable or not, DMA enable or not, SDHC Channel
// Output : NONE
void SDHC_SetTransferModeReg(U32 MultiBlk, U32 DataDirection, U32 AutoCmd12En, U32 BlockCntEn, U32 DmaEn, SDHC* sCh)
{
	sCh->m_usTransMode = (U16)((MultiBlk<<5)|(DataDirection<<4)|(AutoCmd12En<<2)|(BlockCntEn<<1)|(DmaEn<<0));
	SDOutp16( sCh->m_uBaseAddr+SDHC_TRANS_MODE, sCh->m_usTransMode);
}

//////////
// File Name : SDHC_SetSdClockOnOff
// File Description : On/Off Sd Clock
// Input : On or Off, SDHC Channel
// Output : NONE.
void SDHC_SetSdClockOnOff(U8 uOnOff, SDHC* sCh)
{
	if (uOnOff == FALSE)
	{
		SDOutp16( sCh->m_uBaseAddr+SDHC_CLK_CTRL,
			SDInp16(sCh->m_uBaseAddr+SDHC_CLK_CTRL)&(~(1<<2)) );	// SD Clock disable
	}
	else
 	{
		SDOutp16( sCh->m_uBaseAddr+SDHC_CLK_CTRL,
			SDInp16(sCh->m_uBaseAddr+SDHC_CLK_CTRL)|(1<<2) );		// SD Clock enable
		while ( !( SDInp16( sCh->m_uBaseAddr+SDHC_CLK_CTRL ) & (1<<3) ) );// SDHC_clockSource is Stable
	}
}

//////////
// File Name : SDHC_SetSdClock
// File Description : Get SD OCR Register from SD Card.
// Input : SDHC channel
// Output : success or failure.
void SDHC_SetSdClock(SDHC* sCh, SDHC_SpeedMode speed)
{
	if(speed==SDHC_NORMAL_SPEED) {
		SDOutp32( sCh->m_uBaseAddr+SDHC_CONTROL2, (1<<30)|(0<<15)|(0<<14)|(0x1<<8)|(sCh->m_eClockSource<<4) );
		SDOutp32( sCh->m_uBaseAddr+SDHC_CONTROL3, (0<<31)|(0<<23)|(0<<15)|(0<<7) );
	}
	else {
		// SD : Setup Time 6ns, Hold Time 2ns
		if ( sCh->m_eCardType == SDHC_SD_CARD || sCh->m_eCardType == SDHC_SDIO_CARD ) {
			SDOutp32( sCh->m_uBaseAddr+SDHC_CONTROL2, (1<<30)|(0<<15)|(0<<14)|(0x1<<8)|(sCh->m_eClockSource<<4) );
			SDOutp32( sCh->m_uBaseAddr+SDHC_CONTROL3, (0<<31)|(0<<23)|(0<<15)|(0<<7) );
		}
		else {
			Assert( "Not support card type");
		}
	}

	// SDCLK Value Setting + Internal Clock Enable
	SDOutp16( sCh->m_uBaseAddr+SDHC_CLK_CTRL,
		(SDInp16(sCh->m_uBaseAddr+SDHC_CLK_CTRL)&(~(0xff<<8)))|(sCh->m_uClockDivision<<8)|(1<<0) );

	// CheckInternalClockStable
	while (!(SDInp16( sCh->m_uBaseAddr+SDHC_CLK_CTRL )&0x2));

	SDHC_SetSdClockOnOff( TRUE, sCh);
	debug("rHM_CONTROL2 = %x\n",SDInp32( sCh->m_uBaseAddr+SDHC_CONTROL2 ));
	debug("rHM_CLKCON = %x\n",SDInp16( sCh->m_uBaseAddr+SDHC_CLK_CTRL ));
}


//////////
// File Name : SDHC_SetDataTransferWidth
// File Description : Set SD/MMC Host and Card data transfer width.
// Input : SDHC Channel
// Output : success or failure.
U8 SDHC_SetDataTransferWidth( SDHC* sCh)
{
//	U32 uArg=0;

	SDHC_SetSdhcCardIntEnable(FALSE, sCh); // Disable sd card interrupt

	// bandwidth check
	if ( sCh->m_eCardType == SDHC_SD_CARD ) {
			sCh->m_ucBandwidth = 4;
	}
	// SDHC_SD_CARD
	if ( sCh->m_eCardType == SDHC_SD_CARD ) {
		// Application Command.
		if (!SDHC_IssueCommand( sCh, 55, sCh->m_uRca<<16, SDHC_CMD_AC_TYPE, SDHC_RES_R1_TYPE) )
			return FALSE;

		// ACMD6 - Set Bus Width. 0->1bit, 2->4bit
		if( !SDHC_IssueCommand( sCh, 6, (sCh->m_ucBandwidth==1)?(0):(2), SDHC_CMD_AC_TYPE, SDHC_RES_R1_TYPE ) )
			return FALSE;
	}

	// default 1 bit bus mode...
	SDOutp8( sCh->m_uBaseAddr+SDHC_HOST_CTRL,
		SDInp8( sCh->m_uBaseAddr+SDHC_HOST_CTRL)& ~((1<<5)|(1<<1)) );
	if( sCh->m_ucBandwidth == 8 ) {
		SDOutp8( sCh->m_uBaseAddr+SDHC_HOST_CTRL,
			SDInp8( sCh->m_uBaseAddr+SDHC_HOST_CTRL)|(1<<5) ); // 8 bit bus mode.
	}
	else if( sCh->m_ucBandwidth == 4 ) {
		SDOutp8( sCh->m_uBaseAddr+SDHC_HOST_CTRL,
			SDInp8( sCh->m_uBaseAddr+SDHC_HOST_CTRL)|(1<<1) ); // 4 bit bus mode.
	}
	
	SDHC_SetSdhcCardIntEnable(TRUE, sCh);  // Enable sd card interrupt

	return TRUE;
}


//////////
// File Name : SDHC_WaitForCard2TransferState
// File Description : Get error source data. 
// Input : SDHC channel
// Output : Success or Failure
U8 SDHC_WaitForCard2TransferState(SDHC* sCh) {
	U32 uStatus;

	// do until programming status.
	do {
		if ( !SDHC_IssueCommand( sCh, 13, sCh->m_uRca<<16, SDHC_CMD_AC_TYPE, SDHC_RES_R1B_TYPE) ) {
			return FALSE;
		}
		uStatus = (SDInp32( sCh->m_uBaseAddr+SDHC_RSP0)>>9) & 0xf;
	} while(uStatus==7||uStatus==6);

	return (uStatus==4) ? TRUE : FALSE;
}

void SDHC_Set_InitClock( SDHC* sCh ) {
	SDOutp32( sCh->m_uBaseAddr+SDHC_CONTROL2, (0x0<<15)|(0x0<<14)|(0x1<<8)|(SDHC_HCLK<<4) );
	SDOutp32( sCh->m_uBaseAddr+SDHC_CONTROL3, (0<<31) | (0<<23) | (0<<15) | (0<<7) );

	// SDCLK Value Setting + Internal Clock Enable
	SDOutp16( sCh->m_uBaseAddr+SDHC_CLK_CTRL,
		(SDInp16(sCh->m_uBaseAddr+SDHC_CLK_CTRL)&(~(0xff<<8)))|(0x80<<8)|(1<<0) );

	// CheckInternalClockStable
	while (!(SDInp16( sCh->m_uBaseAddr+SDHC_CLK_CTRL )&0x2));
	SDHC_SetSdClockOnOff( TRUE, sCh);
	//CONSOL_Printf("rHM_CONTROL2 = %x\n",SDInp32( sCh->m_uBaseAddr+SDHC_CONTROL2 ));
	//CONSOL_Printf("rHM_CLKCON = %x\n",SDInp16( sCh->m_uBaseAddr+SDHC_CLK_CTRL ));
}

//////////
// File Name : SDHC_ReadOneBlock
// File Description : This function reads one block data by CPU transmission mode.
// Input : SDHC( assert buffer pointer and remain data length.)
// Output : NONE.
void SDHC_ReadOneBlock(U32 uBufAddr) {
	SDHC* sCh = &SDHC_descriptor;
	U32* target_Ptr = (U32 *)uBufAddr;
	int block_size;
	int i;
	
	//delay();
	//puts("SDHC_ReadOneBlock : ");
#if 0
	puts("SDHC_ReadOneBlock : ");
	putx(uBufAddr);
	puts("\n");
#endif
	
	block_size = SDInp16( (sCh)->m_uBaseAddr + SDHC_BLK_SIZE ) & 0xFFF;
	
	// Wait for buffer read ready...- SDHC_BUFFER_READREADY_STS_INT_EN
	SDHC_INT_WAIT_CLEAR( sCh, 5, i );
	//if ( 100000 - i > 1 )
		//CONSOL_Printf( "100000 to time:%d\n", 100000-i);
		
	block_size = (block_size+3) >> 2;	// block_size = (block_size+3) / 4;
	for(i=block_size; i>0; i--)
	{
		*target_Ptr++ = SDInp32( sCh->m_uBaseAddr+SDHC_BUF_DAT_PORT );
	}

	sCh->m_uRemainBlock--;
	sCh->m_uBufferPtr = target_Ptr;	
}


//////////
// File Name : SDHC_DisplayCardInformation
// File Description : Display and set card CSD information after CSD command.
// Input : SDHC.
// Output : NONE.
void SDHC_DisplayCardInformation(SDHC * sCh)
{
	U32 CardSize, OneBlockSize;
	
	if(sCh->m_eCardType == SDHC_MMC_CARD)
	{
		sCh->m_ucSpecVer=(SDInp32( sCh->m_uBaseAddr+SDHC_RSP3 )>>18)& 0xF;
		
		debug("m_ucSpecVer=%d\n", sCh->m_ucSpecVer);
	}

	sCh->m_sReadBlockLen = (U16)((SDInp32( sCh->m_uBaseAddr+SDHC_RSP2 )>>8) & 0xf);
	sCh->m_sReadBlockPartial = (U16)((SDInp32( sCh->m_uBaseAddr+SDHC_RSP2 )>>7) & 0x1);
	sCh->m_sCSize = (U16)(((SDInp32( sCh->m_uBaseAddr+SDHC_RSP2 ) & 0x3) << 10) | ((SDInp32( sCh->m_uBaseAddr+SDHC_RSP1 ) >> 22) & 0x3ff));
	sCh->m_sCSizeMult = (U16)((SDInp32( sCh->m_uBaseAddr+SDHC_RSP1 )>>7)&0x7);
	
	CardSize = ((U32)(1<<sCh->m_sReadBlockLen))*(sCh->m_sCSize+1)*(1<<(sCh->m_sCSizeMult+2))/1048576;
	OneBlockSize = (1<<sCh->m_sReadBlockLen);
	SDHC_global_card_size = ((U32)(1<<sCh->m_sReadBlockLen))*(sCh->m_sCSize+1)*(1<<(sCh->m_sCSizeMult+2))/512;

#if 0	
	puts("OneBlockSize\r\n");	
	putx(OneBlockSize);
	puts("CardSize\r\n");	
	putx(CardSize+1);
	puts("CardSize\r\n");	
	putx(SDHC_global_card_size);
	puts("\r\n");
	putx((SDInp32( sCh->m_uBaseAddr+SDHC_RSP0 )));
	puts("\r\n");
	putx((SDInp32( sCh->m_uBaseAddr+SDHC_RSP1 )));
	puts("\r\n");
	putx((SDInp32( sCh->m_uBaseAddr+SDHC_RSP2 )));
	puts("\r\n");
	putx((SDInp32( sCh->m_uBaseAddr+SDHC_RSP3 )));
	puts("\r\n");
#endif	
	//CONSOL_Printf("READ_BL_LEN: %d\n",sCh->m_sReadBlockLen);
	//CONSOL_Printf("READ_BL_PARTIAL: %d\n",sCh->m_sReadBlockPartial);
	//CONSOL_Printf("C_SIZE: %d\n",sCh->m_sCSize);
	//CONSOL_Printf("C_SIZE_MULT: %d\n",sCh->m_sCSizeMult);

	printf("One Block Size: 0x%xByte\n",OneBlockSize);
	printf("Total Card Size: 0x%xMByte\n",CardSize+1);
	printf("global_card_size: 0x%xMByte\n", SDHC_global_card_size);
	printf("SDHC_RSP0: %x\n", SDInp32( sCh->m_uBaseAddr+SDHC_RSP0));
	printf("SDHC_RSP1: %x\n", SDInp32( sCh->m_uBaseAddr+SDHC_RSP1));
	printf("SDHC_RSP2: %x\n", SDInp32( sCh->m_uBaseAddr+SDHC_RSP2));
	printf("SDHC_RSP3: %x\n", SDInp32( sCh->m_uBaseAddr+SDHC_RSP3));
}
