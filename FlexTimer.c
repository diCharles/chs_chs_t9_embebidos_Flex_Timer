/*
 * FlexTimer.c
 *
 *  Created on: 25/03/2019
 *      Author: Diego Charles and Ruben Charles
 *     brief: implementation of functions in FlexTimer.h
 */
#include "FlexTimer.h"

void FlexTimer_update_channel_value(int16_t channel_value)
{
	/**Assigns a new value for the duty cycle*/
	FTM0->CONTROLS[0].CnV = channel_value;
}


void FlexTimer_Init(flexTimer_channels_t channel)
{

	/**Clock gating for FlexTimers*/
	SIM->SCGC6 |= SIM_SCGC6_FTM0(1);

	/**Selects the FTM behavior in BDM mode.In this case in functional mode pag 1076*/
	FTM0->CONF |= FTM_CONF_BDMMODE(3);

	/*select flexTimer channel mode and configuration*/
	Fleximer_mode(CHANNEL_0,PWM ,CLEAR_OUTPUT_ON_MATCH);
	FlexTimer_mod(CHANNEL_0, 20);
	/*select flexTimer frequency with the new three function calls*/
	FlexTimer_update_channel_value(16);
	FlexTimer_clockSource_and_prescaler(CHANNEL_0,FLEX_TIMER_CLKS_1,FLEX_TIMER_PS_1);

}
void Fleximer_mode(flexTimer_channels_t channel, flexTimer_modes_t mode, flexTimer_mode_configurations_t config)
{

	switch(mode)
	{
	case OUTPUT_COMPARE:
		/**It enable the FTM*/
		FTM0->MODE |= FTM_MODE_FTMEN_MASK;


		/* setting  output compare configuration*/
		if( TOGGLE_OUTPUT_ON_MATCH== config)
			/**Configure FlexTimer in output compare in toggle mode*/
			FTM0->CONTROLS[0].CnSC |= FTM0->CONTROLS[0].CnSC |= FTM_CnSC_MSA(1)| FTM_CnSC_ELSA(1);
		else if(CLEAR_OUTPUT_ON_MATCH== config)
		{
			/**Configure FlexTimer in output compare in clear output on match*/
			FTM0->CONTROLS[0].CnSC |=   FTM_CnSC_MSA(1) |FTM_CnSC_ELSB(1);
			FTM0->CONTROLS[0].CnSC &=~(FTM_CnSC_ELSA(1));
		}
		else if(SET_OUTPUT_ON_MATCH== config)
			/**Configure FlexTimer in output compare  in set output on match*/
			FTM0->CONTROLS[0].CnSC |=   FTM_CnSC_MSA(1) |FTM_CnSC_ELSB(1) | FTM_CnSC_ELSA(1);
		break;

	case TIMER_OVERFLOW:
		FTM0->MODE |= FTM_MODE_FTMEN_MASK;
		/**Selects the FTM behavior in BDM mode.In this case in functional mode*/
		FTM0->CONF |= FTM_CONF_BDMMODE(3);
		/**Enabling the interrupt*/
		FTM0->SC |=  FTM_SC_TOIE(1);
		break;
	case PWM		   :
		/**When write protection is enabled (WPDIS = 0), write protected bits cannot be written.
		 * When write protection is disabled (WPDIS = 1), write protected bits can be written.*/
		FTM0->MODE |= FTM_MODE_WPDIS_MASK;
		/**Enables the writing over all registers*/
		FTM0->MODE &= ~ FTM_MODE_FTMEN_MASK;
		/**Selects the Edge-Aligned PWM mode mode*/
		FTM0->CONTROLS[0].CnSC = FTM_CnSC_MSB(1) | FTM_CnSC_ELSB(1);
		/**Assign a duty cycle of 50%*/



		break;

	}

}
void FlexTimer_clockSource_and_prescaler(flexTimer_channels_t channel, uint8_t clockSource, uint8_t prescaler )
{
	/* for  SC register descrption view page 961 */
	/* clock source selction CLKS
	 * 00 No clock selected. This in effect disables the FTM counter.
	  01 System clock
	  10 Fixed frequency clock
	  11 External clock*/
	/* prescaler selection PS
	 * 000 Divide by 1
		001 Divide by 2
		010 Divide by 4
		011 Divide by 8
		100 Divide by 16
		101 Divide by 32
		110 Divide by 64
		111 Divide by 128*/


	switch(channel)
	{
	case CHANNEL_0:
		FTM0->SC |= FTM_SC_CLKS(clockSource)| FTM_SC_PS(prescaler);
		break;
	case CHANNEL_1:
		FTM1->SC |= FTM_SC_CLKS(FLEX_TIMER_CLKS_1)| FTM_SC_PS(FLEX_TIMER_PS_128);
		break;
	case CHANNEL_2:
		FTM2->SC |= FTM_SC_CLKS(FLEX_TIMER_CLKS_1)| FTM_SC_PS(FLEX_TIMER_PS_128);
		break;
	default:
		break;
	}


}
void FlexTimer_mod(flexTimer_channels_t channel, uint16_t modulo)
{
	switch(channel)
	{
	case CHANNEL_0:
		FTM0->MOD=modulo;
		break;
	case CHANNEL_1:
		FTM1->MOD=modulo;
		break;
	case CHANNEL_2:
		FTM2->MOD=modulo;
		break;
	default:
		break;
	}
}


