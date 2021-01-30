/* =============================================                    */
/* Copyright (c) 2021 poemoep                                       */
/* This software is released under the MIT License, see LICENSE.    */
/* =============================================                    */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <math.h>

// #include <sys/ioctl.h>
// #include <asm/ioctl.h>
 #include <linux/spi/spidev.h>

#include "wiringPi.h"
#include "wiringPiSPI.h"

#define L6470_SPI_MODE SPI_MODE_3 // default

#include "L6470.h"
// #include "L6470_const.h"
#include "L6470_user.h"

// uint8_t *REG_SIZE;
L6470_packet *L6470_setting[L6470_DEV_NUM];

static uint32_t spiSpeeds [2];
static int 	spiFds [2];

// extern  L6470_PARAM L6470_param[PARAM_NUM];
// extern const L6470_CMD L6470_cmd[CMD_NUM];

const  L6470_PARAM L6470_param[PARAM_NUM] =
{
{ enum_L6470_ABS_POS      , REG_ABS_POS,      REG_SIZE_ABS_POS,       READONLY | WRITABLE_MStop },
{ enum_L6470_EL_POS       , REG_EL_POS ,      REG_SIZE_EL_POS ,       READONLY | WRITABLE_MStop},
{ enum_L6470_MARK         , REG_MARK,         REG_SIZE_MARK,          READONLY | WRITABLE_Always},
{ enum_L6470_SPEED        , REG_SPEED,        REG_SIZE_SPEED,         READONLY }, //readonly
{ enum_L6470_ACC          , REG_ACC,          REG_SIZE_ACC,           READONLY | WRITABLE_MStop},
{ enum_L6470_DEC          , REG_DEC,          REG_SIZE_DEC,           READONLY | WRITABLE_MStop},
{ enum_L6470_MAX_SPEED    , REG_MAX_SPEED,    REG_SIZE_MAX_SPEED,     READONLY | WRITABLE_Always},
{ enum_L6470_MIN_SPEED    , REG_MIN_SPEED,    REG_SIZE_MIN_SPEED,     READONLY | WRITABLE_MStop},
{ enum_L6470_KVAL_HOLD    , REG_KVAL_HOLD,    REG_SIZE_KVAL_HOLD,     READONLY | WRITABLE_Always},
{ enum_L6470_KVAL_RUN     , REG_KVAL_RUN,     REG_SIZE_KVAL_RUN,      READONLY | WRITABLE_Always},
{ enum_L6470_KVAL_ACC     , REG_KVAL_ACC,     REG_SIZE_KVAL_ACC,      READONLY | WRITABLE_Always},
{ enum_L6470_KVAL_DEC     , REG_KVAL_DEC,     REG_SIZE_KVAL_DEC,      READONLY | WRITABLE_Always},
{ enum_L6470_INT_SPEED    , REG_INT_SPEED,    REG_SIZE_INT_SPEED,     READONLY | WRITABLE_HiZ},
{ enum_L6470_ST_SLP       , REG_ST_SLP,       REG_SIZE_ST_SLP,        READONLY | WRITABLE_HiZ},
{ enum_L6470_FN_SLP_ACC   , REG_FN_SLP_ACC,   REG_SIZE_FN_SLP_ACC,    READONLY | WRITABLE_HiZ},
{ enum_L6470_FN_SLP_DEC   , REG_FN_SLP_DEC,   REG_SIZE_FN_SLP_DEC,    READONLY | WRITABLE_HiZ},
{ enum_L6470_K_THERM      , REG_K_THERM,      REG_SIZE_K_THERM,       READONLY | WRITABLE_Always},
{ enum_L6470_ADC_OUT      , REG_ADC_OUT,      REG_SIZE_ADC_OUT,       READONLY }, //readonly
{ enum_L6470_OCD_TH       , REG_OCD_TH,       REG_SIZE_OCD_TH,        READONLY | WRITABLE_Always},
{ enum_L6470_STALL_TH     , REG_STALL_TH,     REG_SIZE_STALL_TH,      READONLY | WRITABLE_Always},
{ enum_L6470_FS_SPD       , REG_FS_SPD,       REG_SIZE_FS_SPD,        READONLY | WRITABLE_Always},
{ enum_L6470_STEP_MODE    , REG_STEP_MODE,    REG_SIZE_STEP_MODE,     READONLY | WRITABLE_HiZ},
{ enum_L6470_ALARM_EN     , REG_ALARM_EN,     REG_SIZE_ALARM_EN,      READONLY | WRITABLE_MStop},
{ enum_L6470_CONFIG       , REG_CONFIG,       REG_SIZE_CONFIG,        READONLY | WRITABLE_HiZ},
{ enum_L6470_STATUS       , REG_STATUS,       REG_SIZE_STATUS,        READONLY }, //readonly
//{ enum_L6470_RESERVED_h1A , REG_RESERVED_h1A, REG_SIZE_RESERVED_h1A,  RESERVED }, //dummy
//{ enum_L6470_RESERVED_h1B , REG_RESERVED_h1B, REG_SIZE_RESERVED_h1B,  RESERVED }  //dummy
};

