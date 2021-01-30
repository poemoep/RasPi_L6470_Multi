/* =============================================                    */
/* Copyright (c) 2021 poemoep                                       */
/* This software is released under the MIT License, see LICENSE.    */
/* =============================================                    */

#include "L6470.h"

#ifndef __GENERATOR_H__
#define __GENERATOR_H__

/* daisy cain device num */
#define L6470_DEV_NUM       (2) 


/* All parametors are generated same value. */

/* ABS_POS = -2097152 to 2097151 (-2^21 ~ +2^21-1) [step]*/
#define USER_ABS_POS		(20000)

/* EL_POS = (STEP_ePOS_0deg | STEP_ePOS_90deg | STEP_ePOS_180deg | STEP_ePOS_270deg) */
/*       |= uStep(0 to 127) */ 
#define USER_EL_POS		    (STEP_ePOS_90deg | 63)

/* MARK = -2097152 to 2097151 (-2^21 ~ +2^21-1) [step]*/
#define USER_MARK		    (-2000)

/* ACC = 1455 to 5959000 [x0.01 step/s^2]*/
#define USER_ACC		    (803200)

/* DEC = 1455 to 5959000 [x0.01 step/s^2]*/
#define USER_DEC		    (200800)

/* MAX_SPEED = 1525 to 1561000 [x0.01 step/s]*/
#define USER_MAX_SPEED		(1561000)

/* MIX_SPEED = 0 to 976300 [x0.001 step/s]*/
/*          |= ENABLE_LSPD (if use LSPD)*/
#define USER_MIN_SPEED		(ENABLE_LSPD |200000)

/*KVA_HOLD = 0 to 10000 [0.01%] */
#define USER_KVAL_HOLD		(1602)

/*KVA_HOLD = 0 to 10000 [0.01%] */
#define USER_KVAL_RUN		(3000)

/*KVA_HOLD = 0 to 10000 [0.01%] */
#define USER_KVAL_ACC		(4000)

/*KVA_HOLD = 0 to 10000 [0.01%] */
#define USER_KVAL_DEC		(4000)

/* INT_SPEED = 0 to 9764000 [x0.0001 step/s] */
#define USER_INT_SPEED		(9765000)

/* ST_SLP = 0 to 3825 [x0.000001% s/step] */
#define USER_ST_SLP		    (3825)

/* FN_SLP_ACC = 0 to 3825 [x0.000001 ] */
#define USER_FN_SLP_ACC		(3825)

/* FN_SLP_DEC = 0 to 3825 [x0.000001 ] */
#define USER_FN_SLP_DEC		(3825)

/* K_THERM = 100000 to 146875 [x0.00001]*/
#define USER_K_THERM		(1.46875 * 100000)

/* OCD_TH = 375 to 6000 [mA]*/
#define USER_OCD_TH		    (3.38 * 1000)

/* STALL_TH = 3125 to 400000 [x 0.01 mA]*/
#define USER_STALL_TH		(2030 * 100)

/* FS_SPD = 763 to 1562500 [x0.01 step/s]*/
#define USER_FS_SPD		    (1000)

/* USER_STEP_MODE = SYNC_EN (if use SYNC_EN) */
/*               |= (SYNC_FFS_2DIV | SYNC_FFS | SYNC_FFS_2TIMES | SYNC_FFS_4TIMES | SYNC_FFS_8TIMES | SYNC_FFS_16TIMES | SYNC_FFS_32TIMES | SYNC_FFS_64TIMES) */
/*              |= (STEP_FULL | STEP_HALF | STEP_4DIV | STEP_8DIV | STEP_16DIV | STEP_32DIV | STEP_64DIV | STEP_128DIV) */ 
#define USER_STEP_MODE		(STEP_4DIV)

/* ALARM_EN = ALM_FULL (if use all alarm) */
/*          = ALM_NOTHING (if use no alarm) */
/*          = (ALM_OVERCURRENT | ALM_THERMAL_SHUTDOWN | ALM_THERMAL_WARNING | ALM_UDERVOLTAGE | ALM_STALL_A | ALM_STALL_B | ALM_SW_TURNON |ALM_CMD_WRONG ) */
#define USER_ALARM_EN		(ALM_FULL)

/* CONFIG = F_PWM_INT(x) (see datasheet list) */
/*       |= F_PWM_DEC(x) (see datasheet list) */
/*       |= [POW_SR_VFAST | POW_SR_FAST | POW_SR_MIDDLE | POW_SW_SLOW ] */
/*       |= ENABLE_OC_SD (if use) */
/*       |= ENABLE_VSCOMP (if use) */
/*       |= DISABLE_SW_STOPHARD_IRQ (if use)*/
/*       |= USE_INTCLK_16M (if use no XTAL or EXTCLKSRC)*/
/*       |= [USE_XTLA_8M | USE_XTLA_16M | USE_XTLA_24M | USE_XTLA_32M] (if use XTAL)*/   
/*       |= [USE_CLKOUT_2M | USE_CLKOUT_4M | USE_CLKOUT_8M | USE_CLKOUT_16M] (if use CLKOUT to other L6470) */ 
/*       |= [USE_EXTCLKSRC_8M | USE_EXTCLKSRC_16M | USE_EXTCLKSRC_24M | USE_EXTCLKSRC_32M] (if use EXTCLKSRC like another L6470) */
#define USER_CONFIG		    (USE_INTCLK_16M | USE_CLKOUT_2M | ENABLE_OC_SD | POW_SR_FAST | F_PWM_INT(0b001) | F_PWM_DEC(0b011) )


/* prototype */

union L6470_packet gen_ABS_POS(int32_t abs_pos);
union L6470_packet gen_EL_POS(int32_t step_el_step);
union L6470_packet gen_MARK(int32_t mark);
union L6470_packet gen_ACC(int32_t step_per_ss);
union L6470_packet gen_DEC(int32_t step_per_ss);
union L6470_packet gen_MAX_SPEED(int32_t step_per_s);
union L6470_packet gen_MIN_SPEED(int32_t enable_LSPD_step_per_s);
union L6470_packet gen_KVAL_HOLD(int32_t percentage);
union L6470_packet gen_KVAL_RUN(int32_t percentage);
union L6470_packet gen_KVAL_ACC(int32_t percentage);
union L6470_packet gen_KVAL_DEC(int32_t percentage);
union L6470_packet gen_INT_SPEED(int32_t step_per_s);
union L6470_packet gen_ST_SLP(int32_t slp);
union L6470_packet gen_FN_SLP_ACC(int32_t slp_acc);
union L6470_packet gen_FN_SLP_DEC(int32_t slp_dec);
union L6470_packet gen_K_THERM(int32_t k_therm);
union L6470_packet gen_OCD_TH(int32_t ocd_th);
union L6470_packet gen_STALL_TH(int32_t stall_th);
union L6470_packet gen_FS_SPD(int32_t fs_spd);
union L6470_packet gen_STEP_MODE(int32_t sync_en_sync_sel_step_sel);
union L6470_packet gen_ALARM_EN(int32_t alm);
union L6470_packet gen_CONFIG(int32_t param);



#endif
