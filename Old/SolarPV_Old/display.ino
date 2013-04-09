//GLCD fonts - part of GLCD lib
#include "utility/font_helvB14.h" 	//big numberical digits 
#include "utility/font_clR6x8.h" 	//title
#include "utility/font_clR4x6.h" 	//kwh

//Icons
static uint8_t icon_heating_8x16 [5][16] PROGMEM = {
{
0x00, 0xFE, 0x03, 0x55, 0x55, 0x03, 0xFE, 0x00, 0x1E, 0x63, 0x5C, 0xDD, 0xDD, 0x5C, 0x63, 0x1E
},{
0x00, 0xFE, 0x03, 0x55, 0x55, 0x03, 0xFE, 0x00, 0x1E, 0x63, 0x5C, 0xDF, 0xDF, 0x5C, 0x63, 0x1E
},{
0x00, 0xFE, 0x03, 0xD5, 0xD5, 0x03, 0xFE, 0x00, 0x1E, 0x63, 0x5C, 0xDF, 0xDF, 0x5C, 0x63, 0x1E
},{
0x00, 0xFE, 0x03, 0xF5, 0xF5, 0x03, 0xFE, 0x00, 0x1E, 0x63, 0x5C, 0xDF, 0xDF, 0x5C, 0x63, 0x1E
},{
0x0E, 0xF3, 0x01, 0xFD, 0xFD, 0x01, 0xF3, 0x0E, 0x1E, 0x63, 0x5C, 0xDF, 0xDF, 0x5C, 0x63, 0x1E
}
};

static uint8_t icon_home_13x12 [] PROGMEM = {
0x60, 0x30, 0xDF, 0xEF, 0xF6, 0xFB, 0xFD, 0xFB, 0xF6, 0xEC, 0xD8, 0x30, 0x60, 0x00, 0x00, 0x00,
0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00
};

