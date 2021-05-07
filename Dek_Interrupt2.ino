class dekatronStep
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



public:
	dekatronStep(int DekNum, int pinCnt, int pin1, int pin2, int pin3, bool direction, int sDelay)  //Guide1, Guide2, Index,  Direction, StepDelay
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

	void updateStep(unsigned long currentMillis, unsigned long stepDelay)
	{
		cli(); // stop interrupts

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
			}

		}

		pinCount++;


		// see if Index is High or Low
		byte indexState = digitalRead(Index);

		// has index state changed since last time?
		if (indexState)
			if (indexState != oldIndexState)
				pinCount = 0;
		{
			// ignore time.
			if (millis() - indexHighTime >= ignoreTime) // does not really seem to be needed.



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

		sei(); // allow interrupts
	}


	//index ignore timout settings.
	byte oldIndexState = HIGH;
	const unsigned long ignoreTime = 50;  // milliseconds
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

//30 pins on deks ?
// Class		Object
//setup physical pins here. 
//In this case 63 and 62 are G1 and G2. The index is 61.
dekatronStep Dek1(1, 0, 12, 13, 11, true, 0);
dekatronStep Dek2(2, 0, 9, 10, 8, true, 0);
dekatronStep Dek3(3, 0, 6, 7, 5, true, 0);
dekatronStep Dek4(4, 0, 3, 4, 2, true, 0);
dekatronStep Dek5(5, 0, 30, 32, 28, true, 0);
dekatronStep Dek6(6, 0, 26, 24, 22, true, 0);
dekatronStep Dek7(7, 0, 25, 23, 27, true, 0);
dekatronStep Dek8(8, 0, 29, 31, 33, true, 0); // fault in hardware
dekatronStep Dek9(9, 0, 35, 39, 37, true, 0);
dekatronStep Dek10(10, 0, 41, 45, 43, true, 0);

//not connected
dekatronStep Dek11(11, 0, 40, 42, 44, true, 0);
dekatronStep Dek12(12, 0, 34, 38, 36, true, 0);
dekatronStep Dek13(13, 0, 69, 68, 67, true, 0);
dekatronStep Dek14(14, 0, 66, 65, 64, true, 0);
dekatronStep Dek15(15, 0, 63, 62, 61, true, 0);


// Interrupt is called once a millisecond
ISR(TIMER1_COMPA_vect)
{

}



unsigned long A; 
void loop() {
	unsigned long currentMillis = millis();

	A = A + 1;
	Serial.println(A);
	
	if (A >= 10000)
		A = 0;


	Dek1.updateStep(currentMillis, 100);
	Dek2.updateStep(currentMillis, A);
	Dek3.updateStep(currentMillis, 20);
	Dek4.updateStep(currentMillis, 30);
	Dek5.updateStep(currentMillis, 1);
	Dek6.updateStep(currentMillis, 40);
	Dek7.updateStep(currentMillis, 50);
	Dek8.updateStep(currentMillis, 100); // problem with hardware
	Dek9.updateStep(currentMillis, 1000);
	Dek10.updateStep(currentMillis, 50);

	Dek11.updateStep(currentMillis, 1);
	Dek12.updateStep(currentMillis, 1);
	Dek13.updateStep(currentMillis, 1);
	Dek14.updateStep(currentMillis, 1);
	Dek15.updateStep(currentMillis, 1);

}
