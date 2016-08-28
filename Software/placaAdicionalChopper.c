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
#include <string.h>

#define BYTES_TO_RECEIVE 	18 // Number of bytes that uC have to receive on communication

#define SETWORDSIZE 		8  // Number of bits of word to set parameter, used in the comunication with PC
#define GETWORDSIZE 		4  // Number of bits of word to get parameter, used in the comunication with PC

#define INICIODOPACOTE		'@'
#define FINALDOPACOTE		'*'

#define F_CPU 			16000000UL

#define CURRENT_CHANNEL ADC_CHANNEL_0
#define POT_CHANNEL 	ADC_CHANNEL_1
#define VOLTAGE_CHANNEL ADC_CHANNEL_2
#define TEMP_CHANNEL	ADC_CHANNEL_3

#define FIRST_CHANNEL	CURRENT_CHANNEL
#define LAST_CHANNEL	TEMP_CHANNEL

#define CURRENT_BIT 	PC0
#define POT_BIT 		PC1
#define VOLTAGE_BIT 	PC2
#define TEMP_BIT		PC3

#define ON_PIN			PIND
#define DMS_PIN			PIND
#define ON_PORT			PORTD
#define DMS_PORT		PORTD
#define ON_BIT 			PD5
#define DMS_BIT 		PD4

//definem em qual pino sairá o PWM
#define PWM_DDR 		DDRB
#define PWM_PORT 		PORTB
#define PWM_BIT 		PB1


#define BUZZER_DDR 		DDRC
#define BUZZER_PORT 	PORTC
#define BUZZER_BIT 		PC4

#define LED_DDR 		DDRC
#define LED_PORT 		PORTC
#define LED_BIT 		PC5

//modos de operação, definem quem controla o acionamento
#define CAN_MODE		0
#define SERIAL_MODE		1
#define POT_MODE		2

#define maxCont 		3

//uint8 mode = SERIAL;

 // Bit field flags
struct system_flags{
	uint8 warnnig		: 1;
	uint8 erro 			: 1;
	uint8 mode			: 2;
	uint8 on			: 1;
	uint8 dms	    	: 1;
	uint8 				: 0;//completa a variavel
};
typedef struct system_flags flags_t;

flags_t flags;
int16 cont = 0;

uint8 channel = POT_CHANNEL;	//canal do ad
uint16 freq = 1000;				//freq do pwm
uint8 current = 0;
uint8 maxCurrent = 100;
uint8 minDC = 10;
uint8 dc = 0;
uint8 dcReq = 0;				//armazena o valor do dc requisitado
uint8 maxDC = 90;
uint8 maxDV = 7;				//define a variação máxima, x V/s				
uint8 temperature = 0;
uint8 maxTemp = 70;				//temperatura maxima, desliga o sistema
uint8 criticalTemp = 60;		//temperatura critica
uint8 voltage = 0;
uint8 minVotage = 30;

void seta_dc(uint8 d_cycle)		//função para definição do Duty Cicle do PWM
{
	dcReq = d_cycle;
	if(dcReq < minDC)				// Comparação com o valor mínimo de Duty Cicle
		dc = 0;
	else
	{
		if(dcReq > maxDC)			//Comparação com o valor máximo de Duty Cicle
			dc = 100;
		else
		{
			dc = dcReq;
			timer1SetCompareBValue((dc * (timer1GetCompareAValue()))/100);		//seta o valor do comparador B para gerar o DC requerido
		}
	}
}

//esvazia o buffer de entrada da usart
void esvaziaBuffer(){
	while(!usartIsReceiverBufferEmpty())
		usartGetDataFromReceiverBuffer();
}

//envia uma msg usando o protocolo GUI
void stringTransmit(char* texto){
	uint8 i = 0;
	usartTransmit(INICIODOPACOTE);
	for(i = 0; texto[i] != '\0'; i++)
		usartTransmit(texto[i]);
	usartTransmit(FINALDOPACOTE);
}

//convert uint16 to string of 4 characters
void uint16ToString4(char* str,uint16 value){
	str[4] = '\0';
	str[3] = (char ) (value%10 + 48);
	str[2] = (char) ((value%100) / 10 + 48);
	str[1] = (char) ((value%1000) / 100 + 48);
	str[0] = (char) ((value%10000) / 1000 + 48);
}

//convert uint8 to string of 4 characters
void uint8ToString4(char* str,uint16 value){
	str[4] = '\0';
	str[3] = (char ) (value%10 + 48);
	str[2] = (char) ((value%100) / 10 + 48);
	str[1] = (char) ((value%1000) / 100 + 48);
	str[0] = '0';
}

//convert string of 4 characters uint16
uint16 string4ToUint16(char* str){
	uint16 value;
	value = (uint16) (str[0]-48)*1000 + (str[1]-48)*100 
	+ (str[2]-48)*10 + str[3]-48;
	return value;
}

