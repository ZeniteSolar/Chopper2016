/*
 * Chopper_MPDC_2016.c
 *
 * Created: 18/8/2016 13:25:59
 *  Author: Augusto D. Rodrigues
 *	Equipe Zênite Solar - Projeto Chopper MPDC 2016
 */ 

//Include header files
#include "bibliotecas/globalDefines.h"
#include "bibliotecas/ATmega328.h"

#define F_CPU 16000000UL
#define POT_CHANNEL ADC_CHANNEL_1		
#define POT_BIT PC1
#define PWM_DDR DDRB
#define PWM_PORT PORTB
#define PWM_BIT PB1


uint16 freq = 300;
uint8 current = 80;
uint8 maxCurrent = 100;
uint8 minDC = 10;
uint8 dc = 0;
uint8 dcReq = 0;				//armazena o valor do dc requisitado
uint8 maxDC = 90;
uint8 maxDV = 10;
uint8 on = 1;					//false
uint8 DMS = 1;					//false
uint8 temperature = 50;
uint8 maxTemp = 70;
uint8 voltage = 36;
uint8 minVotage = 30;

void seta_dc(uint8 d_cycle)		//função para definição do Duty Cicle do PWM
{
	dcReq = d_cycle;
	dc = dcReq;
	if(dc < minDC)				// Comparação com o valor mínimo de Duty Cicle
		dc = 0;
	else
		if(dc > maxDC)			//Comparação com o valor máximo de Duty Cicle
			dc = 100;
	timer1SetCompareBValue((dc * (timer1GetCompareAValue()))/100);//set o valor do comparador B para gerar o DC requerido
}

int main(void)
{
	clrBit(DDRC,POT_BIT);	//SETA O PINO DO ADC COMO ENTRADA
	adcConfig(ADC_MODE_SINGLE_CONVERSION, ADC_REFRENCE_POWER_SUPPLY , ADC_PRESCALER_128);
	adcSelectChannel(POT_CHANNEL);
	adcClearInterruptRequest();
	adcActivateInterrupt();
	adcEnable();
	adcStartConversion();
	
	timer1Config(TIMER_B_MODE_CTC, TIMER_A_PRESCALER_1024);
	timer1ClearCompareBInterruptRequest();									
	timer1ClearCompareAInterruptRequest();
	timer1ActivateCompareBInterrupt();									//ativa a interrupcao do compA
	timer1ActivateCompareAInterrupt();									//ativa a interrupcao do compB
	timer1SetCompareAValue((F_CPU/1024)/freq);							//valor do comparador A,  define a frequencia
	timer1SetCompareBValue((dc * (timer1GetCompareAValue()))/100);		//valor do comparador B,  define Duty Cicle
	
	sei();
	
	setBit(PWM_DDR,PWM_BIT);		//define o pino do pwm como saída
	
    while(1)
    {
       
    }
}

ISR(ADC_vect){
	if(dc != ADC/10)
		seta_dc(ADC/10);				//definição do Duty Cicle do PWM
	adcStartConversion();
}

ISR(TIMER1_COMPA_vect){
	if(dc > 0)
		setBit(PWM_PORT,PWM_BIT);		//Inicia o período em nível alto do PWM
}

ISR(TIMER1_COMPB_vect){
	if(dc < 100)
		clrBit(PWM_PORT,PWM_BIT);		//Inicia o período em nível baixo do PWM
}
