void setup()
{
  // sets the digital input pins
  pinMode(2, INPUT);      //new game
  pinMode(6, INPUT);      //button1
  pinMode(7, INPUT);      //button2
  pinMode(8, INPUT);      //button3
  pinMode(9, INPUT);      //tilt sensor

  // sets the analog input pins
  pinMode(1, INPUT);      //pressure pad

  // sets the digital output pins
  pinMode(3, OUTPUT);     //speaker

  // sets the analog output pins
  pinMode(4, OUTPUT);     //display SDA
  pinMode(5, OUTPUT);     //display SCL
}




//global variables
bool game_running = 0;
bool clock_ticking;
int score;
int task;


//if "new game" button pressed...
if(digitalRead(2)==HIGH)
{
  game_running = 1;
}

void loop() {
    int allotted_time = 5000; //5 seconds?
    int start_time, current_time;


    while(game_running)
    {
      score = 0;
      digitalWrite(3, HIGH);     // speaker signifies game start
      
      for(int i=0; i<99; i++)  //this means for a max of 99 rounds
      {
        task = random(0,3);  //let choose_it=0...flip_it=1...squeeze_it=2

        if(task == 0) //if choose_it
        {
          outputsound(0);
          int which_buttons;
          which_buttons = random(0,3);   //this determines which buttons to press
          
          start_time = millis();
          clock_ticking = 1;
          while(clock_ticking)
          {
              current_time = millis();
              if (current_time >= start_time + allotted_time)
              {
                game_running = 0;
              }

              if(which_buttons == 0)
              {
                if(digitalRead(6)==HIGH && digitalRead(7)==HIGH && digitalRead(8)==LOW)
                break;
              }
              if(which_buttons == 1)
              {
                if(digitalRead(7)==HIGH && digitalRead(8)==HIGH && digitalRead(6)==LOW)
                break;
              }
              if(which_buttons == 2)
              {
                if(digitalRead(6)==HIGH && digitalRead(8)==HIGH && digitalRead(7)==LOW)
                break;
              }
                  

            
          }
          score = score + 1;
          
          
          
          
        }

        allotted_time -= 10; //minus a tenth of a second for the next round
      }

     

      
    }





  

  //if(digitalRead(8)==HIGH) 
  //{
    digitalWrite(13, HIGH); // sets the digital pin 13 on
    delay(80);            // waits for a second
    digitalWrite(13, LOW);  // sets the digital pin 13 off
    delay(80);            // waits for a second
  //}
  //else
  //{
     //digitalWrite(13, LOW);  // sets the digital pin 13 off
  //}
  
}
