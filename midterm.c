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

float getDistance()
{
    struct timeval tv1;
    struct timeval tv2;
    long start, end;
    float distance;
    output_low(TrigPin);
    my_delay_ms(2);
    output_high(TrigPin);
    my_delay_ms(10);
    output_low(TrigPin);
    while( !(digitalRead(Echo) == HIGH));
    gettimeofday(&tv1, NULL);
    while( !(digitalRead(Echo) == LOW));
    gettimeofday(&tv2, NULL);
    start = tv1.tv_sec * 1000000 + tv1.tv_usec;
    end  = tv2.tv_sec * 1000000 + tv2.tv_usec;
    distance = (float)(stop - end) / 1000000 * 34000 / 2;
    return distance;
}

unsigned wait_sensor(){
  i = 0;
  set_timer0(0);
  count = 0;
  while(!input(EchoPin) && (i < 1000))
    i = count * 256 + get_timer0();
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