const  L6470_CMD L6470_cmd[CMD_NUM] =
{	
{  enum_L6470_NOP,            CMD_NOP,            CMD_SIZE_NOP},
{  enum_L6470_SETPARAM,       CMD_SETPARAM,       CMD_SIZE_SETPARAM},
{  enum_L6470_GETPARAM,       CMD_GETPARAM,       CMD_SIZE_GETPARAM},
{  enum_L6470_MoveRun,       CMD_MOVECONT,       CMD_SIZE_MOVECONT},
{  enum_L6470_MOVESTEPCLOCK,  CMD_MOVESTEPCLOCK,  CMD_SIZE_MOVESTEPCLOCK},
{  enum_L6470_MOVESTEP,       CMD_MOVESTEP,       CMD_SIZE_MOVESTEP},
{  enum_L6470_MOVEGOTO,       CMD_MOVEGOTO,       CMD_SIZE_MOVEGOTO},
{  enum_L6470_MOVEGOTODIR,    CMD_MOVEGOTODIR,    CMD_SIZE_MOVEGOTODIR},
{  enum_L6470_MOVEGOTOUNTIL,  CMD_MOVEGOTOUNTIL,  CMD_SIZE_MOVEGOTOUNTIL},
{  enum_L6470_MOVERELEASE,    CMD_MOVERELEASE,    CMD_SIZE_MOVERELEASE},
{  enum_L6470_GOHOME,         CMD_GOHOME,         CMD_SIZE_GOHOME},
{  enum_L6470_GOMARK,         CMD_GOMARK,         CMD_SIZE_GOMARK},
{  enum_L6470_RESETPOS,       CMD_RESETPOS,       CMD_SIZE_RESETPOS},
{  enum_L6470_RESETDEVICE,    CMD_RESETDEVICE,    CMD_SIZE_RESETDEVICE},
{  enum_L6470_STOPSOFT,       CMD_STOPSOFT,       CMD_SIZE_STOPSOFT},
{  enum_L6470_STOPHARD,       CMD_STOPHARD,       CMD_SIZE_STOPHARD},
{  enum_L6470_HIZSOFT,        CMD_HIZSOFT,        CMD_SIZE_HIZSOFT},
{  enum_L6470_HIZHARD,        CMD_HIZHARD,        CMD_SIZE_HIZHARD},
{  enum_L6470_GETSTATUS,      CMD_GETSTATUS,      CMD_SIZE_GETSTATUS}
};

L6470_packet L6470_makeCmd( L6470_CMD cmd, int orprm, uint32_t arg_param);
static void L6470_ExecCmd( L6470_CMD cmd, int orprm, uint32_t arg_param,const char* msg);
static void L6470_ExecCmd_NoArg( L6470_CMD cmd, const char* msg);


#if defined (L6470_PRINT_MESSAGE)
static void L6470_debug_print(const char *msg,L6470_packet* send, L6470_packet* get);
#endif

