/* =============================================                    */
/* Copyright (c) 2021 poemoep                                       */
/* This software is released under the MIT License, see LICENSE.    */
/* =============================================                    */

#include <stdio.h>
#include <math.h>
#include "L6470.h"
#include "generator.h"

static int output_header(void);
static int output_footer(void);
static int print_pkt(union L6470_packet pkt);
static union L6470_packet generate_pkt(int enum_param,int32_t val);
static union L6470_packet generate_pkt_with_percentage(int enum_param, int32_t percentage);

int main (int argc, char** argv)
{
    output_header();

    for(int i = 0; i < L6470_DEV_NUM; i++){

        output_packetStart();

        print_pkt(gen_ABS_POS(USER_ABS_POS));
        print_pkt(gen_EL_POS(USER_EL_POS));
        print_pkt(gen_MARK(USER_MARK));
        printf("    {{0x04,         {0x00,   0x00,   0x00}}}, //dummy\n");
        print_pkt(gen_ACC(USER_ACC));
        print_pkt(gen_DEC(USER_DEC));
        print_pkt(gen_MAX_SPEED(USER_MAX_SPEED));
        print_pkt(gen_MIN_SPEED(USER_MIN_SPEED));
        print_pkt(gen_KVAL_HOLD(USER_KVAL_HOLD));
        print_pkt(gen_KVAL_RUN(USER_KVAL_RUN));
        print_pkt(gen_KVAL_ACC(USER_KVAL_ACC));
        print_pkt(gen_KVAL_DEC(USER_KVAL_DEC));
        print_pkt(gen_INT_SPEED(USER_INT_SPEED));
        print_pkt(gen_ST_SLP(USER_ST_SLP));
        print_pkt(gen_FN_SLP_ACC(USER_FN_SLP_ACC));
        print_pkt(gen_FN_SLP_DEC(USER_FN_SLP_DEC));
        print_pkt(gen_K_THERM(USER_K_THERM));
        printf("    {{0x12,       {0x00,   0x00,   0x00}}}, //dummy\n");
        print_pkt(gen_OCD_TH(USER_OCD_TH));
        print_pkt(gen_STALL_TH(USER_STALL_TH));
        print_pkt(gen_FS_SPD(USER_FS_SPD));
        print_pkt(gen_STEP_MODE(USER_STEP_MODE));
        print_pkt(gen_ALARM_EN(USER_ALARM_EN));
        print_pkt(gen_CONFIG(USER_CONFIG));

        output_packetEnd();
        if((L6470_DEV_NUM - 1) == i)
        {
            printf(";");
        }else
        {
            printf(",");
        }
        
    }

    output_footer();

    return 0;

}

static void print_pkt(union L6470_packet pkt)
{
    printf("    {{\t0x%02x,\t{0x%02x,\t0x%02x,\t0x%02x}}},\n",pkt.value8b[0], pkt.value8b[1], pkt.value8b[2], pkt.value8b[3]);
}

static void output_header()
{
    printf("#include \"L6470.h\"\n");
    printf("#ifndef __L6470_USER_HEADER__\n");
    printf("#define __L6470_USER_HEADER__\n");
    printf("\n");
    printf("//#define L6470_DAISY_CHAIN\n");
    printf("#define L6470_SPI_CH 0  // 0 or 1\n");
    printf("#define SPI_SPEED 1000000 // [Hz]\n");
    printf("\n");
    // printf("#if defined (L6470_DAISY_CHAIN)\n"); 
    printf("#define L6470_DEV_NUM (%d)\n",L6470_DEV_NUM);
    // printf("#endif\n");
    printf("\n");
    printf("const union L6470_packet L6470_user_setting[L6470_DEV_NUM][PARAM_NUM] =\n");
}

static void output_packetStart()
{
    printf("{// L6470_PARAM_addr,     setting[2],setting[1],setting[0]} //reset_val\n");
    printf("//Left justified, MSB first\n");
    printf("\n");
}

static void output_packetEnd()
{
    // printf("    //Dummy READONLY param &  RESERVED param\n");
    printf("    {{0x19,        {0x00,   0x00,   0x00}}}//, //dummy\n");
    printf("//    {{L6470_param[enum_L6470_RESERVED_h1A].addr,  {0x00,   0x00,   0x00}}}, //dummy\n");
    printf("//    {{L6470_param[enum_L6470_RESERVED_h1B].addr,  {0x00,   0x00,   0x00}}}, //dummy\n");
    printf("\n");
    printf("}");
}

