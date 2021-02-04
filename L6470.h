/* =============================================                    */
/* Copyright (c) 2021 poemoep                                       */
/* This software is released under the MIT License, see LICENSE.    */
/* =============================================                    */


#ifndef __L6470_HEADER__
#define __L6470_HEADER__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

//----define inline function----
#define bit2byte(x) ( ( x + 8 - 1 ) / 8 ) 
#define ustep(x)    ( ( x & 127) )
//----define const value----

#define ADDR_SIZE             (8 )
#define L6470_TICK            (250) // ns
#define SPEED_RESOLUTION      (15) // [x0.001 step/s]
#define ACC_RESOLUTION        (1455) //[x0.01 step/s^2]
#define DEC_RESOLUTION        ACC_RESOLUTION
#define MAX_SPEED_RESOLUTION  (1525) //[x0.01 step/s]
#define MAX_SPEED_MIN         (1525) //[x0.01 step/s]
#define MIN_SPEED_RESOLUTION  (238) //[x0.001 step/s]
#define INT_SPEED_RESOLUTION  (596) //[x0.0001 step/s]
#define ST_SLP_RESOLUTION     (15) // [x0.000001]
#define FN_SLP_ACC_RESOLUTION (15) //[x0.000001]
#define FN_SLP_DEC_RESOLUTION (15) //[x0.000001]
#define K_THERM_RESOLUTION    (  3125) //[x0.00001]
#define K_THERM_MIN           (100000) // [x0.00001]
#define OCD_TH_RESOLUTION     (375) // [mA]
#define STALL_TH_RESOLUTION   (3125) // [x0.01 mA]
#define FS_SPD_RESOLUTION     (1525) // [x0.01 step/s]
#define FS_SPD_MIN            (763) // [step/s]

#define ENABLE  (1)
#define DISABLE (0)

#define MIN_SPEED_MASK    (0b11111111111111111111) /* use 20 bits */
#define ENABLE_LSPD_MAGIC (25) /* magic number */
#define ENABLE_LSPD (ENABLE << ENABLE_LSPD_MAGIC)
#define ENABLE_LSPD_BIT (ENABLE << 12)

#define KVAL_MASK   (255)

// define for EL_POS 
#define STEP_ePOS_ofset     (7)
#define STEP_ePOS_MASK      (11 << STEP_ePOS_ofset)
#define STEP_ePOS_0deg      (0b00 << STEP_ePOS_ofset)
#define STEP_ePOS_90deg     (0b01 << STEP_ePOS_ofset)
#define STEP_ePOS_180deg    (0b10 << STEP_ePOS_ofset)
#define STEP_ePOS_270deg    (0b11 << STEP_ePOS_ofset)

#define STEP_u_STEP_MASK   (127)

// define for STEP_MODE
#define SYNC_EN_ofset (7)
#define SYNC_EN     (ENABLE << SYNC_EN_ofset) //Output sync_clock from ^BUSY/SYNC
#define SYNC_SEL_ofset (4)
#define SYNC_SEL_MASK (0b0111 << SYNC_SEL_ofset)
#define STEP_SEL_MASK (0b00000111)

#define STEP_FULL   (0b000)
#define STEP_HALF   (0b001)
#define STEP_4DIV   (0b010)
#define STEP_8DIV   (0b011)
#define STEP_16DIV  (0b100)
#define STEP_32DIV  (0b101)
#define STEP_64DIV  (0b110)
#define STEP_128DIV (0b111)

#define SYNC_FFS_ofset   (4)
#define SYNC_FFS_2DIV    (0b000 << SYNC_FFS_ofset) // EL_POS[7]
#define SYNC_FFS         (0b001 << SYNC_FFS_ofset) // EL_POS[6]
#define SYNC_FFS_2TIMES  (0b010 << SYNC_FFS_ofset) // EL_POS[5]
#define SYNC_FFS_4TIMES  (0b011 << SYNC_FFS_ofset) // EL_POS[4]
#define SYNC_FFS_8TIMES  (0b100 << SYNC_FFS_ofset) // EL_POS[3]
#define SYNC_FFS_16TIMES (0b101 << SYNC_FFS_ofset) // EL_POS[2]
#define SYNC_FFS_32TIMES (0b110 << SYNC_FFS_ofset) // EL_POS[1]
#define SYNC_FFS_64TIMES (0b111 << SYNC_FFS_ofset) // EL_POS[0]

