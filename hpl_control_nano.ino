#include "U8glib.h"
#include <Wire.h>
#include <RTClib.h>

RTC_DS1307 RTC;

int buttonPin4 = 5;
int buttonPin3 = 4;
int buttonPin2 = 3;
int buttonPin1 = 2;

int bluePin = 10;
int redPin = 9;


U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI 
int state = 0;          //1 state for each screen, 0-menu, 1-select levels, 2-timing, 3-runstate
int menu_button = 1;
int select_button = 1;
int time_button = 1;
int stop_button = 1;
unsigned long select_delay;
int run_state = 0;
int cycle_state = 0 ;

int maxday=0;
int red_value = 0;
int blue_value = 0;
int red_valuem = 0;
int blue_valuem = 0;
float houron = 0;
float houroff = 0;
int startyear = 0;
int startmonth = 11;
int startday = 23;
int starthour = 0;
int startmin = 0;


String rtcdate;
String rtctime;

#define u8g_logo_width 60
#define u8g_logo_height 32
static unsigned char u8g_logo_bits[] U8G_PROGMEM = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x07, 0xfc,
   0x3f, 0xc0, 0xff, 0x07, 0x80, 0xff, 0x3f, 0xfc, 0x3f, 0xc0, 0xff, 0x07,
   0xe0, 0xff, 0x7f, 0xfc, 0x7f, 0xc0, 0xff, 0x07, 0xf0, 0xff, 0xff, 0xfc,
   0x7f, 0xc0, 0xff, 0x07, 0xf0, 0xff, 0xff, 0xfd, 0x7f, 0xe0, 0xff, 0x07,
   0xf8, 0xff, 0xff, 0xfd, 0x7f, 0xe0, 0xff, 0x07, 0xf8, 0x1f, 0xff, 0xff,
   0xff, 0xe0, 0xff, 0x07, 0xf8, 0x07, 0xfc, 0xff, 0xff, 0xe0, 0xff, 0x07,
   0xfc, 0x07, 0xfc, 0xff, 0xff, 0xf0, 0xff, 0x07, 0xfc, 0x03, 0xfc, 0xff,
   0xff, 0xf0, 0xff, 0x07, 0x00, 0x00, 0xfe, 0xff, 0xff, 0xf1, 0xff, 0x07,
   0x00, 0xf8, 0xff, 0xfd, 0xff, 0xf1, 0xff, 0x07, 0x00, 0xf8, 0xff, 0xfd,
   0xfb, 0xf1, 0xff, 0x07, 0x00, 0xf8, 0x7f, 0xfc, 0xfb, 0xf9, 0xff, 0x07,
   0x00, 0xf8, 0xff, 0xfc, 0xfb, 0xfb, 0xfd, 0x07, 0x00, 0xf8, 0xff, 0xfd,
   0xfb, 0xfb, 0xfd, 0x07, 0x00, 0xf8, 0xff, 0xff, 0xfb, 0xfb, 0xfd, 0x07,
   0x00, 0x00, 0xff, 0xff, 0xf3, 0xff, 0xfd, 0x07, 0xfe, 0x03, 0xfe, 0xff,
   0xf3, 0xff, 0xfd, 0x07, 0xfe, 0x03, 0xfe, 0xff, 0xf3, 0xff, 0xfc, 0x07,
   0xfe, 0x03, 0xfe, 0xff, 0xf3, 0xff, 0xfc, 0x07, 0xfc, 0x03, 0xfe, 0xff,
   0xe3, 0xff, 0xfc, 0x07, 0xfc, 0x07, 0xff, 0xff, 0xe3, 0xff, 0xfc, 0x07,
   0xfc, 0xff, 0xff, 0xff, 0xe3, 0x7f, 0xfc, 0x07, 0xf8, 0xff, 0xff, 0xff,
   0xe3, 0x7f, 0xfc, 0x07, 0xf8, 0xff, 0xff, 0xfd, 0xc3, 0x7f, 0xfc, 0x07,
   0xf0, 0xff, 0xff, 0xfc, 0xc3, 0x7f, 0xfc, 0x07, 0xe0, 0xff, 0x7f, 0xfc,
   0xc3, 0x3f, 0xfc, 0x07, 0x80, 0xff, 0x1f, 0xfc, 0xc3, 0x3f, 0xfc, 0x07,
   0x00, 0xfc, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00 
 };


void logo(){
  u8g.drawXBMP( 34, 16, u8g_logo_width, u8g_logo_height, u8g_logo_bits);
  u8g.setFont(u8g_font_helvR08);
  u8g.drawStr(24, 60,"HPL Plant Lighting");
};

