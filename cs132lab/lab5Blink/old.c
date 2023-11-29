#include "libopencm3/stm32/rcc.h"   //Needed to enable clocks for particular GPIO ports
#include "libopencm3/stm32/gpio.h"  //Needed to define things on the GPIO

int main(void) {
    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO5);            //GPIO Port Name, GPIO Mode, GPIO Push Up Pull Down Mode, GPIO Pin Number
    gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ, GPIO5);   //GPIO Port Name, GPIO Pin Driver Type, GPIO Pin Speed, GPIO Pin Number
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO6);    
    gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ, GPIO6);
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO7);    
    gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ, GPIO7);
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO8);  
    gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ, GPIO8);
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO9);  
    gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ, GPIO9);
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO10);  
    gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ, GPIO10);

    gpio_set(GPIOA, GPIO6);
    while(true){
        for (volatile unsigned int tmr=5e5; tmr > 0; tmr--);
        gpio_toggle(GPIOA, GPIO6);
        gpio_toggle(GPIOA, GPIO7);
        for (volatile unsigned int tmr=5e5; tmr > 0; tmr--);
        gpio_toggle(GPIOA, GPIO7);
        gpio_toggle(GPIOA, GPIO8); 
        for (volatile unsigned int tmr=5e5; tmr > 0; tmr--);
        gpio_toggle(GPIOA, GPIO8);
        gpio_toggle(GPIOA, GPIO9);  
        for (volatile unsigned int tmr=5e5; tmr > 0; tmr--);
        gpio_toggle(GPIOA, GPIO9);
        gpio_toggle(GPIOA, GPIO10);
        for (volatile unsigned int tmr=5e5; tmr > 0; tmr--);
        gpio_toggle(GPIOA, GPIO10);
        gpio_toggle(GPIOA, GPIO9);
        for (volatile unsigned int tmr=5e5; tmr > 0; tmr--);
        gpio_toggle(GPIOA, GPIO9);
        gpio_toggle(GPIOA, GPIO8);
        for (volatile unsigned int tmr=5e5; tmr > 0; tmr--);
        gpio_toggle(GPIOA, GPIO8);
        gpio_toggle(GPIOA, GPIO7);
        for (volatile unsigned int tmr=5e5; tmr > 0; tmr--);
        gpio_toggle(GPIOA, GPIO7);
        gpio_toggle(GPIOA, GPIO6);     

    }

    printf(“%X”, GPIO2);
    
    return 0;
}