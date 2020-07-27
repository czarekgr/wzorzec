
// Arduino Uno ATmega328
//wzór
//https://embedds.com/programming-16-bit-timer-on-atmega328/

#include <avr/io.h>
#include <avr/interrupt.h>

//ICP1 na 8 Arduino Nano

// #define kwarc 16000000UL //znamionowa częstotliwość taktowania
// #define kwarc 15992296UL //zmierzona częstotliwość kwarcu, k... prawie 8kHz odchyłki!!!


volatile uint16_t Capt, faza;


volatile uint8_t Flag = 0;
byte k = 2;
void InitTimer1(void)
{
  //Set Initial Timer value
  TCCR1A = 0;
  TCNT1 = 0;
  OCR1A = 48757;  // do tylu ma liczyć licznik


  //First capture on rising edge
  TCCR1B |= (1 << ICES1);

  //Enable input capture and overflow interrupts
  TIMSK1 |= (1 << ICIE1) | (1 << TOIE1);
}


void StartTimer1(void)
{
  TCCR1B |= (1 << WGM12);

  //Start timer without prescaller
  TCCR1B |= (1 << CS10);
  TCCR1B &= ~(1 << CS11);
  TCCR1B &= ~(1 << CS12);
  //Enable global interrutps
  sei();
}


//Overflow ISR
ISR(TIMER1_OVF_vect)
{
  //increment overflow counter
  // T1Ovs2++;
}


ISR(TIMER1_CAPT_vect)
{
  //save captured timestamp
  Capt = ICR1;
  Flag = 1;
}

void setup() {
  Serial.begin(9600); //Uruchomienie komunikacji
  Serial.println(F("Start"));
  InitTimer1();
  StartTimer1();

}


void loop() {
  if (Flag == 1) {
    if (k) {
      faza = Capt;
      k--;
    } else
    {

      Serial.println((int)(Capt - faza) );
    };
    Flag = 0;
  }
}
