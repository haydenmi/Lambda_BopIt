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
  randomSeed(analogRead(0));
  

}




void loop() {

    //global variables
    bool game_running = 0;
    bool clock_ticking;
    int task;
    int allotted_time = 5000; //5 seconds?
    unsigned long start_time, current_time;
    int score = 0;
    double pressure;
    double pressed_threshold;
    bool success = 0;


    //tell user to press button to start a new game
    
    lcd.setCursor(2,0);
    lcd.print("Press START!");

    //if "new game" button pressed...
    if(digitalRead(2)==HIGH)
    {
      game_running = 1;
      //set seed based on current time so no 2 games are the same
    }
    
    while(game_running)
    {
      //game start message
      lcd.clear();
      lcd.home();
      lcd.setCursor(7,0);
      lcd.print("3");
      delay(800);
      lcd.clear();
      lcd.setCursor(7,0);
      lcd.print("2");
      delay(800);
      lcd.clear();
      lcd.setCursor(7,0);
      lcd.print("1");
      delay(800);
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("GAME START!");
      
      
      // speaker signifies game start
      tone(3,33,2000);   //c1
      tone(3,65,2000);   //c2
      tone(3,131,2000);   //c3
      delay(750);
      
      tone(3,262,1000);   //c4
      tone(3,1047,1000);    //c6
      delay(250);

      tone(3,311,750);    //ds4
      tone(3,1245,750);     //ds6
      delay(500);

      tone(3,349,750);    //f4
      tone(3,1397,750);     //f6
      delay(500);

      tone(3,370,500);    //fs4
      tone(3,1480,500);     //fs6
      
      delay(1000);
      for(int i=0; i<99; i++)  //this means for a max of 99 rounds
      {
        lcd.clear();
        success = 0;
        
        task = random(3);  //let choose_it=0...flip_it=1...squeeze_it=2


        if(task == 0) //if choose_it
        {
          int which_buttons;
          which_buttons = random(3);   //this determines which buttons to press
          
          //notify user that the current task is Choose-It!
          lcd.setCursor(3,0);
          lcd.print("Choose-It!");
          
          tone(3,262,500);   //c4
          tone(3,1047,500);    //c6
      
          start_time = millis();
          clock_ticking = 1;
          while(clock_ticking)
          {
              if ((millis() >= start_time + allotted_time))
              {
                i = 100;          //will finish 99 round for-loop
                break;            //will break out of clock_ticking loop
              }
              
              //fail condition if squeezed during choose it
              pressure = analogRead(A1);
              if (pressure >= 500)
              {
                i=100;
                break;
              }
              if(which_buttons == 0)
              {
                //notify user of which buttons to press (on the screen)
                lcd.setCursor(1,1);
                lcd.print("Press 1 and 2");
                if(digitalRead(6)==HIGH && digitalRead(7)==HIGH && digitalRead(8)==LOW)
                  success = 1;
                if(digitalRead(8)==HIGH)
                {
                  i=100;
                  break;
                }

              }
              else if(which_buttons == 1)
              {
                lcd.setCursor(1,1);
                lcd.print("Press 2 and 3");
                if(digitalRead(7)==HIGH && digitalRead(8)==HIGH && digitalRead(6)==LOW)
                  success = 1;
                if(digitalRead(6)==HIGH)
                {
                  i=100;
                  break;
                }
              }
              else if(which_buttons == 2)
              {
                lcd.setCursor(1,1);
                lcd.print("Press 1 and 3");
                if(digitalRead(6)==HIGH && digitalRead(8)==HIGH && digitalRead(7)==LOW)
                  success = 1;
                if(digitalRead(7)==HIGH)
                {
                  i=100;
                  break;
                }
              }

              //if they correctly press the buttons, leave the loop
              if (success == 1)
              {
                //notify the user that they were successful
                lcd.clear();
                lcd.setCursor(4,0);
                lcd.print("SUCCESS!");


                //increment and display score
                score++;
                lcd.setCursor(4,1);
                lcd.print("Score: ");
                lcd.setCursor(10,1);
                lcd.print(score); 
                delay(500);

                //go to next round
                break;
              }
          }
        }

        else if (task == 1) //if flip_it
        {
          //notify user that the current task is Flip-It!
          lcd.clear();
          lcd.setCursor(4,0);
          lcd.print("Flip-It!");

          tone(3,311,500);    //ds4
          tone(3,1245,500);     //ds6
          
          start_time = millis();
          clock_ticking = 1;
          while(clock_ticking)
          {
              if (millis() >= start_time + allotted_time)
              {
                i = 100;          //will finish 99 round for-loop
                break;            //will break out of clock_ticking loop
              }

              //fail condition if buttons pressed during flip it
              if(digitalRead(6)==HIGH || digitalRead(7)==HIGH || digitalRead(8)==HIGH)
              {
                i=100;
                break; 
              }

              //fail condition if squeezed during flip it
              pressure = analogRead(A1);
              if (pressure >= 500)
              {
                i=100;
                break;
              }

              if (digitalRead(9) == HIGH)
                success = 1; 

              if (success == 1)
              {
                //notify the user that they were successful
                lcd.clear();
                lcd.home();
                lcd.setCursor(4,0);
                lcd.print("SUCCESS!");


                //increment and display score
                score++;
                lcd.setCursor(4,1);

                lcd.print("Score: ");
                lcd.setCursor(10,1);

                lcd.print(score); 
                delay(500);


                //go to next round
                break;
              }

          }
          
        }
        else if (task == 2) //if squeeze_it
        {
          //notify user that the current task is Squeeze-It!
          lcd.clear();
          lcd.setCursor(2,0);
          lcd.print("Squeeze-It!");

          tone(3,349,500);    //f4
          tone(3,1397,500);     //f6
          
          start_time = millis();
          clock_ticking = 1;
          while(clock_ticking)
          {
              if (millis() >= start_time + allotted_time)
              {
                i = 100;          //will finish 99 round for-loop
                break;            //will break out of clock_ticking loop
              }
              
              


              //fail condition if buttons pressed during squeeze it
              if(digitalRead(6)==HIGH || digitalRead(7)==HIGH || digitalRead(8)==HIGH)
              {
                i=100;
                break; 
              }

              
              pressure = analogRead(A1);
              if (pressure >= 500)
                success = 1;

              if (success == 1)
              {
                //notify the user that they were successful
                lcd.clear();
                lcd.setCursor(4,0);
                lcd.print("SUCCESS!");


                //increment and display score
                score++;
                lcd.setCursor(4,1);
                lcd.print("Score: ");
                lcd.setCursor(10,1);
                lcd.print(score); 
                delay(500);

                //go to next round
                break;
              }
          }
        }
        else
        {
           lcd.print("bruhhhhh");
        }
        delay(500);
        allotted_time -= 40; //minus a tenth(is that good?) of a second for the next round     }

    } //end 99-round for loop

    if(score == 99)
    {
      lcd.clear();
      lcd.home();
      lcd.print("CONGRATULATIONS!");

      // speaker signifies game start
      tone(3,33,2000);   //c1
      tone(3,65,2000);   //c2
      tone(3,131,2000);   //c3
      delay(300);
      
      tone(3,262,1000);   //c4
      tone(3,1047,1000);    //c6
      delay(300); //250

      tone(3,311,750);    //ds4
      tone(3,1245,750);     //ds6
      delay(300);

      tone(3,349,750);    //f4
      tone(3,1397,750);     //f6
      delay(300);

      tone(3,370,500);    //fs4
      tone(3,1480,500);     //fs6
      delay(300);

      tone(3,349,750);    //f4
      tone(3,1397,750);     //f6
      delay(300);
      
      tone(3,311,750);    //ds4
      tone(3,1245,750);     //ds6
      delay(300);

      tone(3,262,1000);   //c4
      tone(3,1047,1000);    //c6
      delay(750);

      tone(3,932,250);   //bf5
      delay(250);
      tone(3,1175,250);   //d6
      delay(250);
      tone(3,1047,500);   //c6
    }
    else
    {
    //Game over operations
    lcd.clear();
    lcd.home();
    lcd.setCursor(3,0);
    lcd.print("GAME OVER");

    tone(3,523,1500);    //c5
    delay(1000);
    tone(3,494,750);    //b4
    delay(750);
    tone(3,1976,100);    //b6
    delay(250);
    tone(3,1976,100);    //b6
    delay(250);
    tone(3,1976,100);    //b6
    delay(250);
    tone(3,1976,100);    //b6
    delay(250);
    tone(3,1976,100);    //b6
    delay(250);
    tone(3,1976,100);    //b6
    delay(250);
    tone(3,1976,500);    //b6
    }
    delay(500);
    lcd.setCursor(1,1);
    lcd.print("Final score:");
    lcd.print(score); 
    game_running = 0;
    delay(5000);
    lcd.clear();
    
    
  }//end game_running while loop  


} //end loop()