void L6470_setting_init(void)
{

#ifdef L6470_PRINT_MESSAGE
    printf("%s setting_init start\n",L6470_PRINT_HEADER);
#endif

    // for(int itr = 0; itr < L6470_DEV_NUM; itr++)
    // {
    //     L6470_setting[itr] = (L6470_packet*)malloc((PARAM_NUM) * sizeof(L6470_packet));

    //     for (int enum_param = 0; enum_param < (PARAM_NUM); enum_param++)
    //     {
    //         if(L6470_param[itr][enum_param].rw == RESERVED){
    //             continue;
    //         }else if(L6470_param[itr][enum_param].rw == READONLY){
    //             L6470_GetParam(itr, enum_param);
    //         }else{
    //             /* copy to buf from const */
    //             L6470_setting[enum_param] = L6470_user_setting[enum_param];
    //             //make temp because wiringPiSPIDataRW rewrite send data
    //             L6470_packet pkt_temp;
    //             pkt_temp = L6470_user_setting[enum_param];

    //             int len, SPI_res = 0;
    //             len = L6470_param[enum_param].param_size;
    // #ifdef L6470_PRINT_MESSAGE
    //             L6470_packet send = L6470_user_setting[enum_param];
    // #endif
    //             SPI_res = L6470_rw(&(pkt_temp), (int)(bit2byte(len + ADDR_SIZE)), NULL);
    // #ifdef L6470_PRINT_MESSAGE
    //             L6470_debug_print("setting_init",&(send),&(pkt_temp));
    // #endif
    //         }
    //     }
    // }

#ifdef L6470_PRINT_MESSAGE
    printf("%s setting_init end\n",L6470_PRINT_HEADER);
#endif

}


void L6470_SPI_init(void)
{
    int SPI_res;
    SPI_res = wiringPiSPISetupMode(L6470_SPI_CH,SPI_SPEED,L6470_SPI_MODE);
    // SPI_res = mywiringPiSPISetupMode(SPI_CH,SPI_SPEED,SPI_WMODE, SPI_RMODE);

#ifdef L6470_PRINT_MESSAGE
    printf("%s SPI_init ch:%d\n",L6470_PRINT_HEADER,SPI_res);
#endif
}

void L6470_init(void)
{

    L6470_SPI_init();
    L6470_ResetDevice();
    L6470_setting_init();

#ifdef L6470_PRINT_MESSAGE
    printf("%s init Done\n",L6470_PRINT_HEADER);
#endif
}

int L6470_rw(L6470_packet *pkt,int len, const char* msg)
{
    //uint8_t *data;
    //data = pkt->value8b;

#ifdef L6470_PRINT_MESSAGE
    L6470_packet send = *pkt;
#endif


	int i = 0,j = 0;
	for (i = 0; i<len; i++){
	//	j += wiringPiSPIDataRW(L6470_SPI_CH, data,1);
		j += wiringPiSPIDataRW(L6470_SPI_CH, (unsigned char *)(pkt->value8b + i),1);
	//	data++;
	}

#ifdef L6470_PRINT_MESSAGE
    L6470_debug_print(msg,&(send),pkt);
#endif

   return j; 
}

