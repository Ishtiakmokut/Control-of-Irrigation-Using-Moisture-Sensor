#include <mega32.h>
#include <delay.h>
#include <stdlib.h>
#include <alcd.h>
#define push1 PIND.0
#define push2 PIND.1


float adc_data;
char m_limit[5];
float limit=25;

void disp_limit()
{
      delay_ms(110);  
      ftoa(limit,0,m_limit);
      lcd_gotoxy(0,1);
      lcd_puts("Limits: ");
      lcd_puts(m_limit);                         

}
interrupt [ADC_INT] void adc_isr(void)
{

    char disp[16];
    // Read the AD conversion result
    adc_data=ADCW/4; //1.5/10.24;
    ftoa(adc_data,0,disp);
    lcd_gotoxy(0,0);
    
    lcd_puts("Moist: ");
    lcd_puts(disp);
    lcd_puts(" ");
    
}
void main(void)
{


ADMUX=0b11000000;
ADCSRA=(1<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);


lcd_init(16);
lcd_clear();
#asm("sei")
DDRB=0xFF;
DDRD=0x00;
PORTB=0x00;




while (1)
      {      
      //delay_us(10);
      ADCSRA |= (1<<ADSC);
      delay_ms(10);   
            
      if(adc_data<limit)   PORTB =0x80;
      else PORTB = 0x00;
      
      //limit
      disp_limit();
      if(push1==0) {
        limit+=1;
        disp_limit();
        }
      if(push2==0) {
       limit-=1;
       disp_limit();
        }
                            
      }
}