//convert string of 4 characters uint8
uint8 string4Touint8(char* str){
	uint8 value;
	value = (uint8) ((str[1]-48)*100 
	+ (str[2]-48)*10 + str[3]-48);
	return value;
}

int main(void)
{
	flags.mode = POT_MODE;
	// VARIAVEIS LOCAIS;
	char frameData[50];
	uint8 frameIndex = 0;
	char recebido[100] = "";
	char msgToSend[8] = "";
	uint8 pos =  0;
	
	// CONFIGURA ADC
	clrBit(DDRC,POT_BIT);		//SETA O PINO DO ADC COMO ENTRADA
	adcConfig(ADC_MODE_SINGLE_CONVERSION, ADC_REFRENCE_POWER_SUPPLY , ADC_PRESCALER_128);
	adcSelectChannel(POT_CHANNEL);
	adcClearInterruptRequest();
	adcActivateInterrupt();
	adcEnable();
	adcStartConversion();
	
	// CONFIGURA PWM
	timer1Config(TIMER_B_MODE_CTC, TIMER_A_PRESCALER_64);
	timer1ClearCompareBInterruptRequest();									
	timer1ClearCompareAInterruptRequest();
	timer1ActivateCompareBInterrupt();									//ativa a interrupcao do compA
	timer1ActivateCompareAInterrupt();									//ativa a interrupcao do compB
	timer1SetCompareAValue((F_CPU/64)/freq);							//valor do comparador A,  define a frequencia
	timer1SetCompareBValue((dc * (timer1GetCompareAValue()))/100);		//valor do comparador B,  define Duty Cicle

	// CONFIGURA A INTERRUPÇÃO DE CONTROLE(60Hz)
	timer0Config(TIMER_A_MODE_NORMAL, TIMER_A_PRESCALER_1024);			
	timer0ClearOverflowInterruptRequest();								//limpa a interrupcao de OVF
	timer0ActivateOverflowInterrupt();							
	
	// CONFIGURA A USART
	usartConfig(USART_MODE_ASYNCHRONOUS,USART_BAUD_9600 ,USART_DATA_BITS_8,USART_PARITY_NONE,USART_STOP_BIT_SINGLE);
	usartEnableReceiver();
	usartEnableTransmitter();
	usartActivateReceptionCompleteInterrupt();

	sei();
	
	setBit(PWM_DDR,PWM_BIT);			//define o pino do pwm como saída

	setBit(ON_PORT,ON_BIT);				//habilita o pull-up da chave on
	setBit(DMS_PORT,DMS_BIT);			//habilita o pull-up da chave dms

//configura o buzzer e da sinal de alerta de ligação
	setBit(BUZZER_DDR,BUZZER_BIT);			
	setBit(BUZZER_PORT,BUZZER_BIT);
	_delay_ms(1000);
	clrBit(BUZZER_PORT,BUZZER_BIT);

/*
	//pino usado somente para teste no proteus
	setBit(DDRD,PD0);			
	setBit(PORTD,PD0);			
*/

    while(1)
    {
    	if(flags.mode == SERIAL_MODE)
    	{
	    	while(!usartIsReceiverBufferEmpty()){
				frameData[frameIndex++] = usartGetDataFromReceiverBuffer();
				if ((frameData[frameIndex-1] == FINALDOPACOTE)){//se esta no final da palavra
					if(frameData[0] == INICIODOPACOTE ){//verifica se o inicio da palavra esta correto
						strcpy(recebido,frameData);
						pos = (recebido[2]-48) + (recebido[1] - 48)*10;
						if(frameIndex == GETWORDSIZE){
							memcpy( recebido,  (recebido+1), 2);
							recebido[2] = '\0';//isola o id
							switch (pos){
								case 0:
									strcpy(msgToSend,"OK");
									break;
								case 1:
									uint16ToString4(msgToSend,freq);
									break;
								case 2:
									uint8ToString4(msgToSend,maxCurrent);
									break;
								case 3:
									uint8ToString4(msgToSend,maxDC);
									break;
								case 4:
									uint8ToString4(msgToSend,minDC);
									break;
								case 5:
									uint8ToString4(msgToSend,maxDV);
									break;
								case 6 :
									if(flags.on)
										strcpy(msgToSend, "0001");
									else
										strcpy(msgToSend, "0000");
									break;
								case 7:
									if(flags.dms)
										strcpy(msgToSend, "0001");
									else
										strcpy(msgToSend, "0000");
									break;
								case 8:
									uint8ToString4(msgToSend,maxTemp);
									break;
								case 9:
									uint8ToString4(msgToSend,minVotage);
									break;
								case 10:
									uint8ToString4(msgToSend,dc);
									break;
								case 11:
									uint8ToString4(msgToSend,temperature);
									break;
								case 12:
									uint8ToString4(msgToSend,current);
									break;
								case 13:
									uint8ToString4(msgToSend,voltage);
									break;
								default:
									strcpy(msgToSend,"ERRO");
							}
							strcat(recebido,msgToSend);
							strcpy(msgToSend,recebido);

							stringTransmit(msgToSend);
						}
						else{
							if(frameIndex == SETWORDSIZE){
							
								memcpy((void *) recebido, (void *) (recebido+3), 4);//isola somente o valor, usando 4 caracteres
								recebido[4] = '\0';

								switch (pos){
									case 1:
										freq = string4ToUint16(recebido);
										timer1SetCompareAValue((F_CPU/1024)/freq);
										timer1SetCompareBValue((dc * (timer1GetCompareAValue()))/100);//bota o dc
										break;
									case 2:
										maxCurrent = string4Touint8(recebido);
										break;
									case 3:
										maxDC = string4Touint8(recebido);
										if(dcReq > maxDC)
											dc = 100;
										else
											dc = dcReq;
										timer1SetCompareBValue((dc * (timer1GetCompareAValue()))/100);//bota o dc
										break;
									case 4:
										minDC = string4Touint8(recebido);
										if(dcReq < minDC)
											dc = 0;
										else
											dc = dcReq;
										timer1SetCompareBValue((dc * (timer1GetCompareAValue()))/100);//bota o dc
										break;
									case 5:
										maxDV = string4Touint8(recebido);
										break;
									case 6 :
										if(recebido[3] == '1'){
											flags.on = 1;
										}
										if(recebido[3] == '0'){
											flags.on = 0;
										}
										break;
									case 7:
										if(recebido[3] == '1'){
											flags.dms = 1;
										}
										if(recebido[3] == '0'){
											flags.dms = 0;
										}
										break;
									case 8:
										maxTemp = string4Touint8(recebido);
										break;
									case 9:
										minVotage = string4Touint8(recebido);
										break;
									case 10:
										dcReq = string4Touint8(recebido);
										dc = dcReq;
										if(dc < minDC)
											dc = 0;
										else
											if(dc > maxDC)
												dc = 100;
										timer1SetCompareBValue((dc * (timer1GetCompareAValue()))/100);//bota o dc
										break;
									case 11:
									case 12:
									case 13:

									default:
										stringTransmit("ERRO");
								}
							}
							else{
								stringTransmit("wrong size");	
								esvaziaBuffer();
							}
						}
					}
					else{//se o inicio da palavra nao esta correto
						esvaziaBuffer();
					}
					frameIndex = 0;
				}
			}
		}
    }
}

