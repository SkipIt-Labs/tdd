#include "defs.h"
#include "iofn.h"
#include <stdio.h>

char buf[17];

#define TIMER2 2
#define N 250
#define P 256
#define K 125

void systime_init();
unsigned long get_systime();

int main(){
     char code_prev = NOKEY;
     char code_now = NOKEY;
     unsigned long ts_now = 0;
     unsigned long ts_ante = 0;
    char kbcode, kbhit = 0;
    unsigned char cycles=0;
    initLCD_DLNF();
    initLCD_DCBIDS();
    systime_init();
    PORTA = 0;
    
    char buf[17];
    
  unsigned char ore = 23, minute = 59, secunde = 50;
  // TCCR2 setting is done EXACTLY as below. The exam requires this style.
  // Replace? with calculated values
  // ????CTC mode
  TCCR2=0b00011110;
  // ?? ????P=256
  // ??????? OC2 Toggle Mode :)
  OCR2=249;
  while(1){
     ts_now = get_systime();
     if(ts_now - ts_ante >= KB_DELAY){ //citirile se fac din 50 ms in 50 ms
        code_now = kbscan();
        
        if(code_prev == NOKEY && code_now != NOKEY) {
           kbcode = code_now;
           kbhit = 1;
        }
        code_prev = code_now;
        ts_prev = ts_now;
     }
     if (kbhit){
        kbhit = 0;
        gotoLC(1, 1)
        putchLCD(kbcode);
     }
     if(testbit(TIFR, OCF2) ){
        TIFR = 1<< OCF2; //clear flag
        cycles++;
        if(cycles == K){

           // a second elapsed
           cycles=0;
           // update seconds, minutes, hours
           secunde++;
            if(secunde == 60) {
               secunde = 0;
               minute++;
               if(minute == 60) {
                  minute = 0;
                  ore++;
                  if(ore == 24) {
                     ore = 0;
                  }
               }
            }
           // display time
           sprintf(buf, "%2d:%02d:%02d", ore, minute, secunde);
           gotoLC(2, 1);
           putsLCD(buf);
           // Check the time framing:
           if( testbit(TIFR, OCF2) ){
              //Display error message
              clrLCD();
              gotoLC(2,1);
              putsLCD("ERROR!!!");
              //and enter an infinite loop.
              while(1){}
           }
        }
     }//end if TIFR
  }//end while
}//end main


