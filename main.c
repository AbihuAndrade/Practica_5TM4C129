#include "lib/include.h"
int x;
int i;
int main(void)

{
    volatile uint16_t freq;
     freq=1250;

    uint16_t Result[2];
    uint16_t duty[2];
    Configurar_PLL();  //Confiuracion de velocidad de reloj
    Configura_Reg_ADC0();
    Configurar_UART0();
    Configura_Reg_PWM1(1250);//Configuro a 1khz el pwm
    while(1)
    {
         //llamada a la conversion por procesador
       for(x=159;x>=1;x--)
       {
        PWM0->_1_CMPA = (x*100);
        delayms(20);
       }
       for(x=1;x<=159;x--)
       {
         PWM0->_1_CMPA = x*100;
         delayms(20);
       }
       delayms(50);


}
}