void menu() {
  u8g.setFont(u8g_font_helvR08);

  u8g.setPrintPos(0,8);
  u8g.print(rtcdate);
  u8g.setPrintPos(80,8);
  u8g.print(rtctime);
 
  
  
  u8g.drawStr( 15, 32, "Select Light Levels");
   u8g.drawStr( 15, 48, "Select Running Time");

   if (run_state==0){
   u8g.drawStr( 15, 64, "<Run>");
   };
   if (run_state==1 && cycle_state ==1){
   u8g.drawStr( 15, 64, "<Running> On:");
   };
   if (run_state==1 && cycle_state==0){
   u8g.drawStr( 15, 64, "<Running> Off:");
   };

   if (menu_button==1){
   u8g.drawStr( 0, 32, "x");
   };
      if (menu_button==2){
   u8g.drawStr( 0, 48, "x");
   };
      if (menu_button==3){
   u8g.drawStr( 0, 64, "x");
   };

   
};


void select() {
    u8g.setFont(u8g_font_helvR08);

  u8g.setPrintPos(0,8);
  u8g.print(rtcdate);
  u8g.setPrintPos(80,8);
  u8g.print(rtctime);
    
    u8g.drawStr( 0, 32, "Red Levels:");
    u8g.setPrintPos(25,48);
    u8g.print(red_value);
    u8g.drawStr( 64, 32, "Blue Levels:");
    u8g.setPrintPos(90,48);
    u8g.print(blue_value); 
    if (select_button==1){
      u8g.drawStr( 25, 64, "x");
    };
    if (select_button==2){
      u8g.drawStr( 90, 64, "x");
    };  
};

void timing() {
    u8g.setFont(u8g_font_helvR08);

  u8g.setPrintPos(0,8);
  u8g.print(rtcdate);
  u8g.setPrintPos(80,8);
  u8g.print(rtctime);

  u8g.drawStr(0, 28, "Hours On:");
  u8g.setPrintPos(20,38);
  u8g.print(houron);
  
  u8g.drawStr(65, 28, "Hours Off:");
  u8g.setPrintPos(85,38);
  u8g.print(houroff);
  
  u8g.drawStr(0, 50, "Start YYYYMMDD HH:MM");
  u8g.setPrintPos(27,61);
  u8g.print(startyear);
  u8g.setPrintPos(56,61);
  u8g.print(startmonth);
  u8g.setPrintPos(74,61);
  u8g.print(startday);
  u8g.setPrintPos(93,61);
  u8g.print(starthour);
  u8g.setPrintPos(112,61);
  u8g.print(startmin);
  



   if (time_button==1){
      u8g.drawStr(0, 38, "x");
   };

      if (time_button==2){
      u8g.drawStr(65, 38, "x");
   };

      if (time_button==3){
      u8g.drawStr(27, 64, ".......");
   };

   if (time_button==4){
    u8g.drawStr(56, 64, ".....");
   }

      if (time_button==5){
    u8g.drawStr(74, 64, ".....");
   }

   if (time_button==6){
    u8g.drawStr(93, 64, ".....");
   }
     if (time_button==7){
    u8g.drawStr(112, 64, ".....");
   }
  
};


void run_stop(){
  u8g.setFont(u8g_font_helvR08);
  u8g.setPrintPos(0,8);
  u8g.print(rtcdate);
  u8g.setPrintPos(80,8);
  u8g.print(rtctime);
  
  u8g.setFont(u8g_font_helvR08);
  u8g.drawStr(0, 32,"Stop Run??");
  if (stop_button==1){
    u8g.drawStr(58,50,"Yes"); 
  }
    if (stop_button==2){
    u8g.drawStr(58,50,"No"); 
  }
};

void setup(void) {
  Wire.begin();
  RTC.begin();
  select_delay = millis();
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
  Serial.begin(9600);
}

