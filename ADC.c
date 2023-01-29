/*
 * ADC.c
 *
 *  Created on: 04/11/2019
 *      Author: ravenelco
 */
#include "lib/include.h"

extern void Configura_Reg_ADC0(void)
{
    /*
    Habilitar el modulo 0 del ADC con dos canales analogicos 
    en el puerto E a una velocidad de conversion de 250ksps
    dandole la mayor prioridad al secuenciador 2 con evento
    de procesador 
    */
     //Pag 396 para inicializar el modulo de reloj del adc RCGCADC
    SYSCTL->RCGCADC = (1<<0); 
    //Pag 382 (RGCGPIO) Puertos base habilitación del reloj
    //                     F     E      D       C      B     A
    SYSCTL->RCGCGPIO |= (1<<5)|(1<<4)|(0<<3)|(0<<2)|(0<<1)|(1<<0)|(1<<12)|(1<<8);

    //Pag 760 (GPIODIR) Habilta los pines como I/O un cero para entrada y un uno para salida
    GPIOE_AHB->DIR = (0<<0)|(0<<2)|(0<<4); //Usaremos los puertos F y E en esta práctica

    //(GPIOAFSEL) pag.770 Enable alternate función 
    //En esta parte, activamos el módulo anañógico para que tenga control de esos pines
    GPIOE_AHB->AFSEL =  (1<<0)|(1<<2)|(1<<4);


    //(GPIODEN) pag.781 desabilita el modo digital
    //No usaremos el modo digital de estos pines
    GPIOE_AHB->DEN =  (0<<0)|(0<<2)|(0<<4);


    //Pag 787 GPIOPCTL registro combinado con el GPIOAFSEL y la tabla pag 1808
    //Activamos el número de pin que usaremos, en nuestro caso, el E0, E2 y E4
    GPIOE_AHB->PCTL = GPIOE_AHB->PCTL & (0xFFF0F0F0);


    //(GPIOAMSEL) pag.786 habilitar analogico
    //El comando amsel habilita la función analógica de los pines
    GPIOE_AHB->AMSEL = (1<<0)|(1<<2)|(1<<4);


    //Pag 1159 El registro (ADCPC) establece la velocidad de conversión por segundo
    ADC0->PC = 0x5;//250ksps

    //Pag 1099 Este registro (ADCSSPRI) configura la prioridad de los secuenciadores
    //Modificamos qué secuenciadores usaremos para esta práctica, del menos significativo al más
    ADC0->SSPRI = 0x3012;

    //Pag 1077 (ADCACTSS) Este registro controla la activación de los secuenciadores
    //Debemos apagar los secuenciadores para poder configurarlos
    ADC0->ACTSS  =   (0<<3) | (0<<2) | (0<<1) | (0<<0);

    //Pag 1091 Este registro (ADCEMUX) selecciona el evento que activa la conversión (trigger)
    ADC0->EMUX  = (0x0<<8)|(0x0<<4)|(0x0<<0);
    
    //Pag 1129 Este registro (ADCSSMUX2) define las entradas analógicas con el canal y secuenciador seleccionado
    //Con cada secuenciador, definimos las entradas que usaremos para estos
    ADC0->SSMUX2 = (1<<0);
    ADC0->SSMUX1 = (3<<0);
    ADC0->SSMUX0 = (9<<0);
    
    //pag 868 Este registro (ADCSSCTL2), configura el bit de control de muestreo y la interrupción
    
    ADC0->SSCTL2 = (1<<2) | (1<<1);
    ADC0->SSCTL1 = (1<<2) | (1<<1);
    ADC0->SSCTL0 = (1<<2) | (1<<1);
    
    /* Enable ADC Interrupt */
    ADC0->IM |= (1<<2)|(1<<1)|(1<<0); /* Unmask ADC0 sequence 2 interrupt pag 1082*/
    //NVIC_PRI4_R = (NVIC_PRI4_R & 0xFFFFFF00) | 0x00000020;
    //NVIC_EN0_R = 0x00010000;
    //Pag 1077 (ADCACTSS) Este registro controla la activación de los secuenciadores
    ADC0->ACTSS = (0<<3) | (1<<2) | (1<<1) | (1<<0);
    
}
extern void ADC0_InSeq2(uint16_t *Result,uint16_t *duty){

    //ADC Processor Sample Sequence Initiate (ADCPSSI)
       ADC0->PSSI |= (1<<2)|(1<<1)|(1<<0);
       while((ADC0->RIS&0x04)==0){}; // espera al convertidor
       Result[0] = ADC0->SSFIFO2&0xFFF; //  Leer  el resultado almacenado en la pila2
       //Result[0] = ADC0->SSFIFO2&0xFFF;
       duty[0] =(Result[0]*6250)/4096;
        delayms(10);
        
        
       Result[1] = ADC0->SSFIFO1&0xFFF; //  Leer  el resultado almacenado en la pila2
       duty[1] =(Result[1]*6250)/4096;
        delayms(10);
        
       Result[2] = ADC0->SSFIFO0&0xFFF; //  Leer  el resultado almacenado en la pila2
       duty[2] =(Result[2]*6250)/4096;
       //duty[1] = (Result[1]*1250*3.3)/4096;
       ADC0->ISC =(1<<2)|(1<<1)|(1<<0);  //Conversion finalizada

}