static void output_footer()
{
    printf(";\n\n");
    printf("#endif\n");

}


/* (-2^21 <= abs_pos <= +2^21 -1) with the selected step mode */
union L6470_packet gen_ABS_POS(int32_t abs_pos)
{
#if defined (L6470_PRINT_MESSAGE)
    if( (abs_pos > (int32_t)(pow(2,21)-1)) | (abs_pos < (int32_t)((-1)*pow(2,21))) ) 
        printf("// %s %s abs_pos is over/under flow\n",L6470_PRINT_HEADER,L6470_PRINT_CAUTION);
#endif
    int32_t val = abs_pos & (int32_t)(pow(2,21)-1);
    union L6470_packet pkt = generate_pkt(enum_L6470_ABS_POS, val);
    return pkt;
}

/* step = 0 to 3, el_step = 0 to 127(masked with selected step mode in IC) */
union L6470_packet gen_EL_POS(int32_t step_u_step)
{
    int32_t step = (step_u_step & STEP_ePOS_MASK);
    int32_t u_step = (step_u_step & STEP_u_STEP_MASK);
#if 0
#if defined (L6470_PRINT_MESSAGE)
    printf("// %s %s step is selected STEP_ePOS_%ddeg.\n",
            L6470_PRINT_HEADER, L6470_PRINT_CAUTION, 90 * (step >> STEP_ePOS_ofset));
    printf("// %s %s el_step is selected %d\n",L6470_PRINT_HEADER, L6470_PRINT_CAUTION, u_step));
#endif
#endif

    uint32_t val;
    val = (step + u_step);
    union L6470_packet pkt = generate_pkt(enum_L6470_EL_POS, val);
    return pkt;
}

/* (-2^21 <= mark <= +2^21 -1) with the selected step mode */
union L6470_packet gen_MARK(int32_t mark)
{
#if defined (L6470_PRINT_MESSAGE)
    if( (mark > (int32_t)(pow(2,21)-1)) | (mark < (int32_t)((-1)*pow(2,21))) )
        printf("// %s %s MARK is over/under flow\n",L6470_PRINT_HEADER,L6470_PRINT_CAUTION);
#endif
    int32_t val = ( mark & (int32_t)(pow(2,21) - 1) );
    union L6470_packet pkt = generate_pkt(enum_L6470_MARK, mark);
    return pkt;
}

/* step_per_ss = N x ACC_RESOLUTION [x0.01 step/s^2]*/
union L6470_packet gen_ACC(int32_t step_per_ss)
{
    int32_t val = (int32_t)round((double)step_per_ss / ACC_RESOLUTION) ;
#if defined (L6470_PRINT_MESSAGE)
    if(val == 0){
        printf("// %s %s ACC step_per_ss is more than equal %d [x0.01 step/s^2]\n",
                        L6470_PRINT_HEADER, L6470_PRINT_CAUTION, ACC_RESOLUTION);
        printf("// %s %s set minumum value.\n",
                        L6470_PRINT_HEADER, L6470_PRINT_CAUTION);
        val = 1;
    } else if((int32_t)round((double)val * ACC_RESOLUTION) != step_per_ss){
        printf("// %s %s ACC is rounded to %d [x0.01 step/s^2]\n",
                        L6470_PRINT_HEADER, L6470_PRINT_CAUTION, (int32_t)round((double)val * ACC_RESOLUTION));
    } 
#endif

    union L6470_packet pkt = generate_pkt(enum_L6470_ACC, val);
    return pkt;
}

/* step_per_ss = N x DEC_RESOLUTION [0.01 step/s^2] */
union L6470_packet gen_DEC(int32_t step_per_ss)
{
    int32_t val = (int32_t)round((double)step_per_ss / DEC_RESOLUTION);
#if defined (L6470_PRINT_MESSAGE)
    if(val == 0){
        printf("// %s %s DEC step_per_ss is more than equal %d [x0.01 step/s^2]\n",
                        L6470_PRINT_HEADER, L6470_PRINT_CAUTION, DEC_RESOLUTION);
        printf("// %s %s set minumum value.\n",L6470_PRINT_HEADER, L6470_PRINT_CAUTION);
        val = 1;
    } else if((int32_t)round((double)val * DEC_RESOLUTION) != step_per_ss){
        printf("// %s %s DEC is rounded to %d [x0.01 step/s^2]\n",
                        L6470_PRINT_HEADER, L6470_PRINT_CAUTION, (int32_t)round((double)val * DEC_RESOLUTION));
    } 
#endif

