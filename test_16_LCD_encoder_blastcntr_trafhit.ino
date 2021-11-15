#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x27 for a 20 chars x4 lines
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Make custom characters:

byte Sship[] = {
  B00100,
  B00100,
  B01110,
  B11111,
  B11111,
  B11111,
  B01010,
  B10001
  };

byte Blast1[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00100,
  B00100,
  B00100
  };

byte Blast2[] = {
  B00000,
  B00100,
  B00100,
  B00100,
  B00000,
  B00000,
  B00000,
  B00000
  };

byte Destroy[] = {
  B00100,
  B10000,
  B00001,
  B00000,
  B10000,
  B00001,
  B01000,
  B00010
  };

volatile boolean fired;
volatile boolean up;
volatile boolean blasted;

#define PINA 2
#define PINB 4
#define PINC 3
#define INTERRUPT 0  //  pin 2
#define INTERRUPT2 1  //  pin 3

// Interrupt for encoder pin A
void isr ()
{
  if (digitalRead (PINA))
    up = digitalRead (PINB);
  else
    up = !digitalRead (PINB);
  fired = true;
}  // end of isr


void isr2 ()
{
blasted = true;
      Serial.print ("BLAST ");  
    Serial.println (" triggered ");
  
}  // end of isr

void setup ()
{
// initialize LCD
 lcd.init();
// backlight
lcd.backlight();

 // Custom symbols:
lcd.createChar(8, Sship);
lcd.createChar(2, Blast1);
lcd.createChar(3, Blast2);
lcd.createChar(4, Destroy);
  
  digitalWrite (PINA, HIGH);     // enable pull-ups
  digitalWrite (PINB, HIGH); 
  digitalWrite (PINC, HIGH);
  
  attachInterrupt (INTERRUPT, isr, CHANGE);   // interrupt 0 is pin 2, interrupt 1 is pin 3
attachInterrupt (INTERRUPT2, isr2, CHANGE);   // interrupt 0 is pin 2, interrupt 1 is pin 3

  Serial.begin (115200);

  lcd.setCursor(0, 0);
  lcd.print("Space Charge ");
    lcd.setCursor(0, 1);
    lcd.print("by Bujnos Bujnowski");
    lcd.setCursor(0, 2);
    lcd.print("v2.0  2021");
  delay(750);
}  // end of setup

void loop ()
{
static long rotaryCount = 0;
static int posit=9;

static int targetx=9;
static int targety=0;
static int points=0;

static int blastlast=0;
static int blastcntr=0;
static int blastx=22;
static int blasty=5;
static int hit=0;
 
///rows cntr - target position
static int timecntr =0;

if(timecntr>7)
  {
    timecntr=0;

    targety++;
    if (targety>3) {
            hit=0;
            targety=0;
            targetx=random(2,18);
          }
  }


  if (fired)   //  trig interrupt
    {
    if (up)
    {  rotaryCount++;
      posit++;}
    else
    {  rotaryCount--;
      posit--;}
    fired = false;
        
    // Serial.print ("Count = ");  
    // Serial.println (rotaryCount);
    Serial.print ("pos ");  
    Serial.println (posit);
    }  // end if fired

if (posit<0) posit=0;
if (posit>19) posit=19;

  lcd.clear();

/// cele
lcd.setCursor(targetx, targety);
if (targetx==blastx and targety==blasty and hit==0 and blastlast==1)
    {
    Serial.println (" HIT ");
    lcd.write(4);
    if (hit==0 ) points++;
    blastlast=0;
    blastcntr=0;
    hit=1;
    }
    else
    {
      if (hit==1)
          {
            lcd.write(4);
          }
            else
          {
            lcd.print("O");
          }   
    }


  ////  pojazd
 lcd.setCursor(posit, 2);
  lcd.write(8);

////   blaster init
if (blasted)
    {
      blastlast=1;
      blastx=posit;
      blasty=1;
      blastcntr=0;
      blasted=false;
    }

/// blaster ray 
if (blastlast==1)
      { 
      lcd.setCursor(blastx, blasty);
      if ( (blastcntr % 2) ==0)
          {
           lcd.write(2);
          }
          else
          {
            lcd.write(3);
            blasty--;
          }
        
      blastcntr++;
      }

if (blasty<0)
    {
    blastlast=0;
    blastcntr=0;
    }


/// points for raming and hit

if (timecntr==1)
{
    if (targetx==posit && targety==2)   /// raming
      {
      if (hit==0) points++;
      hit=1;
      Serial.println ("RAM");
      }
      
}



 lcd.setCursor(0, 0);
 lcd.print(points);

timecntr++;
  
delay(70);

}  // end of loop

