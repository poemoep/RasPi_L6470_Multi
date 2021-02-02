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
L6470_u_packet *L6470_setting[L6470_DEV_NUM];

static uint32_t spiSpeeds [2];
static int 	spiFds [2];

// extern  L6470_PARAM L6470_param[PARAM_NUM];
// extern const L6470_CMD L6470_cmd[CMD_NUM];

const  L6470_PARAM L6470_param[PARAM_NUM] =
{
    { enum_L6470_ABS_POS      , REG_ABS_POS,      REG_SIZE_ABS_POS,       READONLY | WRITABLE_MStop },
    { enum_L6470_EL_POS       , REG_EL_POS ,      REG_SIZE_EL_POS ,       READONLY | WRITABLE_MStop },
    { enum_L6470_MARK         , REG_MARK,         REG_SIZE_MARK,          READONLY | WRITABLE_Always},
    { enum_L6470_SPEED        , REG_SPEED,        REG_SIZE_SPEED,         READONLY                  }, //readonly
    { enum_L6470_ACC          , REG_ACC,          REG_SIZE_ACC,           READONLY | WRITABLE_MStop },
    { enum_L6470_DEC          , REG_DEC,          REG_SIZE_DEC,           READONLY | WRITABLE_MStop },
    { enum_L6470_MAX_SPEED    , REG_MAX_SPEED,    REG_SIZE_MAX_SPEED,     READONLY | WRITABLE_Always},
    { enum_L6470_MIN_SPEED    , REG_MIN_SPEED,    REG_SIZE_MIN_SPEED,     READONLY | WRITABLE_MStop },
    { enum_L6470_KVAL_HOLD    , REG_KVAL_HOLD,    REG_SIZE_KVAL_HOLD,     READONLY | WRITABLE_Always},
    { enum_L6470_KVAL_RUN     , REG_KVAL_RUN,     REG_SIZE_KVAL_RUN,      READONLY | WRITABLE_Always},
    { enum_L6470_KVAL_ACC     , REG_KVAL_ACC,     REG_SIZE_KVAL_ACC,      READONLY | WRITABLE_Always},
    { enum_L6470_KVAL_DEC     , REG_KVAL_DEC,     REG_SIZE_KVAL_DEC,      READONLY | WRITABLE_Always},
    { enum_L6470_INT_SPEED    , REG_INT_SPEED,    REG_SIZE_INT_SPEED,     READONLY | WRITABLE_HiZ   },
    { enum_L6470_ST_SLP       , REG_ST_SLP,       REG_SIZE_ST_SLP,        READONLY | WRITABLE_HiZ   },
    { enum_L6470_FN_SLP_ACC   , REG_FN_SLP_ACC,   REG_SIZE_FN_SLP_ACC,    READONLY | WRITABLE_HiZ   },
    { enum_L6470_FN_SLP_DEC   , REG_FN_SLP_DEC,   REG_SIZE_FN_SLP_DEC,    READONLY | WRITABLE_HiZ   },
    { enum_L6470_K_THERM      , REG_K_THERM,      REG_SIZE_K_THERM,       READONLY | WRITABLE_Always},
    { enum_L6470_ADC_OUT      , REG_ADC_OUT,      REG_SIZE_ADC_OUT,       READONLY                  }, //readonly
    { enum_L6470_OCD_TH       , REG_OCD_TH,       REG_SIZE_OCD_TH,        READONLY | WRITABLE_Always},
    { enum_L6470_STALL_TH     , REG_STALL_TH,     REG_SIZE_STALL_TH,      READONLY | WRITABLE_Always},
    { enum_L6470_FS_SPD       , REG_FS_SPD,       REG_SIZE_FS_SPD,        READONLY | WRITABLE_Always},
    { enum_L6470_STEP_MODE    , REG_STEP_MODE,    REG_SIZE_STEP_MODE,     READONLY | WRITABLE_HiZ   },
    { enum_L6470_ALARM_EN     , REG_ALARM_EN,     REG_SIZE_ALARM_EN,      READONLY | WRITABLE_MStop },
    { enum_L6470_CONFIG       , REG_CONFIG,       REG_SIZE_CONFIG,        READONLY | WRITABLE_HiZ   },
    { enum_L6470_STATUS       , REG_STATUS,       REG_SIZE_STATUS,        READONLY                  } //readonly
    //{ enum_L6470_RESERVED_h1A , REG_RESERVED_h1A, REG_SIZE_RESERVED_h1A,  RESERVED }, //dummy
    //{ enum_L6470_RESERVED_h1B , REG_RESERVED_h1B, REG_SIZE_RESERVED_h1B,  RESERVED }  //dummy
};

const  L6470_CMD L6470_cmd[CMD_NUM] =
{	
    {  enum_L6470_NOP,            CMD_NOP,            CMD_SIZE_NOP          },
    {  enum_L6470_SETPARAM,       CMD_SETPARAM,       CMD_SIZE_SETPARAM     },
    {  enum_L6470_GETPARAM,       CMD_GETPARAM,       CMD_SIZE_GETPARAM     },
    {  enum_L6470_MoveRun,       CMD_MOVECONT,       CMD_SIZE_MOVERUN       },
    {  enum_L6470_MOVESTEPCLOCK,  CMD_MOVESTEPCLOCK,  CMD_SIZE_MOVESTEPCLOCK},
    {  enum_L6470_MOVESTEP,       CMD_MOVESTEP,       CMD_SIZE_MOVESTEP     },
    {  enum_L6470_MOVEGOTO,       CMD_MOVEGOTO,       CMD_SIZE_MOVEGOTO     },
    {  enum_L6470_MOVEGOTODIR,    CMD_MOVEGOTODIR,    CMD_SIZE_MOVEGOTODIR  },
    {  enum_L6470_MOVEGOTOUNTIL,  CMD_MOVEGOTOUNTIL,  CMD_SIZE_MOVEGOTOUNTIL},
    {  enum_L6470_MOVERELEASE,    CMD_MOVERELEASE,    CMD_SIZE_MOVERELEASE  },
    {  enum_L6470_GOHOME,         CMD_GOHOME,         CMD_SIZE_GOHOME       },
    {  enum_L6470_GOMARK,         CMD_GOMARK,         CMD_SIZE_GOMARK       },
    {  enum_L6470_RESETPOS,       CMD_RESETPOS,       CMD_SIZE_RESETPOS     },
    {  enum_L6470_RESETDEVICE,    CMD_RESETDEVICE,    CMD_SIZE_RESETDEVICE  },
    {  enum_L6470_STOPSOFT,       CMD_STOPSOFT,       CMD_SIZE_STOPSOFT     },
    {  enum_L6470_STOPHARD,       CMD_STOPHARD,       CMD_SIZE_STOPHARD     },
    {  enum_L6470_HIZSOFT,        CMD_HIZSOFT,        CMD_SIZE_HIZSOFT      },
    {  enum_L6470_HIZHARD,        CMD_HIZHARD,        CMD_SIZE_HIZHARD      },
    {  enum_L6470_GETSTATUS,      CMD_GETSTATUS,      CMD_SIZE_GETSTATUS    }
};

