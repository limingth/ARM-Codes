
typedef unsigned int U32;
typedef unsigned short U16;
typedef unsigned char U8;

#define rGPGCON		(*(volatile unsigned int *)(0xE02001A0))
#define rGPGPUD		(*(volatile unsigned int *)(0xE02001A8))

#define ELFIN_HSMMC_0_BASE		0xEB000000
#define ELFIN_HSMMC_1_BASE		0xEB100000
#define ELFIN_HSMMC_2_BASE		0xEB200000

#define oSDMASYSAD				0x000
#define oBLKSIZE				0x004
#define oBLKCNT					0x006
#define oARGUMENT				0x008
#define oTRNMOD					0x00c
#define oCMDREG					0x00e
#define oRSPREG0				0x010
#define oRSPREG1				0x014
#define oRSPREG2				0x018
#define oRSPREG3				0x01c
#define oBDATA					0x020
#define oPRNSTS					0x024
#define oHOSTCTL				0x028
#define oPWRCON					0x029
#define oBLKGAP					0x02a
#define oWAKCON					0x02b
#define oCLKCON					0x02c
#define oTIMEOUTCON				0x02e
#define oSWRST					0x02f
#define oNORINTSTS				0x030
#define oERRINTSTS				0x032
#define oNORINTSTSEN			0x034
#define oERRINTSTSEN			0x036
#define oNORINTSIGEN			0x038
#define oERRINTSIGEN			0x03a
#define oACMD12ERRSTS			0x03c
#define oCAPAREG				0x040
#define oMAXCURR				0x048
#define oFEAER					0x050
#define oFEERR					0x052
#define oADMAERR				0x054
#define oADMASYSADDR			0x058
#define oCONTROL2				0x080
#define oCONTROL3				0x084
#define oCONTROL4				0x08c
#define oHCVER					0x0fe

#define SD0SDMASYSAD			(ELFIN_HSMMC_0_BASE + oSDMASYSAD)
#define SD0BLKSIZE				(ELFIN_HSMMC_0_BASE + oBLKSIZE)
#define SD0BLKCNT				(ELFIN_HSMMC_0_BASE + oBLKCNT)
#define SD0ARGUMENT				(ELFIN_HSMMC_0_BASE + oARGUMENT)
#define SD0TRNMOD				(ELFIN_HSMMC_0_BASE + oTRNMOD)
#define SD0CMDREG				(ELFIN_HSMMC_0_BASE + oCMDREG)
#define SD0RSPREG0				(ELFIN_HSMMC_0_BASE + oRSPREG0)
#define SD0RSPREG1				(ELFIN_HSMMC_0_BASE + oRSPREG1)
#define SD0RSPREG2				(ELFIN_HSMMC_0_BASE + oRSPREG2)
#define SD0RSPREG3				(ELFIN_HSMMC_0_BASE + oRSPREG3)
#define SD0BDATA				(ELFIN_HSMMC_0_BASE + oBDATA)
#define SD0PRNSTS				(ELFIN_HSMMC_0_BASE + oPRNSTS)
#define SD0HOSTCTL				(ELFIN_HSMMC_0_BASE + oHOSTCTL)
#define SD0PWRCON				(ELFIN_HSMMC_0_BASE + oPWRCON)
#define SD0BLKGAP				(ELFIN_HSMMC_0_BASE + oBLKGAP)
#define SD0WAKCON				(ELFIN_HSMMC_0_BASE + oWAKCON)
#define SD0CLKCON				(ELFIN_HSMMC_0_BASE + oCLKCON)
#define SD0TIMEOUTCON			(ELFIN_HSMMC_0_BASE + oTIMEOUTCON)
#define SD0SWRST				(ELFIN_HSMMC_0_BASE + oSWRST)
#define SD0NORINTSTS			(ELFIN_HSMMC_0_BASE + oNORINTSTS)
#define SD0ERRINTSTS			(ELFIN_HSMMC_0_BASE + oERRINTSTS)
#define SD0NORINTSTSEN			(ELFIN_HSMMC_0_BASE + oNORINTSTSEN)
#define SD0ERRINTSTSEN			(ELFIN_HSMMC_0_BASE + oERRINTSTSEN)
#define SD0NORINTSIGEN			(ELFIN_HSMMC_0_BASE + oNORINTSIGEN)
#define SD0ERRINTSIGEN			(ELFIN_HSMMC_0_BASE + oERRINTSIGEN)
#define SD0ACMD12ERRSTS			(ELFIN_HSMMC_0_BASE + oACMD12ERRSTS)
#define SD0CAPAREG				(ELFIN_HSMMC_0_BASE + oCAPAREG)
#define SD0MAXCURR				(ELFIN_HSMMC_0_BASE + oMAXCURR)
#define SD0FEAER				(ELFIN_HSMMC_0_BASE + oFEAER)
#define SD0FEERR				(ELFIN_HSMMC_0_BASE + oFEERR)
#define SD0ADMAERR				(ELFIN_HSMMC_0_BASE + oADMAERR)
#define SD0ADMASYSADDR			(ELFIN_HSMMC_0_BASE + oADMASYSADDR)
#define SD0CONTROL2				(ELFIN_HSMMC_0_BASE + oCONTROL2)
#define SD0CONTROL3				(ELFIN_HSMMC_0_BASE + oCONTROL3)
#define SD0CONTROL4				(ELFIN_HSMMC_0_BASE + oCONTROL4)
#define SD0HCVER				(ELFIN_HSMMC_0_BASE + oHCVER)

