// /* =============================================                    */
// /* Copyright (c) 2021 poemoep                                       */
// /* This software is released under the MIT License, see LICENSE.    */
// /* =============================================                    */ 

// #include "L6470.h"
// #include "L6470_const.h"

// L6470_PARAM L6470_param[PARAM_NUM] =
// {
// { enum_L6470_ABS_POS      , REG_ABS_POS,      REG_SIZE_ABS_POS,       READONLY | WRITABLE_MStop },
// { enum_L6470_EL_POS       , REG_EL_POS ,      REG_SIZE_EL_POS ,       READONLY | WRITABLE_MStop},
// { enum_L6470_MARK         , REG_MARK,         REG_SIZE_MARK,          READONLY | WRITABLE_Always},
// { enum_L6470_SPEED        , REG_SPEED,        REG_SIZE_SPEED,         READONLY }, //readonly
// { enum_L6470_ACC          , REG_ACC,          REG_SIZE_ACC,           READONLY | WRITABLE_MStop},
// { enum_L6470_DEC          , REG_DEC,          REG_SIZE_DEC,           READONLY | WRITABLE_MStop},
// { enum_L6470_MAX_SPEED    , REG_MAX_SPEED,    REG_SIZE_MAX_SPEED,     READONLY | WRITABLE_Always},
// { enum_L6470_MIN_SPEED    , REG_MIN_SPEED,    REG_SIZE_MIN_SPEED,     READONLY | WRITABLE_MStop},
// { enum_L6470_KVAL_HOLD    , REG_KVAL_HOLD,    REG_SIZE_KVAL_HOLD,     READONLY | WRITABLE_Always},
// { enum_L6470_KVAL_RUN     , REG_KVAL_RUN,     REG_SIZE_KVAL_RUN,      READONLY | WRITABLE_Always},
// { enum_L6470_KVAL_ACC     , REG_KVAL_ACC,     REG_SIZE_KVAL_ACC,      READONLY | WRITABLE_Always},
// { enum_L6470_KVAL_DEC     , REG_KVAL_DEC,     REG_SIZE_KVAL_DEC,      READONLY | WRITABLE_Always},
// { enum_L6470_INT_SPEED    , REG_INT_SPEED,    REG_SIZE_INT_SPEED,     READONLY | WRITABLE_HiZ},
// { enum_L6470_ST_SLP       , REG_ST_SLP,       REG_SIZE_ST_SLP,        READONLY | WRITABLE_HiZ},
// { enum_L6470_FN_SLP_ACC   , REG_FN_SLP_ACC,   REG_SIZE_FN_SLP_ACC,    READONLY | WRITABLE_HiZ},
// { enum_L6470_FN_SLP_DEC   , REG_FN_SLP_DEC,   REG_SIZE_FN_SLP_DEC,    READONLY | WRITABLE_HiZ},
// { enum_L6470_K_THERM      , REG_K_THERM,      REG_SIZE_K_THERM,       READONLY | WRITABLE_Always},
// { enum_L6470_ADC_OUT      , REG_ADC_OUT,      REG_SIZE_ADC_OUT,       READONLY }, //readonly
// { enum_L6470_OCD_TH       , REG_OCD_TH,       REG_SIZE_OCD_TH,        READONLY | WRITABLE_Always},
// { enum_L6470_STALL_TH     , REG_STALL_TH,     REG_SIZE_STALL_TH,      READONLY | WRITABLE_Always},
// { enum_L6470_FS_SPD       , REG_FS_SPD,       REG_SIZE_FS_SPD,        READONLY | WRITABLE_Always},
// { enum_L6470_STEP_MODE    , REG_STEP_MODE,    REG_SIZE_STEP_MODE,     READONLY | WRITABLE_HiZ},
// { enum_L6470_ALARM_EN     , REG_ALARM_EN,     REG_SIZE_ALARM_EN,      READONLY | WRITABLE_MStop},
// { enum_L6470_CONFIG       , REG_CONFIG,       REG_SIZE_CONFIG,        READONLY | WRITABLE_HiZ},
// { enum_L6470_STATUS       , REG_STATUS,       REG_SIZE_STATUS,        READONLY }, //readonly
// //{ enum_L6470_RESERVED_h1A , REG_RESERVED_h1A, REG_SIZE_RESERVED_h1A,  RESERVED }, //dummy
// //{ enum_L6470_RESERVED_h1B , REG_RESERVED_h1B, REG_SIZE_RESERVED_h1B,  RESERVED }  //dummy
// };

// const L6470_CMD L6470_cmd[CMD_NUM] =
// {	
// {  enum_L6470_NOP,            CMD_NOP,            CMD_SIZE_NOP},
// {  enum_L6470_SETPARAM,       CMD_SETPARAM,       CMD_SIZE_SETPARAM},
// {  enum_L6470_GETPARAM,       CMD_GETPARAM,       CMD_SIZE_GETPARAM},
// {  enum_L6470_MoveRun,       CMD_MOVECONT,       CMD_SIZE_MOVECONT},
// {  enum_L6470_MOVESTEPCLOCK,  CMD_MOVESTEPCLOCK,  CMD_SIZE_MOVESTEPCLOCK},
// {  enum_L6470_MOVESTEP,       CMD_MOVESTEP,       CMD_SIZE_MOVESTEP},
// {  enum_L6470_MOVEGOTO,       CMD_MOVEGOTO,       CMD_SIZE_MOVEGOTO},
// {  enum_L6470_MOVEGOTODIR,    CMD_MOVEGOTODIR,    CMD_SIZE_MOVEGOTODIR},
// {  enum_L6470_MOVEGOTOUNTIL,  CMD_MOVEGOTOUNTIL,  CMD_SIZE_MOVEGOTOUNTIL},
// {  enum_L6470_MOVERELEASE,    CMD_MOVERELEASE,    CMD_SIZE_MOVERELEASE},
// {  enum_L6470_GOHOME,         CMD_GOHOME,         CMD_SIZE_GOHOME},
// {  enum_L6470_GOMARK,         CMD_GOMARK,         CMD_SIZE_GOMARK},
// {  enum_L6470_RESETPOS,       CMD_RESETPOS,       CMD_SIZE_RESETPOS},
// {  enum_L6470_RESETDEVICE,    CMD_RESETDEVICE,    CMD_SIZE_RESETDEVICE},
// {  enum_L6470_STOPSOFT,       CMD_STOPSOFT,       CMD_SIZE_STOPSOFT},
// {  enum_L6470_STOPHARD,       CMD_STOPHARD,       CMD_SIZE_STOPHARD},
// {  enum_L6470_HIZSOFT,        CMD_HIZSOFT,        CMD_SIZE_HIZSOFT},
// {  enum_L6470_HIZHARD,        CMD_HIZHARD,        CMD_SIZE_HIZHARD},
// {  enum_L6470_GETSTATUS,      CMD_GETSTATUS,      CMD_SIZE_GETSTATUS}
// };