L6470_DATA_T L6470_makeCmd( L6470_CMD cmd, int orprm, uint32_t arg_param);
// static void L6470_ExecCmd( L6470_CMD cmd, int orprm, uint32_t arg_param,const char* msg);
// static void L6470_ExecCmd_NoArg( L6470_CMD cmd, const char* msg);
static L6470_u_packet generate_pkt(int enum_param,int32_t val);
static L6470_u_packet generate_pkt_with_percentage(int enum_param, int32_t percentage);

typedef L6470_u_packet (*f)(int32_t);

struct {
    f gen_func;
    int32_t param;
} fp_array[PARAM_NUM] = {
   /* [enum_L6470_ABS_POS   ] = */ { gen_ABS_POS  ,     USER_ABS_POS  },
   /* [enum_L6470_EL_POS    ] = */ { gen_EL_POS   ,     USER_EL_POS   },
   /* [enum_L6470_MARK      ] = */ { gen_MARK     ,     USER_MARK     },
   /* [enum_L6470_SPEED     ] = */ { NULL         ,     0             }, //readonly
   /* [enum_L6470_ACC       ] = */ { gen_ACC      ,     USER_ACC      },
   /* [enum_L6470_DEC       ] = */ { gen_DEC      ,     USER_DEC      },
   /* [enum_L6470_MAX_SPEED ] = */ { gen_MAX_SPEED,     USER_MAX_SPEED},
   /* [enum_L6470_MIN_SPEED ] = */ { gen_MIN_SPEED,     USER_MIN_SPEED},
   /* [enum_L6470_KVAL_HOLD ] = */ { gen_KVAL_HOLD,     USER_KVAL_HOLD},
   /* [enum_L6470_KVAL_RUN  ] = */ { gen_KVAL_RUN ,     USER_KVAL_RUN },
   /* [enum_L6470_KVAL_ACC  ] = */ { gen_KVAL_ACC ,     USER_KVAL_ACC },
   /* [enum_L6470_KVAL_DEC  ] = */ { gen_KVAL_DEC ,     USER_KVAL_DEC },
   /* [enum_L6470_INT_SPEED ] = */ { gen_INT_SPEED,     USER_INT_SPEED},
   /* [enum_L6470_ST_SLP    ] = */ { gen_ST_SLP   ,     USER_ST_SLP   },
   /* [enum_L6470_FN_SLP_ACC] = */ { gen_FN_SLP_ACC,    USER_FN_SLP_ACC},
   /* [enum_L6470_FN_SLP_DEC] = */ { gen_FN_SLP_DEC,    USER_FN_SLP_DEC},
   /* [enum_L6470_K_THERM   ] = */ { gen_K_THERM  ,     USER_K_THERM  },
   /* [enum_L6470_ADC_OUT   ] = */ { NULL         ,     0             }, //readonly
   /* [enum_L6470_OCD_TH    ] = */ { gen_OCD_TH   ,     USER_OCD_TH   },
   /* [enum_L6470_STALL_TH  ] = */ { gen_STALL_TH ,     USER_STALL_TH },
   /* [enum_L6470_FS_SPD    ] = */ { gen_FS_SPD   ,     USER_FS_SPD   },
   /* [enum_L6470_STEP_MODE ] = */ { gen_STEP_MODE,     USER_STEP_MODE},
   /* [enum_L6470_ALARM_EN  ] = */ { gen_ALARM_EN ,     USER_ALARM_EN },
   /* [enum_L6470_CONFIG    ] = */ { gen_CONFIG   ,     USER_CONFIG   },
   /* [enum_L6470_STATUS    ] = */ { NULL         ,     0             } //readonly
//  enum_L6470_RESERVED_h1A,
//  enum_L6470_RESERVED_h1B,

};


#if defined (L6470_PRINT_MESSAGE)
static void L6470_debug_print(const char *msg,L6470_u_packet* send, L6470_u_packet* get);
#endif

