/*
 * music.c
 *
 * Author: Shreya Mukherjee
 *
 */
#include "sound.h"
Note_t Song[] = { { NOTE_C6, ONE_QUARTER, true },  // Tone, Time, Delay
		{ NOTE_B5, ONE_QUARTER, true }, { NOTE_D6, ONE_QUARTER, true }, {
		NOTE_C6, ONE_QUARTER, true }, { NOTE_F6, ONE_QUARTER, true }, {
		NOTE_E6, ONE_QUARTER, true }, { NOTE_G6, ONE_QUARTER, true }, {
		NOTE_F6, ONE_QUARTER, true }, { NOTE_A6, ONE_QUARTER, true }, {
		NOTE_A6, ONE_QUARTER, true }, { NOTE_A6, ONE_QUARTER, true }, {
		NOTE_A6, ONE_QUARTER, true }, { NOTE_A6, ONE_HALF, false }, {
		NOTE_A6, ONE_HALF, true }, { NOTE_G6, ONE_QUARTER, true }, {
		NOTE_F6S, ONE_QUARTER, true }, { NOTE_G6, ONE_QUARTER, true }, {
		NOTE_A6, ONE_QUARTER, true }, { NOTE_F6, ONE_QUARTER, true }, {
		NOTE_G6, ONE_QUARTER, true }, { NOTE_A6, ONE_QUARTER, true }, {
		NOTE_F6, ONE_QUARTER, true }, { NOTE_E6, ONE_QUARTER, true }, {
		NOTE_D6, ONE_QUARTER, true }, { NOTE_E6, ONE_QUARTER, true }, {
		NOTE_F6, ONE_QUARTER, true }, { NOTE_G6, ONE_HALF, false }, {
		NOTE_G6, ONE_HALF, true } };

Note_t hit_low[] = { { NOTE_C6, (MEASURE_DURATION * MEASURE_RATIO) / 64, true }, // Tone, Time, Delay
		};

Note_t hit_avg[] = { { NOTE_D6, (MEASURE_DURATION * MEASURE_RATIO) / 64, true }, // Tone, Time, Delay
		};

Note_t hit_high[] =
		{ { NOTE_G6, (MEASURE_DURATION * MEASURE_RATIO) / 64, true }, // Tone, Time, Delay
		};

//***************************************************************
// This function returns how long an individual  notes is played
//***************************************************************
uint32_t music_get_time_delay(measure_time_t time) {
	uint32_t time_return = 0;
	time_return = MEASURE_DURATION * MEASURE_RATIO;
	switch (time) {
	case ONE_QUARTER: {
		time_return = time_return / 4;
		break;
	}
	case ONE_HALF: {
		time_return = time_return / 2;
		break;
	}
	case ONE_EIGTH: {
		time_return = time_return / 8;
		break;
	}
	case THREE_EIGTH: {
		time_return = time_return * 3;
		time_return = time_return / 8;
		break;
	}
	default: {
		break;
	}
	}
	return time_return - DELAY_AMOUNT;
}

//***************************************************************************
// Plays a single note of the song based on note_index.  After
// the note is played, there is an optional delay in between
// notes.
//
// Examples
// Song[note_index].period       -- Used to determine the period of the
//                                  PWM pulse.
//
// Song[note_index].time         -- 1/4 or 1/2 time note.  Call
//                                  music_get_time_delay() to determine how
//                                  long to play the note
//
// Song[note_index].delay        -- If true, add a period of silence for
//                                  DELAY_AMOUNT
//
//                                  If false, return without delaying.
//*************************************************************************
static void music_play_note(uint16_t note_index) {
	//Setting the period of the timer
	TIMER_A0->CCR[0] = (Song[note_index].period) - 1;
	//Setting BUZZER duty cycle to 50%
	TIMER_A0->CCR[4] = ((Song[note_index].period) / 2) - 1;

	// turn on Buzzer
	//Clear the current mode control bits
	TIMER_A0->CTL &= ~TIMER_A_CTL_MC_MASK;
	//Set mode control to UP and clear the current count
	TIMER_A0->CTL |= TIMER_A_CTL_MC__UP | TIMER_A_CTL_CLR;

	// wait for the time given in the time field
	uint32_t note_time = music_get_time_delay(Song[note_index].time);
	ece353_T32_1_wait(note_time);

	// Turning off the Buzzer by turning off timer
	TIMER_A0->CTL &= ~TIMER_A_CTL_MC_MASK;

	// if delay is true, timer waits for the required time
	if (Song[note_index].delay) {
		ece353_T32_1_wait(DELAY_AMOUNT);
	}
}

void music_play_hit(void) {
	// initialize Buzzer
	ece353_MKII_Buzzer_Init();

	// initialize Timer_32_1
	TIMER32_1->CONTROL = 0; //Turn off Timer32_1
	TIMER32_1->CONTROL = TIMER32_CONTROL_ONESHOT | TIMER32_CONTROL_SIZE;

	int i;
	if (intensity_light == LIGHT_LOW) {
		for (i = 0; i < (sizeof(hit_low) / sizeof(Note_t)); ++i) {
			music_play_note(i);
		}
	} else if (intensity_light == LIGHT_HIGH) {
		for (i = 0; i < (sizeof(hit_high) / sizeof(Note_t)); ++i) {
			music_play_note(i);
		}
	} else {
		for (i = 0; i < (sizeof(hit_avg) / sizeof(Note_t)); ++i) {
			music_play_note(i);
		}
	}

}

void ece353_MKII_Buzzer_Init() {
	// Setting P2.7 to be GPIO Output
	P2->DIR |= BIT7;

	// Configuring TIMER_A on buzzer for PWM
	// P2.7 - TA0.4
	P2->SEL0 |= BIT7;
	P2->SEL1 &= ~BIT7;

	// Turning off TA0
	TIMER_A0->CTL = 0;

	// Configuring TIMER_A0.4
	TIMER_A0->CCTL[4] = TIMER_A_CCTLN_OUTMOD_7; // Setting it to RESET/SET mode
	TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK; // Selecting master clock

}

//***************************************************************
// Helper function that runs Timer32_1 to busy wait for the
// number of ticks passed to it
//***************************************************************
void func_timer32(uint32_t ticks) {
	//Load ticks
	TIMER32_1->LOAD = ticks;

	//Enable timer
	TIMER32_1->CONTROL |= TIMER32_CONTROL_ENABLE;

	//Busy wait until timer hits 0
	while (TIMER32_1->VALUE != 0)
		;
}
