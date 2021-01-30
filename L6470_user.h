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

extern const L6470_packet L6470_user_setting[L6470_DEV_NUM][PARAM_NUM];
extern L6470_packet *L6470_setting[L6470_DEV_NUM];

#if defined (L6470_PRINT_MESSAGE)
#define L6470_PRINT_HEADER "[L6470 DEBUG]: "
#define L6470_PRINT_CAUTION "Caution"
#endif

#endif