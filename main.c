/* =============================================                    */
/* Copyright (c) 2021 poemoep                                       */
/* This software is released under the MIT License, see LICENSE.    */
/* =============================================                    */

#include <stdio.h>
#include "L6470.h"


void MoveRun_Test(uint8_t dir_1, uint32_t speed_1, uint32_t dir_2, uint32_t speed_2);
void StopSoft_Test(void);
void MoveStepClock_Test(uint8_t dir_1, uint8_t dir_2);
void MoveStep_Test(uint8_t dir_1,uint32_t speed_1, uint8_t dir_2, uint32_t speed_2);

L6470_DATA_ARRAY d;
L6470_ABSPOS_T pos;

int main(int argc, char** argv)
{

    L6470_init();

    char c = 0;

    MoveRun_Test(DIR_FWD,200000, DIR_RVS, 200000);
    sleep(5);
    pos = L6470_GetAbsPos();

    MoveRun_Test(DIR_FWD,400000, DIR_RVS, 400000);
    sleep(5);
    pos = L6470_GetAbsPos();
    
    MoveRun_Test(DIR_FWD,800000, DIR_RVS, 800000);
    sleep(5);
    pos = L6470_GetAbsPos();

    MoveRun_Test(DIR_FWD,1600000, DIR_RVS, 1600000);
    sleep(5);
    pos = L6470_GetAbsPos();
    
    StopSoft_Test();
    sleep(5);
    pos = L6470_GetAbsPos();

    MoveRun_Test(DIR_RVS,1600000, DIR_FWD, 1600000);
    sleep(5);
    StopSoft_Test();
    sleep(2);
    pos = L6470_GetAbsPos();

    MoveStepClock_Test(DIR_RVS,DIR_FWD);
    sleep(3);
    pos = L6470_GetAbsPos();

    StopSoft_Test();
    sleep(5);
    pos = L6470_GetAbsPos();

    for(int i = 0; i<3; i++){
        for(int j = 0; j< 5; j++){
            MoveStep_Test(DIR_FWD,200*256,DIR_FWD,200*256);
            sleep(1);
        	pos = L6470_GetAbsPos();
        }

        for(int j = 0; j< 5; j++){
            MoveStep_Test(DIR_RVS,400*256,DIR_RVS,400*256);
            sleep(2);
        	pos = L6470_GetAbsPos();
        }
    }

    int pos_aim[L6470_DEV_NUM];

    for(int itr = 0; itr < L6470_DEV_NUM; itr++){
        pos_aim[itr]=10000;
    }
    
    d.dev[0] = L6470_MoveGoTo(pos_aim[0]);
    d.dev[1] = L6470_MoveGoTo(pos_aim[1]);
    L6470_rw_multi(&d, "MoveGoTo");

    char flag = 0;
    while(flag == 0){
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

    
    sleep(1);

    pos_aim[0] = -1;
    pos_aim[1] = 20000;    
    
    d.dev[0] = L6470_MoveGoToDir(DIR_RVS,pos_aim[0]);
    d.dev[1] = L6470_MoveGoToDir(DIR_FWD,pos_aim[1]);
    L6470_rw_multi(&d, "MoveGoToDir");

    flag = 0;
    while(flag == 0){
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

    d.dev[0] = L6470_MoveGoToUntil(ACT_POS_TO_MARK,DIR_FWD,200000);
    d.dev[1] = L6470_MoveGoToUntil(ACT_POS_TO_MARK,DIR_RVS,400000);
    L6470_rw_multi(&d, "MoveGoToUntil");

    flag = 0;
    while(flag == 0){
    	pos = L6470_GetAbsPos();
        for(int itr = 0; itr <L6470_DEV_NUM; itr++){
    	    mark[itr][0] = mark[itr][1];
	        mark[itr][1] = pos.dev[itr];
            printf("Dev %d, mark[0] = %d, mark[1] = %d\n",itr, mark[0],mark[1]);

            if(mark[itr][0] == mark[itr][1]){
                printf("Dev %d, SW pushed",itr);
                flag++;
            }else{
                printf("Dev %d, SW  no pushed yet",itr);
                flag = 0;
            }
        }
        if(flag != L6470_DEV_NUM) flag = 0;
        sleep(1);
    }

    d.dev[0] = L6470_GoHome();
    d.dev[1] = L6470_GoHome();
    L6470_rw_multi(&d, "MoveGoHome");

    flag = 0;
    while(flag == 0){
    	pos = L6470_GetAbsPos();
        for(int itr = 0; itr <L6470_DEV_NUM; itr++){
    	    if(pos.dev[itr] == 0){
                flag++;
            }else{
                flag = 0;
            }
        }
        if(flag != L6470_DEV_NUM) flag = 0;
        sleep(1);
    }

    d.dev[0] = L6470_GoMark();
    d.dev[1] = L6470_GoMark();
    L6470_rw_multi(&d, "MoveGoMark");

    flag = 0;
    while(flag == 0){
    	pos = L6470_GetAbsPos();
        for(int itr = 0; itr <L6470_DEV_NUM; itr++){
            if(mark[itr][0] == pos.dev[itr]){
                flag++;
            }else{
                flag = 0;
            }
        }
        if(flag != L6470_DEV_NUM) flag = 0;
        sleep(1);
    }

    d.dev[0] = L6470_HiZHard();
    d.dev[1] = L6470_HiZHard();
    L6470_rw_multi(&d, "HiZHard");
    
    return 0;
}

void MoveRun_Test(uint8_t dir_1, uint32_t speed_1, uint32_t dir_2, uint32_t speed_2){
    d.dev[0] = L6470_MoveRun(dir_1, speed_1);
    d.dev[1] = L6470_MoveRun(dir_2, speed_2);
    L6470_rw_multi(&d, "MoveRun_TEST");
}

void StopSoft_Test(void){
    d.dev[0] = L6470_StopSoft();
    d.dev[1] = L6470_StopSoft();
    L6470_rw_multi(&d, "StopSoft_Test");
}

void MoveStepClock_Test(uint8_t dir_1, uint8_t dir_2){
    d.dev[0] = L6470_MoveStepClock(dir_1);
    d.dev[1] = L6470_MoveStepClock(dir_2);
    L6470_rw_multi(&d, "MoveStepClock_Test");
}

void MoveStep_Test(uint8_t dir_1,uint32_t speed_1, uint8_t dir_2, uint32_t speed_2){
    d.dev[0] = L6470_MoveStep(dir_1,speed_1);
    d.dev[1] = L6470_MoveStep(dir_2,speed_2);
    L6470_rw_multi(&d, "MoveStep_Test");
}