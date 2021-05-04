class dekStep
{

public:
	int DekNumber;
	int pinCount;
	int Guide1;
	int Guide2;
	int Index;
	int previousGuideState;
	int stepDelay;
	bool clockwise;
	unsigned long previousMillis;
	int Acceleration;


public:
	dekStep(int DekNum, int pinCnt, int pin1, int pin2, int pin3, bool direction, int sDelay)  //Guide1, Guide2, Index,  Direction, StepDelay
	{
		Guide1 = pin1;
		Guide2 = pin2;
		Index = pin3;
		clockwise = direction;
		stepDelay = sDelay;
		DekNumber = DekNum;
		pinCount = pinCnt;

		pinMode(Guide1, OUTPUT);
		pinMode(Guide2, OUTPUT);
		pinMode(Index, INPUT);
	}

	void updateStep(int stepDelay)
	{
		//cli(); // stop interrupts
	// will need adjusting depending on processor speed. This is runing at 16mHz.
		//delayMicroseconds(10);

		//if ((currentMillis - previousMillis >= stepDelay))
		delay(stepDelay);
		//{
			switch (previousGuideState) {
			case 0:
				previousGuideState = 1;
				digitalWrite(Guide1, LOW);
				digitalWrite(Guide2, LOW);
		//		previousMillis = currentMillis;
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
		//		previousMillis = currentMillis;
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
		//		previousMillis = currentMillis;
				break;
		//	}
			
			
		}

		//if (Ndx)Acceleration = Acceleration + pow(1, Acceleration);

		//sei(); // allow interrupts
		pinCount++;
		
		// see if Index is High or Low
		byte NDX = digitalRead(Index);

		
		if (NDX)
			pinCount = 0;
		{
			// has index state changed since last time?
			if (NDX != oldNDX)

			{
			
				oldNDX = NDX;  // remember for next time 

					if ((NDX == 1) && (clockwise == false))
					{
					clockwise = true;
					}
					else if (((NDX == 1)) && (clockwise == true))
					{
					clockwise = true;
					}

			} 

		} 


	}

	byte oldNDX = 0;

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
//12 and 13 are G1 and G2. The index is pin 11.

dekStep Dek1(1, 0, 12, 13, 11, true, 1);
dekStep Dek2(2, 0, 9, 10, 8, true, 10);
dekStep Dek3(3, 0, 6, 7, 5, true, 50);
dekStep Dek4(4, 0, 3, 4, 2, true, 500);
dekStep Dek5(5, 0, 30, 32, 28, true, 1000);
dekStep Dek6(6, 0, 26, 24, 22, true, 500);
dekStep Dek7(7, 0, 25, 23, 27, true, 50);
//dekStep Dek8(8, 0, 29, 31, 33, true, 600); // fault in hardware
dekStep Dek9(9, 0, 35, 39, 37, true, 10);
dekStep Dek10(10, 0, 41, 45, 43, true, 1);

//not connected
//dekStep Dek11(11, 0, 40, 42, 44, true, 900);
//dekStep Dek12(12, 0, 34, 38, 36, true, 1000);
//dekStep Dek13(13, 0, 69, 68, 67, true, 1100);
//dekStep Dek14(14, 0, 66, 65, 64, true, 1200);
//dekStep Dek15(15, 0, 63, 62, 61, true, 1300);


// Interrupt is called once a millisecond
ISR(TIMER1_COMPA_vect)
{

}


void loop() {
	unsigned long currentMillis = millis();

	
	Dek1.updateStep(1000);
	Dek2.updateStep(233);
	Dek3.updateStep(50);
	Dek3.updateStep(100);
	Dek3.updateStep(100);
	Dek3.updateStep(100);
	Dek4.updateStep(100);
	Dek5.updateStep(100);
	Dek6.updateStep(100);
	Dek7.updateStep(100);
	//Dek8.updateStep(currentMillis); // problem with hardware
	Dek9.updateStep(100);
	Dek10.updateStep(100);

}
