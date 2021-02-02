/* =============================================                    */
/* Copyright (c) 2021 poemoep                                       */
/* This software is released under the MIT License, see LICENSE.    */
/* =============================================                    */

#include <stdio.h>
#include "L6470.h"

int main(int argc, char** argv)
{

    L6470_init();

    L6470_DATA_ARRAY d;
    L6470_ABSPOS_T pos;
    char c = 0;

    d.dev[0] = L6470_MoveRun(DIR_FWD,256000);
    d.dev[1] = L6470_MoveRun(DIR_FWD,256000);
    c = getchar();
    L6470_rw_multi(&d, "MoveRun_TEST_1");
    c = getchar();
    sleep(5);
    pos = L6470_GetAbsPos();

    d.dev[0] = L6470_MoveRun(DIR_FWD,512000);
    d.dev[1] = L6470_MoveRun(DIR_RVS,512000);
    c = getchar();
    L6470_rw_multi(&d, "MoveRun_TEST_2");
    c = getchar();
    sleep(5);
    pos = L6470_GetAbsPos();
    
    
    
    d.dev[0] = L6470_StopSoft();
    d.dev[1] = L6470_StopSoft();
    L6470_rw_multi(&d, "MoveRun_StopSoft");
    sleep(5);
    pos = L6470_GetAbsPos();

    d.dev[0] = L6470_MoveRun(DIR_RVS,1024000);
    d.dev[1] = L6470_MoveRun(DIR_FWD,1024000);
    c = getchar();
    L6470_rw_multi(&d, "MoveRun_TEST_3");
    c = getchar();
    sleep(5);
    pos = L6470_GetAbsPos();


    d.dev[0] = L6470_StopSoft();
    d.dev[1] = L6470_StopSoft();
    c = getchar();
    L6470_rw_multi(&d, "MoveRun_StopSoft");
    c = getchar();
    sleep(5);
    pos = L6470_GetAbsPos();
    
    
    d.dev[0] = L6470_MoveStepClock(DIR_FWD);
    d.dev[1] = L6470_nop(1);
    c = getchar();
    L6470_rw_multi(&d, "MoveStepClock_nop");
    c = getchar();
    sleep(3);
    pos = L6470_GetAbsPos();

    d.dev[0] = L6470_nop(1);
    d.dev[1] = L6470_MoveStepClock(DIR_RVS);
    c = getchar();
    L6470_rw_multi(&d, "nop_MoveStepClock");
    c = getchar();
    sleep(3);
    pos = L6470_GetAbsPos();

    for(int i = 0; i<1; i++){
        d.dev[0] = L6470_MoveStep(DIR_FWD,2000);
        d.dev[1] = L6470_MoveStep(DIR_RVS,4000);
        c = getchar();
        L6470_rw_multi(&d, "MoveStep");
        c = getchar();
        sleep(1);
    	pos = L6470_GetAbsPos();

        d.dev[0] = L6470_MoveStep(DIR_RVS,4000);
        d.dev[1] = L6470_MoveStep(DIR_FWD,2000);
        c = getchar();
        L6470_rw_multi(&d, "MoveStep");
        c = getchar();
        sleep(1);
    	pos = L6470_GetAbsPos();
    }
    sleep(3);

    int pos_aim[L6470_DEV_NUM];

    for(int itr = 0; itr < L6470_DEV_NUM; itr++){
        pos_aim[itr]=10000;
        pos_now[itr] = pos_aim[itr] + 1;

    }
    
    d.dev[0] = L6470_MoveGoto(pos_aim[0]);
    d.dev[1] = L6470_MoveGoTo(pos_aim[1]);
    c = getchar();
    L6470_rw_multi(&d, "MoveGoTo");

    char flag = 0;
    while(flag == 0){
   // for(int l = 0 ; l < 20; l++){
	    pos = L6470_GetAbsPos();
        for(int itr = 0; itr <L6470_DEV_NUM; itr++){
            if(pos.dev[itr] == pos_aim[itr]){
                flag++; 
            }else{
                flag = 0;
            }
        }
        if(flag != L6470_DEV_NUM) flag = 0;    
        sleep(1);
    }

    
   // L6470_MoveStep(DIR_RVS,200);
    sleep(1);

    pos_aim[0] = -1;
    pos_aim[1] = 20000;    
    
    d.dev[0] = L6470_MoveGoToDir(DIR_RVS,pos_aim[0]);
    d.dev[1] = L6470_MoveGoToDir(DIR_FWD,pos_aim[1]);
    c = getchar();
    L6470_rw_multi(&d, "MoveGoToDir");

    flag = 0;
    while(flag == 0){
   // for(int l = 0 ; l < 20; l++){
	    pos = L6470_GetAbsPos();
        for(int itr = 0; itr <L6470_DEV_NUM; itr++){
            if(pos.dev[itr] == pos_aim[itr]){
                flag++; 
            }else{
                flag = 0;
            }
        }
        if(flag != L6470_DEV_NUM) flag = 0;    
        sleep(1);
    }

    int mark[2][2] = {{1,2},{1,2}}; 

    d.dev[0] = L6470_MoveGoToUntil(ACT_POS_TO_MARK,DIR_FWD,20000);
    d.dev[1] = L6470_MoveGoToUntil(ACT_POS_TO_MARK,DIR_RVS,80000);
    c = getchar();
    L6470_rw_multi(&d, "MoveGoToUntil");

    while(flag == 0){
    	pos = L6470_GetAbsPos();
        for(int itr = 0; itr <L6470_DEV_NUM; itr++){
    	    mark[itr][0] = mark[itr][1];
	        mark[itr][1] = pos.dev[itr];

            if(mark[itr][0] == mark[itr][1]){
                flag++;
            }else{
                flag = 0:
            }
        }
        if(flag != L6470_DEV_NUM) flag = 0;
        sleep(1);
    }

    flag = 0;
    d.dev[0] = L6470_GoHome();
    d.dev[1] = L6470_GoHome();
    c = getchar();
    L6470_rw_multi(&d, "MoveGoHome");
    while(flag == 0){
    	pos = L6470_GetAbsPos();
        for(int itr = 0; itr <L6470_DEV_NUM; itr++){
    	    if(pos.dev[itr] == 0){
                flag++;
            }else{
                flag = 0:
            }
        }
        if(flag != L6470_DEV_NUM) flag = 0;
        sleep(1);
    }

    flag = 0;
    d.dev[0] = L6470_GoMark();
    d.dev[1] = L6470_GoMark();
    c = getchar();
    L6470_rw_multi(&d, "MoveGoMark");

    while(flag == 0){
    	pos = L6470_GetAbsPos();
        for(int itr = 0; itr <L6470_DEV_NUM; itr++){
            if(mark[itr][0] == pos.dev[itr]){
                flag++;
            }else{
                flag = 0:
            }
        }
        if(flag != L6470_DEV_NUM) flag = 0;
        sleep(1);
    }

    d.dev[0] = L6470_HiZHard();
    d.dev[1] = L6470_HiZHard();
    c = getchar();
    L6470_rw_multi(&d, "HiZHard");
    
    return 0;
}
