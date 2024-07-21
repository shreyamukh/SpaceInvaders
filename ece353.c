#include "ece353.h"

//initialize hardware resources used to control LED1
void ece353_init_led1(void)
{
	//set dir as an output
	P1->DIR |= BIT0;
	//LED off
	P1->OUT &= ~BIT0;

}
//initialize hardware resources used to control Button1
void ece353_init_button1(void)
{
	//Pin 1.1 to be input
	P1->DIR &= ~BIT1;
	//Internal Pull Up 
	P1->REN |= BIT1;
	//Select pull up
	P1->OUT |= BIT1;
}

//led1 on/off
//true, LED on; false, LED off
void ece353_led1(bool on)
{
	if (on)
	{
		//LED On
		P1->OUT |= BIT0;
	}
	else
	{
		//LED off
		P1->OUT &= ~BIT0;
	}
}

//Button is pressed
bool ece353_button1(void)
{
	if ((P1->IN & BIT1 ) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
//****************************************************
//****************************************************
// 02-ICE-Code-Organization
//****************************************************
//****************************************************
/*****************************************************
 * Initialize hardware resources used to control RGBLED
 *****************************************************/
void ece353_rgb_init(void)
{
	P2->DIR |= BIT0; //Red to output
	P2->DIR |= BIT1; //Green to output
	P2->DIR |= BIT2; //Blue to output

	//All lights start off as off
	P2->OUT &= ~BIT0;
	P2->OUT &= ~BIT1;
	P2->OUT &= ~BIT2;
}
/*****************************************************
 * Turn RGBLED ON/Off.
 *
 * Parameters
 *  red_on      :   if true,  turn RGBLED.RED on
 *                  if false, turn RGBLED.RED off
 *  green_on    :   if true,  turn RGBLED.GREEN on
 *                  if false, turn RGBLED.GREEN off
 *  blue_on     :   if true,  turn RGBLED.BLUE on
 *                  if false, turn RGBLED.BLUE off
 *****************************************************/
void ece353_rgb(bool red_on, bool green_on, bool blue_on)
{
	if (red_on)
	{ // red
		P2->OUT |= BIT0;
		P2->OUT &= ~BIT1;
		P2->OUT &= ~BIT2;
	}
	else if (green_on)
	{ // green
		P2->OUT &= ~BIT0;
		P2->OUT |= BIT1;
		P2->OUT &= ~BIT2;
	}
	else if (blue_on)
	{ // blue
		P2->OUT &= ~BIT0;
		P2->OUT &= ~BIT1;
		P2->OUT |= BIT2;
	}
	else
	{
		P2->OUT &= ~BIT0;
		P2->OUT &= ~BIT1;
		P2->OUT &= ~BIT2;
	}
}
/*****************************************************
 * Initialize hardware resources used to control Button2
 *****************************************************/
void ece353_button2_init(void)
{
	//Pin 1.4 to be input
	P1->DIR &= ~BIT4;
	//Internal Pull Up
	P1->REN |= BIT4;
	//Select pull up
	P1->OUT |= BIT4;
}
/*****************************************************
 * Returns if Button2 is currently pressed.
 *
 * Parameters
 *
 * Returns
 *      true    :   Button2 is pressed
 *      false   :   Button2 is NOT pressed
 *****************************************************/
bool ece353_button2(void)
{
	if ((P1->IN & BIT4 ) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
 * waits for a certain number of clock cycles
 *
 * Parameters
 * 	ticks	:	Number of ticks to wait
 * Returns
 * 	None
 */
void ece353_T32_wait(uint32_t ticks)
{
	//stop the timer
	TIMER32_1->CONTROL = 0;
	//set the timer to be 32 bit, one shot
	TIMER32_1->CONTROL = TIMER32_CONTROL_ONESHOT | TIMER32_CONTROL_SIZE;
	//set the load Register
	TIMER32_1->LOAD = ticks;
	//Start the timer
	TIMER32_1->CONTROL |= TIMER32_CONTROL_ENABLE;
	//wait till zero
	while (TIMER32_1->VALUE != 0)
	{
		//wait
	}
}

//*****************************************************************************
//*****************************************************************************
// ICE 03 - Timer32_1 Push Button Detection
//*****************************************************************************
//*****************************************************************************
/*****************************************************
 * Busy waits for 100mS and then returns.
 *
 * Timer32_1 MUST be configured as a 16-bit timer.
 * Assume that the MCU clock runs at 3MHz.  You will
 * need to use a pre-scalar in order to achieve a delay
 * of 100mS.
 *
 * Parameters:
 *      None
 * Returns
 *      None
 *****************************************************/
void ece353_T32_1_wait_100mS(void)
{
	//stop the timer
	TIMER32_1->CONTROL = 0;
	//set the timer to be 16 bit, one shot
	TIMER32_1->CONTROL = TIMER32_CONTROL_ONESHOT | TIMER32_CONTROL_PRESCALE_1; //16 bit
	//set the load Register
	TIMER32_1->LOAD = ((3000000 * 0.1) / 16);
	//Start the timer
	TIMER32_1->CONTROL |= TIMER32_CONTROL_ENABLE;
	//wait till zero
	while (TIMER32_1->VALUE != 0)
	{
		//wait
	}
}
/*****************************************************
 * Debounces Button1 using Timer32_1.
 * This function does not return until Button 1 has
 * been pressed for a minimum of 5 seconds.
 *
 * Waiting 5 Seconds will require you to call
 * ece353_T32_1_wait_100mS multiple times.
 *
 * Parameters
 *      None
 * Returns
 *      None
 *****************************************************/
void ece353_button1_wait_for_press(void)
{
	int i;
	for (i = 0; i < 50; i++)
	{
		if (ece353_button1())
		{
			ece353_T32_1_wait_100mS();
		}
		else
		{
			i = 0;
		}

	}
}

/* PWM a magnetic buzzer
 * Initialize hardware to control S1
 */
void ece353_MKII_S1_Init(void)
{
	//input
	P5->DIR &= ~BIT1;

	//enable resistor
	P5->REN |= BIT1;

	//resistor as a pull up
	P5->OUT |= BIT1;
}

/*
 * Initialize hardware to control S2
 */
void ece353_MKII_S2_Init(void)
{
	//input
	P3->DIR &= ~BIT5;

	//enable resistor
	//P3->REN |= BIT5;

	//resistor as a pull up
	//P3->OUT |= BIT5;
}

/*****************************************************
 * Returns if MKII.S1 is currently pressed.
 *
 * Parameters
 *
 * Returns
 *      true    :   S1 is pressed
 *      false   :   S1 is NOT pressed
 *****************************************************/
bool ece353_MKII_S1(void)
{
	if ((P5->IN & BIT1 ) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*****************************************************
 * Returns if MKII.S2 is currently pressed.
 *
 * Parameters
 *
 * Returns
 *      true    :   S2 is pressed
 *      false   :   S2 is NOT pressed
 *****************************************************/
bool ece353_MKII_S2(void)
{
	if ((P3->IN & BIT5 ) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*****************************************************
 * Sets the PWM of the buzzer
 * 50% duty cycle
 *
 * Parameters
 *		ticks_period 	:	period of pwm pulse
 * Returns
 *
 *****************************************************/
/**
void ece353_MKII_Buzzer_Init(uint16_t ticks_period)
{
	//Set 2.7 to be GPIO OUT
	P2->DIR |= BIT7;

	//The TIMERA PWM controller will control the buzzer
	P2->SEL0 |= BIT7;
	P2->SEL1 &= ~BIT7;

	//Turn off TA0
	TIMER_A0->CTL = 0;

	//Period
	TIMER_A0->CCR[0] = ticks_period ;

	//50% duty cycle
	TIMER_A0->CCR[4] = (ticks_period / 2) ;

	//Configure TA0.4 for reset/set
	TIMER_A0->CCTL[4] = TIMER_A_CCTLN_OUTMOD_7;

	//set master clock as timer source
	TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK;
}
*/
/*****************************************************
 * Buzzer on
 *
 * Parameters
 *
 * Returns
 *
 *****************************************************/
void ece353_MKII_Buzzer_On(void)
{
	//clear current mode bits
	TIMER_A0->CTL &= ~TIMER_A_CTL_MC_MASK;

	//a set mode control and clear the current count
	TIMER_A0->CTL |= TIMER_A_CTL_MC__UP | TIMER_A_CTL_CLR;
}
/*****************************************************
 * Buzzer off
 *
 * Parameters
 *
 * Returns
 *
 *****************************************************/
void ece353_MKII_Buzzer_Off(void)
{
	TIMER_A0->CTL &= ~TIMER_A_CTL_MC_MASK;
}

/*****************************************************
 * Buzzer status
 *
 * Parameters
 *
 * Returns
 *		true    :   TimerA0 is on
 *      false   :   TimerA0 is off
 *****************************************************/
/**
bool ece353_MKII_Buzzer_Run_Status(void)
{
	if ((TIMER_A0->CTL & TIMER_A_CTL_MC_MASK) == TIMER_A_CTL_MC__STOP)
	{
		return false;
	}
	else
	{
		return true;
	}
}
**/

//*****************************************************************************
//*****************************************************************************
// ICE 04 - PWMing MKII tri-color LED.
//*****************************************************************************
//*****************************************************************************
/*****************************************************
 * Initialize the 3 GPIO pins that control the RGB
 * LED on the MKII.
 *
 * Parameters
 *      None
 * Returns
 *      None
 *****************************************************/
void ece353_MKII_RGB_IO_Init(bool en_primary_function)
{
	// Complete the comments below to identify which pins
	// control which LEDs.
	//
	// Replace a and c with the correct port number
	// Replace b and d with the correct pin numbers.
	// RED      : P2.6
	// GREEN    : P2.4
	// BLUE     : P5.6
	//
	// ADD CODE that configures the RED, GREEN, and
	// BLUE LEDs to be outputs
	P2->DIR |= BIT6;
	P2->DIR |= BIT4;
	P5->DIR |= BIT6;

	// ADD CODE that selects the Primary module function
	// for all 3 pins
	if (en_primary_function)
	{
		// Primary module function for Red
		P2->SEL0 |= BIT6;
		P2->SEL1 &= ~BIT6;
		// green
		P2->SEL0 |= BIT4;
		P2->SEL1 &= ~BIT4;
		// blue
		P5->SEL0 |= BIT6;
		P5->SEL1 &= ~BIT6;
	}
}
/*****************************************************
 * Sets the PWM levels for the MKII RGBLED
 *
 * Parameters
 *      ticks_period    :   Period of PWM Pulse
 *      red_num         :   RED RGB Number
 *      green_num       :   GREEN RGB Number
 *      blue_num        :   BLUE RGB Number
 * Returns
 *      None
 *****************************************************/
void ece353_MKII_RGB_PWM(uint16_t ticks_period, uint16_t red_num,
							uint16_t green_num, uint16_t blue_num)
{
	// This code converts the HTML color codes into a
	// number of clock ticks used to generate the duty cycle of
	// the PWM signal.
	uint16_t ticks_red_on = (red_num * ticks_period) / 255;
	uint16_t ticks_green_on = (green_num * ticks_period) / 255;
	uint16_t ticks_blue_on = (blue_num * ticks_period) / 255;

	// Initialize the IO pins used to control the
	// tri-color LED.
	ece353_MKII_RGB_IO_Init(true);

	// Complete the comments below that identify which
	// TimerA outputs will control the IO pins.
	//
	// Replace w and y with the correct TimerA number
	// Replace x and z with the correct CCR number.
	// RED      <--> TA0.3
	// GREEN    <--> TA0.1
	// BLUE     <--> TA2.1
	// ADD CODE BELOW
	//Turn off TA0
	TIMER_A0->CTL = 0;
	//Turn off TA2
	TIMER_A2->CTL = 0;

	//TA0 period
	TIMER_A0->CCR[0] = ticks_period;
	//TA2 period
	TIMER_A2->CCR[0] = ticks_period;

	//Configure TA0.3 duty cycle
	TIMER_A0->CCR[3] = (ticks_red_on);
	//Configure TA0.1 duty cycle
	TIMER_A0->CCR[1] = (ticks_green_on);
	//Configure TA2.1 duty cycle
	TIMER_A2->CCR[1] = (ticks_blue_on);

	//Configure TA0.3 for reset/set
	TIMER_A0->CCTL[3] = TIMER_A_CCTLN_OUTMOD_7;
	//Configure TA0.1 for reset/set
	TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;
	//Configure TA2.1 for reset/set
	TIMER_A2->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;

	//set master clock as timer source
	TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK;
	TIMER_A2->CTL = TIMER_A_CTL_SSEL__SMCLK;

	//TA0 on
	//clear current mode bits
	TIMER_A0->CTL &= ~TIMER_A_CTL_MC_MASK;
	//a set mode control and clear the current count
	TIMER_A0->CTL |= TIMER_A_CTL_MC__UP | TIMER_A_CTL_CLR;

	//TA2 on
	//clear current mode bits
	TIMER_A2->CTL &= ~TIMER_A_CTL_MC_MASK;
	//a set mode control and clear the current count
	TIMER_A2->CTL |= TIMER_A_CTL_MC__UP | TIMER_A_CTL_CLR;

	// Turn off the timer peripherals
	// Set the period for both TimerA peripherals.
	// Configure RED PWM duty cycle
	// Configure GREEN PWM duty cycle
	// Configure BLUE PWM duty cycle
	// Set the OUT MODE to be mode 7 for each
	// PWM output
	// Turn the first TimerA peripheral
	// Turn the second TimerA peripheral

}

/*****************************************************
 * Configures Timer32_1 to generate a periodic interrupt
 *
 * Parameters
 *      ticks	:	ms per interrupt+
 * Returns
 *      None
 *****************************************************/
void ece353_T32_1_Interrupt_Ms(uint16_t ms)
{
	//ticks = desired period / core clock period
	//20e-3/(1/3e6) = (3e6*20)/1000
	uint32_t ticks = ((SystemCoreClock * ms) / 1000) - 1;

	//stop the timer
	TIMER32_1->CONTROL = 0;

	//set load register
	TIMER32_1->LOAD = ticks;

	//enable the Timer32 interrupt in NVIC
	__enable_irq();
	NVIC_EnableIRQ(T32_INT1_IRQn);
	NVIC_SetPriority(T32_INT1_IRQn, 0);

	//Start the Timer32 and enable interrupt
	TIMER32_1->CONTROL = TIMER32_CONTROL_ENABLE | //timer on
			TIMER32_CONTROL_MODE |	//periodic mode
			TIMER32_CONTROL_SIZE |	//32 bit
			TIMER32_CONTROL_IE;		//enable interrupts
}
void ece353_T32_2_Interrupt_Ms(uint16_t ms)
{
	//ticks = desired period / core clock period
	//20e-3/(1/3e6) = (3e6*20)/1000
	uint32_t ticks = ((SystemCoreClock * ms) / 1000) - 1;

	//stop the timer
	TIMER32_2->CONTROL = 0;

	//set load register
	TIMER32_2->LOAD = ticks;

	//enable the Timer32 interrupt in NVIC
	__enable_irq();
	NVIC_EnableIRQ(T32_INT2_IRQn);
	NVIC_SetPriority(T32_INT2_IRQn, 0);

	//Start the Timer32 and enable interrupt
	TIMER32_2->CONTROL = TIMER32_CONTROL_ENABLE | //timer on
			TIMER32_CONTROL_MODE |	//periodic mode
			TIMER32_CONTROL_SIZE |	//32 bit
			TIMER32_CONTROL_IE;		//enable interrupts
}

void ece353_ADC14_PS2_X(void)
{
	//configure P6.0 (A15) the X direction as an analog input pin
	P6->SEL0 |= 0;
	P6->SEL1 |= 0;

	//Configure the ADC Control Register
	//Sample time of 16 ADC cycles for the first 8 analog channels
	//allow the ADC timer to control pulsed samples
	ADC14->CTL0 = ADC14_CTL0_SHP | ADC14_CTL0_SHT02;

	//use sampling timer, 12-bit conversion results
	ADC14->CTL1 = ADC14_CTL1_RES_2;

	//Configure Memory control register so that we can associate the correct
	//analog channel with MEM[0]
	ADC14->MCTL[0] = ADC14_MCTLN_INCH_15;

	//enable interrupts after the conversion of MEM[0] completes
	ADC14->IER0 = ADC14_IER0_IE0;

	//enable ADC Interrupt
	NVIC_EnableIRQ(ADC14_IRQn);

	//Turn the ADC on
	ADC14->CTL0 |= ADC14_CTL0_ON;
}

//*****************************************************************************
//*****************************************************************************
// ICE 06 - ADC14
//*****************************************************************************
/******************************************************************************
 * Configure the IO pins for BOTH the X and Y directions of the analog
 * joystick.  The X direction should be configured to place the results in
 * MEM[0].  The Y direction should be configured to place the results in MEM[1].
 *
 * After BOTH analog signals have finished being converted, a SINGLE interrupt
 * should be generated.
 *
 * Parameters
 *      None
 * Returns
 *      None
 ******************************************************************************/
void ece353_ADC14_PS2_XY(void)
{
	// Configure the X direction as an analog input pin. P6.0
	P6->SEL0 |= BIT0;
	P6->SEL1 |= BIT0;

	// Configure the Y direction as an analog input pin. P4.4
	P4->SEL0 |= BIT4;
	P4->SEL1 |= BIT4;

	// Configure CTL0 to sample 16-times in pulsed sample mode.
	ADC14->CTL0 = ADC14_CTL0_SHP | ADC14_CTL0_SHT0_2;

	// NEW -- Indicate that this is a sequence-of-channels.
	ADC14->CTL0 |= ADC14_CTL0_CONSEQ_1;

	// Configure ADC to return 12-bit values
	ADC14->CTL1 = ADC14_CTL1_RES__12BIT;

	// Associate the X direction analog signal with MEM[0]
	ADC14->MCTL[0] = ADC14_MCTLN_INCH_15;

	// Associate the Y direction analog signal with MEM[1]
	ADC14->MCTL[1] = ADC14_MCTLN_INCH_9;

	// NEW -- Make sure to indicate this is the end of a sequence.
	ADC14->MCTL[1] = ADC14_MCTLN_EOS;

	// Enable interrupts in the ADC AFTER a value is written into MEM[1].
	ADC14->IER0 |= ADC14_IER0_IE15;
	ADC14->IER0 |= ADC14_IER0_IE9;

	// NEW: This is not the same as what is demonstrated in the example
	// coding video.
	// Enable ADC Interrupt in the NVIC
	NVIC_EnableIRQ(ADC14_IRQn);

	// Turn ADC ON
	ADC14->CTL0 |= ADC14_CTL0_ON;

}
/*****************************************************
 * Turn RGB on the MKII LED ON/Off.
 *
 * Parameters
 *  red:    if true,  turn RED LED on
 *          if false, turn RED LED off
 *  green:  if true,  turn GREEN LED on
 *          if false, turn GREEN LED off
 *  blue:   if true,  turn BLUE LED on
 *          if false, turn BLUE LED off
 *****************************************************/
/**
void ece353_MKII_RGB_LED(bool red, bool green, bool blue)
{
	if (red)
	{
		//on
		P2->OUT |= BIT6;
	}
	else
	{
		P2->OUT &= ~BIT6;
	}
	if (green)
	{
		//on
		P2->OUT |= BIT4;
	}
	else
	{
		P2->OUT &= ~BIT4;
	}
	if (blue)
	{
		//on
		P5->OUT |= BIT6;
	}
	else
	{
		P5->OUT &= ~BIT6;
	}
}
*/


//*****************************************************************************
//*****************************************************************************
// ICE 07 - ADC14 Comparator
//*****************************************************************************
#define VOLT_0P85 ((int)(0.85/(3.3/4096)))
#define VOLT_2P50 ((int)(2.5/(3.3/4096)))
/******************************************************************************
 * Configure the IO pins for the X direction of the analog
 * joystick.  The X direction should be configured to place the results in
 * MEM[0].
 *
 * The ADC14 should be configured to generate interrupts using the Window
 * comparator.  The HI0 threshold should be set to 2.5V.  The LO0 threshold
 * should be set to 0.85V.
 *
 * Parameters
 *      None
 * Returns
 *      None
 ******************************************************************************/
void ece353_ADC14_PS2_XY_COMP(void)
{
    // Configure the X direction as an analog input pin.
	P6->SEL0 |= BIT0;
	P6->SEL1 |= BIT0;

    // Configure CTL0 to sample 16-times in pulsed sample mode.
	ADC14->CTL0 = ADC14_CTL0_SHP | ADC14_CTL0_SHT0_2;

    // Indicate that this is a sequence of samples.
	ADC14->CTL0 |= ADC14_CTL0_CONSEQ_1;

    // Configure CTL1 to return 12-bit values
	ADC14->CTL1 = ADC14_CTL1_RES__12BIT;

    // Set up the HI0 Window
	ADC14->HI0 = VOLT_2P50;

    // Set up the LO0 Window
	ADC14->LO0 = VOLT_0P85;

    // Associate the X direction analog signal with MEM[0], indicate the end of sequence,
	// turn on the window comparator
	ADC14->MCTL[0] = ADC14_MCTLN_INCH_15;
	ADC14->MCTL[0] = ADC14_MCTLN_EOS;

    // Enable interrupts when either the HI or LO thresholds of the window
    // comparator has been exceeded.  Disable all other interrupts
	ADC14->IER0 |= ADC14_IER0_IE15;
	ADC14->IER1 |= ADC14_IER1_HIIE;
	ADC14->IER1 |= ADC14_IER1_INIE;

    // Enable ADC Interrupt
	__enable_irq();
	NVIC_EnableIRQ(ADC14_IRQn);

    // Turn ADC ON
	ADC14->CTL0 |= ADC14_CTL0_ON;
}

/*****************************************************
 * initializes the Joystick and the Accelerometer
 *****************************************************/
void ece353_ADC14_PS2_XY_Acc_Init(void)
{
    P6->DIR &= ~BIT1;

    // Configure the X direction as an analog input pin.
    P6->SEL0 |= BIT0;
    P6->SEL1 |= BIT0;

    // Configure the Y direction as an analog input pin.
    P4->SEL0 |= BIT4;
    P4->SEL1 |= BIT4;

    // Configure the X direction of accelerometer
    P6->SEL0 |= BIT1;
    P6->SEL1 |= BIT1;

    // Configure the Y direction of accelerometer
    P4->SEL0 |= BIT0;
    P4->SEL1 |= BIT0;

    // Configure the Z direction of accelerometer
    P4->SEL0 |= BIT2;
    P4->SEL1 |= BIT2;

    // Configure CTL0 to sample 16-times in pulsed sample mode.
    // NEW -- Indicate that this is a sequence-of-channels.
    ADC14->CTL0 = ADC14_CTL0_SHP | ADC14_CTL0_SHT02 | ADC14_CTL0_CONSEQ_1;

    // Configure ADC to return 12-bit values
    ADC14->CTL1 = ADC14_CTL1_RES_2;

    // Associate the X direction analog signal with MEM[0]
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_15;

    // Associate the Y direction analog signal with MEM[1]
    ADC14->MCTL[1] = ADC14_MCTLN_INCH_9;

    // Associate the X direction analog signal with MEM[2]
    ADC14->MCTL[2] = ADC14_MCTLN_INCH_14;

    // Associate the Y direction analog signal with MEM[3]
    ADC14->MCTL[3] = ADC14_MCTLN_INCH_13;

    // Associate the Z direction analog signal with MEM[4]
    ADC14->MCTL[4] = ADC14_MCTLN_INCH_11;

    // NEW -- Make sure to indicate this is the end of a sequence.
    ADC14->MCTL[4] |= ADC14_MCTLN_EOS;

    // Enable interrupts in the ADC AFTER a value is written into MEM[1].
    ADC14->IER0 = ADC14_IER0_IE1;

    // Enable ADC Interrupt in the NVIC
    NVIC_EnableIRQ(ADC14_IRQn);
    NVIC_SetPriority(ADC14_IRQn, 2);

    // Turn ADC ON
    ADC14->CTL0 |= ADC14_CTL0_ON;

    __enable_irq();

}

/*****************************************************
 * setup all the peripherials for the project
 *****************************************************/
void ece353_setup_all(void)
{
    //Buttons
    ece353_MKII_S1_Init();
    ece353_MKII_S2_Init();

    //UART
    serial_debug_init();

    //Joystick and Accelerometer
    ece353_ADC14_PS2_XY_Acc_Init();

    //Buzzer
    ece353_MKII_Buzzer_Init();

    //I2C
    i2c_init();

    //Light Sensor
    light_init();

    //LCD
    lcd_init();
}



