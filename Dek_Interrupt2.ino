class dekatronStep
{

public:
	int DekNumber;
	int Guide1;
	int Guide2;
	int Index;
	int previousGuideState;
	int stepDelay;
	bool clockwise;
	unsigned long previousMillis;
	

public:
	dekatronStep(int DekNum, int pin1, int pin2, int pin3, bool direction, int sDelay)  //Guide1, Guide2, Index,  Direction, StepDelay
	{
		Guide1 = pin1;
		Guide2 = pin2;
		Index = pin3;
		clockwise = direction;
		stepDelay = sDelay;
		DekNumber = DekNum;
		
		pinMode(Guide1, OUTPUT);
		pinMode(Guide2, OUTPUT);
		pinMode(Index, INPUT);
	}

	void updateStep(unsigned long currentMillis)
	{
		//Delay needed if there is not enough delay in the loop when calling.
		// will need adjusting depending on processor speed. This is runing at 16mHz.
		delayMicroseconds(20);
		
		if ((currentMillis - previousMillis >= stepDelay))
		{
			switch (previousGuideState) {
			case 0:
				previousGuideState = 1;
				digitalWrite(Guide1, LOW);
				digitalWrite(Guide2, LOW);
				previousMillis = currentMillis;
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
				break;
			} // end of switch case

		}


	}
	

	void updateIndex(unsigned long currentMillis) {

		// see if Index is High or Low
		byte indexState = digitalRead(Index);
		
	

		// has index state changed since last time?
		if (indexState != oldIndexState)
			
		{
			// ignore time.
			if (millis() - indexHighTime >= ignoreTime)
				
				//pinCount++;
				Serial.println(DekNumber);

			{
				indexHighTime = currentMillis;  // when index was high
				oldIndexState = indexState;  // remember for next time 

				if ((indexState == HIGH) && (clockwise == false))
				{
					clockwise = true;
				}
				else if (((indexState == HIGH)) && (clockwise == true))
				{
					clockwise = false;
				}
				
			}  // end if ignore time up
		}  // end of state change

	}
	
	//index ignore timout settings.
	byte oldIndexState = HIGH;
	const unsigned long ignoreTime = 5;  // milliseconds
	unsigned long indexHighTime;  // when the index last changed state
	
};




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

//30 pins on deks
// Class		Object
//setup physical pins here. 
//In this case 63 and 62 are G1 and G2. The index is 61.
dekatronStep Dek1(1, 12, 13, 11, true, 50);
dekatronStep Dek2(2, 9, 10, 8, true, 50);
dekatronStep Dek3(3, 6, 7, 5, true, 5000);
dekatronStep Dek4(4, 3, 4, 2, true, 5000);
dekatronStep Dek5(5, 30, 32, 28, true, 5000);
dekatronStep Dek6(6, 26, 24, 22, true, 5000);
dekatronStep Dek7(7, 25, 23, 27, true, 5000);
dekatronStep Dek8(8, 29, 31, 33, true, 5000); // fault in hardware
dekatronStep Dek9(9, 35, 39, 37, true, 5000);
dekatronStep Dek10(10, 41, 45, 43, true, 5000);

//not connected
dekatronStep Dek11(11, 40, 42, 44, true, 50);
dekatronStep Dek12(12, 34, 38, 36, true, 50);
dekatronStep Dek13(13, 69, 68, 67, true, 50);
dekatronStep Dek14(14, 66, 65, 64, true, 50);
dekatronStep Dek15(15, 63, 62, 61, true, 50);


// Interrupt is called once a millisecond
ISR(TIMER1_COMPA_vect)
{
	unsigned long currentMillis = millis();
	
	Dek1.updateStep(currentMillis);
	Dek1.updateIndex(currentMillis);
	

	Dek2.updateStep(currentMillis);
	Dek2.updateIndex(currentMillis);

	Dek3.updateStep(currentMillis);
	Dek3.updateIndex(currentMillis);

	Dek4.updateStep(currentMillis);
	Dek4.updateIndex(currentMillis);

	Dek5.updateStep(currentMillis);
	Dek5.updateIndex(currentMillis);

	Dek6.updateStep(currentMillis);
	Dek6.updateIndex(currentMillis);

	Dek7.updateStep(currentMillis);
	Dek7.updateIndex(currentMillis);

	//Dek8.updateStep(currentMillis); // problem with hardware
	
	Dek9.updateStep(currentMillis);
	Dek9.updateIndex(currentMillis);

	Dek10.updateStep(currentMillis);
	Dek10.updateIndex(currentMillis);

	//not connected
	Dek11.updateStep(currentMillis);
	Dek12.updateStep(currentMillis);
	Dek13.updateStep(currentMillis);
	Dek14.updateStep(currentMillis);
	Dek15.updateStep(currentMillis);
	
}





void loop() {


}