    union L6470_packet pkt = generate_pkt(enum_L6470_DEC, val);
    return pkt;
}

/* step_per_s = N x MAX_SPEED_RESOLUTION [x0.01 step/s^2]*/
union L6470_packet gen_MAX_SPEED(int32_t step_per_s)
{
    int32_t val = (int32_t)round(((double)step_per_s - MAX_SPEED_MIN) / MAX_SPEED_RESOLUTION);
#if defined (L6470_PRINT_MESSAGE)
    if(val == 0){
        printf("// %s %s MAX_SPEED is more than equal %d\n",
                        L6470_PRINT_HEADER, L6470_PRINT_CAUTION, MAX_SPEED_MIN);
        printf("// %s %s set minumum value.\n",L6470_PRINT_HEADER, L6470_PRINT_CAUTION);
        val = 0;
    } else if((int32_t)round((double)val * MAX_SPEED_RESOLUTION) != step_per_s){
        printf("// %s %s MAX_SPEED is rounded to %d [x0.01 step/s]\n",
                        L6470_PRINT_HEADER, L6470_PRINT_CAUTION, (int32_t)round((double)val * MAX_SPEED_RESOLUTION));
    } 
#endif

    union L6470_packet pkt = generate_pkt(enum_L6470_MAX_SPEED, val);
    return pkt;
}

/* step_per_s = M x MIN_SPEED_RESOLUTION, enable_LSPD = ENABLE_LSPD or DISABLE */
union L6470_packet gen_MIN_SPEED(int32_t enable_LSPD_step_per_s)
{
    union L6470_packet pkt = {0};
    if( 0 != (enable_LSPD_step_per_s & (0b01111110 << (ENABLE_LSPD_MAGIC)) ) ) {
#if defined (L6470_PRINT_MESSAGE)
        printf("// %s %s MIN_SPEED is over\n",L6470_PRINT_HEADER,L6470_PRINT_CAUTION);
#endif
    return pkt;
    } /*check 0b1111 1110 000~ */

    int32_t step_per_s = (enable_LSPD_step_per_s & MIN_SPEED_MASK); /* 20 bits */
    int32_t enable_LSPD = (enable_LSPD_step_per_s & ENABLE_LSPD); /* 25 bit */

    int32_t val = (int32_t)round((double)step_per_s / MIN_SPEED_RESOLUTION);

#if defined (L6470_PRINT_MESSAGE)
    if((int32_t)round((double)val * MIN_SPEED_RESOLUTION) != step_per_s){
        printf("// %s %s MIN_SPEED is rounded to %d [x0.001 step/s]\n",
                        L6470_PRINT_HEADER, L6470_PRINT_CAUTION, (int32_t)round((double)val * MIN_SPEED_RESOLUTION));
    } 
#endif
    if(DISABLE != enable_LSPD) val |= ENABLE_LSPD_BIT;

    pkt = generate_pkt(enum_L6470_MIN_SPEED, val);
    return pkt;
}

/* percentage = 0 to 10000 [0.01%]*/
union L6470_packet gen_KVAL_HOLD(int32_t percentage)
{
    union L6470_packet pkt = generate_pkt_with_percentage(enum_L6470_KVAL_HOLD, percentage);
    return pkt;
}

/* percentage = 0 to 10000 [0.01%] */
union L6470_packet gen_KVAL_RUN(int32_t percentage)
{
    union L6470_packet pkt = generate_pkt_with_percentage(enum_L6470_KVAL_RUN, percentage);
    return pkt;
}

/* percentage = 0 to 10000 [0.01%] */
union L6470_packet gen_KVAL_ACC(int32_t percentage)
{
    union L6470_packet pkt = generate_pkt_with_percentage(enum_L6470_KVAL_ACC, percentage);
    return pkt;
}

/* percentage = 0 to 10000 [0.01%] */
union L6470_packet gen_KVAL_DEC(int32_t percentage)
{
    union L6470_packet pkt = generate_pkt_with_percentage(enum_L6470_KVAL_DEC, percentage);
    return pkt;
}