// define for ALARM
#define ALM_NOTHING          (0b00000000)
#define ALM_OVERCURRENT      (0b00000001)
#define ALM_THERMAL_SHUTDOWN (0b00000010)
#define ALM_THERMAL_WARNING  (0b00000100)
#define ALM_UDERVOLTAGE      (0b00001000)
#define ALM_STALL_A          (0b00010000)
#define ALM_STALL_B          (0b00100000)
#define ALM_SW_TURNON        (0b01000000)
#define ALM_CMD_WRONG        (0b10000000)
#define ALM_FULL             (0b11111111)

// define for CONFIG

#define OSC_SEL_ofset (0)
#define OSC_SEL_MASK  (0b111 << OSC_SEL_ofset)

#define EXT_CLK_ofset (3)
#define EXT_CLK_MASK  (1 << EXT_CLK_ofset) 

#define USE_INTCLK_16M (0b0000) /* No output clk */
#define USE_CLKOUT_2M  (0b1000) /* output 2M */
#define USE_CLKOUT_4M  (0b1001) /* output 4M */
#define USE_CLKOUT_8M  (0b1010) /* output 8M */
#define USE_CLKOUT_16M (0b1011) /* output 16M */
#define USE_XTLA_8M    (0b0100) /* Crystal or resonator */
#define USE_XTLA_16M   (0b0101) /* Crystal or resonator */
#define USE_XTLA_24M   (0b0110) /* Crystal or resonator */
#define USE_XTLA_32M   (0b0111) /* Crystal or resonator */

#define USE_EXTCLKSRC_8M  (0b1100) /* Forcibly output 8M (Inverted) */
#define USE_EXTCLKSRC_16M (0b1101) /* Forcibly output 16M (Inverted) */
#define USE_EXTCLKSRC_24M (0b1110) /* Forcibly output 24M (Inverted) */
#define USE_EXTCLKSRC_32M (0b1111) /* Forcibly output 32M (Inverted) */

#define SW_MODE_ofset                (4)
#define SW_MODE_MASK                 (1 << SW_MODE_ofset)
#define DISABLE_SW_STOPHARD_IRQ (SW_MODE_MASK)

/* VS COMPensate */
#define EN_VSCOMP_ofset (5)
#define EN_VSCOMP_MASK  (1 << EN_VSCOMP_ofset)
#define ENABLE_VSCOMP       EN_VSCOMP_MASK

/* RESERVED 6bit */

/* Overcurrent Shut Down */
#define OC_SD_ofset     (7)
#define OC_SD_MASK      (1 << OC_SD_ofset)
#define ENABLE_OC_SD    (OC_SD_MASK)  

/* POWer Sley Rate */
#define POW_SR_ofset    (8)
#define POW_SR_MASK     (1 << POW_SR_ofset)

#define POW_SR_VFAST    (0b00 << POW_SR_ofset)
#define POW_SR_FAST     (0b11 << POW_SR_ofset)
#define POW_SR_MIDDLE   (0b10 << POW_SR_ofset)
#define POW_SR_SLOW     (0b01 << POW_SR_ofset)

/* F_PWM_DEC */
#define F_PWM_DEC_ofset (10)
#define F_PWM_DEC_MASK  (0b111 << F_PWM_DEC_ofset)
#define F_PWM_DEC(x)    (x << F_PWM_DEC_ofset)

/* F_PWM_INT */
#define F_PWM_INT_ofset (13)
#define F_PWM_INT_MASK  (0b111 << F_PWM_INT_ofset)
#define F_PWM_INT(x)    (x << F_PWM_INT_ofset)


// define 
#define DIR_RVS 0x00
#define DIR_FWD 0x01