static uint8_t icon_lines_12x12 [] PROGMEM = {
0x6C, 0x24, 0x36, 0xFE, 0x25, 0x25, 0xFE, 0x36, 0x24, 0x6C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x0C, 0x06, 0x05, 0x06, 0x06, 0x05, 0x06, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static uint8_t icon_solar_12x12 [5] [32] PROGMEM = {
{
0xC0, 0x60, 0x30, 0x18, 0x08, 0x0C, 0x92, 0xA2, 0xE2, 0xC4, 0x58, 0x60, 0x00, 0x00, 0x00, 0x00,
0x01, 0x03, 0x02, 0x02, 0x02, 0x07, 0x0D, 0x08, 0x08, 0x08, 0x0D, 0x06, 0x00, 0x00, 0x00, 0x00
},{
0x00, 0x20, 0x04, 0x60, 0x90, 0x08, 0x0A, 0x90, 0x60, 0x04, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x02, 0x00, 0x00, 0x01, 0x05, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
},{
0x20, 0x20, 0x04, 0x68, 0x90, 0x08, 0x0B, 0x90, 0x68, 0x04, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x02, 0x01, 0x00, 0x01, 0x0D, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
},{
0x60, 0x60, 0x0C, 0x6C, 0x90, 0x0B, 0x0B, 0x90, 0x6C, 0x0C, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x03, 0x03, 0x00, 0x0D, 0x0D, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
},{
0x60, 0x66, 0x0E, 0x6C, 0x90, 0x0B, 0x0B, 0x90, 0x6C, 0x0E, 0x66, 0x60, 0x00, 0x00, 0x00, 0x00,
0x00, 0x06, 0x07, 0x03, 0x00, 0x0D, 0x0D, 0x00, 0x03, 0x07, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00
}
};

double cval, cval2, cval3, cval4;   //values to calulate smoothing
byte imageindex;

void draw_main_screen()
{
  glcd.clear();
  glcd.fillRect(0,0,128,64,0);
  
  glcd.drawLine(64, 0, 64, 64, WHITE);      //top vertical line
  glcd.drawLine(0, 32, 128, 32, WHITE);     //middle horizontal line 

  //variables to store conversion
  char str[50];
  char str2[5];

  // Last seen information from EmonTX
  if ((millis()-last_emontx)>20000)
  {
    // small font
    glcd.setFont(font_clR4x6);
    itoa((millis()-last_emontx)/1000, str, 10);
    strcat(str,"s TX!");
    glcd.drawString(3,0,str);
  }
  if ((millis()-last_emonbase)>20000)
  {
    // small font
    glcd.setFont(font_clR4x6);
    itoa((millis()-last_emonbase)/1000, str, 10);
    strcat(str,"s base!");
    glcd.drawString(67,34,str);
  }

  // medium font
  glcd.setFont(font_clR6x8);

  // Indicator for consuming or generating   
  glcd.drawBitmap(51,34,icon_lines_12x12,16,12,1);
  if (importing==1) glcd.drawString_P(35,34,PSTR("<<")); else glcd.drawString_P(35,34,PSTR(">>"));

  //big bold font
  glcd.setFont(font_helvB14);

  // Amount of power currently being used
  cval = cval + (consuming - cval)*0.50;
  if(cval>1000)
  {
    dtostrf(cval/1000,2,1,str);
    strcat(str,"kw");   
  }
  else
  {
    itoa((int)cval,str,10);
    strcat(str,"w");   
  }  
  glcd.drawBitmap(49,0,icon_home_13x12,16,12,1);
  glcd.drawString(3,9,str);

  // Amount of energy being generated
  cval2 = cval2 + (gen - cval2)*0.5;
  if(cval2>1000)
  {
    dtostrf(cval2/1000,2,1,str);
    strcat(str,"kw");   
  }
  else
  {
    itoa((int)cval2,str,10);
    strcat(str,"w");   
  }
  if (cval2>maxgen) maxgen=cval2;
  imageindex=int(cval2/maxgen*5-0.5);
  glcd.drawBitmap(115,0,icon_solar_12x12[imageindex],16,12,1);
  glcd.drawString(71,9,str);

  // Amount of energy coming from or going into the grid
  cval3 = cval3 + (grid - cval3)*0.5;
  if(cval3>1000)
  {
    dtostrf(cval3/1000,2,1,str);
    strcat(str,"kw");   
  }
  else
  {
    itoa((int)cval3,str,10);
    strcat(str,"w");   
  }
  glcd.drawString(3,42,str);

  // Temperature               
  dtostrf(temp,0,1,str); 
  strcat(str,"c");
  if (temp>MAXTEMP)
  {
    imageindex=5;
  }
  else
  {
    if (temp<MINTEMP) imageindex=0; else imageindex=int((temp-MINTEMP)/(MAXTEMP-MINTEMP)*5-0.5);
  }
  glcd.drawBitmap(120,40,icon_heating_8x16[imageindex],8,16,1);
  glcd.drawString(70,42,str);   

  glcd.setFont(font_clR4x6);   		//small font - Kwh

  // Kwh consumed today
  dtostrf((wh_consuming[0]/1000),0,1,str);
  strcat(str,"Kwh today");  
  glcd.drawString(6,26,str);

  // Kwh generated today
  dtostrf((wh_gen[0]/1000),0,1,str); 
  strcat(str,"Kwh today");  
  glcd.drawString(71,26,str);

  // Minimum and maximum temperatures
  itoa((int)mintemp,str,10);
  strcat(str,"c");
  glcd.drawString_P(68,58,PSTR("min"));
  glcd.drawString(82,58,str);
  itoa((int)maxtemp,str,10); 
  strcat(str,"c");
  glcd.drawString_P(97,58,PSTR("max"));
  glcd.drawString(111,58,str);

  // Current time
  DateTime now = RTC.now();
  glcd.drawString_P(5,58,PSTR("Time:"));
  itoa((int)now.hour(),str,10);
  if  (now.minute()<10) strcat(str,":0"); else strcat(str,":"); 
  itoa((int)now.minute(),str2,10);
  strcat(str,str2); 
  glcd.drawString(28,58,str); 

  glcd.refresh();
}

void draw_clock()
{
  glcd.clear;
  glcd.fillRect(0,0,128,64,0);

  glcd.setFont(font_clR6x8);
  glcd.drawString_P(2,0,PSTR("Current Time:"));

  DateTime now = RTC.now();
  char str[20];
  char str2[5];
  itoa((int)now.hour(),str,10);
  if  (now.minute()<10) strcat(str,":0"); else strcat(str,":");
  itoa((int)now.minute(),str2,10);
  strcat(str,str2); 

  glcd.setFont(font_helvB14); //big bold font
  glcd.drawString(2,10,str);

  glcd.refresh();
  
  delay(2000); 
}

void draw_history()
{
  glcd.clear;
  glcd.fillRect(0,0,128,64,0);
  
  char str[50]; 
  
  glcd.setFont(font_clR6x8);
  glcd.drawString_P(40,0,PSTR("History"));
  
  glcd.setFont(font_clR4x6);   	

  glcd.drawString_P(2,16,PSTR("Today"));
  glcd.drawString_P(2,23,PSTR("Yesterday"));
  glcd.drawString_P(2,30,PSTR("2 days ago"));
  glcd.drawString_P(2,37,PSTR("3 days ago"));
  glcd.drawString_P(2,44,PSTR("4 days ago"));
  glcd.drawString_P(2,51,PSTR("5 days ago"));
  glcd.drawString_P(2,58,PSTR("6 days ago"));
  
  // draw grid consumption history
  glcd.drawString_P(50,9,PSTR("Power"));

  dtostrf((wh_consuming[0]/1000),0,1,str);
  glcd.drawString(52,16,str);
  
  dtostrf((wh_consuming[1]/1000),0,1,str);
  glcd.drawString(52,23,str);
  
  dtostrf((wh_consuming[2]/1000),0,1,str);
  glcd.drawString(52,30,str);
  
  dtostrf((wh_consuming[3]/1000),0,1,str);
  glcd.drawString(52,37,str);
  
  dtostrf((wh_consuming[4]/1000),0,1,str);
  glcd.drawString(52,44,str);
  
  dtostrf((wh_consuming[5]/1000),0,1,str);
  glcd.drawString(52,51,str);
  
  dtostrf((wh_consuming[6]/1000),0,1,str);
  glcd.drawString(52,58,str);
  
  
  // draw solar PV generation history
  glcd.drawString_P(78,9,PSTR("PV"));

  dtostrf((wh_gen[0]/1000),0,1,str);
  glcd.drawString(76,16,str);
  
  dtostrf((wh_gen[1]/1000),0,1,str);
  glcd.drawString(76,23,str);
  
  dtostrf((wh_gen[2]/1000),0,1,str);
  glcd.drawString(76,30,str);
  
  dtostrf((wh_gen[3]/1000),0,1,str);
  glcd.drawString(76,37,str);
  
  dtostrf((wh_gen[4]/1000),0,1,str);
  glcd.drawString(76,44,str);
  
  dtostrf((wh_gen[5]/1000),0,1,str);
  glcd.drawString(76,51,str);
  
  dtostrf((wh_gen[6]/1000),0,1,str);
  glcd.drawString(76,58,str);
  
  
  
  glcd.refresh();
  //delay(20000);    //20 sec delay
}

void backlight_control()
{
   int LDR = analogRead(LDRpin);                     // Read the LDR Value so we can work out the light level in the room.
                                                     // GLCD settings
   int LDRbacklight = map(LDR, 0, 1023, 50, 250);    // Map the data from the LDR from 0-1023 (Max seen 1000) to var GLCDbrightness min/max
   LDRbacklight = constrain(LDRbacklight, 0, 255);   // Constrain the value to make sure its a PWM value 0-255

  //-----------------------------------------------------------------------------
  // Turn off backlight and indicator LED's between 11pm and 6am or manual switch
  //-------------------------------------------------------------------- --------
  DateTime now = RTC.now();
  if (now.hour()>0) int hour = now.hour();                    //get hour digit in 24hr from software RTC
 
  if ((hour > 22) ||  (hour < 5) && ((millis()-last_emonbase)<20000)){       // turn off backlight between 11pm and 6am
    night=1; 
    glcd.backLight(0);
  } else {
    night=0; 
    if (backlightbr == 1) {
      glcd.backLight(LDRbacklight);
    } else {
      glcd.backLight(0);
    }
  }
}

//--------------------------------------------------------------------
//Change color of LED on top of emonGLCD, red if consumption exceeds gen or green if gen is greater than consumption 
//-------------------------------------------------------------------- 
void led_control()
{
   int PWRleds= map(cval3, 0, 4000, 0, 255);     // Map importing value from 0-4Kw (LED brightness - cval3 is the smoothed grid value - see display above 
   PWRleds = constrain(PWRleds, 0, 255);             // Constrain the value to make sure its a PWM value 0-255
   
    if ((gen>0) && (night==0)) {
      if (cval2 > cval) {                //show green LED when gen>consumption cval are the smooth curve values  
        analogWrite(redLED, 0);         
        analogWrite(greenLED, PWRleds);    
      } else {                              //red if consumption>gen
        analogWrite(greenLED, 0); 
        analogWrite(redLED, PWRleds);   
      }
    } else {                                 //Led's off at night and when solar PV is not generating
      analogWrite(redLED, 0);
      analogWrite(greenLED, 0);
    }
}
