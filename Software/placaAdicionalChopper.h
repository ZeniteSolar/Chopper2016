/*****************************************************
*
* Desenvolvedor: Marcio Antonio Augusto
* E-mail: marcioantonioaugusto@hotmail.com
*
* Equipe Zênite Solar 2016
*
******************************************************/

// System definitions
#define F_CPU       12000000UL //frequencia do cristal
#define PWM_freq	1000//frequncia do sinal de saida, Hz
#define DvDtMax		4 //Variacao maxima da tensao de saida, V/s

//Include header files
#include "bibliotecas/globalDefines.h"
#include "bibliotecas/ATmega328.h"

#define PWM_DDR			DDRB
#define PWM_PORT		PORTB
#define PWM_BIT			PB1

#define POT_CHANNEL		ADC_CHANNEL_1
#define POT_BIT			PC1

// Bit field flags
struct system_flags
{
	uint16 newAdcValue		: 1;
	uint16 					: 0;//completa a variavel
};
typedef struct system_flags flags_t;

//global variables
//flags_t flags;
uint16 adcAnt;