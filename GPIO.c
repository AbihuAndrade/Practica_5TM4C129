#include "lib/include.h"
extern void Configurar_GPIO(void)
{
    //Paso 1 Enceder el periferico Run Clock Gate Control GPIO
    SYSCTL->RCGCGPIO |= (1<<1) | (1<<5) | (1<<8) | (1<<12);//enable GPIOF,GPION,GPIOJ
    while((SYSCTL->PRGPIO&0x20)==0){};
    //Paso 2
 //   GPIO_PORTB_AHB_LOCK_R = 0x4C4F434B;
 //   GPIO_PORTB_AHB_CR_R = (1<<3);

    //Habilitamos los piertos de las salidas
    //Usaremos los puertos F, N y B
    GPIOF_AHB->DIR |= (1<<0) | (1<<4);
    GPION->DIR |= (1<<0) | (1<<1);
    GPIOB_AHB->DIR |= (1<<3);

    //Nuestros puertos de las entradas serán los puertos J
    GPIOJ_AHB->DIR |= (0<<1) | (0<<0);
    GPIOJ_AHB->PUR =  (1<<1) | (1<<0);
    
    //Activamos el modo alternativo analógico de cada uno de estos pines
    GPIOJ_AHB->AFSEL |= 0x00;
    GPION->AFSEL |= 0x00;
    GPIOF_AHB->AFSEL |= 0x00;
    GPIOB_AHB->AFSEL |= 0x00;

    GPIOJ_AHB->PCTL|= 0x00;
    GPION->PCTL |= 0x00;
    GPIOF_AHB->PCTL |= 0x00;
    GPIOB_AHB->PCTL |= 0x00;

    //Desactivamos el modo digital de los pines que usaremos
    GPIOF_AHB->DEN |= (1<<0) | (1<<4);
    GPION->DEN |= (1<<0) | (1<<1);
    GPIOJ_AHB->DEN |= (1<<1) | (1<<0);
    GPIOB_AHB->DEN |=  (1<<3);

    //configurar evento de interrupcion PORTJ
    GPIOJ_AHB->IM |= (0<<1) | (0<<0); //Limpiar los bits
    GPIOJ_AHB->IS |= (0<<1) | (0<<0);
    GPIOJ_AHB->IBE |= (0<<1) | (0<<0);
    GPIOJ_AHB->IEV |= (1<<1) | (1<<0);
    GPIOJ_AHB->RIS |= (0<<1) | (0<<0);
    GPIOJ_AHB->IM |= (1<<1) | (1<<0);
    // numero de interrupcion PORTJ = 51
    // n=12 ----> [4n+3] [4n+2] [4n+1] [4n] ---> [4n+3]
 //   NVIC_PRI12_R = (NVIC_PRI12_R&0X00FFFFFF) | 0X80000000; //Jerarquia 4
 //   NVIC_EN1_R = 0X00080000;
}

extern void Prender_LED(void)
{
    static int i=0;
    GPIOF_AHB->DATA = i;
    i = i + 1;
}

extern void GPIOJ_INT_ISR(void)
{
    if (GPIOJ_AHB->RIS == (1<<0))//J0  el cero representara que se presiono el boton 0 y en pycharm se contara los ceros enviados
    {
        GPIOF_AHB->DATA = (0<<4);
      //  tarar(20,128);  //numero de datos, a que ganancia
    }
    if (GPIOJ_AHB->RIS == (1<<1))//J1 el uno representara que se presiono el boton 1
    {

        GPIOF_AHB->DATA = (0<<0);

        GPIOF_AHB->DATA = (1<<0);
   
    }
    GPIOJ_AHB->ICR |=(1<<0)|(1<<1);
}

