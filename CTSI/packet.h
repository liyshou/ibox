#ifndef __PACKET_H_
#define __PACKET_H_







			





/*****************************define***************************/









#define ECARD_TABLE_MAX 1
#define CMB_TABLE_MAX 13
#define ICBC_TABLE_MAX  12
#define ABC_TABLE_MAX 6	
#define PSD_TABLE_MAX 3
#define CCB_TABLE_MAX 11
#define BOC_TABLE_MAX 8
#define GDB_TABLE_MAX 2
#define NXB_TABLE_MAX 4
#define CIT_TABLE_MAX 6
#define MSB_TABLE_MAX 1
#define ZXB_TABLE_MAX 3
#define GCB_TABLE_MAX 3

extern char *CARD_TABLE_ECARD[ECARD_TABLE_MAX];
extern char *CARD_TABLE_CMB[CMB_TABLE_MAX];
extern char *CARD_TABLE_ABC[ABC_TABLE_MAX];
extern char *CARD_TABLE_CCB[CCB_TABLE_MAX];
extern char *CARD_TABLE_CIT[CIT_TABLE_MAX];
extern char *CARD_TABLE_GCB[GCB_TABLE_MAX];
/*
extern char *CARD_TABLE_ICBC[ICBC_TABLE_MAX];
extern char *CARD_TABLE_PSD[PSD_TABLE_MAX];
extern char *CARD_TABLE_BOC[BOC_TABLE_MAX];
extern char *CARD_TABLE_GDB[GDB_TABLE_MAX];
extern char *CARD_TABLE_NXB[NXB_TABLE_MAX];
extern char *CARD_TABLE_MSB[MSB_TABLE_MAX];
extern char *CARD_TABLE_ZXB[ZXB_TABLE_MAX];
*/
#endif