int L6470_rw_multi(L6470_packet *pkt,int len, const char* msg, ...)
{
    //uint8_t *data;
    //data = pkt->value8b;

    va_list args;
    va_start(args,L6470_DEV_NUM * 3);

#ifdef L6470_PRINT_MESSAGE
    L6470_packet* send[L6470_DEV_NUM];
#endif

    L6470_packet* input_pkt[L6470_DEV_NUM] = {0};
    int input_len[L6470_DEV_NUM] = {0};
    char* input_msg[L6470_DEV_NUM] = {0};
    int max_len = 0;

    /* change args to array for useful format */
    for(int itr = 0; itr < L6470_DEV_NUM; itr++)
    {
        input_pkt[itr] = va_arg(args, L6470_packet *);
        input_len[itr] = va_arg(args, int);
        input_msg[itr] = va_arg(args, char *);
    }

    uint8_t trans_pkt[L6470_DEV_NUM*4] = {0};
    /* summarize pkts to uint8_t array */
    for(int itr = 0; itr < L6470_DEV_NUM; itr++){
        /* get a pkt */
        for(int pkt_num = 0; pkt_num < 4; pkt_num++)
            trans_pkt[itr + (L6470_DEV_NUM * pkt_num)] = input_pkt[itr]->value8b[pkt_num];

        /* get a len */
        if(max_len < input_len[itr]) max_len = input_len[itr];

        /* get a msg */
        input_msg[itr] = input_msg[itr];

#ifdef L6470_PRINT_MESSAGE
        send[itr] = input_pkt[itr];
#endif
    }

    /*  transmission all packets */
	int i = 0,j = 0;
	for (i = 0; i<max_len; i++){

	//	j += wiringPiSPIDataRW(L6470_SPI_CH, data,1);
		// j += wiringPiSPIDataRW(L6470_SPI_CH, 
        //         (unsigned char *)(pkt->value8b + i),L6470_DEV_NUM);
        j += wiringPiSPIDataRW(L6470_SPI_CH,
                (uint8_t *)(trans_pkt + i*L6470_DEV_NUM), L6470_DEV_NUM);

	//	data++;
	}

    /* distribute packets to original pointer */
    for(int itr = 0; itr < L6470_DEV_NUM; itr++)
    {
        for(int pkt_num = 0; pkt_num < 4; pkt_num++)
            input_pkt[itr]->value8b[pkt_num] = trans_pkt[itr + (L6470_DEV_NUM * pkt_num)];

#ifdef L6470_PRINT_MESSAGE
        L6470_debug_print(input_msg[itr], send[itr], input_pkt[itr]);
#endif
    }


// #ifdef L6470_PRINT_MESSAGE
//     L6470_debug_print(msg,&(send),pkt);
// #endif

    va_end(args);

    return j; 
}

L6470_packet L6470_nop(int times)
{
    L6470_packet pkt = {0};
    // int SPI_res = 0;
    // int size = L6470_cmd[enum_L6470_NOP].send_bit_size;

    // L6470_rw(&(pkt),times, "NOP");
    return pkt;
}

L6470_packet L6470_SetParam(int enum_param, uint32_t value)
{
    L6470_packet pkt;
    int SPI_res = 0;
    int size = L6470_param[enum_param].param_size;
    
    pkt.data.reg_addr = (L6470_param[enum_param].addr | L6470_cmd[enum_L6470_SETPARAM].addr);

    if(8 >= size){
        pkt.value8b[1] = ((value & 0xFF));
    }else if (16 >= size){
        pkt.value8b[1] = ((value & 0xFF00) >> 8);
        pkt.value8b[2] = ((value & 0x00FF));
    }else{
        pkt.value8b[1] = ((value & 0xFF0000) >> 16);
        pkt.value8b[2] = ((value & 0x00FF00) >> 8);
        pkt.value8b[3] = (value & 0x0000FF);
    }
    
    // SPI_res = L6470_rw(&(pkt),bit2byte(size + ADDR_SIZE), "SetParam");

    return pkt;
}

