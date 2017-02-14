#include "U8glib.h"

int buttonPin4 = 5;
int buttonPin3 = 4;
int buttonPin2 = 3;
int buttonPin1 = 2;

int bluePin = 10;
int redPin = 9;


U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI 
int state = 0;          //1 state for each screen, 0-menu, 1-select red, 2-select blue, 3-timing, 4-runstate
int menu_button = 1;
int red_button = 1;
int blue_button = 1;
int time_button = 1;

int red_value = 0;
int blue_value = 0;
int red_valuem = 0;
int blue_valuem = 0;


void logo(){
  u8g.setFont(u8g_font_helvB24);
  u8g.drawStr( 40, 40, "3M");
  u8g.setFont(u8g_font_helvR08);
  u8g.drawStr(24, 60,"HPL Plant Lighting");
};

void menu() {
  u8g.setFont(u8g_font_helvR08);
  u8g.drawStr( 15, 16, "Select Red PPFD");
  u8g.drawStr( 15, 32, "Select Blue PPFD");
   u8g.drawStr( 15, 48, "Select Running Time");
   u8g.drawStr( 15, 64, "<Run>");

   if (menu_button==1){
   u8g.drawStr( 0, 16, "x");
   };
   if (menu_button==2){
   u8g.drawStr( 0, 32, "x");
   };
      if (menu_button==3){
   u8g.drawStr( 0, 48, "x");
   };
      if (menu_button==4){
   u8g.drawStr( 0, 64, "x");
   };
};

void red() {
    u8g.setFont(u8g_font_helvR08);
    u8g.drawStr( 0, 10, "Selecting Red levels:");
    u8g.setFont(u8g_font_helvR08);
    u8g.setPrintPos(40,40);
    u8g.print(red_value);
};

void blue() {
    u8g.setFont(u8g_font_helvR08);
    u8g.drawStr( 0, 10, "Selecting Blue levels:");
    u8g.setFont(u8g_font_helvR08);
    u8g.setPrintPos(40,40);
    u8g.print(blue_value);
};

void setup(void) {
  pinMode(buttonPin1,INPUT);
  pinMode(buttonPin2,INPUT);
  pinMode(buttonPin3,INPUT);
  pinMode(buttonPin4,INPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(redPin, OUTPUT);
  analogWrite(redPin,255);
  analogWrite(bluePin,255);
  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
  u8g.firstPage();  
  do {
    logo();
  } while( u8g.nextPage() );
  delay(2000);

}

void loop(void) {
  // picture loop
  
  if (state==0){                          //start menu                      
   u8g.firstPage();  
    do {
    menu();
    } while( u8g.nextPage() );
  };

  if (state==1){                          //start menu                      
   u8g.firstPage();  
    do {
    red();
    } while( u8g.nextPage() );
  };

  if (state==2){                          //start menu                      
   u8g.firstPage();  
    do {
    blue();
    } while( u8g.nextPage() );
  };

////////////////////////////////////////////////////select red

  if (state==0 && menu_button==1 && digitalRead(buttonPin3)==HIGH){       //go into selecting red ppfd
    state = 1;
  };

  if (state==1 && digitalRead(buttonPin1)==HIGH){
    red_value+=1;
     if (red_value>255){
      red_value=0;
     }
  };

  if (state==1 && digitalRead(buttonPin2)==HIGH){
    red_value-=1;
     if (red_value<0){
      red_value=255;
     }
  };

  if (state==1 && digitalRead(buttonPin4)==HIGH){
    state=0;
  }
/////////////////////////////////////////////////////////select blue

  if (state==0 && menu_button==2 && digitalRead(buttonPin3)==HIGH){       //go into selecting red ppfd
    state = 2;
  };

  if (state==2 && digitalRead(buttonPin1)==HIGH){
    blue_value+=1;
     if (blue_value>255){
      blue_value=0;
     }
  };

  if (state==2 && digitalRead(buttonPin2)==HIGH){
    blue_value-=1;
     if (blue_value<0){
      blue_value=255;
     }
  };

  if (state==2 && digitalRead(buttonPin4)==HIGH){
    state=0;
  }

 
///////////////////////////////////////////////////////////menu
  if (state==0 && digitalRead(buttonPin1)==HIGH)
  {
    menu_button+=1;
    if (menu_button>4){
      menu_button=1;
    }
  }
  
    if (state==0 && digitalRead(buttonPin2)==HIGH)
  {
    menu_button-=1;
    if (menu_button<1){
      menu_button=4;
    }
  }
  blue_valuem = map(blue_value, 0, 255, 255, 0);
  red_valuem = map(red_value, 0,255,255,0);
  
  analogWrite(bluePin,blue_valuem);
  analogWrite(redPin,red_valuem);

  
    
}

