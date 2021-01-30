/* =============================================                    */
/* Copyright (c) 2021 poemoep                                       */
/* This software is released under the MIT License, see LICENSE.    */
/* =============================================                    */


#ifndef __L6470_USER_HEADER__
#define __L6470_USER_HEADER__

#include "L6470.h"

//----define debug message function ON/OFF----
#define L6470_PRINT_MESSAGE 

/*----define SPI setting----*/
#define L6470_SPI_CH 0  // 0 or 1
#define SPI_SPEED 1000000 // [Hz]

/* ----define SPI slave setting----*/
#define L6470_DEV_NUM (2)

const L6470_packet L6470_user_setting[L6470_DEV_NUM][PARAM_NUM] =
{
    {
    // L6470_PARAM_addr,     setting[2],setting[1],setting[0]} //reset_val
    //Left justified, MSB first
    {{REG_ABS_POS,       {0x00,   0x00,   0x00}}}, //0x000000
    {{REG_EL_POS,        {0x00,   0x00,   0x00}}}, //0x000
    {{REG_MARK,          {0x00,   0x00,   0x00}}}, //0x000000
    {{REG_SPEED,         {0x00,   0x00,   0x00}}}, //dummy
    {{REG_ACC,           {0x00,   0x8A,   0x00}}}, //0x08A
    {{REG_DEC,           {0x00,   0x8A,   0x00}}}, //0x08A
    {{REG_MAX_SPEED,     {0x00,   0x41,   0x00}}}, //0x041
    {{REG_MIN_SPEED,     {0x00,   0x00,   0x00}}}, //0x000
    {{REG_KVAL_HOLD,     {0x29,   0x00,   0x00}}}, //0x29
    {{REG_KVAL_RUN,      {0x29,   0x00,   0x00}}}, //0x29
    {{REG_KVAL_ACC,      {0x29,   0x00,   0x00}}}, //0x29
    {{REG_KVAL_DEC,      {0x29,   0x00,   0x00}}}, //0x29
    {{REG_INT_SPEED,     {0x04,   0x08,   0x00}}}, //0x0408
    {{REG_ST_SLP,        {0x19,   0x00,   0x00}}}, //0x19 
    {{REG_FN_SLP_ACC,    {0x29,   0x00,   0x00}}}, //0x29
    {{REG_FN_SLP_DEC,    {0x29,   0x00,   0x00}}}, //0x29
    {{REG_K_THERM,       {0x00,   0x00,   0x00}}}, //0x0
    {{REG_ADC_OUT,       {0x00,   0x00,   0x00}}}, //dummy
    {{REG_OCD_TH,        {0x08,   0x00,   0x00}}}, //0x8
    {{REG_STALL_TH,      {0x40,   0x00,   0x00}}}, //0x40
    {{REG_FS_SPD,        {0x00,   0x27,   0x00}}}, //0x027
    {{REG_STEP_MODE,     {0x07,   0x00,   0x00}}}, //0x07
    {{REG_ALARM_EN,      {0xFF,   0x00,   0x00}}}, //0xFF
    {{REG_CONFIG,        {0x2E,   0x88,   0x00}}}, //0x2E88
    {{REG_STATUS,        {0x00,   0x00,   0x00}}}//, //dummy
//    {{REG_RESERVED_h1A,  {0x00,   0x00,   0x00}}}, //dummy
//    {{REG_RESERVED_h1B,  {0x00,   0x00,   0x00}}}, //dummy
    },
    {
    
    // L6470_PARAM_addr,     setting[2],setting[1],setting[0]} //reset_val
    //Left justified, MSB first
    {{REG_ABS_POS,       {0x00,   0x00,   0x00}}}, //0x000000
    {{REG_EL_POS,        {0x00,   0x00,   0x00}}}, //0x000
    {{REG_MARK,          {0x00,   0x00,   0x00}}}, //0x000000
    {{REG_SPEED,         {0x00,   0x00,   0x00}}}, //dummy
    {{REG_ACC,           {0x00,   0x8A,   0x00}}}, //0x08A
    {{REG_DEC,           {0x00,   0x8A,   0x00}}}, //0x08A
    {{REG_MAX_SPEED,     {0x00,   0x41,   0x00}}}, //0x041
    {{REG_MIN_SPEED,     {0x00,   0x00,   0x00}}}, //0x000
    {{REG_KVAL_HOLD,     {0x29,   0x00,   0x00}}}, //0x29
    {{REG_KVAL_RUN,      {0x29,   0x00,   0x00}}}, //0x29
    {{REG_KVAL_ACC,      {0x29,   0x00,   0x00}}}, //0x29
    {{REG_KVAL_DEC,      {0x29,   0x00,   0x00}}}, //0x29
    {{REG_INT_SPEED,     {0x04,   0x08,   0x00}}}, //0x0408
    {{REG_ST_SLP,        {0x19,   0x00,   0x00}}}, //0x19 
    {{REG_FN_SLP_ACC,    {0x29,   0x00,   0x00}}}, //0x29
    {{REG_FN_SLP_DEC,    {0x29,   0x00,   0x00}}}, //0x29
    {{REG_K_THERM,       {0x00,   0x00,   0x00}}}, //0x0
    {{REG_ADC_OUT,       {0x00,   0x00,   0x00}}}, //dummy
    {{REG_OCD_TH,        {0x08,   0x00,   0x00}}}, //0x8
    {{REG_STALL_TH,      {0x40,   0x00,   0x00}}}, //0x40
    {{REG_FS_SPD,        {0x00,   0x27,   0x00}}}, //0x027
    {{REG_STEP_MODE,     {0x07,   0x00,   0x00}}}, //0x07
    {{REG_ALARM_EN,      {0xFF,   0x00,   0x00}}}, //0xFF
    {{REG_CONFIG,        {0x2E,   0x88,   0x00}}}, //0x2E88
    {{REG_STATUS,        {0x00,   0x00,   0x00}}}//, //dummy
//    {{REG_RESERVED_h1A,  {0x00,   0x00,   0x00}}}, //dummy
//    {{REG_RESERVED_h1B,  {0x00,   0x00,   0x00}}}, //dummy
    }
};

extern L6470_packet *L6470_setting[L6470_DEV_NUM];
#if defined (L6470_PRINT_MESSAGE)
#define L6470_PRINT_HEADER "[L6470 DEBUG]: "
#define L6470_PRINT_CAUTION "Caution"
#endif

#endif