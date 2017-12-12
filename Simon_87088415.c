/*
 * Author: Alex Vazquez
 * Student #: 87088415
 * E-Mail: a.vazquez@alumni.ubc.ca
 * Date: friday 1st Dec, 2017
 * Section:L1U
 *
 *Purpose:
 * To create a simple simon says game, with the use of a virtual DAQ interfeace that consist's of 4 LED lights and 4 buttons.
 */

#define _CRT_SECURE_NO_WARNINGS

/* header files */
#include <stdlib.h>
#include <stdio.h>
#include <DAQlib.h>
#include <Windows.h>
#include <time.h>

/*  input/output channels */
 
#define Green  0
#define Red    1
#define Yellow 2
#define Blue   3

/*symbolic constants*/

#define TRUE 1
#define FALSE 0

#define RandNumMax 4

#define delay 500

#define SequenceMax 5
#define endGame 3


/*function prototypes*/
void runSimon(void);
int readSwitch(int CH);
void GenerateSeq(int sequence[]);
void BlinkLED(int LED_CH);
void Win_Loose(int colour);

/*initializes the game, by asking player to type 6*/
int main(void)
{
	int setupNum;

	printf("The Simon Game for DAQ.\n\n\n");
	printf("Enter 6 to begin game: ");
	scanf("%d", &setupNum);

	if (setupDAQ(setupNum) == TRUE)
		runSimon();
	else
		printf("ERROR: CANNOT BEGIN GAME........\n");

	system("PAUSE");
	return 0;
}

/* Where the game occurs*/
void runSimon(void)
{
	int sequence[SequenceMax];
	int index;


	while (continueSuperLoop()==TRUE){
		GenerateSeq(sequence);
		int cont = TRUE;
		int level = 1;

		while (cont == TRUE){

			Sleep(delay);

			for (index = 0; index < level; index++){
				BlinkLED(sequence[index]);
				Sleep(delay);
			}

			for (index = 0; index < level; index++){
				if (readSwitch(sequence[index]) == FALSE)
				{
					cont = FALSE;
					break;
				}
					
				else
					Sleep(delay);
			}

			if (level == SequenceMax) {
				Win_Loose(Green);
				cont = FALSE;
			}
			else if (level < SequenceMax && cont == FALSE)
				Win_Loose(Red);

			level++;
		}
		
	}
}

/*Checks if button is pressed, and returns if button pressed is the right one or wrong one*/
int readSwitch(int CH) {
	int Correct;
	int Loop = TRUE;

	do
	{

		if ((digitalRead(Red) && CH == Red)|| (digitalRead(Green) && CH == Green)|| (digitalRead(Yellow) && CH == Yellow)|| (digitalRead(Blue) && CH == Blue)) {
			Correct = TRUE;
			break;
		}
		
		else if ((digitalRead(Red) && CH != Red) || (digitalRead(Green) && CH != Green) || (digitalRead(Yellow) && CH != Yellow) || (digitalRead(Blue) && CH != Blue)) {
			Correct = FALSE;
			break;
	}

	} while (Loop==TRUE);
	
	return Correct;
}
/*Generates the pattern LEDS will follow*/
void GenerateSeq(int sequence[]) {
	int index;
	srand((unsigned)time(NULL));
	for (index = 0; index < SequenceMax; index++){
		sequence[index] = rand()%RandNumMax;
	}
}
/*Takes as parameter the LED colour to blink*/
void BlinkLED(int LED_CH) {
	digitalWrite(LED_CH, TRUE);
	Sleep(delay);
	digitalWrite(LED_CH, FALSE);
}

/*Blinks an LED 3 times, takes as parameter the LED to blink, Green = player wins, Red= Player lost*/
void Win_Loose(int Colour) {
	int i;
	for ( i = 0; i < endGame; i++)
	{
		BlinkLED(Colour);
		Sleep(delay);
	}
	Sleep(2*delay);
}
