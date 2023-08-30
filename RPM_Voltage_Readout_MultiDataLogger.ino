#include <Wire.h>
#include <Adafruit_SH110X.h>

#include <SPI.h>
#include <SD.h>

// Define the screen dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Initialize the display
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

const int dataSelectIncPin = 7;
const int dataSelectDecPin = 6;

int dataSelect = 1;   

const int chipSelect = 4;

const int tickPin = 2;  

float rotationSegment = 36; 

float time = 0;
float timeSinceLastTick = 0;

float RPM;
float torqueVoltage;
float scaledTorqueVoltage;

bool buttonDebounce(int buttonPin); // Function prototype

void setup() {
  // put your setup code here, to run once:

  pinMode(tickPin, INPUT);
  pinMode(dataSelectIncPin, INPUT);
  pinMode(dataSelectIncPin, INPUT);

  attachInterrupt(digitalPinToInterrupt(2), ISR_tick, RISING);

  Serial.begin(9600);

  String dataSelectString; 

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  /*Wire.begin();

  if (!display.begin()) {
    Serial.println(F("Display initialization failed!"));
    while (1);
  }

  display.display(); // Show splash screen
  delay(2000); // Pause for 2 seconds
  display.clearDisplay(); // Clear the buffer
  */
    Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
   /* 
    File dataFile = SD.open("datalog.txt", FILE_WRITE);
    dataFile.println("--------");
    dataFile.println("NEW TEST:");
    dataFile.println("--------");
    dataFile.close();
    */
}

void loop() {
  // put your main code here, to run repeatedly:
torqueVoltage = (analogRead(A0)*5.0)/1024.0;
scaledTorqueVoltage = torqueVoltage*10;

  // check data select

if(buttonDebounce(dataSelectIncPin) == HIGH){

  dataSelect = dataSelect + 1;

  while(buttonDebounce(dataSelectIncPin) == HIGH)
  {
    delay(1);
  }

}
else if(buttonDebounce(dataSelectDecPin) == HIGH){

  dataSelect = dataSelect - 1;

    while(buttonDebounce(dataSelectDecPin) == HIGH)
  {
    delay(1);
  }

}
  

  // update display output

/*
display.clearDisplay();

display.setTextSize(1); // Set text size
display.setTextColor(SH110X_WHITE); // Set text color

display.setCursor(0, 10); // Set cursor position

display.print(F("RPM:"));
display.print(RPM);

display.setCursor(0, 20); // Set cursor position

display.print(F("V:"));
display.print(torqueVoltage);

display.setCursor(0, 40); // Set cursor position

display.print(F("Test "));
display.print(dataSelect);

display.display();
*/
}

void ISR_tick() {

  if((micros()-timeSinceLastTick) > 2048){
    time = micros() - timeSinceLastTick;
    RPM = 60000000.0 / (time*rotationSegment);


    /*Serial.print(100);
    Serial.print(",");
    Serial.print(RPM);
    Serial.print(",");
    Serial.print(0);

    Serial.print(100);
    Serial.print(",");
    Serial.print(scaledTorqueVoltage);
    Serial.print(",");
    Serial.println(0);*/

    timeSinceLastTick = micros();

  String dataSelectString = "Test_";

  dataSelectString.concat(dataSelect);
  dataSelectString.concat(".txt");

    File dataFile = SD.open(dataSelectString, FILE_WRITE);

    dataFile.print(RPM);
    dataFile.print(",");
    dataFile.print(torqueVoltage);
    dataFile.print(",");
    dataFile.println(millis());
    dataFile.close();

  }

}

bool buttonDebounce(int buttonPin) {

  if (digitalRead(buttonPin) == HIGH){
    delay(1);
    if(digitalRead(buttonPin) == HIGH){
      return HIGH; 
    }
    else{
      return LOW;
    }
  }
  else {
    return LOW;
  }

}


