// MODULES
#include <LiquidCrystal.h>
#include <math.h>

// Initaite Pins
int RelayOut = 6; // Relay
int green = 9; // colour LED
int amber = 10; // colour LED
int red = 8; // colour LED
int sensor_pin = A0; // Moisture sensor
int output_value; // Moisture sensor
int water_pin = A1; // Water level sensor
int PUMPS = 0; // amount of pumps

// Create a custom value for the LCD screen
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
uint8_t testChar[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}; // Custom character

// This will be use for the loading bar
int lodval = 0;


// Setup Function
void setup(){
  Serial.begin(9600);
  digitalWrite(green,HIGH);
  digitalWrite(amber, HIGH);
  digitalWrite(red, HIGH);
  Serial.println("starting");
  pinMode(RelayOut, OUTPUT); // Relay
  lcd.begin(16, 2); // Custom character
  lcd.createChar(0, testChar); // Sends the custom char to lcd
  // Following is all just Printing to LCD
  lcd.setCursor(0, 0);
  lcd.print("Starting");
  lcd.setCursor(7, 1);
  lcd.print("Program");
  delay(5000);
  lcd.setCursor(0,0);
  lcd.print("Moisture:");
  lcd.setCursor(15,0);
  lcd.print("%");
  lcd.setCursor(7, 1);
  lcd.print("       ");
  // Prints End

  digitalWrite(green,LOW);
  digitalWrite(amber, LOW);
  digitalWrite(red, LOW);
  
}

// Reads the moisture level
int moist_read(){
  output_value = analogRead(sensor_pin);
  output_value = map(output_value,550,0,0,100);
  int moist = (100 - output_value);
  if (moist > 100){
    lodval = 0;
    return 100;
  } else if (moist < 35){
    lodval = lodval + 1;
    return 0;
  } else {
    lodval = 0;
    return moist;
  }
}

// turns on the Pump
void pump_on(){
  PUMPS = PUMPS + 1;
  digitalWrite(green,HIGH);
  digitalWrite(amber, HIGH);
  digitalWrite(red, HIGH);
  digitalWrite(RelayOut, HIGH);
  delay(45000);
  digitalWrite(RelayOut, LOW);
  digitalWrite(green,LOW);
  digitalWrite(amber, LOW);
  digitalWrite(red, LOW);
  lodval = -1;
}

// Loading Bar function
int loading(){
  int tfbool;
  
  if(lodval == 0){
    lcd.setCursor(0,1);
    lcd.print("                ");
    tfbool =  false;
  } else if(lodval == 16){
    lcd.setCursor(lodval - 1, 1);
    lodval = 1;
    lcd.print(char(0));
    tfbool = true;
  } else{
    lcd.setCursor(lodval - 1, 1);
    lcd.print(char(0));
    tfbool =  false;
  }
  delay(1000);
  return tfbool;
}


// LCD printing function
int LCDprint(){
  lcd.setCursor(11,0);
  lcd.print("   ");
  lcd.setCursor(11,0);
  int mst_val = moist_read();
  lcd.print(mst_val);
  int bol = loading();
  return bol;
}

// Water level display & Test
void reservoir(){
  int wtrlvl = analogRead(water_pin);
  Serial.println(wtrlvl);
  if (wtrlvl<=35){ 
    Serial.println("red");
    digitalWrite(red, HIGH);
    digitalWrite(green,LOW);
    digitalWrite(amber, LOW); 
  }
  else if (wtrlvl>35 && wtrlvl<=50){ 
    Serial.println("amber");
    digitalWrite(amber, HIGH);
    digitalWrite(green,LOW);
    digitalWrite(red, LOW);
  }
  else if (wtrlvl>50){ 
    Serial.println("green");
    digitalWrite(green, HIGH);
    digitalWrite(red,LOW);
    digitalWrite(amber, LOW);
  }
  
}

// Loops through functions
void loop() {
  Serial.println(analogRead(water_pin));
  int bl = LCDprint();
  reservoir();
  if(bl){
    pump_on();
    delay(500);
  } 
}
