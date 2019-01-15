#include<avr/io.h>
#include<util/delay.h>

void init_adc()
{
ADCSRA=ADCSRA|0b00000001<<ADEN;				//Ennable ADC by accessing "ADCSRA" register's "ADEN" bit (7th bit)
ADCSRA=ADCSRA|0B00000001<<ADPS2;			//Calculate division factor and accordingly make the necessary pins high 
ADMUX=ADMUX|0b00000001<<REFS0;				//Set reference voltage to 5V
}

unsigned int read_adc()
{
int p=0b00000001;
ADMUX=ADMUX|p;								//MUX0 is set HIGH in order to take input from pin 39 "ADC1"
ADCSRA=ADCSRA|(0b00000001<<ADSC);			//To start conversion, ADSC is set high
while((ADCSRA&(0b00000001<<ADIF))==0)		//ADIF becomes HIGH when conversion ends
{}
ADCSRA=ADCSRA|(0b00000001<<ADIF);			//Reset ADIF bit (ATmega 16 is so designed that once ADIF is set HIGH, it becomes LOW!)
return(ADC);								//Get the output from ADC
}

void main()
{
DDRD=0b00000000;	//SETTING PART D AS AN INPUT PART
DDRB=0b11111111;	//SETTING PART B AS AN OUTPUT PART
PORTD=0b11111111;	//PULLING UP ALL THE PINS OF PART D

init_adc();			//Call init_adc function
_delay_ms(500);
unsigned int a;
unsigned int i;
unsigned int T;
unsigned int TON;
unsigned int TOFF;

int c;

while(1)
{

c=PIND;

//Outside cave

if(c==0b11111010)	//No obstacle and both sensors on white surface
PORTB=0b00001010;	//Move forward

if(c==0b11111110)	//No obstacle and left sensor on black line
PORTB=0b00000110;	//Turn left

if(c==0b11111011)	//No obstacle and right sensor on black line
PORTB=0b00001001;	//Turn right

if(c==0b11111111)	//Junction
PORTB=0b00011010;	//Beep and move forward

if(c==0b11111000)	//Obstacle on black line
PORTB=0b00010000; 	//Stop and beep

//Inside cave

if(c==0b11110010)	//No obstacle and both sensors on white surface
{
i=read_adc();
a=(i/40);			//Scaling down (40 is an optimum tested value, however can be changed for tuning)
T=25.6;				//Dividing 1024 (2^10 quantization levels) by above factor we get scaled down total time period
TON=a;
TOFF=T-TON;
PORTB=0b00001010;	//Move forward
_delay_ms(TON);		//PWM
PORTB=0b00000000;
_delay_ms(TOFF);
}

if(c==0b11110110)	//No obstacle and left sensor on black line
{
i=read_adc();
a=(i/40);			//Scaling down (40 is an optimum tested value, however can be changed for tuning)
T=25.6;				//Dividing 1024 (2^10 quantization levels) by above factor we get scaled down total time period
TON=a;
TOFF=T-TON;
PORTB=0b00000110;	//Turn left
_delay_ms(TON);		//PWM
PORTB=0b00000000;
_delay_ms(TOFF);
}

if(c==0b11110011)	//No obstacle and right sensor on black line
{
i=read_adc();
a=(i/40);			//Scaling down (40 is an optimum tested value, however can be changed for tuning)
T=25.6;				//Dividing 1024 (2^10 quantization levels) by above factor we get scaled down total time period
TON=a;
TOFF=T-TON;
PORTB=0b00001001;	//Turn right
_delay_ms(TON);		//PWM
PORTB=0b00000000;
_delay_ms(TOFF);
}

if(c==0b11110111)	//Junction
{
PORTB=0b00011010;	//Beep and move forward
_delay_ms(100);
}

}

}