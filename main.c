/* =============================================                    */
/* Copyright (c) 2021 poemoep                                       */
/* This software is released under the MIT License, see LICENSE.    */
/* =============================================                    */

#include <stdio.h>
#include "L6470.h"

int main(int argc, char** argv)
{

    L6470_init();


    int pos_aim,pos;
    L6470_packet pkt_r = L6470_MoveRun(DIR_FWD,25000000);
    L6470_packet pkt_l = pkt_r;
    char c = 0;
    c = getchar();
    // pos = L6470_GetAbsPos();
    L6470_rw_multi(&pkt_r,4,NULL,&pkt_l,4,NULL);
    sleep(5);
    pos = L6470_GetAbsPos();
    L6470_MoveRun(DIR_FWD, 5120000);
    sleep(5);
    pos = L6470_GetAbsPos();
    
    
    
    L6470_StopSoft();
    sleep(2);
    pos = L6470_GetAbsPos();

    L6470_MoveRun(DIR_RVS, 1600000);
    sleep(5);
    pos = L6470_GetAbsPos();
    L6470_StopSoft();
    sleep(2);
    pos = L6470_GetAbsPos();
    
    
    L6470_MoveStepClock(DIR_FWD);
    sleep(3);
    pos = L6470_GetAbsPos();
    L6470_MoveStepClock(DIR_RVS);
    sleep(3);
    pos = L6470_GetAbsPos();

    for(int i = 0; i<1; i++){
        L6470_MoveStep(DIR_FWD,200);
        sleep(1);
    	pos = L6470_GetAbsPos();
        L6470_MoveStep(DIR_RVS,200);
        sleep(1);
    	pos = L6470_GetAbsPos();
    }
    sleep(3);

    pos_aim=10000;
    pos = pos_aim + 1;
    
    L6470_MoveGoTo(pos_aim);
    while(pos != pos_aim){
   // for(int l = 0 ; l < 20; l++){
	pos = L6470_GetAbsPos();
        sleep(1);
    }

    
   // L6470_MoveStep(DIR_RVS,200);
    sleep(1);
    
    pos_aim=-1;
    pos = pos_aim + 1;
    
    L6470_MoveGoToDir(DIR_RVS, pos_aim);
    while(pos != pos_aim){
    //for(int l = 0 ; l < 20; l++){
	pos = L6470_GetAbsPos();
        sleep(1);
    }

    int mark[2] = {1,2};   
    L6470_MoveGoToUntil(ACT_POS_TO_MARK,DIR_FWD,10000);
    while(mark[0] != mark[1]){
	pos = L6470_GetAbsPos();
	mark[0] = mark[1];
	mark[1] = pos;
        sleep(1);
    }
    pos = 100;
    L6470_GoHome();
    while(pos != 0){
	pos = L6470_GetAbsPos();
        sleep(1);
    }


   L6470_GoMark();
   while(pos != mark[0]){
	pos = L6470_GetAbsPos();
        sleep(1);
    }

    L6470_HiZSoft();
    
    return 0;
}
