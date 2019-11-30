/* Name: main.c
 * Author: Christopher Bright
 * Copyright (c) 2019 Christopher Bright

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void load_h20();
void start_timer1();
void stop_timer1();
#define output_low(port,pin) port &= ~(1<<pin)
#define output_high(port,pin) port |= (1<<pin)
#define set_input(portdir,pin) portdir &= ~(1<<pin)
#define set_output(portdir,pin) portdir |= (1<<pin)
#define pin_is_set(port,pin) (port & (1 << pin)) >> pin
#define FALSE 0
#define TRUE (!FALSE)

volatile uint16_t seconds;
uint8_t isH20Loading;

int main(void)
{
    // enable global interrupts
    sei();

    set_input(PORTB,1)

    while(1)
    {
        if(pin_is_set(PORTB,1)){
            load_h20(660);
        }

    }
    return 0;               /* never reached */
}

void load_h20(uint16_t mLiters)
{
    float const H20_PUMP_ML_PER_SEC = 28.33;
    if(isH20Loading == FALSE){
        start_timer1();
        output_high(PORTA,5);
        isH20Loading = TRUE;
    }

    if(isH20Loading == TRUE){
        if((mLiters / H20_PUMP_ML_PER_SEC) >= seconds){
            stop_timer1();
            output_low(PORTA,5);
            isH20Loading = FALSE;
        }
    }
}

ISR (TIMER1_COMPA_vect){
    //increment seconds counter
    seconds++;
{

void start_timer1(){
    // set up timer with prescaler = 64 and CTC mode
    TCCR1B |= (1 << WGM12)|(1 << CS11)|(1 << CS10);
  
    // initialize counter
    TCNT1 = 0;
  
    // initialize compare value; 1 sec at 1Mhz 64 perscalar
    OCR1A = 15624;
  
    //init seconds
    seconds = 0;

    // enable compare interrupt
    TIMSK |= (1 << OCIE1A);
}

void stop_timer1(){
    //disable timer
    TCCR1B &= ~((1 << WGM12)|(1 << CS11)|(1 << CS10));

    //disable timer interrupt
    TIMSK &= ~(1 << OCIE1A);
}