ISR(ADC_vect){
	switch (channel)
	{
		case CURRENT_CHANNEL:
			current = ADC / 5;
			break;
		case POT_CHANNEL:
			if(flags.mode == POT_MODE)
				dcReq = ADC / 10;
			channel = TEMP_CHANNEL;
			break;
		case VOLTAGE_CHANNEL:
			voltage = ADC / 30;
			break;
		case TEMP_CHANNEL:
			temperature = ADC / 2;
			channel = POT_CHANNEL;
			break;
		default: 
			break;
	}
	/*if(channel == LAST_CHANNEL)
		channel = FIRST_CHANNEL;
	else
		channel ++;
	*/
	adcSelectChannel(channel);
	adcStartConversion();
}

ISR(TIMER1_COMPA_vect)
{
	if(dc > 0 && flags.on && flags.dms)
		setBit(PWM_PORT,PWM_BIT);		//Inicia o período em nível alto do PWM
}

ISR(TIMER1_COMPB_vect)
{
	if(dc < 100)
		clrBit(PWM_PORT,PWM_BIT);		//Inicia o período em nível baixo do PWM
}

//60Hz
ISR(TIMER0_OVF_vect)
{
	//setBit(PIND,PD0);
	flags.on = isBitClr(ON_PIN,ON_BIT);
	flags.dms = isBitClr(DMS_PIN,DMS_BIT);
	if(flags.on && flags.dms)
	{
    	if(dc != dcReq)
    	{
    		if(dcReq > dc && dcReq > (minDC + 5))
    		{
    			if(cont == maxCont)
    			{
    				if(dc == 0)
    					seta_dc(minDC);
    				else
    					seta_dc(dc+1);
    				cont = 0;
    			}
    			else
    				cont++;
    		}
    		else
    			if (dcReq < dc)
    				seta_dc(dcReq);			//definição do Duty Cicle do PWM
    	}
	}
	else
	{
		if(dc != 0)					//se o sistema ainda nao esta desligado
			seta_dc(0);				//desliga o sistema
	}
	if(temperature > criticalTemp && !flags.warnnig)
	{
		flags.warnnig = 1;
		setBit(BUZZER_PORT,BUZZER_BIT);
	}
	else
		if(temperature < criticalTemp && flags.warnnig)
		{
			flags.warnnig = 0;
			clrBit(BUZZER_PORT,BUZZER_BIT);
		}
}

ISR(USART_RX_vect)
{
	usartAddDataToReceiverBuffer(UDR0);
}