#define DIR_CCW 0x00
#define DIR_CW  0x01

#define ACT_POS_CLEAR   0b00000000
#define ACT_POS_TO_MARK 0b00001000

#define STATUS_MASK_HiZ		      0b0000000000000001
#define STATUS_MASK_BUSY	      0b0000000000000010 //Constant speed / positioning / motion command
#define STATUS_MASK_SW_F	      0b0000000000000100 //SW input status
#define STATUS_MASK_SW_EVN	    0b0000000000001000 //Normaly L, SW turn-on event
#define STATUS_MASK_DIR		      0b0000000000010000 //1: FWD, 0: RVS
#define STATUS_MASK_MOT_sTATUS 	0b0000000001100000 //00: Stop, 01: Acceleration, 10: Deceleration, 11: Constant speed
#define STATUS_MASK_NOTPERF_CMD	0b0000000010000000 //Normaly L
#define STATUS_MASK_WRONG_CMD	  0b0000000100000000 //Normaly L
#define STATUS_MASK_UVLO	      0b0000001000000000 //Normaly H
#define STATUS_MASK_TH_WRN	    0b0000010000000000 //Normaly H
#define STATUS_MASK_TH_SD	      0b0000100000000000 //Normaly H
#define STATUS_MASK_OCD		      0b0001000000000000 //Normaly H
#define STATUS_MASK_STEP_LOSS_A	0b0010000000000000 //Normaly H
#define STATUS_MASK_STEP_LOSS_B	0b0100000000000000 //Normaly H
#define STATUS_MASK_SCK_MOD	    0b1000000000000000 //Normaly L

#define MOTOR_STATUS_STOP	  0b00
#define MOTOR_STATUS_ACC	  0b01
#define MOTOR_STATUS_DEC	  0b10
#define MOTOR_STATUS_CONST	0b11

#define RESERVED  (0x00)
#define READONLY	(0x01)
#define WRITABLE	(0x10)
#define WRITABLE_HiZ	(WRITABLE | 0x02)
#define WRITABLE_MStop	(WRITABLE | 0x04)
#define WRITABLE_Always (WRITABLE | 0x08)
//----define L6470 parametor as userful number----

typedef enum {
  enum_L6470_ABS_POS=0 ,
  enum_L6470_EL_POS ,
  enum_L6470_MARK,
  enum_L6470_SPEED, //readonly
  enum_L6470_ACC ,
  enum_L6470_DEC,
  enum_L6470_MAX_SPEED,
  enum_L6470_MIN_SPEED,
  enum_L6470_KVAL_HOLD,
  enum_L6470_KVAL_RUN,
  enum_L6470_KVAL_ACC,
  enum_L6470_KVAL_DEC,
  enum_L6470_INT_SPEED,
  enum_L6470_ST_SLP,
  enum_L6470_FN_SLP_ACC,
  enum_L6470_FN_SLP_DEC,
  enum_L6470_K_THERM,
  enum_L6470_ADC_OUT, //readonly
  enum_L6470_OCD_TH,
  enum_L6470_STALL_TH,
  enum_L6470_FS_SPD,
  enum_L6470_STEP_MODE,
  enum_L6470_ALARM_EN,
  enum_L6470_CONFIG,
  enum_L6470_STATUS, //readonly
//  enum_L6470_RESERVED_h1A,
//  enum_L6470_RESERVED_h1B,
  enum_L6470_PARAM_FIN
}enum_L6470_PARAM;

#define PARAM_NUM (enum_L6470_PARAM_FIN)

