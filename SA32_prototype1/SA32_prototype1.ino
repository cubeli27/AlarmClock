#include <Wire.h>             //librry wire
#include "pitches.h"          //tones for buzzer
#include <LiquidCrystal_I2C.h>  //library LCD

LiquidCrystal_I2C lcd(0x27,16,2); // LCD object

#include <virtuabotixRTC.h>   //library RTC
virtuabotixRTC myRTC(27, 26, 25); // Creation of the Real Time Clock Object(CLOCK, IO, ENABLE)

//JUST DECLARING SOME FUNCTIONS THAT WILL BE ON OTHER TABS TO MAKE SURE THEY WORK 
void TakeOnMe();
void Alarm();
void option1();
void PlayMelody();
void Clock();
void ShowAlarm();
void DisableAlarm(); 
void SetAlarm();
void LastFunc();

volatile byte prevS=-1;     //variables for alarm
volatile byte alarmTime=-1;
bool AlarmFlag=true;
bool AlarmEnableFlag=false;
bool DisplayFlag=true;
volatile int AlarmHour=0;
volatile int AlarmMinute=0;
bool AlarmBeingSet=false;

int buzzer = 12;  
int Right= 33;
int Left= 32;
int Enter= 34;
int Back= 35;
int Rightstate;
int Leftstate;
int Enterstate;
int Backstate;

unsigned long lastRightDebounceTime = 0;
unsigned long lastLeftDebounceTime = 0;
unsigned long lastEnterDebounceTime = 0;
unsigned long lastBackDebounceTime = 0;
const unsigned long debounceDelay = 50;

// Define menu node structure
struct menuNode {
  String title;
  int numOptions;
  String options[5];
  void (*function[5])();
};

// Define menu nodes
menuNode timeMenu = {"TIME MENU", 3, {"Show Alarm", "Disable Alarm", "SetAlarm"}, {ShowAlarm, DisableAlarm, SetAlarm}};
menuNode mainMenu = {"MAIN MENU", 5, {"Option 1", "Play Melody", "Clock", "Time", "LastFunc"}, {option1, PlayMelody, Clock, NULL, LastFunc}};

// Initialize current menu node and selected option
menuNode currentMenu = mainMenu;
int selectedOption = 0;
bool menuAccessed = false;

// Define menu functions
void option1() {
  lcd.clear();
  lcd.print("Option 1...");
  delay(700);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print(currentMenu.options[selectedOption]);  
}

void PlayMelody() {
  lcd.clear();
  lcd.print("Melody...");
  TakeOnMe(buzzer); 
  delay(700);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print(currentMenu.options[selectedOption]);  
}

void Clock() {
  lcd.clear();
  while(DisplayFlag){
    DisplayTime();
  } 
}

void ShowAlarm() {
  lcd.clear();
  if(AlarmEnableFlag){
    lcd.print("Alarm Hour=");
    lcd.print(AlarmHour);
    lcd.setCursor(0,1);
    lcd.print("Alarm Min=");
    lcd.print(AlarmMinute);
    delay(1500);
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print(currentMenu.options[selectedOption]); 
  }else{
      lcd.print("Alarm Disabled"); 
      delay(700);
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print(currentMenu.options[selectedOption]);      
    }
}


void DisableAlarm() {
  lcd.clear();
  lcd.print("Disable Alarm...");
  AlarmEnableFlag=false;
  AlarmHour=0;
  AlarmMinute=0;
  delay(700);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print(currentMenu.options[selectedOption]); 
}

void SetAlarm() {
  lcd.clear();  
  if(AlarmEnableFlag == false){
    AlarmBeingSet=true;
  } else{
    lcd.print("ALARM ACTIVE !");
    delay(700);
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print(currentMenu.options[selectedOption]); 
  }
}
void LastFunc() {
  lcd.clear();
  lcd.print("Last Function");
  delay(700);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print(currentMenu.options[selectedOption]); 
}



void setup()  {  
  // seconds, minutes, hours, day of the week, day of the month, month, year
  //myRTC.setDS1302Time(00, 13, 13, 7, 5, 3, 2023);  // Dont have in uploaded code otherwise every time power goes off will go to this time.

  Wire.setClock(100000);
        
  lcd.begin(); // begins connection to the LCD module
  lcd.backlight(); // turns on the backlight  

  pinMode(buzzer,OUTPUT);
  pinMode(Right,INPUT);
  pinMode(Left,INPUT);
  pinMode(Enter,INPUT);
  pinMode(Back,INPUT);

  lcd.setCursor(0, 0);
  lcd.print("Press Enter to");
  lcd.setCursor(0, 1);
  lcd.print("access menu");
}

