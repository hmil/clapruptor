#include <avr/sleep.h>
#include <avr/io.h>
#include <avr/interrupt.h>


#define SIG_PIN 0x02
// #define LED_PIN 0x01
#define CMD_PIN 0x04


enum { STATE_INIT, STATE_AWAIT, STATE_READY, STATE_DONE };

static uint8_t state = STATE_INIT;

void start_timer(uint16_t count) {
    TIMSK0 = 0; // Disable interrupts while changing parameters

    OCR0AH = count >> 8;
    OCR0AL = count & 0xff;
    TCNT0 = 0;
    TIFR0 = 0x02; // Reset compare match

    TIMSK0 = 0x02; // Enable interrupts on compare match
}

void start_await() {
    // Initialize the timer
    start_timer(1024); // 256 us increments
    state = STATE_AWAIT;
}

// The sound is a valid control request. process it.
void accept_request() {
    PORTB ^= CMD_PIN;
    state = STATE_DONE;
}

ISR (TIM0_COMPA_vect)
{
    TIMSK0 = 0;
    switch (state) {
        case STATE_AWAIT:
            state = STATE_READY;
            start_timer(2048);
            break;
        case STATE_READY:
            state = STATE_INIT;
            break;
        case STATE_DONE:
            state = STATE_INIT;
            break;
    }
}

ISR (ANA_COMP_vect)
{
    switch (state) {
        case STATE_INIT:
            start_await();
            break;
        case STATE_READY:
            accept_request();
            break;
        case STATE_AWAIT:
            start_await();
            break;
    }
}


int main(void)
{
    DDRB |= CMD_PIN; // Enable output on CMD pins

    SREG |= 0x80; // Enable global interrupts
    // EICRA |= 0x03;
    PCICR |= 0x01; // Enable pin change interrupts (PCIE0)
    PCMSK |= SIG_PIN; // Enable interrupt on signal pin

    TCCR0A = 0; // Normal mode,
    TCCR0B = 0x04; // Timer clock source: clock / 256

    ACSR |= 0x0a; // Enable comparator interrupt on falling edge

    sei(); // Enable interrupts

    start_timer(2048);

    /* sleep forever */
    for (;;)
        // PORTB = (PORTB & ~0x04) | (PINB & 0x02) << 1;
        sleep_mode();
    return (0);
}
