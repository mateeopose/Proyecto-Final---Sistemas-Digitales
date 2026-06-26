/* USER CODE BEGIN PV */
volatile uint8_t flag = 0;
volatile uint8_t MV_RUN = 0;
volatile uint8_t MD_RUN = 0;
volatile uint8_t ST_RUN = 0;
volatile uint16_t sound_cnt = 0;
/* USER CODE END PV */
//*******************************
/* USER CODE BEGIN PFP */
void MELODIA_DERROTA(void);
void MELODIA_VICTORIA(void);
void SONIDO_TECLA(void);
/* USER CODE END PFP */
//*******************************
/* USER CODE BEGIN 4 */
void SONIDO_TECLA(void){
	if (flag==0){
		ST_RUN=1;
		TIM4->ARR=23888; //TONO DO 523.25 HZ
		uint16_t ciclomaximo = TIM4->ARR; // Valor máximo de ciclo de trabajo
		// Configuramos el PWM para CICLO DE 50%
		TIM4->CCR2 = ciclomaximo/2;//Ciclo de trabajo del 50%
		HAL_TIM_Base_Start_IT(&htim14);//Se inicia el temporizador de interrupciones
		HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2); // Inicio de la modulación PWM
		flag=1;//Bandera que indica que comenzo el ciclo de la melodia
		}
	if (sound_cnt==20){
				HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2); // Inicio de la modulación PWM
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
	TIM4->ARR=23888; //TONO DO 523.25 HZ
	uint16_t ciclomaximo = TIM4->ARR; // Valor máximo de ciclo de trabajo
	// Configuramos el PWM para CICLO DE 50%
	TIM2->CCR2 = ciclomaximo/2;//Ciclo de trabajo del 50%
	HAL_TIM_Base_Start_IT(&htim14);//Se inicia el temporizador de interrupciones
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2); // Inicio de la modulación PWM
	flag=1;//Bandera que indica que comenzo el ciclo de la melodia
	}
	if (sound_cnt==30)
		HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
	if (sound_cnt==40)
			HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
	if (sound_cnt==70)
				HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
	if (sound_cnt==80)
				HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
	if (sound_cnt==95)
				HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
	if (sound_cnt==105)
				HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
	if (sound_cnt==135){
			HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
	        TIM4->ARR=20088;//Nota RE#
	}
	if (sound_cnt==145){
			HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2); // Inicio de la modulación PWM
		}
		if (sound_cnt==160){
				HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2); // Inicio de la modulación PWM
		        TIM4->ARR=21282;//Nota RE
		}
		if (sound_cnt==170){
					HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2); // Inicio de la modulación PWM
				}
		if (sound_cnt==185)
					HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2); // Inicio de la modulación PWM
		if (sound_cnt==195)
					HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2); // Inicio de la modulación PWM
		if (sound_cnt==225){
					HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2); // Inicio de la modulación PWM
					TIM4->ARR=23888; //TONO DO 523.25 HZ

		}
		if (sound_cnt==235){
				HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2); // Inicio de la modulación PWM
		}
		if (sound_cnt==250)
							HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2); // Inicio de la modulación PWM
		if (sound_cnt==260)
							HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2); // Inicio de la modulación PWM
		if (sound_cnt==290){
		HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2); // Inicio de la modulación PWM
		TIM4->ARR=25309;//Nota SI
		}

		if (sound_cnt==300)
							HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2); // Inicio de la modulación PWM
		if (sound_cnt==330){
			HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2); // Inicio de la modulación PWM
								TIM4->ARR=23888; //TONO DO 523.25 HZ
		}
		if (sound_cnt==340)
									HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2); // Inicio de la modulación PWM

		if (sound_cnt==370){
					HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2); // Inicio de la modulación PWM
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
		TIM4->ARR=15943; //TONO SOL
		uint16_t ciclomaximo_2 = TIM4->ARR; // Valor máximo de ciclo de trabajo
		// Configuramos el PWM para CICLO DE 50%
		TIM2->CCR2 = ciclomaximo_2/2;//Ciclo de trabajo del 50%
		HAL_TIM_Base_Start_IT(&htim14);//Se inicia el temporizador de interrupciones
		HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2); // Inicio de la modulación PWM
		flag=1;//Bandera que indica que comenzo el ciclo de la melodia
		}
		if (sound_cnt==20)
			HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
		if (sound_cnt==25)
				HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
		if (sound_cnt==45){
					HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
		}
		if (sound_cnt==50)
					HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
		if (sound_cnt==70){
					HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
		TIM4->ARR=20088; //TONO RE_#
}
		if (sound_cnt==75)
					HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
		if (sound_cnt==115){
				HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
		        TIM4->ARR=17896;//Nota FA
		}
		if (sound_cnt==120){
				HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2); // Inicio de la modulación PWM
			}
			if (sound_cnt==140){
					HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2); // Inicio de la modulación PWM

			}
			if (sound_cnt==145){
						HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2); // Inicio de la modulación PWM
					}
			if (sound_cnt==165){
						HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2); // Inicio de la modulación PWM
			}
			if (sound_cnt==170)
									HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2); // Inicio de la modulación PWM
						if (sound_cnt==190){
									HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2); // Inicio de la modulación PWM
									TIM4->ARR=21282;//Nota RE
						}
			if (sound_cnt==195)
						HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2); // Inicio de la modulación PWM
			if (sound_cnt==235){
						HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2); // Inicio de la modulación PWM
						HAL_TIM_Base_Stop_IT(&htim14);
						__HAL_TIM_SET_COUNTER(&htim4, 0);
						__HAL_TIM_SET_COUNTER(&htim14, 0);
												sound_cnt=0;
												flag=0;
												MV_RUN=0;
			}
	}
// Rutina de atención a la interrupción del temporizador 14
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM14) {
       sound_cnt++;
       if (MV_RUN==1)
       MELODIA_VICTORIA();
       if (MD_RUN==1)
       MELODIA_DERROTA();
       if (ST_RUN==1)
       SONIDO_TECLA();
	}
}

/* USER CODE END 4 */
