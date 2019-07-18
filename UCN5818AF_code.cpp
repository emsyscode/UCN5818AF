/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void send_char(unsigned char a);
void send_data(unsigned char a);
void send_update_clock(void);
void DigitTo7SegEncoder(unsigned char digit);
void update_clock(void );
//ISR(TIMER1_COMPA_vect );

#define VFD_in 7
#define VFD_clk 8
#define VFD_stb 9
#define VFD_latch 10

#define AdjustPins    PIND // before is C, but I'm use port C to VFC Controle signals
#define SegCntrlDDR   DDRD

#define BIN(x) \
( ((0x##x##L & 0x00000001L) ? 0x01 : 0) \
| ((0x##x##L & 0x00000010L) ? 0x02 : 0) \
| ((0x##x##L & 0x00000100L) ? 0x04 : 0) \
| ((0x##x##L & 0x00001000L) ? 0x08 : 0) \
| ((0x##x##L & 0x00010000L) ? 0x10 : 0) \
| ((0x##x##L & 0x00100000L) ? 0x20 : 0) \
| ((0x##x##L & 0x01000000L) ? 0x40 : 0) \
| ((0x##x##L & 0x10000000L) ? 0x80 : 0))



int timer=0;
bool state=0;
unsigned char secs;
unsigned int miliSecs;
unsigned char minutes;
unsigned char hours;
unsigned char day;
unsigned char SegLSB =0;
unsigned char SegMSB = 0;

unsigned char Digit;
unsigned char number, DisplayCommonPin;
unsigned char Grid;
unsigned long time;

unsigned long regist32 = 0b11111111;  //Hex value of 32 bits FF FF FF FF
unsigned long lng32, lngTmp;

unsigned long segments[] ={
  //font data
  //  fge.987654321dcba_0-------------  // the "." is dot and "_" is under number... the semicolon is not connected
    0b10100000000001111000000000000000, //0
    0b00000000000000110000000000000000, //1
    0b01100000000001011000000000000000, //2
    0b01000000000001111000000000000000, //3
    0b11000000000000110000000000000000, //4
    0b11000000000001101000000000000000, //5
    0b11100000000001101000000000000000, //6
    0b00000000000000111000000000000000, //7
    0b11100000000001111000000000000000, //8
    0b11000000000001111000000000000000, //9
    0b01000000000000000000000000000000  //-
  };
unsigned long grids[] ={
  //font data
  //  fge.987654321dcba_0-------------  // 
    0b00000000000010000000000000000000, //  Grid 1
    0b00000000000100000000000000000000, //  Grid 2
    0b00000000001000000000000000000000, //  Grid 3
    0b00000000010000000000000000000000, //  Grid 4
    0b00000000100000000000000000000000, //  Grid 5
    0b00000001000000000000000000000000, //  Grid 6
    0b00000010000000000000000000000000, //  Grid 7
    0b00000100000000000000000000000000, //  Grid 8
    0b00001000000000000000000000000000, //  Grid 9
    0b00000000000000000010000000000000  //  Grid 10
  };
//                      fge.234567890dcba_1-------------  // the "." is dot and "_" is under number... the semicolon is not connected
unsigned long Long0 = 0b10100000000001111000000000000000; // Size of long 4294967295;
unsigned long Long1 = 0b00000000000000110000000000000000; // Size of long 4294967295;
unsigned long Long2 = 0b01100000000001011000000000000000; // Size of long 4294967295;
unsigned long Long3 = 0b01000000000001111000000000000000; // Size of long 4294967295;
unsigned long Long4 = 0b11000000000000110000000000000000; // Size of long 4294967295;
unsigned long Long5 = 0b11000000000001101000000000000000; // Size of long 4294967295;
unsigned long Long6 = 0b11100000000001101000000000000000; // Size of long 4294967295;
unsigned long Long7 = 0b00000000000000111000000000000000; // Size of long 4294967295;
unsigned long Long8 = 0b11100000000001111000000000000000; // Size of long 4294967295;
unsigned long Long9 = 0b11000000000001111000000000000000; // Size of long 4294967295;
unsigned long Long_ = 0b01000000000000000000000000000000; // Score

unsigned char word1 = 0;
unsigned char word2 = 0;
unsigned char word3 = 0;
unsigned char word4 = 0;





//**************************************************************//



void send_data(unsigned char a)
{
  unsigned char transmit = 7; //define our transmit pin
  unsigned char data = 170; //value to transmit, binary 10101010
  unsigned char mask = 1; //our bitmask
  
  data=a;
  //This don't send the strobe signal, to be used in burst data send
   for (mask = 00000001; mask>0; mask <<= 1) { //iterate through bit mask
     digitalWrite(VFD_clk, LOW);
     if (data & mask){ // if bitwise AND resolves to true
        digitalWrite(VFD_in, HIGH);
     }
     else{ //if bitwise and resolves to false
       digitalWrite(VFD_in, LOW);
     }
     delayMicroseconds(5);
    digitalWrite(VFD_clk, HIGH);
    delayMicroseconds(5);
    //digitalWrite(VFD_clk, LOW);
    //delayMicroseconds(5);
   }
   digitalWrite(VFD_clk, HIGH);
}

void send_update_clock(void)
{
  if (secs >=60){
    secs =0;
    minutes++;
  }
  if (minutes >=60){
    minutes =0;
    hours++;
  }
  if (hours >=24){
    hours =0;
  }
    //*************************************************************
    DigitTo7SegEncoder(secs%10);
    lngTmp  = lng32 | grids[1];
    SegTo32Bits();
    DigitTo7SegEncoder(secs/10);
    lngTmp  = lng32 | grids[2];
    SegTo32Bits();
    DigitTo7SegEncoder('-');
    lngTmp  = lng32 | grids[3];
    SegTo32Bits();
    //*************************************************************
    DigitTo7SegEncoder(minutes%10);
    lngTmp  = lng32 | grids[4];
    SegTo32Bits();
    DigitTo7SegEncoder(minutes/10);
    lngTmp  = lng32 | grids[5];
    SegTo32Bits();
    DigitTo7SegEncoder('-');
    lngTmp  = lng32 | grids[6];
    SegTo32Bits();
    //**************************************************************
    DigitTo7SegEncoder(hours%10);
    lngTmp  = lng32 | grids[7];
    SegTo32Bits();
    DigitTo7SegEncoder(hours/10);
    lngTmp  = lng32 | grids[8];
    SegTo32Bits();
    //**************************************************************
}
void SegTo32Bits(){
       word1 = (lngTmp & 0xff000000UL) >> 24;
       word2 = (lngTmp & 0x00ff0000UL) >> 16;
       word3 = (lngTmp & 0x0000ff00UL) >>  8;
       word4 = (lngTmp & 0x000000ffUL) >>  0;
       send_char(word4);
       send_char(word3);
       send_char(word2);
       send_char(word1);
       digitalWrite(VFD_latch, HIGH);
       delayMicroseconds(50);
       digitalWrite(VFD_latch, LOW);
       delayMicroseconds(50);
}


void DigitTo7SegEncoder( unsigned char digit)
{
  switch(digit)
  {
    case 0: lng32=Long0;
    break;
    case 1: lng32=Long1;
    break;
    case 2: lng32=Long2;
    break;
    case 3: lng32=Long3;
    break;
    case 4: lng32=Long4;
    break;
    case 5: lng32=Long5;
    break;
    case 6: lng32=Long6;
    break;
    case 7: lng32=Long7;
    break;
    case 8: lng32=Long8;
    break;
    case 9: lng32=Long9;
    break;
    case '-':  lng32 = Long_;
    break;
  }
  
} 

//*************************************************//
void setup() {
  pinMode(VFD_clk, OUTPUT);
  pinMode(VFD_in, OUTPUT);
  pinMode(VFD_stb, OUTPUT);
  pinMode(VFD_latch, OUTPUT);
  pinMode(13, OUTPUT);

  int timer1_counter;
  unsigned short m= 15625;
  Serial.begin(9600);

  /*CS12  CS11 CS10 DESCRIPTION
  0        0     0  Timer/Counter1 Disabled 
  0        0     1  No Prescaling
  0        1     0  Clock / 8
  0        1     1  Clock / 64
  1        0     0  Clock / 256
  1        0     1  Clock / 1024
  1        1     0  External clock source on T1 pin, Clock on Falling edge
  1        1     1  External clock source on T1 pin, Clock on rising edge
 */
  // initialize timer1 
  cli();           // disable all interrupts
  // initialize timer1 
  //noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;// This initialisations is very important, to have sure the trigger take place!!!
  TCNT1  = 0;
  // Use 62499 to generate a cycle of 1 sex 2 X 0.5 Secs (16MHz / (2*256*(1+62449) = 0.5
  OCR1A = 62499;            // compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= ((1 << CS12) | (0 << CS11) | (0 << CS10));    // 256 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  //interrupts();             // enable all interrupts

  sei();             // enable all interrupts

  digitalWrite(VFD_stb, LOW);

  digitalWrite(13, LOW);
  delay(500);
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  delay(500);
  digitalWrite(13, HIGH);
  delay(500);
  check_VFD();
}
void send_char(unsigned char a)
{
 //this code is to SN75518
 unsigned char transmit = 7; //define our transmit pin
 unsigned char data = 170; //value to transmit, binary 10101010
 unsigned char mask = 1; //our bitmask
  data=a;
  digitalWrite(VFD_clk, HIGH);
  delayMicroseconds(5);
  for (mask = 00000001; mask>0; mask <<= 1) { //iterate through bit mask
    digitalWrite(VFD_clk, LOW);
    delayMicroseconds(5);
    if (data & mask){ // if bitwise AND resolves to true
      digitalWrite(VFD_in, HIGH);
    }
    else{ //if bitwise and resolves to false
      digitalWrite(VFD_in, LOW);
    }
    delayMicroseconds(5);
    digitalWrite(VFD_clk, HIGH);
    delayMicroseconds(5); 
  }
}

void printOut(char c) {
// Prints out Binary value (1 or 0) of byte
  for (int bits = 7; bits > -1; bits--) {
    // Compare bits 7-0 in byte
    if (c & (1 << bits)) {
      Serial.print ("1");
    }
    else {
      Serial.print ("0");
    }
  }
  Serial.println();
}

void loop() {
  //
 
// Notes related to the IC SN75518:
// SN75518 Serial data is entered into the shift register on the low-to-high transition of CLOCK
// While LATCH ENABLE is high, parallel data is transferred to the output buffers through a 32-bit latch
// Data present in the latch during the high-to-low transition of LATCH ENABLE is latched
// When STROBE is low, all Q outputs are enabled. When STROBE is high, all Q outputs are low.

  send_update_clock();
  /*
  delay(20);
  
  // only to test unsigned long
  Serial.print("Time: ");
  time = millis();
  //prints time since program started
  Serial.println(time);
  // wait a second so as not to send massive amounts of data
  //delay(1000);
*/
}


//*************************************************//

void check_VFD()
{
  digitalWrite(VFD_stb, LOW);
  for (int j=0; j<9; j++)
  {
  lng32 = segments[j] | grids[j];
 digitalWrite(VFD_latch, HIGH);
 delayMicroseconds(50);
 word1 = (lng32 & 0xff000000UL) >> 24;
 word2 = (lng32 & 0x00ff0000UL) >> 16;
 word3 = (lng32 & 0x0000ff00UL) >>  8;
 word4 = (lng32 & 0x000000ffUL) >>  0;

 send_char(word4);
  printOut(word4);
  send_char(word3);
  printOut(word3);
  send_char(word2);
  printOut(word2);
  send_char(word1);
  printOut(word1);
 
  digitalWrite(VFD_latch, LOW);
  delayMicroseconds(50);
  delay(1000);
  }
digitalWrite(VFD_stb, LOW);
  for (int j=9; j>-1; j--)
  {
  lng32 = segments[j] | grids[j];
 digitalWrite(VFD_latch, HIGH);
 delayMicroseconds(50);
 word1 = (lng32 & 0xff000000UL) >> 24;
 word2 = (lng32 & 0x00ff0000UL) >> 16;
 word3 = (lng32 & 0x0000ff00UL) >>  8;
 word4 = (lng32 & 0x000000ffUL) >>  0;

 send_char(word4);
 printOut(word4);
 send_char(word3);
 printOut(word3);
 send_char(word2);
 printOut(word2);
 send_char(word1);
 printOut(word1);
 
  digitalWrite(VFD_latch, LOW);
  delayMicroseconds(50);
  delay(1000);
  }
}

ISR(TIMER1_COMPA_vect)   {    //This is the interrupt request
                            // https://sites.google.com/site/qeewiki/books/avr-guide/timers-on-the-atmega328
      secs++;
      //check_VFD();
} 