void loop() {
  DisplayFlag=true;
  // Check if Enter button is pressed to access menu
  if (digitalRead(Enter) == LOW && (millis() - lastEnterDebounceTime) > debounceDelay && menuAccessed != true) {
    lastEnterDebounceTime = millis();
    lcd.clear();
    currentMenu = mainMenu;
    selectedOption = 0;
    lcd.print(currentMenu.title);
    lcd.setCursor(0,1);
    lcd.print(currentMenu.options[selectedOption]);
    menuAccessed=true;
    delay(200);
  }    
    
  if (digitalRead(Left) == LOW && (millis() - lastLeftDebounceTime) > debounceDelay && menuAccessed && AlarmBeingSet != true) {
    lastLeftDebounceTime = millis();    
    selectedOption--;
    if (selectedOption < 0) {
      selectedOption = currentMenu.numOptions - 1;
    }
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print(currentMenu.options[selectedOption]);
    delay(200);
  }

  // Check if Right button is pressed
  if (digitalRead(Right) == LOW && (millis() - lastRightDebounceTime) > debounceDelay && menuAccessed && AlarmBeingSet != true) {
    lastRightDebounceTime = millis();
    selectedOption++;
    if (selectedOption >= currentMenu.numOptions) {
    selectedOption = 0;
    }
    lcd.clear();
    lcd.setCursor(0,1);    
    lcd.print(currentMenu.options[selectedOption]);
    delay(200);
  }

    // Check if Back button is pressed
  if (digitalRead(Back) == LOW && (millis() - lastBackDebounceTime) > debounceDelay && menuAccessed) {
    lastBackDebounceTime = millis();
    lcd.clear();
    if (currentMenu.title == "MAIN MENU" && menuAccessed == true) {      
      lcd.setCursor(0,0);
      lcd.print("Press Enter to");
      lcd.setCursor(0,1);
      lcd.print("access menu");            
      menuAccessed = false; // set menuAccessed to false to return to initial screen
    } else { 
      currentMenu = mainMenu;
      selectedOption = 0;
      lcd.print(currentMenu.title);
      lcd.setCursor(0,1);
      lcd.print(currentMenu.options[selectedOption]); 
      delay(200);
    }
  } 

  // Check if Enter button is pressed to select an option
  if (digitalRead(Enter) == LOW && (millis() - lastEnterDebounceTime) > debounceDelay && menuAccessed && AlarmBeingSet != true) {
    lastEnterDebounceTime = millis();
    if (currentMenu.function[selectedOption] == NULL) {
      if (currentMenu.title == "MAIN MENU" && selectedOption == 3) {
        currentMenu = timeMenu;
        selectedOption = 0;
        lcd.clear();
        lcd.print(currentMenu.title);
        lcd.setCursor(0,1);
        lcd.print(currentMenu.options[selectedOption]);                
      }
    } else {
      currentMenu.function[selectedOption]();
    }
    delay(200);    
  }
  // ALARM RING
  myRTC.updateTime();
  if(myRTC.hours == AlarmHour && myRTC.minutes == AlarmMinute && AlarmEnableFlag){
    alarmTime = myRTC.minutes;
    if(AlarmFlag){
      TakeOnMe(buzzer);      
    }
  }
  if(alarmTime != myRTC.minutes){
    AlarmFlag = true;
  }  
  //SETTING THE ALARM
    while(AlarmBeingSet){
      lcd.setCursor(0,0);
      lcd.print("Alarm Hour =");
      lcd.print(AlarmHour);                 
      if (digitalRead(Right) == LOW && (millis() - lastRightDebounceTime) > debounceDelay && AlarmHour>= 0  && AlarmHour< 24  && AlarmBeingSet==true) {
        lastRightDebounceTime = millis();
        AlarmHour++;
        delay(200);
      }
      if (digitalRead(Left) == LOW && (millis() - lastRightDebounceTime) > debounceDelay && AlarmHour> 0  && AlarmHour<= 24  && AlarmBeingSet==true) {
        lastRightDebounceTime = millis();
        if(AlarmHour == 10){
          lcd.clear();
        }
        AlarmHour--;
        delay(200);
      }        
      if (digitalRead(Enter) == LOW && (millis() - lastRightDebounceTime) > debounceDelay && AlarmBeingSet == true) {
        lastRightDebounceTime = millis(); 
        delay(1000);  
            while(AlarmBeingSet){                    
              lcd.setCursor(0,1);
              lcd.print("Alarm Min =");
              lcd.print(AlarmMinute);                 
              if (digitalRead(Right) == LOW && (millis() - lastRightDebounceTime) > debounceDelay && AlarmMinute>= 0  && AlarmMinute< 60  && AlarmBeingSet==true) {
                lastRightDebounceTime = millis();
                AlarmMinute++;
                delay(200);
              }
              if (digitalRead(Left) == LOW && (millis() - lastRightDebounceTime) > debounceDelay && AlarmMinute> 0  && AlarmMinute<= 60  && AlarmBeingSet==true) {
                lastRightDebounceTime = millis();
                if(AlarmMinute == 10){
                  lcd.clear();
                  lcd.setCursor(0,0);
                  lcd.print("Alarm Hour =");
                  lcd.print(AlarmHour);                    
                }
                AlarmMinute--;
                delay(200);
              }        
              if (digitalRead(Enter) == LOW && (millis() - lastRightDebounceTime) > debounceDelay && AlarmBeingSet == true) {
                lastRightDebounceTime = millis(); 
                AlarmBeingSet=false;  
                AlarmEnableFlag=true;             
              } 
            }  
        lcd.clear();
        lcd.print("ALL SET");
        lcd.setCursor(0,1);
        lcd.print("Alarm Enabled!");
        delay(1000);
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print(currentMenu.options[selectedOption]);
      }           
    } 
        
}