void L6470_setting_init(void)
{

#ifdef L6470_PRINT_MESSAGE
    printf("%s setting_init start\n",L6470_PRINT_HEADER);
#endif

    for(int itr = 0; itr < L6470_DEV_NUM; itr++)
    {
        L6470_setting[itr] = (L6470_u_packet*)malloc((PARAM_NUM) * sizeof(L6470_u_packet));
    }

    for (int enum_param = 0; enum_param < (PARAM_NUM); enum_param++)
    {
        // printf("%d\n",enum_param);

        if(L6470_param[enum_param].rw == RESERVED){
            continue;
        }else if(L6470_param[enum_param].rw == READONLY){
            // L6470_GetParam(itr, enum_param);
        }else{
            /* copy to buf from const */
            L6470_u_packet pkt = fp_array[enum_param].gen_func(fp_array[enum_param].param);
            // char c = 0;
            // printf("%02x. %02x, %02x, %02x\n",pkt.value8b[0],pkt.value8b[1],pkt.value8b[2],pkt.value8b[3]);
            // c = getchar();
            L6470_DATA_ARRAY ary;
         for(int itr = 0; itr < L6470_DEV_NUM; itr++)
            {
                L6470_setting[itr][enum_param] = pkt;
                ary.dev[itr].enum_prm = enum_param;
                ary.dev[itr].s_byte = bit2byte(L6470_param[enum_param].param_size) + 1;
                ary.dev[itr].pkt = pkt;
            }       
            //make temp because wiringPiSPIDataRW rewrite send data
            L6470_DATA_ARRAY ary_temp;
            ary_temp = ary;

            // printf("check len\n");
            int SPI_res = 0;
            // len = L6470_param[enum_param].param_size;
// #ifdef L6470_PRINT_MESSAGE
//             L6470_u_packet send = pkt;
// #endif
            // printf("exec rw_all\n");
            // SPI_res = L6470_rw_all(&(pkt_temp), (int)(bit2byte(len + ADDR_SIZE)), NULL);
            SPI_res = L6470_rw_multi(&(ary_temp), "PARAM_INIT");
// #ifdef L6470_PRINT_MESSAGE

        }
    }
    

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

// int L6470_rw(L6470_DATA_T *data,int len, const char* msg)
// {
//     //uint8_t *data;
//     //data = pkt->value8b;

// #ifdef L6470_PRINT_MESSAGE
//     L6470_DATA_T send = *data;
// #endif


// 	int i = 0,j = 0;
// 	for (i = 0; i<len; i++){
// 	//	j += wiringPiSPIDataRW(L6470_SPI_CH, data,1);
// 		j += wiringPiSPIDataRW(L6470_SPI_CH, (unsigned char *)(pkt->value8b + i),1);
// 	//	data++;
// 	}

// #ifdef L6470_PRINT_MESSAGE
//     L6470_debug_print(msg,&(send),pkt);
// #endif

//    return j; 
// }

/* transmite same packet to all device */
// int L6470_rw_all(L6470_DATA_ARRAY *datum, const char* msg)
// {
//     //uint8_t *data;
//     //data = pkt->value8b;

//     // printf("copy pkt\n");
//     // L6470_u_packet pkts[L6470_DEV_NUM] = {0};
//     // for(int itr = 0; itr < L6470_DEV_NUM; itr++){
//     //     pkts[itr] = *pkt;
//     // }

//     /* summlize packet */
//     // uint8_t t_pkt[L6470_DEV_NUM*len] = {0};
//     // printf("summlize pkt\n");
//     int len = datum->.dev[itr]
//     uint8_t t_pkt[L6470_DEV_NUM*len];
//     for(int itr = 0; itr < L6470_DEV_NUM; itr++){
//         for(int pkt_num = 0; pkt_num < len; pkt_num++){
//             t_pkt[itr + L6470_DEV_NUM*pkt_num] = (pkts[itr]).value8b[pkt_num];
//         }
//     }    
// // #ifdef L6470_PRINT_MESSAGE
// //     L6470_u_packet send = *pkt;
// // #endif

//     // printf("transmite pkt\n");
// 	int i = 0,j = 0;
// 	for (i = 0; i<len; i++){
// 	//	j += wiringPiSPIDataRW(L6470_SPI_CH, data,1);
//         // printf("transmit: %02x, %02x, ", t_pkt[i*L6470_DEV_NUM], t_pkt[i*L6470_DEV_NUM + 1]);
// 		j += wiringPiSPIDataRW(L6470_SPI_CH, (unsigned char *)(t_pkt + i*L6470_DEV_NUM),L6470_DEV_NUM);
//         // printf("Receive:  %02x, %02x\n", t_pkt[i*L6470_DEV_NUM], t_pkt[i*L6470_DEV_NUM + 1]);
// 	//	data++;
// 	}


// // #ifdef L6470_PRINT_MESSAGE
// //     L6470_debug_print(msg,&(send),pkt);
// // #endif

//    return j; 
// }

int L6470_rw_multi(L6470_DATA_ARRAY* datum, const char * msg)
{
    int len = 0;
    int ret = 0;
    uint8_t t_pkt[L6470_DEV_NUM*4] = {0};

#ifdef L6470_PRINT_MESSAGE
    L6470_DATA_ARRAY send = *datum;
#endif

    // summrize data 
    for(int itr = 0; itr < L6470_DEV_NUM; itr++){
        for(int pkt_num = 0; pkt_num < 4; pkt_num++){
            t_pkt[itr + L6470_DEV_NUM*pkt_num] = datum->dev[itr].pkt.value8b[pkt_num];
        }
        if(len < datum->dev[itr].s_byte) len = datum->dev[itr].s_byte;
    }

    // transmission SPI data
    for(int tran = 0; tran < len; tran++){
        ret += wiringPiSPIDataRW(L6470_SPI_CH,(unsigned char*)(t_pkt + tran*L6470_DEV_NUM), L6470_DEV_NUM);
    }

    // distributed data
    for(int itr = 0; itr < L6470_DEV_NUM; itr++){
        for(int pkt_num = 0; pkt_num < 4; pkt_num++){
            datum->dev[itr].pkt.value8b[pkt_num] = t_pkt[itr + L6470_DEV_NUM*pkt_num];
        }

#ifdef L6470_PRINT_MESSAGE
        L6470_debug_print(msg,&(send.dev[itr].pkt),&(datum->dev[itr].pkt));
#endif
    }



    return ret;

}

// int L6470_rw_multi(L6470_u_packet *pkt,const char* msg, ...)
// {
//     //uint8_t *data;
//     //data = pkt->value8b;
//     // printf("rw_multi start\n");
//     va_list args;
//     va_start(args,L6470_DEV_NUM * 3);

// #ifdef L6470_PRINT_MESSAGE
//     L6470_u_packet* send[L6470_DEV_NUM];
// #endif
//     // printf("daclare value\n");
//     L6470_u_packet* input_pkt[L6470_DEV_NUM] = {0};
//     int input_len[L6470_DEV_NUM] = {0};
//     char* input_msg[L6470_DEV_NUM] = {0};
//     int max_len = 0;

//     // printf("args to array\n");

//     input_pkt[0] = pkt;
//     input_len[0] = len;
//     input_msg[0] = msg;


//     /* change args to array for useful format */
//     for(int itr = 1; itr < L6470_DEV_NUM; itr++)
//     {
        
//         input_pkt[itr] = va_arg(args, L6470_u_packet *);
//         input_len[itr] = va_arg(args, int);
//         input_msg[itr] = va_arg(args, char *);

//         // printf("%02x, %02x, %02x, %02x\n",
//         //                             input_pkt[itr]->value8b[0],
//         //                             input_pkt[itr]->value8b[1],
//         //                             input_pkt[itr]->value8b[2],
//         //                             input_pkt[itr]->value8b[3]
//         //                             );
//     }

//     // printf("summarize pkts to array\n");
//     uint8_t trans_pkt[L6470_DEV_NUM*4] = {0};
//     /* summarize pkts to uint8_t array */
//     for(int itr = 0; itr < L6470_DEV_NUM; itr++){
//         /* get a pkt */
//         for(int pkt_num = 0; pkt_num < 4; pkt_num++){
//             // printf("summarize pkts itr=%d, pkt_num=%d\n",itr,pkt_num);
//             // printf("[itr + num*pkt_num] = %d, input_pkt[itr]->value8b[pkt_num] = %02x\n",(itr + L6470_DEV_NUM*pkt_num), input_pkt[itr]->value8b[pkt_num]);
//             trans_pkt[itr + (L6470_DEV_NUM * pkt_num)] = input_pkt[itr]->value8b[pkt_num];
//         }
            
//         /* get a len */
//         // printf("len check itr=%d\n",itr);
//         if(max_len < input_len[itr]) max_len = input_len[itr];

//         // /* get a msg */
//         // input_msg[itr] = input_msg[itr];

// #ifdef L6470_PRINT_MESSAGE
//         // printf("copy for send itr=%d\n",itr);
//         send[itr] = input_pkt[itr];
// #endif
//     }

//     // printf("transmite packets\n");
//     /*  transmission all packets */
// 	int i = 0,j = 0;
// 	for (i = 0; i<max_len; i++){

// 	//	j += wiringPiSPIDataRW(L6470_SPI_CH, data,1);
// 		// j += wiringPiSPIDataRW(L6470_SPI_CH, 
//         //         (unsigned char *)(pkt->value8b + i),L6470_DEV_NUM);
//         j += wiringPiSPIDataRW(L6470_SPI_CH,
//                 (uint8_t *)(trans_pkt + i*L6470_DEV_NUM), L6470_DEV_NUM);

// 	//	data++;
// 	}

//     // printf("distribute packets\n");
//     /* distribute packets to original pointer */
//     for(int itr = 0; itr < L6470_DEV_NUM; itr++)
//     {
//         for(int pkt_num = 0; pkt_num < 4; pkt_num++)
//             input_pkt[itr]->value8b[pkt_num] = trans_pkt[itr + (L6470_DEV_NUM * pkt_num)];

// #ifdef L6470_PRINT_MESSAGE
//         L6470_debug_print(input_msg[itr], send[itr], input_pkt[itr]);
// #endif
//     // printf("fin\n");
//     }


// // #ifdef L6470_PRINT_MESSAGE
// //     L6470_debug_print(msg,&(send),pkt);
// // #endif

//     va_end(args);

//     return j; 
// }

/* times = 0 to 4 */
L6470_DATA_T L6470_nop(int times)
{
    L6470_DATA_T data;
    L6470_u_packet pkt = {0};
    // int SPI_res = 0;
    // int size = L6470_cmd[enum_L6470_NOP].send_bit_size;

    // L6470_rw(&(pkt),times, "NOP");
    data.enum_prm = enum_L6470_NOP;
    data.pkt = pkt;
    data.s_byte = times;

    return data;
}

L6470_DATA_T L6470_SetParam(int enum_param, uint32_t value)
{
    L6470_DATA_T data;
    L6470_u_packet pkt = {0};
    // int SPI_res = 0;
    int size = L6470_param[enum_param].param_size;
    
    pkt.value8b[0] = (L6470_param[enum_param].addr | L6470_cmd[enum_L6470_SETPARAM].addr);

    if(8 >= size){
        pkt.value8b[1] = ((value & 0xFF));
    }else if (16 >= size){
        pkt.value8b[1] = ((value & 0xFF00) >> 8);
        pkt.value8b[2] = ((value & 0x00FF));
    }else{
        pkt.value8b[1] = ((value & 0xFF0000) >> 16);
        pkt.value8b[2] = ((value & 0x00FF00) >> 8);
        pkt.value8b[3] =  (value & 0x0000FF);
    }
    
    // SPI_res = L6470_rw(&(pkt),bit2byte(size + ADDR_SIZE), "SetParam");
    data.s_byte = bit2byte(size) + 1;
    data.enum_prm = enum_param;
    data.pkt = pkt;

    return data;
}

L6470_DATA_T L6470_GetParam(int enum_param)
{
    L6470_DATA_T data;
    L6470_u_packet pkt={0};
    int SPI_res = 0;
    int32_t ret = 100;

    int size = L6470_param[enum_param].param_size;
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

    data.s_byte = bit2byte(size) + 1;
    data.enum_prm = enum_param;
    data.pkt = pkt;
    return data;
}

/*speed = 0 to 15625000 [x0.001 step/s] */
L6470_DATA_T L6470_MoveRun(uint8_t dir, uint32_t speed)
{
    int32_t speed_val = (int32_t)round((double)speed / SPEED_RESOLUTION);

#if defined (L6470_PRINT_MESSAGE)
    int32_t temp_speed = (int32_t)round((double)speed_val * SPEED_RESOLUTION);
    if( temp_speed != speed)
        printf("%s %s speed is rounded to %d [x0.001 step/s]\n",L6470_PRINT_HEADER,L6470_PRINT_CAUTION, temp_speed);
    
    for(int itr = 0; itr < L6470_DEV_NUM; itr++){
        L6470_u_packet temp = L6470_setting[itr][enum_L6470_MAX_SPEED];
        int32_t max_speed = (temp.data.value8b[0] << 8) + (temp.data.value8b[1]);
        max_speed = max_speed * MAX_SPEED_RESOLUTION * 10; /* 10times MAX_SPEDD [x0.01 step/s] / SPEED [x0.001 step/s] */ 
        if( temp_speed > max_speed )
            printf("%s %s speed is over MAX_SPEED(Dev %d). rounded to %d [x0.001 step/s]\n",L6470_PRINT_HEADER,L6470_PRINT_CAUTION, itr,max_speed);

    }

#endif
    // L6470_ExecCmd(L6470_cmd[enum_L6470_MoveRun],dir,speed_val, "MoveCont");
    return L6470_makeCmd(L6470_cmd[enum_L6470_MoveRun],dir,speed_val);
}

/* dir = DIR_FWD or DIR_RVS */
L6470_DATA_T L6470_MoveStepClock(uint8_t dir)
{
    // L6470_ExecCmd(L6470_cmd[enum_L6470_MOVESTEPCLOCK],dir,0, "MoveStepClock");
    return L6470_makeCmd(L6470_cmd[enum_L6470_MOVESTEPCLOCK],dir,0);
}

/* step = 0 to 4194303(2^22) [step] */
L6470_DATA_T L6470_MoveStep(uint8_t dir,uint32_t step)
{
    // L6470_ExecCmd(L6470_cmd[enum_L6470_MOVESTEP],dir,step, "MoveStep");
    return L6470_makeCmd(L6470_cmd[enum_L6470_MOVESTEP],dir,step);
}

/* abs_pos = -2^21 to 2^21 -1 [step]*/
L6470_DATA_T L6470_MoveGoTo(int32_t abs_pos)
{
    // L6470_ExecCmd(L6470_cmd[enum_L6470_MOVEGOTO],0,abs_pos, "MoveGoTo");
    return L6470_makeCmd(L6470_cmd[enum_L6470_MOVEGOTO],0,abs_pos);
}

/* dir = DIR_FWD or DIR_RVS, abs_pos = -2^21 to 2^21-1 [step] */
L6470_DATA_T L6470_MoveGoToDir(uint8_t dir,int32_t abs_pos)
{
    // L6470_ExecCmd(L6470_cmd[enum_L6470_MOVEGOTODIR],dir,abs_pos,"MoveGoToDir");
    return L6470_makeCmd(L6470_cmd[enum_L6470_MOVEGOTODIR],dir,abs_pos);
}

/* act = ACT_POS_CLEAR or ACT_POS_TO_MARK */
/* dir = DIR_FWD or DIR_RVS */
/*speed = 0 to 15625000 [x0.001 step/s] */
L6470_DATA_T L6470_MoveGoToUntil(uint8_t act, uint8_t dir,uint32_t speed)
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
L6470_DATA_T L6470_MoveRelease(uint8_t act, uint8_t dir)
{
    // L6470_ExecCmd(L6470_cmd[enum_L6470_MOVERELEASE],act|dir,0,"MoveRelease");
    return L6470_makeCmd(L6470_cmd[enum_L6470_MOVERELEASE],act|dir,0);
}

L6470_DATA_T L6470_GoHome(void)
{
    // L6470_ExecCmd_NoArg(L6470_cmd[enum_L6470_GOHOME], "GoHome");
    return L6470_makeCmd(L6470_cmd[enum_L6470_GOHOME], 0, 0);
}

L6470_DATA_T L6470_GoMark(void)
{
    // L6470_ExecCmd_NoArg(L6470_cmd[enum_L6470_GOMARK], "GoMark");
    return L6470_makeCmd(L6470_cmd[enum_L6470_GOMARK], 0, 0);
}

L6470_DATA_T L6470_ResetPos(void)
{
    // L6470_ExecCmd_NoArg(L6470_cmd[enum_L6470_RESETPOS], "ResetPos");
    return L6470_makeCmd(L6470_cmd[enum_L6470_RESETPOS], 0, 0);
}

L6470_DATA_T L6470_ResetDevice(void)
{
    // L6470_ExecCmd_NoArg(L6470_cmd[enum_L6470_RESETDEVICE],"ResetDevice");
    return L6470_makeCmd(L6470_cmd[enum_L6470_RESETDEVICE], 0, 0);
}

L6470_DATA_T L6470_StopSoft(void)
{
    // L6470_ExecCmd_NoArg(L6470_cmd[enum_L6470_STOPSOFT], "StopSoft");
    return L6470_makeCmd(L6470_cmd[enum_L6470_STOPSOFT], 0, 0);
}

L6470_DATA_T L6470_StopHard(void)
{
    // L6470_ExecCmd_NoArg(L6470_cmd[enum_L6470_STOPHARD], "StopHard");
    return L6470_makeCmd(L6470_cmd[enum_L6470_STOPHARD], 0, 0);
}

L6470_DATA_T L6470_HiZSoft(void)
{
    // L6470_ExecCmd_NoArg(L6470_cmd[enum_L6470_HIZSOFT],"HiZSoft");
    return L6470_makeCmd(L6470_cmd[enum_L6470_HIZSOFT], 0, 0);
}

L6470_DATA_T L6470_HiZHard(void)
{
    // L6470_ExecCmd_NoArg(L6470_cmd[enum_L6470_HIZHARD],"HIZHard");
    return L6470_makeCmd(L6470_cmd[enum_L6470_HIZHARD], 0, 0);
}

L6470_DATA_T L6470_makeCmd( L6470_CMD cmd, int orprm, uint32_t arg_param)
{
    L6470_DATA_T data = {0};
    L6470_u_packet pkt={0};
    // int SPI_res = 0;

    int size = cmd.send_bit_size;
    pkt.data.reg_addr = (cmd.addr | orprm);
    if(pow(2,cmd.send_bit_size) < abs(arg_param))
    {
#if defined (L6470_PRINT_MESSAGE)
        printf("%s AbortCmd size_over cmdsize:2^%d, but arg is %d\n ",L6470_PRINT_HEADER, cmd.send_bit_size,arg_param);
#endif
        return data;
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

    data.enum_prm = cmd.self_num;
    data.s_byte = bit2byte(size) + 1;
    data.pkt = pkt;

    return data;
    // SPI_res = L6470_rw(&(pkt),bit2byte(size + ADDR_SIZE),msg);
}

// static void L6470_ExecCmd( L6470_CMD cmd, int orprm, uint32_t arg_param,const char* msg)
// {
//     L6470_u_packet pkt={0};
//     int SPI_res = 0;

//     int size = cmd.send_bit_size;
//     pkt.data.reg_addr = (cmd.addr | orprm);
//     if(pow(2,cmd.send_bit_size) < abs(arg_param))
//     {
// #if defined (L6470_PRINT_MESSAGE)
//         printf("%s AbortCmd size_over cmdsize:2^%d, but arg is %d\n ",L6470_PRINT_HEADER, cmd.send_bit_size,arg_param);
// #endif
//         return;
//     }
//     if(8 >= size){
//         pkt.data.value8b[0] = ((arg_param & 0x0000FF));
//     }else if (16 >= size){
//         pkt.data.value8b[0] = ((arg_param & 0x00FF00) >> 8);
//         pkt.data.value8b[1] = ((arg_param & 0x0000FF));
//     }else{
//         pkt.data.value8b[0] = ((arg_param & 0xFF0000) >> 16);
//         pkt.data.value8b[1] = ((arg_param & 0x00FF00) >> 8);
//         pkt.data.value8b[2] = ((arg_param & 0x0000FF));
//     }

//     SPI_res = L6470_rw(&(pkt),bit2byte(size + ADDR_SIZE),msg);
// }

// static void L6470_ExecCmd_NoArg( L6470_CMD cmd, const char* msg)
// {
//     L6470_u_packet pkt={0};
//     int SPI_res = 0;
        
//     int size = cmd.send_bit_size;
//     pkt.data.reg_addr = cmd.addr;

//     SPI_res = L6470_rw(&(pkt),bit2byte(size + ADDR_SIZE),msg);

// }

// int32_t L6470_GetAbsPos(void)
L6470_ABSPOS_T L6470_GetAbsPos(void)
{
    // int32_t pos = 0;
    // int32_t ret = 0;
    L6470_ABSPOS_T ret = {0}; 
    L6470_DATA_T data;
    // pos = L6470_GetParam(enum_L6470_ABS_POS);
    data = L6470_GetParam(enum_L6470_ABS_POS);

    L6470_DATA_ARRAY ary;
    for(int itr = 0; itr < L6470_DEV_NUM; itr++) {
        ary.dev[itr] = data;
    }

    // L6470_rw_all(&pkt, L6470_param[enum_L6470_ABS_POS].param_size + 1, "GetAbsPos");
    int SPI_res = L6470_rw_multi(&ary, "GetAbsPos");

    for(int itr = 0; itr < L6470_DEV_NUM; itr++){

        int32_t pos = 0;
        pos |= (ary.dev[itr].pkt.data.value8b[0] << 16); 
        pos |= (ary.dev[itr].pkt.data.value8b[1] << 8); 
        pos |= (ary.dev[itr].pkt.data.value8b[2]);

        if(((pos & 0x200000) >> 21) == 1){
	        pos = (-1) * ((~pos + 1) & 0x3FFFFF);	
        }

        ret.dev[itr] = pos;

#ifdef L6470_PRINT_MESSAGE
    	printf("Dev %d pos: %d\n",itr, pos);
#endif

    }


    return ret;

}


L6470_STATUS_T L6470_GetStatus(void)
{
    L6470_STATUS_T s_ary = {0};
    L6470_DATA_ARRAY d_ary;
    L6470_DATA_T data;
    L6470_u_packet pkt = {0};

    int size = L6470_cmd[enum_L6470_GETSTATUS].send_bit_size;
    pkt.data.reg_addr = L6470_cmd[enum_L6470_GETSTATUS].addr;

    data.enum_prm = enum_L6470_GETSTATUS;
    data.pkt = pkt;
    data.s_byte = bit2byte(size) + 1;

    for(int itr = 0; itr < L6470_DEV_NUM;itr++){
        d_ary.dev[itr] = data;
    }

    int SPI_res = L6470_rw_multi(&d_ary, "GetStatus");
    // SPI_res = L6470_rw(&(pkt),bit2byte(size + ADDR_SIZE),"GetStatus");

    for(int itr = 0; itr < L6470_DEV_NUM; itr++){
        L6470_u_packet temp = d_ary.dev[itr].pkt;

        s_ary.dev[itr] = (temp.data.value8b[0] << 8) & (temp.data.value8b[1]);
    }

    // return ((pkt.value8b[2] << 8) & (pkt.value8b[3]));
    return s_ary;

}


#if defined (L6470_PRINT_MESSAGE)
static void L6470_debug_print(const char *msg,L6470_u_packet* send, L6470_u_packet* get)
{
    if(msg != NULL)
    {
        printf("%s %s send:0x%02x 0x%02x 0x%02x 0x%02x\n", L6470_PRINT_HEADER, msg, 
				send->value8b[0], 
				send->value8b[1], 
				send->value8b[2], 
				send->value8b[3]);
        if (get != (L6470_u_packet*)NULL)
            printf("%s %s  get:0x%02x 0x%02x 0x%02x 0x%02x\n", L6470_PRINT_HEADER, msg,  
			         get->value8b[0], 
			         get->value8b[1], 
			         get->value8b[2], 
			         get->value8b[3]);
    }
}
#endif


/* generator function */

/* (-2^21 <= abs_pos <= +2^21 -1) with the selected step mode */
L6470_u_packet gen_ABS_POS(int32_t abs_pos)
{
#if defined (L6470_PRINT_MESSAGE)
    if( (abs_pos > (int32_t)(pow(2,21)-1)) | (abs_pos < (int32_t)((-1)*pow(2,21))) ) 
        printf(" %s %s abs_pos is over/under flow\n",L6470_PRINT_HEADER,L6470_PRINT_CAUTION);
#endif
    int32_t val = abs_pos & (int32_t)(pow(2,21)-1);
    L6470_u_packet pkt = generate_pkt(enum_L6470_ABS_POS, val);
    return pkt;
}

/* step = 0 to 3, el_step = 0 to 127(masked with selected step mode in IC) */
L6470_u_packet gen_EL_POS(int32_t step_u_step)
{
    int32_t step = (step_u_step & STEP_ePOS_MASK);
    int32_t u_step = (step_u_step & STEP_u_STEP_MASK);
#if 0
#if defined (L6470_PRINT_MESSAGE)
    printf(" %s %s step is selected STEP_ePOS_%ddeg.\n",
            L6470_PRINT_HEADER, L6470_PRINT_CAUTION, 90 * (step >> STEP_ePOS_ofset));
    printf(" %s %s el_step is selected %d\n",L6470_PRINT_HEADER, L6470_PRINT_CAUTION, u_step));
#endif
#endif

    uint32_t val;
    val = (step + u_step);
    L6470_u_packet pkt = generate_pkt(enum_L6470_EL_POS, val);
    return pkt;
}

/* (-2^21 <= mark <= +2^21 -1) with the selected step mode */
L6470_u_packet gen_MARK(int32_t mark)
{
#if defined (L6470_PRINT_MESSAGE)
    if( (mark > (int32_t)(pow(2,21)-1)) | (mark < (int32_t)((-1)*pow(2,21))) )
        printf(" %s %s MARK is over/under flow\n",L6470_PRINT_HEADER,L6470_PRINT_CAUTION);
#endif
    int32_t val = ( mark & (int32_t)(pow(2,21) - 1) );
    L6470_u_packet pkt = generate_pkt(enum_L6470_MARK, mark);
    return pkt;
}

/* step_per_ss = N x ACC_RESOLUTION [x0.01 step/s^2]*/
L6470_u_packet gen_ACC(int32_t step_per_ss)
{
    int32_t val = (int32_t)round((double)step_per_ss / ACC_RESOLUTION) ;
#if defined (L6470_PRINT_MESSAGE)
    if(val == 0){
        printf(" %s %s ACC step_per_ss is more than equal %d [x0.01 step/s^2]\n",
                        L6470_PRINT_HEADER, L6470_PRINT_CAUTION, ACC_RESOLUTION);
        printf(" %s %s set minumum value.\n",
                        L6470_PRINT_HEADER, L6470_PRINT_CAUTION);
        val = 1;
    } else if((int32_t)round((double)val * ACC_RESOLUTION) != step_per_ss){
        printf(" %s %s ACC is rounded to %d [x0.01 step/s^2]\n",
                        L6470_PRINT_HEADER, L6470_PRINT_CAUTION, (int32_t)round((double)val * ACC_RESOLUTION));
    } 
#endif

    L6470_u_packet pkt = generate_pkt(enum_L6470_ACC, val);
    return pkt;
}

/* step_per_ss = N x DEC_RESOLUTION [0.01 step/s^2] */
L6470_u_packet gen_DEC(int32_t step_per_ss)
{
    int32_t val = (int32_t)round((double)step_per_ss / DEC_RESOLUTION);
#if defined (L6470_PRINT_MESSAGE)
    if(val == 0){
        printf(" %s %s DEC step_per_ss is more than equal %d [x0.01 step/s^2]\n",
                        L6470_PRINT_HEADER, L6470_PRINT_CAUTION, DEC_RESOLUTION);
        printf(" %s %s set minumum value.\n",L6470_PRINT_HEADER, L6470_PRINT_CAUTION);
        val = 1;
    } else if((int32_t)round((double)val * DEC_RESOLUTION) != step_per_ss){
        printf(" %s %s DEC is rounded to %d [x0.01 step/s^2]\n",
                        L6470_PRINT_HEADER, L6470_PRINT_CAUTION, (int32_t)round((double)val * DEC_RESOLUTION));
    } 
#endif

    L6470_u_packet pkt = generate_pkt(enum_L6470_DEC, val);
    return pkt;
}

/* step_per_s = N x MAX_SPEED_RESOLUTION [x0.01 step/s^2]*/
L6470_u_packet gen_MAX_SPEED(int32_t step_per_s)
{
    int32_t val = (int32_t)round(((double)step_per_s - MAX_SPEED_MIN) / MAX_SPEED_RESOLUTION);
#if defined (L6470_PRINT_MESSAGE)
    if(val == 0){
        printf(" %s %s MAX_SPEED is more than equal %d\n",
                        L6470_PRINT_HEADER, L6470_PRINT_CAUTION, MAX_SPEED_MIN);
        printf(" %s %s set minumum value.\n",L6470_PRINT_HEADER, L6470_PRINT_CAUTION);
        val = 0;
    } else if((int32_t)round((double)val * MAX_SPEED_RESOLUTION) != step_per_s){
        printf(" %s %s MAX_SPEED is rounded to %d [x0.01 step/s]\n",
                        L6470_PRINT_HEADER, L6470_PRINT_CAUTION, (int32_t)round((double)val * MAX_SPEED_RESOLUTION));
    } 
#endif

    L6470_u_packet pkt = generate_pkt(enum_L6470_MAX_SPEED, val);
    return pkt;
}

/* step_per_s = M x MIN_SPEED_RESOLUTION, enable_LSPD = ENABLE_LSPD or DISABLE */
L6470_u_packet gen_MIN_SPEED(int32_t enable_LSPD_step_per_s)
{
    L6470_u_packet pkt = {0};
    if( 0 != (enable_LSPD_step_per_s & (0b01111110 << (ENABLE_LSPD_MAGIC)) ) ) {
#if defined (L6470_PRINT_MESSAGE)
        printf(" %s %s MIN_SPEED is over\n",L6470_PRINT_HEADER,L6470_PRINT_CAUTION);
#endif
    return pkt;
    } /*check 0b1111 1110 000~ */

    int32_t step_per_s = (enable_LSPD_step_per_s & MIN_SPEED_MASK); /* 20 bits */
    int32_t enable_LSPD = (enable_LSPD_step_per_s & ENABLE_LSPD); /* 25 bit */

    int32_t val = (int32_t)round((double)step_per_s / MIN_SPEED_RESOLUTION);

#if defined (L6470_PRINT_MESSAGE)
    if((int32_t)round((double)val * MIN_SPEED_RESOLUTION) != step_per_s){
        printf(" %s %s MIN_SPEED is rounded to %d [x0.001 step/s]\n",
                        L6470_PRINT_HEADER, L6470_PRINT_CAUTION, (int32_t)round((double)val * MIN_SPEED_RESOLUTION));
    } 
#endif
    if(DISABLE != enable_LSPD) val |= ENABLE_LSPD_BIT;

    pkt = generate_pkt(enum_L6470_MIN_SPEED, val);
    return pkt;
}

/* percentage = 0 to 10000 [0.01%]*/
L6470_u_packet gen_KVAL_HOLD(int32_t percentage)
{
    L6470_u_packet pkt = generate_pkt_with_percentage(enum_L6470_KVAL_HOLD, percentage);
    return pkt;
}

/* percentage = 0 to 10000 [0.01%] */
L6470_u_packet gen_KVAL_RUN(int32_t percentage)
{
    L6470_u_packet pkt = generate_pkt_with_percentage(enum_L6470_KVAL_RUN, percentage);
    return pkt;
}

/* percentage = 0 to 10000 [0.01%] */
L6470_u_packet gen_KVAL_ACC(int32_t percentage)
{
    L6470_u_packet pkt = generate_pkt_with_percentage(enum_L6470_KVAL_ACC, percentage);
    return pkt;
}

/* percentage = 0 to 10000 [0.01%] */
L6470_u_packet gen_KVAL_DEC(int32_t percentage)
{
    L6470_u_packet pkt = generate_pkt_with_percentage(enum_L6470_KVAL_DEC, percentage);
    return pkt;
}

/* step_per_s = N x INT_SPEED_RESOLUTION [x0.0001 step/s] */
L6470_u_packet gen_INT_SPEED(int32_t step_per_s)
{
    uint32_t val = (int32_t)round((double)step_per_s / INT_SPEED_RESOLUTION);
#if defined (L6470_PRINT_MESSAGE)
    if((int32_t)round((double)val * INT_SPEED_RESOLUTION) != step_per_s){
        printf(" %s %s INT_SPEED is rounded to %d [x0.0001 step/s] \n",
                        L6470_PRINT_HEADER, L6470_PRINT_CAUTION, (int32_t)round((double)val * INT_SPEED_RESOLUTION));
    } 
#endif

    L6470_u_packet pkt = generate_pkt(enum_L6470_INT_SPEED, val);
    return pkt;
}

/* slp = N x ST_SLP_RESOLUTION [x0.000001]*/
L6470_u_packet gen_ST_SLP(int32_t slp)
{
    uint32_t val = (int32_t)round((double)slp / ST_SLP_RESOLUTION);
#if defined (L6470_PRINT_MESSAGE)
    if((int32_t)round((double)val * ST_SLP_RESOLUTION) != slp){
        printf(" %s %s ST_SLP is rounded to %d [x0.000001] \n",
                        L6470_PRINT_HEADER, L6470_PRINT_CAUTION, (int32_t)round((double)val * ST_SLP_RESOLUTION));
    } 
#endif
    L6470_u_packet pkt = generate_pkt(enum_L6470_ST_SLP, val);
    return pkt;
}

/* slp_acc = N x FN_SLP_ACC_RESOLUTION*/
L6470_u_packet gen_FN_SLP_ACC(int32_t slp_acc)
{
    uint32_t val = (int32_t)round((double)slp_acc / FN_SLP_ACC_RESOLUTION);
#if defined (L6470_PRINT_MESSAGE)
    if((int32_t)round((double)val * FN_SLP_ACC_RESOLUTION) != slp_acc){
        printf(" %s %s FN_SLP_ACC is rounded to %d [x0.000001]\n",
                        L6470_PRINT_HEADER, L6470_PRINT_CAUTION, (int32_t)round((double)val * FN_SLP_ACC_RESOLUTION));
    }
#endif
    L6470_u_packet pkt = generate_pkt(enum_L6470_FN_SLP_ACC, val);
    return pkt;
}

/* slp_dec = N x FN_SLP_DEC_RESOLUTION [x0.000001]*/
L6470_u_packet gen_FN_SLP_DEC(int32_t slp_dec)
{
    uint32_t val = (int32_t)round((double)slp_dec / FN_SLP_DEC_RESOLUTION);
#if defined (L6470_PRINT_MESSAGE)
    if((int32_t)round((double)val * FN_SLP_DEC_RESOLUTION) != slp_dec){
        printf(" %s %s FN_SLP_DEC is rounded to %d [x0.000001]\n",
                        L6470_PRINT_HEADER, L6470_PRINT_CAUTION, (int32_t)round((double)val * FN_SLP_DEC_RESOLUTION));
    } 
#endif
    L6470_u_packet pkt = generate_pkt(enum_L6470_FN_SLP_DEC, val);
    return pkt;
}

/* k_therm = N x K_THERM_RESOLUTION */
L6470_u_packet gen_K_THERM(int32_t k_therm)
{
    int32_t val = (int32_t)round((double)(k_therm - K_THERM_MIN) / K_THERM_RESOLUTION);
#if defined (L6470_PRINT_MESSAGE)
    if(((int32_t)round((double)val * K_THERM_RESOLUTION) + K_THERM_MIN) != k_therm){
        printf(" %s %s K_THERM is rounded to %d [x0.00001]\n",
                        L6470_PRINT_HEADER, L6470_PRINT_CAUTION, (int32_t)round((double)val * K_THERM_RESOLUTION) + K_THERM_MIN);
    } 
#endif
    L6470_u_packet pkt = generate_pkt(enum_L6470_K_THERM, val);
    return pkt;
}

/* ocd_th = 375 to 6000 mA */
L6470_u_packet gen_OCD_TH(int32_t ocd_th)
{
    uint32_t val = (int32_t)round((double)(ocd_th - OCD_TH_RESOLUTION) / OCD_TH_RESOLUTION);
#if defined (L6470_PRINT_MESSAGE)
    if(((int32_t)round((double)val * OCD_TH_RESOLUTION) + OCD_TH_RESOLUTION) != ocd_th){
        printf(" %s %s OCD_TH is rounded to %d [mA]\n",
                        L6470_PRINT_HEADER, L6470_PRINT_CAUTION, ((int32_t)round((double)val * OCD_TH_RESOLUTION) + OCD_TH_RESOLUTION));
    } 
#endif
    L6470_u_packet pkt = generate_pkt(enum_L6470_OCD_TH, val);
    return pkt;
}

/* stall_th = 31.25 to 4000 mA */
L6470_u_packet gen_STALL_TH(int32_t stall_th)
{
    int32_t val = (int32_t)round((double)(stall_th - STALL_TH_RESOLUTION) / STALL_TH_RESOLUTION);
#if defined (L6470_PRINT_MESSAGE)
    if(((int32_t)round((double)val * STALL_TH_RESOLUTION) + STALL_TH_RESOLUTION) != stall_th){
        printf(" %s %s STALL_TH is rounded to %d [x0.01 mA]\n",
                        L6470_PRINT_HEADER, L6470_PRINT_CAUTION, ((int32_t)round((double)val * STALL_TH_RESOLUTION) + STALL_TH_RESOLUTION));
    } 
#endif    
    L6470_u_packet pkt = generate_pkt(enum_L6470_STALL_TH, val);
    return pkt;
}
/* fs_spd = 7.63(FS_SPD_MIN) to 15625 [step/s] */
L6470_u_packet gen_FS_SPD(int32_t fs_spd)
{
    uint32_t val = (int32_t)round((double)(fs_spd - FS_SPD_MIN) / FS_SPD_RESOLUTION);
#if defined (L6470_PRINT_MESSAGE)
    if(((int32_t)round((double)val * FS_SPD_RESOLUTION) + FS_SPD_MIN) != fs_spd){
        printf(" %s %s FS_SPD is rounded to %d [x0.01 step/s]\n",
        L6470_PRINT_HEADER, L6470_PRINT_CAUTION, (int32_t)round((double)val * FS_SPD_RESOLUTION) + FS_SPD_MIN);
    } 
#endif   

    L6470_u_packet pkt = generate_pkt(enum_L6470_FS_SPD, val);
    return pkt;
}

/* sync_en = SYNC_EN or DISABLE, sync_sel = SYNC_FFS_*, step_sel = STEP_* */
L6470_u_packet gen_STEP_MODE(int32_t mode)
{
#if defined (L6470_PRINT_MESSAGE)
    uint8_t sync_en = (mode & SYNC_EN);
    uint8_t sync_sel = (mode & SYNC_SEL_MASK);
    uint8_t step_sel = (mode & STEP_SEL_MASK);
    if( (sync_sel >> SYNC_SEL_ofset) > step_sel)
        printf(" %s %s sync_sel is less than equal step_sel.\n",L6470_PRINT_HEADER, L6470_PRINT_CAUTION);    
#endif
    int32_t val = mode;
    L6470_u_packet pkt = generate_pkt(enum_L6470_STEP_MODE, val);
    return pkt;
}

/* alm = ALM_* | ALM_* ... */
L6470_u_packet gen_ALARM_EN(int32_t alm)
{
    int32_t val = alm;
    L6470_u_packet pkt = generate_pkt(enum_L6470_ALARM_EN, val);
    return pkt;
}

/* f_pwm_int, f_pwm_dec, pow_sr, oc_sd, en_vscomp, sw_mode, ext_clk, osc_sel */
L6470_u_packet gen_CONFIG(int32_t param)
{
#if 0
#if defined (L6470_PRINT_MESSAGE)
    int32_t f_pwm_int   = (param & F_PWM_INT_MASK);
    int32_t f_pwm_dec   = (param & F_PWM_DEC_MASK);
    int32_t pow_sr      = (param & POW_SR_MASK);
    int32_t oc_sd       = (param & OC_SD_MASK);
    int32_t en_vscomp   = (param & EN_VSCOMP_MASK);
    int32_t sw_mode     = (param & SW_MODE_MASK);
    int32_t ext_clk     = (param & EXT_CLK_MASK);
    int32_t osc_sel     = (param & OSC_SEL_MASK);
#endif
#endif

    int32_t val =  param;
    L6470_u_packet pkt = generate_pkt(enum_L6470_CONFIG, val);
    return pkt;
}

static L6470_u_packet generate_pkt(int enum_param,int32_t val)
{
    L6470_u_packet pkt = {0};
    pkt.data.reg_addr = L6470_param[enum_param].addr;

    int size = L6470_param[enum_param].param_size;

    if(8 >= size){
        pkt.data.value8b[0] = (uint8_t)(val & 0xFF);
    } else if (16 >= size){
        pkt.data.value8b[0] = (uint8_t)((val & 0xFF00) >> 8);
        pkt.data.value8b[1] = (uint8_t)((val & 0x00FF));
    } else {
        pkt.data.value8b[0] = (uint8_t)((val & 0xFF0000) >> 16);
        pkt.data.value8b[1] = (uint8_t)((val & 0x00FF00) >> 8);
        pkt.data.value8b[2] = (uint8_t)((val & 0x0000FF));
    }
    
    return pkt;
}

/* percentage = 0 to 10000 [x0.01%] */
static L6470_u_packet generate_pkt_with_percentage(int enum_param, int32_t percentage)
{
    L6470_u_packet pkt = {0};
    if( (percentage < 0) | (10000 < percentage) )
    {
#if defined (L6470_PRINT_MESSAGE)
        printf(" %s %s percentage = 0 to 10000 [%%]\n",L6470_PRINT_HEADER,L6470_PRINT_CAUTION);
#endif
        return pkt; /* 0x00 pkt */  
    } /*0 to 10000*/

    int32_t val = (int32_t)round((255 * (double)percentage) / 10000);
#if defined (L6470_PRINT_MESSAGE)
    if((int32_t)round(((double)val * 10000) / 255) != percentage)
        printf(" %s %s percentage is round to %d[0.01%%]\n",L6470_PRINT_HEADER,L6470_PRINT_CAUTION,(int32_t)round((val * 10000)/ 255));
#endif
    pkt = generate_pkt(enum_param,val);
    return pkt;
}