L6470_packet L6470_GetParam(int enum_param)
{
    L6470_packet pkt={0};
    int SPI_res = 0;
    int32_t ret = 100;

    int size = L6470_cmd[enum_L6470_GETPARAM].send_bit_size;
    pkt.data.reg_addr = (L6470_param[enum_param].addr | L6470_cmd[enum_L6470_GETPARAM].addr);

    // SPI_res = L6470_rw(&(pkt),bit2byte(size + ADDR_SIZE),"GetParam");

    /* rewrite addr */
    // pkt.data.reg_addr = L6470_param[enum_param].addr;
    // L6470_setting[enum_param] = pkt;


    // if(8 >= size){
    //     ret = (pkt.value8b[1]);
    // }else if (16 >= size){
    //     ret = (pkt.value8b[1] << 8) + (pkt.value8b[2]);
    // }else{
    //     ret = (pkt.value8b[1] << 16) + (pkt.value8b[2] << 8) + (pkt.value8b[3]);
    // }

    // return ret;
    return pkt;
}
/*speed = 0 to 15625000 [x0.001 step/s] */
L6470_packet L6470_MoveRun(uint8_t dir, uint32_t speed)
{
    int32_t speed_val = (int32_t)round((double)speed / SPEED_RESOLUTION);
#if defined (L6470_PRINT_MESSAGE)
    int32_t temp_speed = (int32_t)round((double)speed_val * SPEED_RESOLUTION);
    if( temp_speed != speed)
        printf("%s %s speed is rounded to %d [x0.001 step/s]\n",L6470_PRINT_HEADER,L6470_PRINT_CAUTION, temp_speed);
    // L6470_packet temp = L6470_setting[enum_L6470_MAX_SPEED];
    // int32_t max_speed = (temp.data.value8b[0] << 8) + (temp.data.value8b[1]);
    // max_speed = max_speed * MAX_SPEED_RESOLUTION * 10; /* 10times MAX_SPEDD [x0.01 step/s] / SPEED [x0.001 step/s] */ 
    // if( temp_speed > max_speed )
        // printf("%s %s speed is over MAX_SPEED. rounded to %d [x0.001 step/s]\n",L6470_PRINT_HEADER,L6470_PRINT_CAUTION, max_speed);

#endif
    // L6470_ExecCmd(L6470_cmd[enum_L6470_MoveRun],dir,speed_val, "MoveCont");
    return L6470_makeCmd(L6470_cmd[enum_L6470_MoveRun],dir,speed_val);
}
/* dir = DIR_FWD or DIR_RVS */
L6470_packet L6470_MoveStepClock(uint8_t dir)
{
    // L6470_ExecCmd(L6470_cmd[enum_L6470_MOVESTEPCLOCK],dir,0, "MoveStepClock");
    return L6470_makeCmd(L6470_cmd[enum_L6470_MOVESTEPCLOCK],dir,0);
}
/* step = 0 to 4194303(2^22) [step] */
L6470_packet L6470_MoveStep(uint8_t dir,uint32_t step)
{
    L6470_ExecCmd(L6470_cmd[enum_L6470_MOVESTEP],dir,step, "MoveStep");
    return L6470_makeCmd(L6470_cmd[enum_L6470_MOVESTEP],dir,step);
}
/* abs_pos = -2^21 to 2^21 -1 [step]*/
L6470_packet L6470_MoveGoTo(int32_t abs_pos)
{
    // L6470_ExecCmd(L6470_cmd[enum_L6470_MOVEGOTO],0,abs_pos, "MoveGoTo");
    return L6470_makeCmd(L6470_cmd[enum_L6470_MOVEGOTO],0,abs_pos);
}
/* dir = DIR_FWD or DIR_RVS, abs_pos = -2^21 to 2^21-1 [step] */
L6470_packet L6470_MoveGoToDir(uint8_t dir,int32_t abs_pos)
{
    // L6470_ExecCmd(L6470_cmd[enum_L6470_MOVEGOTODIR],dir,abs_pos,"MoveGoToDir");
    return L6470_makeCmd(L6470_cmd[enum_L6470_MOVEGOTODIR],dir,abs_pos);
}
/* act = ACT_POS_CLEAR or ACT_POS_TO_MARK */
/* dir = DIR_FWD or DIR_RVS */
/*speed = 0 to 15625000 [x0.001 step/s] */
L6470_packet L6470_MoveGoToUntil(uint8_t act, uint8_t dir,uint32_t speed)
{
    int32_t speed_val = (int32_t)round((double)speed / SPEED_RESOLUTION);
#if defined (L6470_PRINT_MESSAGE)
    if((int32_t)round((double)speed_val * SPEED_RESOLUTION) != speed)
        printf("%s %s speed is rounded to %d [x0.001 step/s]\n",L6470_PRINT_HEADER,L6470_PRINT_CAUTION, (speed_val) * SPEED_RESOLUTION);
#endif

    // L6470_ExecCmd(L6470_cmd[enum_L6470_MOVEGOTOUNTIL],act|dir,speed_val, "MoveGoToUntil");
    return L6470_makeCmd(L6470_cmd[enum_L6470_MOVEGOTOUNTIL],act|dir,speed_val);
}