//----define L6470 command as userful number----
typedef enum{
  enum_L6470_NOP,
  enum_L6470_SETPARAM, 
  enum_L6470_GETPARAM,
  enum_L6470_MoveRun,
  enum_L6470_MOVESTEPCLOCK,
  enum_L6470_MOVESTEP,
  enum_L6470_MOVEGOTO,
  enum_L6470_MOVEGOTODIR,
  enum_L6470_MOVEGOTOUNTIL,
  enum_L6470_MOVERELEASE,
  enum_L6470_GOHOME,
  enum_L6470_GOMARK,
  enum_L6470_RESETPOS,
  enum_L6470_RESETDEVICE,
  enum_L6470_STOPSOFT,
  enum_L6470_STOPHARD,
  enum_L6470_HIZSOFT,
  enum_L6470_HIZHARD,
  enum_L6470_GETSTATUS,
  enum_L6470_CMD_FIN
}enum_L6470_CMD;
#define CMD_NUM		  (enum_L6470_CMD_FIN)

//----define L6470 registers----

#define REG_ABS_POS     0x01
#define REG_EL_POS      0x02
#define REG_MARK        0x03
#define REG_SPEED       0x04 //readonly
#define REG_ACC         0x05
#define REG_DEC         0x06
#define REG_MAX_SPEED   0x07
#define REG_MIN_SPEED   0x08
#define REG_KVAL_HOLD   0x09
#define REG_KVAL_RUN    0x0A
#define REG_KVAL_ACC    0x0B
#define REG_KVAL_DEC    0x0C
#define REG_INT_SPEED   0x0D
#define REG_ST_SLP      0x0E
#define REG_FN_SLP_ACC  0x0F
#define REG_FN_SLP_DEC  0x10
#define REG_K_THERM     0x11
#define REG_ADC_OUT     0x12 //readonly
#define REG_OCD_TH      0x13
#define REG_STALL_TH    0x14
#define REG_FS_SPD      0x15
#define REG_STEP_MODE   0x16
#define REG_ALARM_EN    0x17
#define REG_CONFIG      0x18
#define REG_STATUS      0x19 //readonly
//#define REG_RESERVED_h1A    0x1A
//#define REG_RESERVED_h1B    0x1B



//----define L6470 registers for functions----
#define CMD_NOP           0b00000000
#define CMD_SETPARAM      0b00000000
#define CMD_GETPARAM      0b00100000
#define CMD_MOVECONT      0b01010000
#define CMD_MOVESTEPCLOCK 0b01011000
#define CMD_MOVESTEP      0b01000000
#define CMD_MOVEGOTO      0b01100000
#define CMD_MOVEGOTODIR   0b01101000
#define CMD_MOVEGOTOUNTIL 0b10000010
#define CMD_MOVERELEASE   0b10010010
#define CMD_GOHOME        0b01110000
#define CMD_GOMARK        0b01111000
#define CMD_RESETPOS      0b11011000
#define CMD_RESETDEVICE   0b11000000
#define CMD_STOPSOFT      0b10110000
#define CMD_STOPHARD      0b10111000
#define CMD_HIZSOFT       0b10100000
#define CMD_HIZHARD       0b10101000
#define CMD_GETSTATUS     0b11010000

#define REG_SIZE_ABS_POS    (22)
#define REG_SIZE_EL_POS     (9 )
#define REG_SIZE_MARK       (22)
#define REG_SIZE_SPEED      (20) //readonly
#define REG_SIZE_ACC        (12)
#define REG_SIZE_DEC        (12)
#define REG_SIZE_MAX_SPEED  (10)
#define REG_SIZE_MIN_SPEED  (13)
#define REG_SIZE_KVAL_HOLD  (8 )
#define REG_SIZE_KVAL_RUN   (8 )
#define REG_SIZE_KVAL_ACC   (8 )
#define REG_SIZE_KVAL_DEC   (8 )
#define REG_SIZE_INT_SPEED  (14)
#define REG_SIZE_ST_SLP     (8 )
#define REG_SIZE_FN_SLP_ACC (8 )
#define REG_SIZE_FN_SLP_DEC (8 )
#define REG_SIZE_K_THERM    (4 )
#define REG_SIZE_ADC_OUT    (5 ) //readonly
#define REG_SIZE_OCD_TH     (4 )
#define REG_SIZE_STALL_TH   (7 )
#define REG_SIZE_FS_SPD     (10)
#define REG_SIZE_STEP_MODE  (8 )
#define REG_SIZE_ALARM_EN   (8 )
#define REG_SIZE_CONFIG     (16)
#define REG_SIZE_STATUS     (16) //readonly
//#define REG_SIZE_RESERVED_h1A     (0) //dummy
//#define REG_SIZE_RESERVED_h1B     (0) //dummy