void loop(void) {
  DateTime now = RTC.now();
  rtcdate = now.year();
  rtcdate = rtcdate+'/'+now.month()+'/'+now.day();
  rtctime = now.hour();
  rtctime = rtctime+':'+now.minute()+':'+now.second();

  if (startmonth==1 || startmonth==3 || startmonth==5||startmonth==7||startmonth==8||startmonth==10||startmonth==12){
    maxday = 31;
  }
  if (startmonth==4 || startmonth==6 || startmonth==9||startmonth==11){
    maxday = 30;
  }
  if (startmonth==2){
    if (startyear%4>0){
      maxday=28;
    }
    if (startyear%4==0){
      maxday=29;
    }
  }
 
  if (state==0){                          //start menu                      
   u8g.firstPage();  
    do {
    menu();
    } while( u8g.nextPage() );
  };

  if (state==1){                          //light level selection                      
   u8g.firstPage();  
    do {
    select();
    } while( u8g.nextPage() );
  };

 if (state==2){                          //time selecting               
   u8g.firstPage();  
    do {
    timing();
    } while( u8g.nextPage() );
  };

 if (state==3){                          //time selecting               
   u8g.firstPage();  
    do {
    run_stop();
    } while( u8g.nextPage() );
  };

////////////////////////////////////////////////////select red

  if (state==0 && menu_button==1 && digitalRead(buttonPin3)==HIGH){       //go into selecting stage
    state = 1;
    select_delay=millis();
  };

  if (state==1 && select_button==1 && digitalRead(buttonPin3)==HIGH && millis()-select_delay>200){
    select_button=2;
    select_delay=millis();
  };

   if (state==1 && select_button==2 && digitalRead(buttonPin3)==HIGH && millis()-select_delay>200){
    select_button=1;
    select_delay=millis();
  };


  if (state==1 && select_button==1 && digitalRead(buttonPin2)==HIGH){
    red_value+=1;
     if (red_value>255){
      red_value=0;
     }
  };
  if (state==1 && select_button==1 && digitalRead(buttonPin1)==HIGH){
    red_value-=1;
     if (red_value<0){
      red_value=255;
     }
  };

    if (state==1 && select_button==2 && digitalRead(buttonPin2)==HIGH){
    blue_value+=1;
     if (blue_value>255){
      blue_value=0;
     }
  };
  if (state==1 && select_button==2 && digitalRead(buttonPin1)==HIGH){
    blue_value-=1;
     if (blue_value<0){
      blue_value=255;
     }
  };



  if (state==1 && digitalRead(buttonPin4)==HIGH){
    state=0;
  }

 
///////////////////////////////////////////////////////////menu
  if (state==0 && digitalRead(buttonPin1)==HIGH)
  {
    menu_button+=1;
    if (menu_button>3){
      menu_button=1;
    }
  }
  
    if (state==0 && digitalRead(buttonPin2)==HIGH)
  {
    menu_button-=1;
    if (menu_button<1){
      menu_button=3;
    }
  }


///////////////////////////////////////////////////timing
  if (state==0 && menu_button==2 && digitalRead(buttonPin3)==HIGH && millis()-select_delay>200){       //go into selecting stage
    state = 2;
    startyear = now.year();
    startmonth = now.month();
    startday = now.day();
    starthour = now.hour();
    startmin = now.minute();
    select_delay=millis();
  };
  
  
  if (state==2 && digitalRead(buttonPin4)==HIGH){
    state=0;
  }

  if (state==2 && digitalRead(buttonPin3)==HIGH&& millis()-select_delay>200){
    time_button+=1;
    select_delay=millis();
    if (time_button>7){
      time_button=1;
    }
  }

  if (state==2 && time_button==1 && digitalRead(buttonPin2)==HIGH){
    houron+=0.5;
  }
 if (state==2 && time_button==1 && digitalRead(buttonPin1)==HIGH){
    houron-=0.5;
    if (houron<0){
      houron=0;
    }
  }
  
  if (state==2 && time_button==2 && digitalRead(buttonPin2)==HIGH){
    houroff+=0.5;
  }
   if (state==2 && time_button==2 && digitalRead(buttonPin1)==HIGH){
    houroff-=0.5;
    if (houroff<0){
      houroff=0;
    }
  }

    if (state==2 && time_button==3 && digitalRead(buttonPin2)==HIGH){
    startyear+=1;
  }
   if (state==2 && time_button==3 && digitalRead(buttonPin1)==HIGH){
     startyear-=1;
    if ( startyear<2000){
       startyear=2000;
    }
  }

  if (state==2 && time_button==4 && digitalRead(buttonPin2)==HIGH){
    startmonth+=1;
    if (startmonth>12){
      startmonth=1;
    }
  }
   if (state==2 && time_button==4 && digitalRead(buttonPin1)==HIGH){
    startmonth-=1;
    if (startmonth<1){
      startmonth=12;
    }
  }

    if (state==2 && time_button==5 && digitalRead(buttonPin2)==HIGH){
    startday+=1;
        if (startday>maxday){
      startday=1;
    }
  }
   if (state==2 && time_button==5 && digitalRead(buttonPin1)==HIGH){
    startday-=1;
    if (startday<1){
      startday=maxday;
    }
  }

    if (state==2 && time_button==6 && digitalRead(buttonPin2)==HIGH){
    starthour+=1;
        if (starthour>23){
      starthour=0;
  }
    }
   if (state==2 && time_button==6 && digitalRead(buttonPin1)==HIGH){
    starthour-=1;
    if (starthour<0){
      starthour=23;
    }
  }

      if (state==2 && time_button==7 && digitalRead(buttonPin2)==HIGH){
    startmin+=1;
        if (startmin>59){
      starthour=0;
  }
      }
   if (state==2 && time_button==7 && digitalRead(buttonPin1)==HIGH){
    startmin-=1;
    if (startmin<0){
      startmin=59;
    }
  }

//////////////////// runningmode

  if (state==0 && menu_button==3 && digitalRead(buttonPin3)==HIGH && run_state ==0 && millis()-select_delay>200){
    run_state = 1;
    select_delay=millis();
  }

  if (state==0 && menu_button==3 && digitalRead(buttonPin3)==HIGH && run_state ==1 && millis()-select_delay>200){
    state == 3;
    select_delay=millis();
  }

  if (state==3 && digitalRead(buttonPin4)==HIGH){
    state = 0;
  }

  
  blue_valuem = map(blue_value, 0, 255, 255, 0);
  red_valuem = map(red_value, 0,255,255,0);
  
  analogWrite(bluePin,blue_valuem);
  analogWrite(redPin,red_valuem);
  
    
}

