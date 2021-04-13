
#include "gpio.h"
#include "stm32f4xx.h" 


void gpio_Ayar()
{


  //D12 Ayarlari
	RCC->AHB1ENR |=1<<3;  //D Clock Aktif.
	
	GPIOD->MODER  = 0x05000000; //Output modu
	GPIOD->OTYPER = 0x00000000; //Push-Pull
	GPIOD->OSPEEDR =0x03000000; //Yuksek hiz
	GPIOD->BSRR =1<<28; //D12 low
	
	
	//B1 Ayarlari
	RCC->AHB1ENR |=1<<1;  //B Clock Aktif.
	
	GPIOB->MODER  = 0x00000004; //Output modu
	GPIOB->OTYPER = 0x00000000; //Push-Pull
	GPIOB->OSPEEDR =0x00000008; //Yuksek hiz
	GPIOB->BSRR =1<<1; //B1 high
	GPIOB->BSRR =1<<17; //B1 low
	
}