/* step_per_s = N x INT_SPEED_RESOLUTION [x0.0001 step/s] */
union L6470_packet gen_INT_SPEED(int32_t step_per_s)
{
    uint32_t val = (int32_t)round((double)step_per_s / INT_SPEED_RESOLUTION);
#if defined (L6470_PRINT_MESSAGE)
    if((int32_t)round((double)val * INT_SPEED_RESOLUTION) != step_per_s){
        printf("// %s %s INT_SPEED is rounded to %d [x0.0001 step/s] \n",
                        L6470_PRINT_HEADER, L6470_PRINT_CAUTION, (int32_t)round((double)val * INT_SPEED_RESOLUTION));
    } 
#endif

    union L6470_packet pkt = generate_pkt(enum_L6470_INT_SPEED, val);
    return pkt;
}

/* slp = N x ST_SLP_RESOLUTION [x0.000001]*/
union L6470_packet gen_ST_SLP(int32_t slp)
{
    uint32_t val = (int32_t)round((double)slp / ST_SLP_RESOLUTION);
#if defined (L6470_PRINT_MESSAGE)
    if((int32_t)round((double)val * ST_SLP_RESOLUTION) != slp){
        printf("// %s %s ST_SLP is rounded to %d [x0.000001] \n",
                        L6470_PRINT_HEADER, L6470_PRINT_CAUTION, (int32_t)round((double)val * ST_SLP_RESOLUTION));
    } 
#endif
    union L6470_packet pkt = generate_pkt(enum_L6470_ST_SLP, val);
    return pkt;
}

/* slp_acc = N x FN_SLP_ACC_RESOLUTION*/
union L6470_packet gen_FN_SLP_ACC(int32_t slp_acc)
{
    uint32_t val = (int32_t)round((double)slp_acc / FN_SLP_ACC_RESOLUTION);
#if defined (L6470_PRINT_MESSAGE)
    if((int32_t)round((double)val * FN_SLP_ACC_RESOLUTION) != slp_acc){
        printf("// %s %s FN_SLP_ACC is rounded to %d [x0.000001]\n",
                        L6470_PRINT_HEADER, L6470_PRINT_CAUTION, (int32_t)round((double)val * FN_SLP_ACC_RESOLUTION));
    }
#endif
    union L6470_packet pkt = generate_pkt(enum_L6470_FN_SLP_ACC, val);
    return pkt;
}

/* slp_dec = N x FN_SLP_DEC_RESOLUTION [x0.000001]*/
union L6470_packet gen_FN_SLP_DEC(int32_t slp_dec)
{
    uint32_t val = (int32_t)round((double)slp_dec / FN_SLP_DEC_RESOLUTION);
#if defined (L6470_PRINT_MESSAGE)
    if((int32_t)round((double)val * FN_SLP_DEC_RESOLUTION) != slp_dec){
        printf("// %s %s FN_SLP_DEC is rounded to %d [x0.000001]\n",
                        L6470_PRINT_HEADER, L6470_PRINT_CAUTION, (int32_t)round((double)val * FN_SLP_DEC_RESOLUTION));
    } 
#endif
    union L6470_packet pkt = generate_pkt(enum_L6470_FN_SLP_DEC, val);
    return pkt;
}

/* k_therm = N x K_THERM_RESOLUTION */
union L6470_packet gen_K_THERM(int32_t k_therm)
{
    int32_t val = (int32_t)round((double)(k_therm - K_THERM_MIN) / K_THERM_RESOLUTION);
#if defined (L6470_PRINT_MESSAGE)
    if(((int32_t)round((double)val * K_THERM_RESOLUTION) + K_THERM_MIN) != k_therm){
        printf("// %s %s K_THERM is rounded to %d [x0.00001]\n",
                        L6470_PRINT_HEADER, L6470_PRINT_CAUTION, (int32_t)round((double)val * K_THERM_RESOLUTION) + K_THERM_MIN);
    } 
#endif
    union L6470_packet pkt = generate_pkt(enum_L6470_K_THERM, val);
    return pkt;
}

/* ocd_th = 375 to 6000 mA */
union L6470_packet gen_OCD_TH(int32_t ocd_th)
{
    uint32_t val = (int32_t)round((double)(ocd_th - OCD_TH_RESOLUTION) / OCD_TH_RESOLUTION);
#if defined (L6470_PRINT_MESSAGE)
    if(((int32_t)round((double)val * OCD_TH_RESOLUTION) + OCD_TH_RESOLUTION) != ocd_th){
        printf("// %s %s OCD_TH is rounded to %d [mA]\n",
                        L6470_PRINT_HEADER, L6470_PRINT_CAUTION, ((int32_t)round((double)val * OCD_TH_RESOLUTION) + OCD_TH_RESOLUTION));
    } 
#endif
    union L6470_packet pkt = generate_pkt(enum_L6470_OCD_TH, val);
    return pkt;
}

