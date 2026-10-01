#include <asf.h>
#include <board.h>
#include <gpio.h>
#include <sysclk.h>
#include "busy_delay.h"

#define CONFIG_USART_IF (AVR32_USART2)

// defines for BRTT interface
#define TEST_A      AVR32_PIN_PA31
#define RESPONSE_A  AVR32_PIN_PA30
#define TEST_B      AVR32_PIN_PA29
#define RESPONSE_B  AVR32_PIN_PA28
#define TEST_C      AVR32_PIN_PA27
#define RESPONSE_C  AVR32_PIN_PB00

volatile int test_A_interrupt = 0;
volatile int test_B_interrupt = 0;
volatile int test_C_interrupt = 0;


__attribute__((__interrupt__)) static void interrupt_J3(void);

void init(){
    sysclk_init();
    board_init();
    busy_delay_init(BOARD_OSC0_HZ);
	
    gpio_configure_pin(TEST_A, GPIO_DIR_INPUT | GPIO_INIT_HIGH);
    gpio_configure_pin(RESPONSE_A, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
    gpio_configure_pin(TEST_B, GPIO_DIR_INPUT | GPIO_INIT_HIGH);
    gpio_configure_pin(RESPONSE_B, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
    gpio_configure_pin(TEST_C, GPIO_DIR_INPUT | GPIO_INIT_HIGH);
    gpio_configure_pin(RESPONSE_C, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
    
	cpu_irq_disable();
    INTC_init_interrupts();
    INTC_register_interrupt(&interrupt_J3, AVR32_GPIO_IRQ_3, AVR32_INTC_INT1);
    
    
	gpio_enable_pin_interrupt(TEST_A, GPIO_PIN_CHANGE);
	gpio_enable_pin_interrupt(TEST_B, GPIO_PIN_CHANGE);
	gpio_enable_pin_interrupt(TEST_C, GPIO_PIN_CHANGE);
	
	cpu_irq_enable();
	
    stdio_usb_init(&CONFIG_USART_IF);

    #if defined(__GNUC__) && defined(__AVR32__)
        setbuf(stdout, NULL);
        setbuf(stdin,  NULL);
    #endif
}

__attribute__((__interrupt__)) static void interrupt_J3(void){ 
    if (gpio_get_pin_interrupt_flag(TEST_A)) {
	    test_A_interrupt = 1;
	    gpio_clear_pin_interrupt_flag(TEST_A);
    }

    if (gpio_get_pin_interrupt_flag(TEST_B)) {
	    test_B_interrupt = 1;
	    gpio_clear_pin_interrupt_flag(TEST_B);
    }

    if (gpio_get_pin_interrupt_flag(TEST_C)) {
	    test_C_interrupt = 1;
	    gpio_clear_pin_interrupt_flag(TEST_C);
    }
}



int main (void){
    init();
    while(1){
		    if (test_A_interrupt) {
			    test_A_interrupt = 0;

			    if (gpio_get_pin_value(TEST_A)==0){
			     //do stuff
			     gpio_set_pin_low(RESPONSE_A);
			     }else{
			     gpio_set_pin_high(RESPONSE_A);
			    }
		    }

		    if (test_B_interrupt) {
			    test_B_interrupt = 0;

			    if (gpio_get_pin_value(TEST_B)==0){
			     //do stuff
				 busy_delay_us(100);
			     gpio_set_pin_low(RESPONSE_B);
			     }else{
			     gpio_set_pin_high(RESPONSE_B);
			    }
		    }

		    if (test_C_interrupt) {
			    test_C_interrupt = 0;

			    if (gpio_get_pin_value(TEST_C)==0){
			     //do stuff
			     gpio_set_pin_low(RESPONSE_C);
			     }else{
			     gpio_set_pin_high(RESPONSE_C);
				}
			}
    }
}
