#include <stdio.h>
#include <avr/io.h>
#include <wiringPi.h>
#include <stdio.h>
#include <sys/time.h>
#define TrigPin 2
#define EchoPin 3

void my_delay_ms(uint32_t timeDelayed);
float getDistance();
unsigned wait_sensor();
void timer0_isr();

unsigned int8 cnt;
unsigned int16 i;
#INT_TIMER0
void timer0_isr(){
  cnt++;
  clear_interrupt(INT_TIMER0);
}

float getDistance()
{
    i = 0;
    set_timer0(0);
    cnt = 0;
    while(input(EchoPin) && (i < 25000))
        i = cnt * 256 + get_timer0();
    return i;
}

unsigned wait_sensor(){
  i = 0;
  set_timer0(0);
  cnt = 0;
  while(!input(EchoPin) && (i < 1000))
    i = cnt * 256 + get_timer0();
  if(i > 990)
    return 0;
  else
    return 1;
}

void my_delay_ms(uint32_t timeDelayed)
{
    unsigned int i;
    for (i=0; i<(timeDelayed/10); i++)
    {
        _delay_ms(10);
    }
    for (i=0; i < (timeDelayed % 10); i++)
    {
        _delay_ms(1);
    }
}

int main(void)
{
    float distance;
    setup_timer_0 (T0_INTERNAL | T0_DIV_2);
    DDRD |= _BV(DDD4);
    DDRD |= _BV(DDD5);
    DDRD |= _BV(DDD6);
    while(1){
        output_high(TrigPin);
        my_delay_ms(10);
        output_low(TrigPin);
        if(wait_sensor()) {
            distance = get_distance();
            if(distance < 20)
            {
                PORTB |= _BV(PORTB6);
            }
            else {
                PORTB &= ~_BV(PORTB6);
            }
            if(distance < 10)
            {
                PORTB |= _BV(PORTB5);
            }
            else {
                PORTB &= ~_BV(PORTB5);
            }
            if(distance < 5)
            {
                PORTB |= _BV(PORTB4);
            }
            else {
                PORTB &= ~_BV(PORTB4);
            }
      }
    return 0;
}