/* act = ACT_POS_CLEAR or ACT_POS_TO_MARK */
/* dir = DIR_FWD or DIR_RVS */
L6470_packet L6470_MoveRelease(uint8_t act, uint8_t dir)
{
    // L6470_ExecCmd(L6470_cmd[enum_L6470_MOVERELEASE],act|dir,0,"MoveRelease");
    return L6470_makeCmd(L6470_cmd[enum_L6470_MOVERELEASE],act|dir,0);
}

L6470_packet L6470_GoHome(void)
{
    // L6470_ExecCmd_NoArg(L6470_cmd[enum_L6470_GOHOME], "GoHome");
    return L6470_makeCmd(L6470_cmd[enum_L6470_GOHOME], 0, 0);
}

L6470_packet L6470_GoMark(void)
{
    // L6470_ExecCmd_NoArg(L6470_cmd[enum_L6470_GOMARK], "GoMark");
    return L6470_makeCmd(L6470_cmd[enum_L6470_GOMARK], 0, 0);
}

L6470_packet L6470_ResetPos(void)
{
    // L6470_ExecCmd_NoArg(L6470_cmd[enum_L6470_RESETPOS], "ResetPos");
    return L6470_makeCmd(L6470_cmd[enum_L6470_RESETPOS], 0, 0);
}

L6470_packet L6470_ResetDevice(void)
{
    // L6470_ExecCmd_NoArg(L6470_cmd[enum_L6470_RESETDEVICE],"ResetDevice");
    return L6470_makeCmd(L6470_cmd[enum_L6470_RESETDEVICE], 0, 0);
}

L6470_packet L6470_StopSoft(void)
{
    // L6470_ExecCmd_NoArg(L6470_cmd[enum_L6470_STOPSOFT], "StopSoft");
    return L6470_makeCmd(L6470_cmd[enum_L6470_STOPSOFT], 0, 0);
}

L6470_packet L6470_StopHard(void)
{
    // L6470_ExecCmd_NoArg(L6470_cmd[enum_L6470_STOPHARD], "StopHard");
    return L6470_makeCmd(L6470_cmd[enum_L6470_STOPHARD], 0, 0);
}

L6470_packet L6470_HiZSoft(void)
{
    // L6470_ExecCmd_NoArg(L6470_cmd[enum_L6470_HIZSOFT],"HiZSoft");
    return L6470_makeCmd(L6470_cmd[enum_L6470_HIZSOFT], 0, 0);
}

L6470_packet L6470_HiZHard(void)
{
    // L6470_ExecCmd_NoArg(L6470_cmd[enum_L6470_HIZHARD],"HIZHard");
    return L6470_makeCmd(L6470_cmd[enum_L6470_HIZHARD], 0, 0);
}

L6470_packet L6470_makeCmd( L6470_CMD cmd, int orprm, uint32_t arg_param)
{
    L6470_packet pkt={0};
    // int SPI_res = 0;

    int size = cmd.send_bit_size;
    pkt.data.reg_addr = (cmd.addr | orprm);
    if(pow(2,cmd.send_bit_size) < abs(arg_param))
    {
#if defined (L6470_PRINT_MESSAGE)
        printf("%s AbortCmd size_over cmdsize:2^%d, but arg is %d\n ",L6470_PRINT_HEADER, cmd.send_bit_size,arg_param);
#endif
        return pkt;
    }
    if(0 == size){

    }else if(8 >= size){
        pkt.data.value8b[0] = ((arg_param & 0x0000FF));
    }else if (16 >= size){
        pkt.data.value8b[0] = ((arg_param & 0x00FF00) >> 8);
        pkt.data.value8b[1] = ((arg_param & 0x0000FF));
    }else{
        pkt.data.value8b[0] = ((arg_param & 0xFF0000) >> 16);
        pkt.data.value8b[1] = ((arg_param & 0x00FF00) >> 8);
        pkt.data.value8b[2] = ((arg_param & 0x0000FF));
    }

    return pkt;
    // SPI_res = L6470_rw(&(pkt),bit2byte(size + ADDR_SIZE),msg);
}