/* stall_th = 31.25 to 4000 mA */
union L6470_packet gen_STALL_TH(int32_t stall_th)
{
    int32_t val = (int32_t)round((double)(stall_th - STALL_TH_RESOLUTION) / STALL_TH_RESOLUTION);
#if defined (L6470_PRINT_MESSAGE)
    if(((int32_t)round((double)val * STALL_TH_RESOLUTION) + STALL_TH_RESOLUTION) != stall_th){
        printf("// %s %s STALL_TH is rounded to %d [x0.01 mA]\n",
                        L6470_PRINT_HEADER, L6470_PRINT_CAUTION, ((int32_t)round((double)val * STALL_TH_RESOLUTION) + STALL_TH_RESOLUTION));
    } 
#endif    
    union L6470_packet pkt = generate_pkt(enum_L6470_STALL_TH, val);
    return pkt;
}
/* fs_spd = 7.63(FS_SPD_MIN) to 15625 [step/s] */
union L6470_packet gen_FS_SPD(int32_t fs_spd)
{
    uint32_t val = (int32_t)round((double)(fs_spd - FS_SPD_MIN) / FS_SPD_RESOLUTION);
#if defined (L6470_PRINT_MESSAGE)
    if(((int32_t)round((double)val * FS_SPD_RESOLUTION) + FS_SPD_MIN) != fs_spd){
        printf("// %s %s FS_SPD is rounded to %d [x0.01 step/s]\n",
        L6470_PRINT_HEADER, L6470_PRINT_CAUTION, (int32_t)round((double)val * FS_SPD_RESOLUTION) + FS_SPD_MIN);
    } 
#endif   

    union L6470_packet pkt = generate_pkt(enum_L6470_FS_SPD, val);
    return pkt;
}

/* sync_en = SYNC_EN or DISABLE, sync_sel = SYNC_FFS_*, step_sel = STEP_* */
union L6470_packet gen_STEP_MODE(int32_t mode)
{
#if defined (L6470_PRINT_MESSAGE)
    uint8_t sync_en = (mode & SYNC_EN);
    uint8_t sync_sel = (mode & SYNC_SEL_MASK);
    uint8_t step_sel = (mode & STEP_SEL_MASK);
    if( (sync_sel >> SYNC_SEL_ofset) > step_sel)
        printf("// %s %s sync_sel is less than equal step_sel.\n",L6470_PRINT_HEADER, L6470_PRINT_CAUTION);    
#endif
    int32_t val = mode;
    union L6470_packet pkt = generate_pkt(enum_L6470_STEP_MODE, val);
    return pkt;
}

/* alm = ALM_* | ALM_* ... */
union L6470_packet gen_ALARM_EN(int32_t alm)
{
    int32_t val = alm;
    union L6470_packet pkt = generate_pkt(enum_L6470_ALARM_EN, val);
    return pkt;
}

/* f_pwm_int, f_pwm_dec, pow_sr, oc_sd, en_vscomp, sw_mode, ext_clk, osc_sel */
union L6470_packet gen_CONFIG(int32_t param)
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
    union L6470_packet pkt = generate_pkt(enum_L6470_CONFIG, val);
    return pkt;
}

static union L6470_packet generate_pkt(int enum_param,int32_t val)
{
    union L6470_packet pkt = {0};
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
static union L6470_packet generate_pkt_with_percentage(int enum_param, int32_t percentage)
{
    union L6470_packet pkt = {0};
    if( (percentage < 0) | (10000 < percentage) )
    {
#if defined (L6470_PRINT_MESSAGE)
        printf("// %s %s percentage = 0 to 10000 [%%]\n",L6470_PRINT_HEADER,L6470_PRINT_CAUTION);
#endif
        return pkt; /* 0x00 pkt */  
    } /*0 to 10000*/

    int32_t val = (int32_t)round((255 * (double)percentage) / 10000);
#if defined (L6470_PRINT_MESSAGE)
    if((int32_t)round(((double)val * 10000) / 255) != percentage)
        printf("// %s %s percentage is round to %d[0.01%%]\n",L6470_PRINT_HEADER,L6470_PRINT_CAUTION,(int32_t)round((val * 10000)/ 255));
#endif
    pkt = generate_pkt(enum_param,val);
    return pkt;
}
