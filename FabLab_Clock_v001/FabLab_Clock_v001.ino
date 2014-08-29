/*
  Clock
  Author: Alberto Tam Yong
  Date: 08-28-14
  
  Credit to: SmartMatrix Features Demo - Louis Beaudoin (Pixelmatix)
  This example code is released into the public domain
 */

#include <SmartMatrix_32x32.h>
#include <i2c_t3.h>

SmartMatrix matrix;

#define DS1307_ADDRESS 0x68
byte zero = 0x00;

int second,minute,hour,weekDay,monthDay,month,year;

const int defaultBrightness = 100*(255/100);    // dim: 50% brightness
const rgb24 defaultBackgroundColor = {0, 0, 0};
const int defaultScrollOffset = 6;

void setup() {
  Wire.begin();
  Serial.begin(38400);
  
  matrix.begin();
  matrix.setBrightness(defaultBrightness);
  matrix.setScrollOffsetFromEdge(defaultScrollOffset);
  matrix.setColorCorrection(cc24);
  delay(100);
  
  matrix.fillScreen(defaultBackgroundColor); //clear screen
  matrix.swapBuffers(true);
  delay(100);
  
  updateTime();
  delay(10);
}

void loop() {
  int i=0, j;
  if(millis()/30000 > 1)
  {
    updateTime();
    for(i = 5; i < 30; i++)
    {
      for( j = 13; j < 21; j++)
      {
        matrix.drawPixel(i,j,defaultBackgroundColor);
      }
    }
  }
  
  char hour_string[2],minute_string[2];
  
  itoa(hour,hour_string,10);
  itoa(minute,minute_string,10);
  
  matrix.setFont(font5x7);
  
  if(hour < 10)
  {
    matrix.drawString(5,13,{0xff,0xff,0xff}," ");
    matrix.drawString(10,13,{0xff,0xff,0xff},hour_string);
  }
  else
  {
    matrix.drawString(5,13,{0xff,0xff,0xff},hour_string);
  }
    
  matrix.drawString(15,13,{0xff,0xff,0xff},":");
  
  if(minute < 10)
  {
    matrix.drawString(20,13,{0xff,0xff,0xff},"0");
    matrix.drawString(25,13,{0xff,0xff,0xff},minute_string);
  }
  else
  {
    matrix.drawString(20,13,{0xff,0xff,0xff},minute_string);
  }
  
  Serial.print("hour: ");
  Serial.print(hour_string);
  Serial.print("\t");
  Serial.print("minute: ");
  Serial.print(minute_string);
  Serial.print("\n");
  
  for(i = 0; i < 24; i++)
  {
    pixelCircle(15,24,{0,0xff,0},i); //green
    matrix.swapBuffers(true);
    delay(1000/24);
  }
  for(i = 0; i < 24; i++)
  {
    pixelCircle(15,24,{0,0,0xff},i); //blue
    matrix.swapBuffers(true);
    delay(1000/24);
  }
}

void pixelCircle(int radius,int parts,rgb24 color,int pos)
{
  //0 degrees = top of the circle
  float x,y,angle;
  angle = pos * (2*PI/parts);
  y = 16.0-(radius * cos(angle));
  x = 16.0+(radius * sin(angle));
  /*
  Serial.print("angle: ");
  Serial.print(angle);
  Serial.print("\t");
  Serial.print("cos: ");
  Serial.print(cos(int(angle)));
  Serial.print("\t");
  Serial.print("sin: ");
  Serial.print(sin(int(angle)));
  Serial.print("\t");
  Serial.print("x: ");
  Serial.print(x);
  Serial.print("\t");
  Serial.print("y: ");
  Serial.print(y);
  Serial.println();
  */
  x += 0.5;
  y += 0.5;
  matrix.drawPixel(int(x),int(y),color);
}

byte bcdToDec(byte val)  {
// Convert binary coded decimal to normal decimal numbers
  return ( (val/16*10) + (val%16) );
}

void updateTime()
{
  // Reset the register pointer
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_ADDRESS, 7);

  second = bcdToDec(Wire.read());
  minute = bcdToDec(Wire.read());
  hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
  weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
  monthDay = bcdToDec(Wire.read());
  month = bcdToDec(Wire.read());
  year = bcdToDec(Wire.read());
}

