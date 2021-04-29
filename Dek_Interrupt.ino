class dekatronStep
{

public:
  int Guide1;
  int Guide2;
  int Index;
  int previousGuideState;
  int stepDelay;
  bool clockwise;
  unsigned long previousMillis;

public:
  dekatronStep(int pin1, int pin2, int pin3, bool direction, int sDelay)  //Guide1, Guide2, Index,  Direction, StepDelay
  {
    Guide1 = pin1;
    Guide2 = pin2;
    Index = pin3;
    stepDelay = sDelay;
    clockwise = direction;

    pinMode(Guide1, OUTPUT);
    pinMode(Guide2, OUTPUT);
    pinMode(Index, INPUT);
  }

  void updateStep(unsigned long currentMillis)
  {
    //Delay needed if there is not enough delay in the loop when calling.
    // will need adjusting depending on processor speed. This is runing at 16mHz.

    delayMicroseconds(20);

    //unsigned long currentMillis = millis();

    if ((currentMillis - previousMillis >= stepDelay))
    {
      //Serial.println(previousGuideState);

      switch (previousGuideState) {
      case 0:
        previousGuideState = 1;
        digitalWrite(Guide1, LOW);
        digitalWrite(Guide2, LOW);
        previousMillis = currentMillis;
        //Serial.println("Case 0");
        break;

      case 1:
        previousGuideState = 2;
        if (clockwise == true)
        {
          digitalWrite(Guide1, HIGH);
          digitalWrite(Guide2, LOW);
        }
        else
        {
          digitalWrite(Guide1, LOW);
          digitalWrite(Guide2, HIGH);
        }
        previousMillis = currentMillis;
        //Serial.println("Case 1");
        break;

      case 2:
        previousGuideState = 0;
        if (clockwise == true)
        {
          digitalWrite(Guide1, LOW);
          digitalWrite(Guide2, HIGH);
        }
        else
        {
          digitalWrite(Guide1, HIGH);
          digitalWrite(Guide2, LOW);
        }
        previousMillis = currentMillis;
        //Serial.println("Case 2");
        break;
      } // end of switch case

    }


  }

};





//index ignore timout settings.
byte oldIndexState = HIGH;
const unsigned long ignoreTime = 5;  // milliseconds
unsigned long indexHighTime;  // when the index last changed state



void setup()
{
  // TIMER 1 for interrupt frequency 2000 Hz:
  cli(); // stop interrupts
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1 = 0; // initialize counter value to 0
         // set compare match register for 2000 Hz increments
  OCR1A = 7999; // = 16000000 / (1 * 2000) - 1 (must be <65536)
          // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12, CS11 and CS10 bits for 1 prescaler
  TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei(); // allow interrupts

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
}

dekatronStep Dek15(63, 62, 61, true, 5); //setup physical pins here. In this case 63 and 62 are G1 and G2. The index is 61.
dekatronStep Dek14(66, 65, 64, true, 10);
dekatronStep Dek13(69, 68, 67, true, 15);
dekatronStep Dek12(34, 38, 36, true, 20);
dekatronStep Dek11(40, 42, 46, true, 30);
dekatronStep Dek10(41, 45, 43, true, 40);
dekatronStep Dek9(35, 39, 37, true, 50);
dekatronStep Dek8(29, 31, 33, true, 60);
dekatronStep Dek7(25, 23, 27, true, 70);
dekatronStep Dek6(26, 24, 22, true, 80);
dekatronStep Dek5(30, 32, 28, true, 1);
dekatronStep Dek4(3, 4, 2, true, 1);
dekatronStep Dek3(6, 7, 5, true, 2);
dekatronStep Dek2(9, 10, 12, true, 2);
dekatronStep Dek1(12, 13, 11, true, 2);


// Interrupt is called once a millisecond
ISR(TIMER1_COMPA_vect)
{
  unsigned long currentMillis = millis();

  Dek15.updateStep(currentMillis);
  Dek14.updateStep(currentMillis);
  Dek13.updateStep(currentMillis);
  Dek12.updateStep(currentMillis);
  Dek11.updateStep(currentMillis);
  Dek10.updateStep(currentMillis);
  Dek9.updateStep(currentMillis);
  Dek8.updateStep(currentMillis);
  Dek7.updateStep(currentMillis);
  Dek6.updateStep(currentMillis);
  Dek5.updateStep(currentMillis);
  Dek4.updateStep(currentMillis);
  Dek3.updateStep(currentMillis);
  Dek2.updateStep(currentMillis);
  Dek1.updateStep(currentMillis);
  
  updateIndex(currentMillis);
}


void updateIndex(unsigned long currentMillis) {

  // see if Index is High or Low
  byte indexState = digitalRead(Dek6.Index);

  // has index state changed since last time?
  if (indexState != oldIndexState)
  {
    // ignore time.
    if (millis() - indexHighTime >= ignoreTime)
    {
      indexHighTime = currentMillis;  // when index was high
      oldIndexState = indexState;  // remember for next time 

      if ((indexState == HIGH) && (Dek7.clockwise == false))
      {
        Dek7.clockwise = true;
        //Serial.println("Clockwise");
        //Serial.println("index high");

      }
      else if (((indexState == HIGH)) && (Dek7.clockwise == true))
      {
        Dek7.clockwise = false;
        //Serial.println("Counter Clockwise");
        //Serial.println("index low");
        

      }

    }  // end if ignore time up


  }  // end of state change

}

void loop() {


}