static void L6470_ExecCmd( L6470_CMD cmd, int orprm, uint32_t arg_param,const char* msg)
{
    L6470_packet pkt={0};
    int SPI_res = 0;

    int size = cmd.send_bit_size;
    pkt.data.reg_addr = (cmd.addr | orprm);
    if(pow(2,cmd.send_bit_size) < abs(arg_param))
    {
#if defined (L6470_PRINT_MESSAGE)
        printf("%s AbortCmd size_over cmdsize:2^%d, but arg is %d\n ",L6470_PRINT_HEADER, cmd.send_bit_size,arg_param);
#endif
        return;
    }
    if(8 >= size){
        pkt.data.value8b[0] = ((arg_param & 0x0000FF));
    }else if (16 >= size){
        pkt.data.value8b[0] = ((arg_param & 0x00FF00) >> 8);
        pkt.data.value8b[1] = ((arg_param & 0x0000FF));
    }else{
        pkt.data.value8b[0] = ((arg_param & 0xFF0000) >> 16);
        pkt.data.value8b[1] = ((arg_param & 0x00FF00) >> 8);
        pkt.data.value8b[2] = ((arg_param & 0x0000FF));
    }

    SPI_res = L6470_rw(&(pkt),bit2byte(size + ADDR_SIZE),msg);
}

static void L6470_ExecCmd_NoArg( L6470_CMD cmd, const char* msg)
{
    L6470_packet pkt={0};
    int SPI_res = 0;
        
    int size = cmd.send_bit_size;
    pkt.data.reg_addr = cmd.addr;

    SPI_res = L6470_rw(&(pkt),bit2byte(size + ADDR_SIZE),msg);

}

int32_t L6470_GetAbsPos(void)
{
    int32_t pos = 0;
    int32_t ret = 0; 
    L6470_packet pkt;
    // pos = L6470_GetParam(enum_L6470_ABS_POS);
 
    if(((pos & 0x200000) >> 21) == 1){
	    pos = (-1) * ((~pos + 1) & 0x3FFFFF);	
    }

#ifdef L6470_PRINT_MESSAGE
	printf("pos: %d\n", pos);
#endif

    return pos;

}


uint16_t L6470_GetStatus(void)
{
    L6470_packet pkt={0};
    int SPI_res = 0;

    int size = L6470_param[enum_L6470_GETSTATUS].param_size;
    pkt.data.reg_addr = L6470_cmd[enum_L6470_GETSTATUS].addr;

    SPI_res = L6470_rw(&(pkt),bit2byte(size + ADDR_SIZE),"GetStatus");

    return ((pkt.value8b[2] << 8) & (pkt.value8b[3]));

}


#if defined (L6470_PRINT_MESSAGE)
static void L6470_debug_print(const char *msg,L6470_packet* send, L6470_packet* get)
{
    if(msg != NULL)
    {
        printf("%s %s send:0x%02x 0x%02x 0x%02x 0x%02x\n", L6470_PRINT_HEADER, msg, 
				send->value8b[0], 
				send->value8b[1], 
				send->value8b[2], 
				send->value8b[3]);
        if (get != (L6470_packet*)NULL)
            printf("%s %s  get:0x%02x 0x%02x 0x%02x 0x%02x\n", L6470_PRINT_HEADER, msg,  
			         get->value8b[0], 
			         get->value8b[1], 
			         get->value8b[2], 
			         get->value8b[3]);
    }
}
#endif