#if 0

#define SD_BASE0            (0x7C200000)
#define SD_BASE1            (0x7C300000)
#define SD_BASE2            (0x7C400000)
#define rSDMASYSAD0         (*(volatile unsigned*)(SD_BASE0+0x00))
#define rSDMASYSAD1         (*(volatile unsigned*)(SD_BASE1+0x00))
#define rSDMASYSAD2         (*(volatile unsigned*)(SD_BASE2+0x00))
#define rBLKSIZE0           (*(volatile unsigned*)(SD_BASE0+0x04))
#define rBLKSIZE1           (*(volatile unsigned*)(SD_BASE1+0x04))
#define rBLKSIZE2           (*(volatile unsigned*)(SD_BASE2+0x04))
#define rBLKCNT0            (*(volatile unsigned*)(SD_BASE0+0x06))
#define rBLKCNT1            (*(volatile unsigned*)(SD_BASE1+0x06))
#define rBLKCNT2            (*(volatile unsigned*)(SD_BASE2+0x06))
#define rARGUMENT0          (*(volatile unsigned*)(SD_BASE0+0x08))
#define rARGUMENT1          (*(volatile unsigned*)(SD_BASE1+0x08))
#define rARGUMENT2          (*(volatile unsigned*)(SD_BASE2+0x08))
#define rTRNMOD0            (*(volatile unsigned*)(SD_BASE0+0x0C))
#define rTRNMOD1            (*(volatile unsigned*)(SD_BASE1+0x0C))
#define rTRNMOD2            (*(volatile unsigned*)(SD_BASE2+0x0C))
#define rCMDREG0            (*(volatile unsigned*)(SD_BASE0+0x0E))
#define rCMDREG1            (*(volatile unsigned*)(SD_BASE1+0x0E))
#define rCMDREG2            (*(volatile unsigned*)(SD_BASE2+0x0E))
#define rRSPREG0_0          (*(volatile unsigned*)(SD_BASE0+0x10))
#define rRSPREG1_0          (*(volatile unsigned*)(SD_BASE0+0x14))
#define rRSPREG2_0          (*(volatile unsigned*)(SD_BASE0+0x18))
#define rRSPREG3_0          (*(volatile unsigned*)(SD_BASE0+0x1C))
#define rRSPREG0_1          (*(volatile unsigned*)(SD_BASE1+0x10))
#define rRSPREG1_1          (*(volatile unsigned*)(SD_BASE1+0x14))
#define rRSPREG2_1          (*(volatile unsigned*)(SD_BASE1+0x18))
#define rRSPREG3_1          (*(volatile unsigned*)(SD_BASE1+0x1C))
#define rRSPREG0_2          (*(volatile unsigned*)(SD_BASE2+0x10))
#define rRSPREG1_2          (*(volatile unsigned*)(SD_BASE2+0x14))
#define rRSPREG2_2          (*(volatile unsigned*)(SD_BASE2+0x18))
#define rRSPREG3_2          (*(volatile unsigned*)(SD_BASE2+0x1C))
#define rBDATA0             (*(volatile unsigned*)(SD_BASE0+0x20))
#define rBDATA1             (*(volatile unsigned*)(SD_BASE1+0x20))
#define rBDATA2             (*(volatile unsigned*)(SD_BASE2+0x20))
#define rPRNSTS0            (*(volatile unsigned*)(SD_BASE0+0x24))
#define rPRNSTS1            (*(volatile unsigned*)(SD_BASE1+0x24))
#define rPRNSTS2            (*(volatile unsigned*)(SD_BASE2+0x24))
#define rHOSTCTL0           (*(volatile unsigned*)(SD_BASE0+0x28))
#define rHOSTCTL1           (*(volatile unsigned*)(SD_BASE1+0x28))
#define rHOSTCTL2           (*(volatile unsigned*)(SD_BASE2+0x28))
#define rPWRCON0            (*(volatile unsigned*)(SD_BASE0+0x29))
#define rPWRCON1            (*(volatile unsigned*)(SD_BASE1+0x29))
#define rPWRCON2            (*(volatile unsigned*)(SD_BASE2+0x29))
#define rBLKGAP0            (*(volatile unsigned*)(SD_BASE0+0x2A))
#define rBLKGAP1            (*(volatile unsigned*)(SD_BASE1+0x2A))
#define rBLKGAP2            (*(volatile unsigned*)(SD_BASE2+0x2A))
#define rWAKCON0            (*(volatile unsigned*)(SD_BASE0+0x2B))
#define rWAKCON1            (*(volatile unsigned*)(SD_BASE1+0x2B))
#define rWAKCON2            (*(volatile unsigned*)(SD_BASE2+0x2B))
#define rCLKCON0            (*(volatile unsigned*)(SD_BASE0+0x2C))
#define rCLKCON1            (*(volatile unsigned*)(SD_BASE1+0x2C))
#define rCLKCON2            (*(volatile unsigned*)(SD_BASE2+0x2C))
#define rTIMEOUTCON0        (*(volatile unsigned*)(SD_BASE0+0x2E))
#define rTIMEOUTCON1        (*(volatile unsigned*)(SD_BASE1+0x2E))
#define rTIMEOUTCON2        (*(volatile unsigned*)(SD_BASE2+0x2E))
#define rSWRST0             (*(volatile unsigned*)(SD_BASE0+0x2F))
#define rSWRST1             (*(volatile unsigned*)(SD_BASE1+0x2F))
#define rSWRST2             (*(volatile unsigned*)(SD_BASE2+0x2F))
#define rNORINTSTS0         (*(volatile unsigned*)(SD_BASE0+0x30))
#define rNORINTSTS1         (*(volatile unsigned*)(SD_BASE1+0x30))
#define rNORINTSTS2         (*(volatile unsigned*)(SD_BASE2+0x30))
#define rERRINTSTS0         (*(volatile unsigned*)(SD_BASE0+0x32))
#define rERRINTSTS1         (*(volatile unsigned*)(SD_BASE1+0x32))
#define rERRINTSTS2         (*(volatile unsigned*)(SD_BASE2+0x32))
#define rNORINTSTSEN0       (*(volatile unsigned*)(SD_BASE0+0x34))
#define rNORINTSTSEN1       (*(volatile unsigned*)(SD_BASE1+0x34))
#define rNORINTSTSEN2       (*(volatile unsigned*)(SD_BASE2+0x34)) 
#define rERRINTSTSEN0       (*(volatile unsigned*)(SD_BASE0+0x36)) 
#define rERRINTSTSEN1       (*(volatile unsigned*)(SD_BASE1+0x36)) 
#define rERRINTSTSEN2       (*(volatile unsigned*)(SD_BASE2+0x36)) 
#define rNORINTSIGEN0       (*(volatile unsigned*)(SD_BASE0+0x38))
#define rNORINTSIGEN1       (*(volatile unsigned*)(SD_BASE1+0x38))
#define rNORINTSIGEN2       (*(volatile unsigned*)(SD_BASE2+0x38))
#define rERRINTSIGEN0       (*(volatile unsigned*)(SD_BASE0+0x3A))
#define rERRINTSIGEN1       (*(volatile unsigned*)(SD_BASE1+0x3A))
#define rERRINTSIGEN2       (*(volatile unsigned*)(SD_BASE2+0x3A))
#define rACMD12ERRSTS0      (*(volatile unsigned*)(SD_BASE0+0x3C))
#define rACMD12ERRSTS1      (*(volatile unsigned*)(SD_BASE1+0x3C))
#define rACMD12ERRSTS2      (*(volatile unsigned*)(SD_BASE2+0x3C))
#define rCAPAREG0           (*(volatile unsigned*)(SD_BASE0+0x40))
#define rCAPAREG1           (*(volatile unsigned*)(SD_BASE1+0x40))
#define rCAPAREG2           (*(volatile unsigned*)(SD_BASE2+0x40))
#define rMAXCURR0           (*(volatile unsigned*)(SD_BASE0+0x48))
#define rMAXCURR1           (*(volatile unsigned*)(SD_BASE1+0x48))
#define rMAXCURR2           (*(volatile unsigned*)(SD_BASE2+0x48))
#define rCONTROL2_0         (*(volatile unsigned*)(SD_BASE0+0x80))
#define rCONTROL2_1         (*(volatile unsigned*)(SD_BASE1+0x80))
#define rCONTROL2_2         (*(volatile unsigned*)(SD_BASE2+0x80))
#define rCONTROL3_0         (*(volatile unsigned*)(SD_BASE0+0x84))
#define rCONTROL3_1         (*(volatile unsigned*)(SD_BASE1+0x84))
#define rCONTROL3_2         (*(volatile unsigned*)(SD_BASE2+0x84))
#define rCONTROL4_0         (*(volatile unsigned*)(SD_BASE0+0x8C))
#define rCONTROL4_1         (*(volatile unsigned*)(SD_BASE1+0x8C))
#define rCONTROL4_2         (*(volatile unsigned*)(SD_BASE2+0x8C))
#define rFEAER0             (*(volatile unsigned*)(SD_BASE0+0x50))
#define rFEAER1             (*(volatile unsigned*)(SD_BASE1+0x50))
#define rFEAER2             (*(volatile unsigned*)(SD_BASE2+0x50))
#define rFEERR0             (*(volatile unsigned*)(SD_BASE0+0x52))
#define rFEERR1             (*(volatile unsigned*)(SD_BASE1+0x52))
#define rFEERR2             (*(volatile unsigned*)(SD_BASE2+0x52))
#define rADMAERR0           (*(volatile unsigned*)(SD_BASE0+0x54))
#define rADMAERR1           (*(volatile unsigned*)(SD_BASE1+0x54))
#define rADMAERR2           (*(volatile unsigned*)(SD_BASE2+0x54))
#define rADMASYSADDR0       (*(volatile unsigned*)(SD_BASE0+0x58))
#define rADMASYSADDR1       (*(volatile unsigned*)(SD_BASE1+0x58))
#define rADMASYSADDR2       (*(volatile unsigned*)(SD_BASE2+0x58))
#define rHCVER0             (*(volatile unsigned*)(SD_BASE0+0xFE))
#define rHCVER1             (*(volatile unsigned*)(SD_BASE1+0xFE))
#define rHCVER2             (*(volatile unsigned*)(SD_BASE2+0xFE))

#endif