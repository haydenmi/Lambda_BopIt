#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); //might be 0x3F

void setup()
{
  //LCD setup
  lcd.init();
  lcd.clear();
  lcd.backlight();
  
  // sets the digital input pins
  pinMode(2, INPUT);      //new game
  pinMode(6, INPUT);      //button1
  pinMode(7, INPUT);      //button2
  pinMode(8, INPUT);      //button3
  pinMode(9, INPUT);      //tilt sensor

  // sets the analog input pins
  pinMode(A1, INPUT);      //pressure pad

  // sets the digital output pins
  pinMode(3, OUTPUT);     //speaker

  // sets the analog output pins
  pinMode(A4, OUTPUT);     //display SDA
  pinMode(A5, OUTPUT);     //display SCL
}




void loop() {
    //global variables
    bool game_running = 0;
    bool clock_ticking;
    int task;
    int allotted_time = 5000; //5 seconds?
    int start_time, current_time;
    int score = 0;
    double pressure;
    double pressed_threshold;
    bool success = 0;


    //tell user to press button to start a new game
    lcd.clear();
    lcd.home();
    lcd.print("Press the start button to begin!");
    
    //if "new game" button pressed...
    if(digitalRead(2)==HIGH)
    {
      game_running = 1;
      //set seed based on current time so no 2 games are the same
      unsigned long seed_time = millis();
      randomSeed(seed_time);
    }
    
    while(game_running)
    {
      //game start message
      lcd.clear();
      lcd.home();
      lcd.print("GAME START!");
      lcd.setCursor(1,0);
      delay(1000);
      
      score = 0;
      lcd.print("Score: ");
      lcd.setCursor(1,7);
      lcd.print(score); //EXPERIMENT: is this how it behaves? Do I need the setCursor(1,7) line?
      
       
      digitalWrite(3, HIGH);     // speaker signifies game start
      
      for(int i=0; i<99; i++)  //this means for a max of 99 rounds
      {
        lcd.clear();
        success = 0;
        
        task = random(0,3);  //let choose_it=0...flip_it=1...squeeze_it=2

        if(task == 0) //if choose_it
        {
          int which_buttons;
          which_buttons = random(0,3);   //this determines which buttons to press
          
          //notify user that the current task is Choose-It!
          lcd.home();
          lcd.println("Choose-It!");
          
          start_time = millis();
          clock_ticking = 1;
          while(clock_ticking)
          {
              current_time = millis();
              if ((current_time >= start_time + allotted_time))
              {
                //game_running = 0; //will break out of game_running loop
                i = 100;          //will finish 99 round for-loop
                break;            //will break out of clock_ticking loop
              }

              if(which_buttons == 0)
              {
                //notify user of which buttons to press (on the screen)
                lcd.print("Press buttons 1 and 2!");
                if(digitalRead(6)==HIGH && digitalRead(7)==HIGH && digitalRead(8)==LOW)
                  success = 1;
              }
              else if(which_buttons == 1)
              {
                lcd.print("Press buttons 2 and 3!");
                if(digitalRead(7)==HIGH && digitalRead(8)==HIGH && digitalRead(6)==LOW)
                  success = 1;
              }
              else if(which_buttons == 2)
              {
                lcd.print("Press buttons 1 and 3!");
                if(digitalRead(6)==HIGH && digitalRead(8)==HIGH && digitalRead(7)==LOW)
                  success = 1;
              }

              //if they correctly press the buttons, leave the loop
              if (success == 1)
              {
                //notify the user that they were successful
                lcd.clear();
                lcd.home();
                lcd.println("SUCCESS!!");


                //increment and display score
                score++;
                lcd.print("Score: ");
                lcd.print(score); //EXPERIMENT: is this how it behaves? Do I need the setCursor(1,7) line?

                //go to next round
                break;
              }
          }
        }

        else if (task == 1) //if flip_it
        {
          //notify user that the current task is Flip-It!
          lcd.clear();
          lcd.home();
          lcd.println("Flip-It!");
          
          start_time = millis();
          clock_ticking = 1;
          while(clock_ticking)
          {
              current_time = millis();
              if (current_time >= start_time + allotted_time)
              {
                //game_running = 0; //will break out of game_running loop
                i = 100;          //will finish 99 round for-loop
                break;            //will break out of clock_ticking loop
              }

              if (digitalRead(9) == HIGH)
                success = 1;

              if (success == 1)
              {
                //notify the user that they were successful
                lcd.clear();
                lcd.home();
                lcd.println("SUCCESS!!");


                //increment and display score
                score++;
                lcd.print("Score: ");
                lcd.print(score); //EXPERIMENT: is this how it behaves? Do I need the setCursor(1,7) line?

                //go to next round
                break;
              }

          }
          
        }
        else if (task == 2) //if squeeze_it
        {
          //notify user that the current task is Squeeze-It!
          lcd.clear();
          lcd.home();
          lcd.println("Squeeze-It!");
          
          start_time = millis();
          clock_ticking = 1;
          while(clock_ticking)
          {
              current_time = millis();
              if (current_time >= start_time + allotted_time)
              {
                //game_running = 0; //will break out of game_running loop
                i = 100;          //will finish 99 round for-loop
                break;            //will break out of clock_ticking loop
              }
              
              //TODO - test pressure sensor to see what expected values of press are
              pressed_threshold = 200; //here temporarily, experiment to see what this should actually be
              
              pressure = analogRead(A1);
              if (pressure >= pressed_threshold)
                success = 1;

              if (success == 1)
              {
                //notify the user that they were successful
                lcd.clear();
                lcd.home();
                lcd.println("SUCCESS!!");


                //increment and display score
                score++;
                lcd.print("Score: ");
                lcd.print(score); //EXPERIMENT: is this how it behaves? Do I need the setCursor(1,7) line?

                //go to next round
                break;
              }
          }
        }

        allotted_time -= 10; //minus a tenth(is that good?) of a second for the next round     }

    } //end 99-round for loop
    
    //Game over operations
    lcd.clear();
    lcd.home();
    lcd.println("GAME OVER");
    lcd.print("Final score: ");
    lcd.print(score); //Again, not sure if this is right yet
    game_running = 0;
    delay(5000);
    
    
  }//end game_running while loop  
  
} //end loop()
