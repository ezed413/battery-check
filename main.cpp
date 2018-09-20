/*--------------------------------------------------------------
  Program:      battery_check

  Description:  Reads analog value on pin A3 that is connected to a voltage divider to
                read the voltage of the battery. A fully charged battery is about 12.5 Vdc.
                Displays the battery charge in percentage on a small 128 x 64 oled display when the battery
                is connected.


  Hardware:     Arduino Nano

  Software:     Developed using MS Visual Studio Code with the Platformio extention


  Date:         18 September 2018
  Author:       Eddie L Hill
  --------------------------------------------------------------*/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


#define NUM_SAMPLES 10                // number of samples taken of the battery voltage measurement
#define VoltageDivider1 11.00         // calibration factor for RX unit
#define VoltageDivider2 10.34         // calibration factor for TX unit
#define timeDelay 1000                // delay in ms to when the voltage is measured when load is applied, 4 sec
#define switchPin 6                   // mode switch input pin
#define RX 0                          // RX mode
#define TX 1                          // TX mode


int sum = 0;                          // sum of samples taken
int firstMeas = 0;                    // first measurement from the A/D
unsigned char sample_count = 0;       // current sample number
float voltage = 0.0;                  // calculated voltage
float measvolts = 0.0;                // measured voltage
int testStatus = 0;                   // boolean variable for pass/fail status
int buttonState = 0;                  // variable for reading the state of the start push button switch
int x;                                // measurement loop index
float adres = 0.0049;                 // A/D resolution
float voltPercentage;                 // the voltage in percentage
int switchVal = 0;                    // switch value
bool mmode;                           // mode place holder

void setup()
{
  // initialize serial port.
  // serial port used for displaying the voltage
  // and other things for debugging purposes
  Serial.begin(9600);
  // initialize display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();
  display.display();
  pinMode(switchPin, INPUT_PULLUP);           // input for toggle switch input
}
/************************************* start of program **********************************************************/

void loop()
{

  /************************************ start of main loop ********************************************************/
  // read the mode switch
  switchVal = digitalRead(switchPin);
  if (switchVal)
    mmode = TX;
  else
    mmode = RX;

  voltage = 0;          // reset voltage variable

  // take a number of analog samples and add them up and average
  firstMeas = analogRead(A3);         // throw first measurement out per Atmel data sheet
  while (sample_count < NUM_SAMPLES) {
    sum += analogRead(A3);
    sample_count++;
    delay(10);
  }

  // calculate the voltage
  measvolts = ((float)sum / (float)(NUM_SAMPLES) * 5.024) / 1024.0;

  // set the mode according to the mode switch position
  if (mmode)
    voltage = measvolts * VoltageDivider1;
  else
    voltage = measvolts * VoltageDivider2;

  // calculate the percentage of voltage
  voltPercentage = voltage / 12.5;  // 12.5 vdc is a fully charged battery
  voltPercentage = voltPercentage * 100;

  // text display
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // display the voltage measured in percentage
  display.println("Battery");
  display.print("level:");
  display.print((int)voltPercentage, DEC);
  display.print("%");
  display.display();
  // wait here
  while (1);
}

/***************************************** end of main loop ***************************************************/



/***************************************** end of program *****************************************************/
