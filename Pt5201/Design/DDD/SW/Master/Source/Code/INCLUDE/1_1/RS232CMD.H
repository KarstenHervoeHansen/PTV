/****************************************************************************/
/* MODULE:                                                                  */
/*  rs232cmd.h -									    										 */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
/*                                                                          */
/* TASKS:                                                                   */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
/*
 *   PTV software for PT5201
 *   Copyright (c)
 *   ProTeleVision Technologies A/S.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/

#include "define.h"

struct CMD_STRUCT {
	char code* LongName;
	char code* ShortName;

	struct CMD_STRUCT code* SameLevel;
	struct CMD_STRUCT code* NextLevel;

	void ( *Suffix)(void);
	void ( *Function)(void);
};

struct PAR_STRUCT {
	char code* LongName;
	char code* ShortName;

	struct PAR_STRUCT code* SameLevel;
};

extern struct CMD_STRUCT code Mandated[12];

extern struct CMD_STRUCT code Subsystem[9];

extern struct CMD_STRUCT code System[6];
extern struct CMD_STRUCT code SystemLevel1[7];

extern struct CMD_STRUCT code Status[3];
extern struct CMD_STRUCT code StatusLevel1_1[3];
extern struct CMD_STRUCT code StatusLevel1_2[3];

extern struct CMD_STRUCT code Input[1];
extern struct CMD_STRUCT code GenlockLevel1[3];
extern struct CMD_STRUCT code GenlockLevel1_1[3];

extern struct CMD_STRUCT code Output[3];
extern struct CMD_STRUCT code BBGenLevel1[3];
extern struct CMD_STRUCT code TSGenLevel1[5];
extern struct CMD_STRUCT code TSGenLevel1_1[2];
extern struct CMD_STRUCT code AudioGenLevel1[3];
extern struct CMD_STRUCT code AudioGenLevel1_1[3];
extern struct CMD_STRUCT code AudioGenLevel1_2[6];

extern struct CMD_STRUCT code Factory[12];

extern struct CMD_STRUCT code FactoryLevel1[2];

extern struct CMD_STRUCT code FactoryLevel2[4];
extern struct CMD_STRUCT code FactoryLevel2_1[2];
extern struct CMD_STRUCT code FactoryLevel2_2[4];

extern struct CMD_STRUCT code FactoryLevel3[6];
extern struct CMD_STRUCT code FactoryLevel3_1[2];
extern struct CMD_STRUCT code FactoryLevel3_2[2];
extern struct CMD_STRUCT code FactoryLevel3_3[2];
extern struct CMD_STRUCT code FactoryLevel3_4[2];

extern struct CMD_STRUCT code FactoryLevel4[6];
extern struct CMD_STRUCT code FactoryLevel5[5];
extern struct CMD_STRUCT code FactoryLevel6[3];
extern struct CMD_STRUCT code FactoryLevel7[1];
extern struct CMD_STRUCT code FactoryLevel8[1];
extern struct CMD_STRUCT code FactoryLevel9[5];
