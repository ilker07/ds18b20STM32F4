#include "stm32f4xx.h"  
#include "math.h"
#include "rcc.h"
#include "gpio.h"
#include "timer.h"
#include "sensor_DS18B20.h"
#include "delay.h"
#include "degiskenler.h"

uint8_t familyCode=0;
uint8_t seriNumber1=0;//Opsiyonel
uint8_t seriNumber2=0;//Opsiyonel
uint8_t seriNumber3=0;//Opsiyonel
uint8_t seriNumber4=0;//Opsiyonel
uint8_t seriNumber5=0;//Opsiyonel
uint8_t seriNumber6=0;//Opsiyonel
uint8_t crcNumber=0;//Opsiyonel
uint8_t thRegister=0;//Opsiyonel
uint8_t tlRegister=0;//Opsiyonel
uint8_t configRegister=0;//Opsiyonel
uint8_t reserved1=0;//Opsiyonel
uint8_t reserved2=0;//Opsiyonel
uint8_t reserved3=0;//Opsiyonel
uint8_t CRCBayt=0;//Opsiyonel

float sicaklikFloat=0;

void sicaklik()
{
  if(!DS18B20_Start())
	return;
  DS18B20_Write (0x33);//Skip ROM komutu.(64 bit doner ilk 8 bit 28h)
	familyCode=DS18B20_Read();
	if(familyCode!=40)
		return;
	seriNumber1=DS18B20_Read();//Opsiyonel
	seriNumber2=DS18B20_Read();//Opsiyonel
	seriNumber3=DS18B20_Read();//Opsiyonel
	seriNumber4=DS18B20_Read();//Opsiyonel
	seriNumber5=DS18B20_Read();//Opsiyonel
	seriNumber6=DS18B20_Read();//Opsiyonel
	crcNumber=DS18B20_Read();//Opsiyonel
	
	if(!DS18B20_Start())
	  return;

	 DS18B20_Write (0xCC);  // skip ROM komutu
	 DS18B20_Write (0x44);  // Sicaklik donusum komutu.
	 bekle(1000000);//750 ms diyor.1 sn verdim.

	if(!DS18B20_Start())
		return;
		  
	DS18B20_Write (0xCC);  // skip ROM komutu.
	DS18B20_Write (0xBE);  // Read Scratch-pad komutu.
		 
	dusukSicaklikBayti = DS18B20_Read();
	yuksekSicaklikBayti = DS18B20_Read();
	thRegister=DS18B20_Read();//Opsiyonel
	tlRegister=DS18B20_Read();//Opsiyonel
	configRegister=DS18B20_Read();//Opsiyonel 127 okumaliyiz default deger
	reserved1=DS18B20_Read();//Opsiyonel 255 okumaliyiz default deger(0xFF)
	reserved2=DS18B20_Read();//Opsiyonel
	reserved3=DS18B20_Read();//Opsiyonel 16 okumaliyiz defaul deger(0x10)
	CRCBayt=DS18B20_Read();//Opsiyonel
	
	
	
	uint8_t dusukDortBit=dusukSicaklikBayti & 0x0F;
	sicaklikFloat=((yuksekSicaklikBayti<<8)|dusukSicaklikBayti)>>4;
	
	uint8_t sayac=1;
	uint8_t deger=0;
	float toplam=0;
	
	for (;sayac<5;sayac++)
	{
	   deger =dusukDortBit>>(4-sayac);
		 deger =deger & 0x01;
		 float us_alma=0;
		 us_alma=pow(2,(-1*sayac)) * deger;
		 toplam +=us_alma;
	
	}
	sicaklikFloat +=toplam;
	
	
	
}


void TIM1_UP_TIM10_IRQHandler(void)  // 1 uslik timer
{  
	
		if (TIM1->SR & (1<<0))  //UIF Bayragi mi
  {
			
			sure++;
		
			if(sure>1000000)  //Flag bayragi 1 sn icin.
				 sure=0;
		
      TIM1->SR &=~(1<<0); //UIF temizlendi.
  }
	
}


int main()
	
{
	
	RCC_Ayar();
	SystemCoreClockUpdate();
	gpio_Ayar();
	tim1_Ayar();

while(1)
{
	
	  sicaklik();
	  bekle(1000000);//1 sn bekle.
	 

}



}

