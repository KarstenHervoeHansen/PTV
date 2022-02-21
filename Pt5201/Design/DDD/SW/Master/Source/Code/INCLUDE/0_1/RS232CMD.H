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
extern struct CMD_STRUCT code SystemLevel1[3];

extern struct CMD_STRUCT code Status[4];
extern struct CMD_STRUCT code StatusLevel1_1[3];
extern struct CMD_STRUCT code StatusLevel1_2[3];

extern struct CMD_STRUCT code Diagnostic[2];
extern struct CMD_STRUCT code DiagnosticLevel1[1];

extern struct CMD_STRUCT code Input[1];
extern struct CMD_STRUCT code GenlockLevel1[2];

extern struct CMD_STRUCT code Output[3];
extern struct CMD_STRUCT code BBGenLevel1[3];
extern struct CMD_STRUCT code CBGenLevel1[5];
extern struct CMD_STRUCT code CBGenLevel1_1[3];
extern struct CMD_STRUCT code AudioGenLevel1[7];

extern struct CMD_STRUCT code Factory[7];
extern struct CMD_STRUCT code FactoryLevel1[2];

extern struct CMD_STRUCT code FactoryLevel2[5];
extern struct CMD_STRUCT code FactoryLevel2_1[2];
extern struct CMD_STRUCT code FactoryLevel2_2[2];

extern struct CMD_STRUCT code FactoryLevel3[5];
extern struct CMD_STRUCT code FactoryLevel3_1[2];
extern struct CMD_STRUCT code FactoryLevel3_2[2];
extern struct CMD_STRUCT code FactoryLevel3_3[2];
extern struct CMD_STRUCT code FactoryLevel3_4[2];

extern struct CMD_STRUCT code FactoryLevel4[4];
extern struct CMD_STRUCT code FactoryLevel4_1[2];
extern struct CMD_STRUCT code FactoryLevel4_2[2];
extern struct CMD_STRUCT code FactoryLevel4_3[3];

extern struct CMD_STRUCT code FactoryLevel5[5];
extern struct CMD_STRUCT code FactoryLevel5_1[2];
extern struct CMD_STRUCT code FactoryLevel5_2[2];

extern struct CMD_STRUCT code FactoryLevel6[2];
extern struct CMD_STRUCT code FactoryLevel6_1[2];