#define CMD_SIZE_NOP              (1 ) // test
#define CMD_SIZE_SETPARAM         (24) //MAX
#define CMD_SIZE_GETPARAM         (24) //MAX
#define CMD_SIZE_MOVERUN          (20)
#define CMD_SIZE_MOVESTEPCLOCK    (0 )
#define CMD_SIZE_MOVESTEP         (22)  
#define CMD_SIZE_MOVEGOTO         (22)
#define CMD_SIZE_MOVEGOTODIR      (22)
#define CMD_SIZE_MOVEGOTOUNTIL    (20)
#define CMD_SIZE_MOVERELEASE      (0 )
#define CMD_SIZE_GOHOME           (0 )
#define CMD_SIZE_GOMARK           (0 )
#define CMD_SIZE_RESETPOS         (0 )
#define CMD_SIZE_RESETDEVICE      (0 )
#define CMD_SIZE_STOPSOFT         (0 )
#define CMD_SIZE_STOPHARD         (0 )
#define CMD_SIZE_HIZSOFT          (0 )
#define CMD_SIZE_HIZHARD          (0 )
#define CMD_SIZE_GETSTATUS        (16)

#define SPI_BPW   (8)
#define SPI_DELAY (0)

//----define struct/union----
typedef struct{
	uint8_t self_num;
	uint8_t addr;
	uint8_t send_bit_size;
}L6470_CMD;

typedef struct{
	uint8_t self_num;
	uint8_t addr;
	uint8_t param_size;
	uint8_t rw;
}L6470_PARAM;

typedef struct{
  uint8_t reg_addr;
  uint8_t value8b[3];
}L6470_packet;

typedef union{
  L6470_packet data;
  uint8_t value8b[4];
  uint32_t value32b;
}L6470_u_packet;

typedef struct{
  enum_L6470_PARAM enum_prm;
  uint8_t s_byte;
  L6470_u_packet pkt;
}L6470_DATA_T;


#include "L6470_user.h"
extern const  L6470_CMD L6470_cmd[CMD_NUM];
extern const  L6470_PARAM L6470_param[PARAM_NUM];
//----prottype declaration---
void L6470_SPI_init(void);
void L6470_reg_size_init(void);
void L6470_setting_init(void);
void L6470_init(void);
int  L6470_rw_multi(L6470_DATA_ARRAY *datum, const char* msg);
L6470_DATA_T L6470_nop(int times);
L6470_DATA_T L6470_SetParam(int enum_param,uint32_t value);
L6470_DATA_T L6470_GetParam(int param);
L6470_DATA_T L6470_MoveRun(uint8_t dir,uint32_t value);
L6470_DATA_T L6470_MoveStepClock(uint8_t dir);
L6470_DATA_T L6470_MoveStep(uint8_t dir, uint32_t step);
L6470_DATA_T L6470_MoveGoTo(int32_t abs_pos);
L6470_DATA_T L6470_MoveGoToDir(uint8_t dir, int32_t abs_pos);
L6470_DATA_T L6470_MoveGoToUntil(uint8_t act, uint8_t dir,uint32_t speed);
L6470_DATA_T L6470_MoveRelease(uint8_t act, uint8_t dir);
L6470_DATA_T L6470_GoHome(void);
L6470_DATA_T L6470_GoMark(void);
L6470_DATA_T L6470_ResetPos(void);
L6470_DATA_T L6470_ResetDevice(void);
L6470_DATA_T L6470_StopSoft(void);
L6470_DATA_T L6470_StopHard(void);
L6470_DATA_T L6470_HiZSoft(void);
L6470_DATA_T L6470_HiZHard(void);
L6470_ABSPOS_T L6470_GetAbsPos(void);
L6470_STATUS_T L6470_GetStatus(void);

#endif

