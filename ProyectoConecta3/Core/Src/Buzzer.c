#include "Buzzer.h"

extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim14;

static volatile uint8_t flag = 0;
static volatile uint8_t MV_RUN = 0;
static volatile uint8_t MD_RUN = 0;
static volatile uint8_t ST_RUN = 0;
static volatile uint16_t sound_cnt = 0;

void SONIDO_TECLA(void){
    if (flag==0){
        ST_RUN=1;
        TIM3->ARR=19110; // TONO DO
        uint16_t ciclomaximo = TIM3->ARR;
        TIM3->CCR2 = ciclomaximo/2; // Ciclo 50%

        HAL_TIM_Base_Start_IT(&htim14);
        HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
        flag=1;
    }
    if (sound_cnt==20){ // Suena por 20ms
        HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
        HAL_TIM_Base_Stop_IT(&htim14);
        __HAL_TIM_SET_COUNTER(&htim4, 0);
        __HAL_TIM_SET_COUNTER(&htim14, 0);
        sound_cnt=0;
        flag=0;
        ST_RUN=0;
    }
}

void MELODIA_DERROTA(void){
    if (flag==0){
        MD_RUN=1;
        TIM3->ARR=19110; // DO
        uint16_t ciclomaximo = TIM3->ARR;
        TIM3->CCR2 = ciclomaximo/2;
        HAL_TIM_Base_Start_IT(&htim14);
        HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
        flag=1;
    }
    if (sound_cnt==30) HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
    if (sound_cnt==40) HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    if (sound_cnt==70) HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
    if (sound_cnt==80) HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    if (sound_cnt==95) HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
    if (sound_cnt==105) HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    if (sound_cnt==135){
        HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
        TIM3->ARR=16070; // RE#
        TIM3->CCR2 = TIM3->ARR/2;
    }
    if (sound_cnt==145) HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    if (sound_cnt==160){
        HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
        TIM3->ARR=17025; // RE
        TIM3->CCR2 = TIM3->ARR/2;
    }
    if (sound_cnt==170) HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    if (sound_cnt==185) HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
    if (sound_cnt==195) HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    if (sound_cnt==225){
        HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
        TIM3->ARR=19110; // DO
        TIM3->CCR2 = TIM3->ARR/2;
    }
    if (sound_cnt==235) HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    if (sound_cnt==250) HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
    if (sound_cnt==260) HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    if (sound_cnt==290){
        HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
        TIM3->ARR=20247; // SI
        TIM3->CCR2 = TIM3->ARR/2;
    }
    if (sound_cnt==300) HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    if (sound_cnt==330){
        HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
        TIM3->ARR=19110; // DO
        TIM3->CCR2 = TIM3->ARR/2;
    }
    if (sound_cnt==340) HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    if (sound_cnt==370){
        HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
        HAL_TIM_Base_Stop_IT(&htim14);
        __HAL_TIM_SET_COUNTER(&htim4, 0);
        __HAL_TIM_SET_COUNTER(&htim14, 0);
        sound_cnt=0;
        flag=0;
        MD_RUN=0;
    }
}

void MELODIA_VICTORIA(void){
    if (flag==0){
        MV_RUN=1;
        TIM3->ARR=12754; // SOL
        uint16_t ciclomaximo = TIM3->ARR;
        TIM3->CCR2 = ciclomaximo/2;
        HAL_TIM_Base_Start_IT(&htim14);
        HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
        flag=1;
    }
    if (sound_cnt==20) HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
    if (sound_cnt==25) HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    if (sound_cnt==45) HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
    if (sound_cnt==50) HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    if (sound_cnt==70){
        HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
        TIM3->ARR=16070; // RE#
        TIM3->CCR2 = TIM3->ARR/2;
    }
    if (sound_cnt==75) HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    if (sound_cnt==115){
        HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
        TIM3->ARR=14317; // FA
        TIM3->CCR2 = TIM3->ARR/2;
    }
    if (sound_cnt==120) HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    if (sound_cnt==140) HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
    if (sound_cnt==145) HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    if (sound_cnt==165) HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
    if (sound_cnt==170) HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    if (sound_cnt==190){
        HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
        TIM3->ARR=17025; // RE
        TIM3->CCR2 = TIM3->ARR/2;
    }
    if (sound_cnt==195) HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    if (sound_cnt==235){
        HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
        HAL_TIM_Base_Stop_IT(&htim14);
        __HAL_TIM_SET_COUNTER(&htim4, 0);
        __HAL_TIM_SET_COUNTER(&htim14, 0);
        sound_cnt=0;
        flag=0;
        MV_RUN=0;
    }
}

// Interrupción del temporizador 14
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM14) {
        sound_cnt++;
        if (MV_RUN==1) MELODIA_VICTORIA();
        if (MD_RUN==1) MELODIA_DERROTA();
        if (ST_RUN==1) SONIDO_TECLA();
    }
}
