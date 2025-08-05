//Libraries
#include <LiquidCrystal_I2C.h>
#include <string> 

//Define constants
const int temp_button = 27;
const int battery_button = 26;
const int button3 = 25;
const int button4 = 33;
String mode;

LiquidCrystal_I2C lcd(0x27, 20, 4);

//List of custom characters

//Battery Characters
byte bat_top_left[] = {
  B00000,
  B00000,
  B00000,
  B00011,
  B00111,
  B00110,
  B00110,
  B00110
};
byte bat_left[] = {
  B00110,
  B00110,
  B00110,
  B00110,
  B00110,
  B00110,
  B00110,
  B00110
};
byte bat_bot_left[] = {
  B00110,
  B00110,
  B00110,
  B00111,
  B00011,
  B00000,
  B00000,
  B00000
};
byte bat_side[] = {
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000
};
byte bat_top_right[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B11000,
  B11000,
  B11000,
  B11000
};
byte bat_right[] = {
  B01100,
  B01111,
  B01111,
  B01111,
  B01111,
  B01111,
  B01111,
  B01100
};
byte bat_bot_right[] = {
  B11000,
  B11000,
  B11000,
  B11000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte bat_charge[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

//Charge animation stuff
enum Charge_state {
  CHARGE_ON,
  CHARGE_OFF
};
Charge_state charge_current_state = CHARGE_ON;
unsigned long charge_last_millis = 0;


//Function
boolean dbRead(int pin, unsigned int dbDelay);

//Setup
void setup() {

  //LCD stuff
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 1);

  //Serial Monitor Stuff
  Serial.begin(115200);
  Serial.println("Sup");

  //IO Pins
  pinMode(temp_button, INPUT);
  pinMode(battery_button, INPUT);
  pinMode(button3, INPUT);
  pinMode(button4, INPUT);

}

//Loop
void loop() {
  
  //Temperature Button Input
  if (dbRead(temp_button, 50) == HIGH) {
    Serial.println("1");
    
    //Wipe screen
    for (int i = 0; i < 100; i++) {
      lcd.print(" ");
    }
    mode = "Temperature";
  }
  //Battery Button Input
  else if (dbRead(battery_button, 50) == HIGH) {
    Serial.println("2");
    for (int i = 0; i < 100; i++) {
      lcd.print(" ");
    }
    mode = "Battery";
  }
  //Speed Button Input
  else if (dbRead(button3, 50) == HIGH) {
    Serial.println("3");
    for (int i = 0; i < 100; i++) {
      lcd.print(" ");
    }
    mode = "Speed";
  }
  //Another cool thing presumably
  else if (dbRead(button4, 50) == HIGH) {
    Serial.println("4");
    for (int i = 0; i < 100; i++) {
      lcd.print(" ");
    }
    mode = "idk";
  }
  
  //Read the mode and do things based on the mode
  if (mode == "Temperature") {
    
    lcd.setCursor(4, 0);
    lcd.print("TEMPERATURE");
    lcd.setCursor(0, 1);
    lcd.print("Battery:");
    lcd.setCursor(0,  2);
    lcd.print("Engine:");
    lcd.setCursor(0, 3);
    lcd.print("Controller:");
  }
  else if (mode == "Battery") {
    //Creating the battery custom characters
    lcd.createChar(0, bat_top_left);
    lcd.createChar(1, bat_left);
    lcd.createChar(2, bat_bot_left);
    lcd.createChar(3, bat_side);
    lcd.createChar(4, bat_top_right);
    lcd.createChar(5, bat_right);
    lcd.createChar(6, bat_bot_right);
    lcd.createChar(7, bat_charge);
    lcd.setCursor(6, 0);
    lcd.print("BATTERY");
    
    //top left
    lcd.setCursor(4, 1);
    lcd.write(0);
    //left
    lcd.setCursor(4,2);
    lcd.write(1);
    //bot left
    lcd.setCursor(4,3);
    lcd.write(2);
    //top and bottom
    for (int col = 5; col <= 14; col++) {
      
      lcd.setCursor(col, 1);
      lcd.write(3);
      lcd.setCursor(col, 3);
      lcd.write(3);
    }
    //top right
    lcd.setCursor(15, 1);
    lcd.write(4);
    //right
    lcd.setCursor(15, 2);
    lcd.write(5);
    //bot right
    lcd.setCursor(15, 3);
    lcd.write(6);

    //displaying charges
    for (int i = 5; i <= 14; i++) {
      lcd.setCursor(i, 2);
      lcd.write(7);
    }

    switch (charge_current_state) {

      case CHARGE_ON:
        if (millis() - charge_last_millis > 1000) {
          charge_current_state = CHARGE_OFF;
          charge_last_millis = millis();

          lcd.setCursor(14, 2);
          lcd.print(" ");
        }
        break;

      case CHARGE_OFF:
        if (millis() - charge_last_millis > 500) {
          charge_current_state = CHARGE_ON;
          charge_last_millis = millis();

          lcd.setCursor(14, 2);
          lcd.write(7);
        }
        break;
    }
  }
  else if (mode == "Speed") {
    lcd.setCursor(7, 0);
    lcd.print("SPEED");
  }

  else {
    lcd.setCursor(0,0);
    lcd.print("Surprise! Nothing!");
  }

}
//Define function
boolean dbRead(int pin, unsigned int dbDelay) {
  if (digitalRead(pin) == HIGH) {
    unsigned long start_time = millis();
    while (digitalRead(pin) == HIGH) {
      if (millis() - start_time > dbDelay) {
        return HIGH;
      }
    }
  }
  return LOW;
}
