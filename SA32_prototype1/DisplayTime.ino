void DisplayTime(){
  myRTC.updateTime(); // This allows for the update of variables for time or accessing the individual elements.

  lcd.setCursor(0, 0);
  lcd.print("Date ");
  lcd.print(myRTC.dayofmonth);
  lcd.print("/");
  lcd.print(myRTC.month);
  lcd.print("/");
  lcd.print(myRTC.year);

  
  if(myRTC.seconds != prevS){
    lcd.setCursor(0,1);
    lcd.print("Time ");
    lcd.print(myRTC.hours);
    lcd.print(":");
    lcd.print(myRTC.minutes);
    lcd.print(":");
    
    if(myRTC.seconds ==00){   
      lcd.print("0    ");
      }  
    else{
      lcd.print(myRTC.seconds);
      }  
    prevS = myRTC.seconds;
    } 
  //checking for button press to exit function
  if (digitalRead(Back) == LOW && (millis() - lastBackDebounceTime) > debounceDelay && menuAccessed) {
    lastBackDebounceTime = millis();
    DisplayFlag=false;
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print(currentMenu.options[selectedOption]);
    delay(200);
  }    
}