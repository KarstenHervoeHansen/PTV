/**********************************************************************/
/*   ____  ____                                                       */
/*  /   /\/   /                                                       */
/* /___/  \  /                                                        */
/* \   \   \/                                                       */
/*  \   \        Copyright (c) 2003-2009 Xilinx, Inc.                */
/*  /   /          All Right Reserved.                                 */
/* /---/   /\                                                         */
/* \   \  /  \                                                      */
/*  \___\/\___\                                                    */
/***********************************************************************/

/* This file is designed for use with ISim build 0xcb73ee62 */

#define XSI_HIDE_SYMBOL_SPEC true
#include "xsi.h"
#include <memory.h>
#ifdef __GNUC__
#include <stdlib.h>
#else
#include <malloc.h>
#define alloca _alloca
#endif
static const char *ng0 = "C:/PT8616/development_src/FPGA/digital_phase_loop_wmults.vhd";
extern char *IEEE_P_0774719531;

char *ieee_p_0774719531_sub_767668596_2162500114(char *, char *, char *, char *, char *, char *);


static void work_a_1460920829_3212880686_p_0(char *t0)
{
    char t11[16];
    char t24[16];
    char t29[16];
    unsigned char t1;
    char *t2;
    unsigned char t3;
    char *t4;
    char *t5;
    unsigned char t6;
    unsigned char t7;
    char *t8;
    unsigned char t9;
    unsigned char t10;
    char *t12;
    char *t13;
    char *t14;
    char *t15;
    char *t16;
    char *t17;
    char *t18;
    char *t19;
    char *t20;
    unsigned int t21;
    unsigned int t22;
    unsigned int t23;
    int t25;
    unsigned int t26;
    unsigned int t27;
    unsigned int t28;
    int t30;
    unsigned int t31;

LAB0:    xsi_set_current_line(57, ng0);
    t2 = (t0 + 568U);
    t3 = xsi_signal_has_event(t2);
    if (t3 == 1)
        goto LAB5;

LAB6:    t1 = (unsigned char)0;

LAB7:    if (t1 != 0)
        goto LAB2;

LAB4:
LAB3:    t2 = (t0 + 2184);
    *((int *)t2) = 1;

LAB1:    return;
LAB2:    xsi_set_current_line(58, ng0);
    t4 = (t0 + 776U);
    t8 = *((char **)t4);
    t9 = *((unsigned char *)t8);
    t10 = (t9 == (unsigned char)3);
    if (t10 != 0)
        goto LAB8;

LAB10:
LAB9:    xsi_set_current_line(64, ng0);
    t2 = (t0 + 1144U);
    t4 = *((char **)t2);
    t21 = (27 - 27);
    t22 = (t21 * 1U);
    t23 = (0 + t22);
    t2 = (t4 + t23);
    t5 = (t24 + 0U);
    t8 = (t5 + 0U);
    *((int *)t8) = 27;
    t8 = (t5 + 4U);
    *((int *)t8) = 12;
    t8 = (t5 + 8U);
    *((int *)t8) = -1;
    t25 = (12 - 27);
    t26 = (t25 * -1);
    t26 = (t26 + 1);
    t8 = (t5 + 12U);
    *((unsigned int *)t8) = t26;
    t8 = (t0 + 1236U);
    t12 = *((char **)t8);
    t26 = (27 - 15);
    t27 = (t26 * 1U);
    t28 = (0 + t27);
    t8 = (t12 + t28);
    t13 = (t29 + 0U);
    t14 = (t13 + 0U);
    *((int *)t14) = 15;
    t14 = (t13 + 4U);
    *((int *)t14) = 0;
    t14 = (t13 + 8U);
    *((int *)t14) = -1;
    t30 = (0 - 15);
    t31 = (t30 * -1);
    t31 = (t31 + 1);
    t14 = (t13 + 12U);
    *((unsigned int *)t14) = t31;
    t14 = ieee_p_0774719531_sub_767668596_2162500114(IEEE_P_0774719531, t11, t2, t24, t8, t29);
    t15 = (t0 + 2264);
    t16 = (t15 + 32U);
    t17 = *((char **)t16);
    t18 = (t17 + 40U);
    t19 = *((char **)t18);
    memcpy(t19, t14, 16U);
    xsi_driver_first_trans_fast_port(t15);
    goto LAB3;

LAB5:    t4 = (t0 + 592U);
    t5 = *((char **)t4);
    t6 = *((unsigned char *)t5);
    t7 = (t6 == (unsigned char)3);
    t1 = t7;
    goto LAB7;

LAB8:    xsi_set_current_line(60, ng0);
    t4 = (t0 + 1144U);
    t12 = *((char **)t4);
    t4 = (t0 + 4336U);
    t13 = (t0 + 1328U);
    t14 = *((char **)t13);
    t13 = (t0 + 4368U);
    t15 = ieee_p_0774719531_sub_767668596_2162500114(IEEE_P_0774719531, t11, t12, t4, t14, t13);
    t16 = (t0 + 2228);
    t17 = (t16 + 32U);
    t18 = *((char **)t17);
    t19 = (t18 + 40U);
    t20 = *((char **)t19);
    memcpy(t20, t15, 28U);
    xsi_driver_first_trans_fast(t16);
    goto LAB9;

}


extern void work_a_1460920829_3212880686_init()
{
	static char *pe[] = {(void *)work_a_1460920829_3212880686_p_0};
	xsi_register_didat("work_a_1460920829_3212880686", "isim/test_ploop_isim_beh.exe.sim/work/a_1460920829_3212880686.didat");
	xsi_register_executes(pe);
}
