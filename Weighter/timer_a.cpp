#include "define.h"
#include "task.h"
#include "interrupt.h"

extern INT8U  ContainerType_change;
extern INT8U  b_node_configured;
extern INT8U  wakeUp_from_wdt;
extern INT8U  channel_getted ;
extern void quit_lp_mode(void);
extern void enter_lp_mode(void);
// 100ms peroid
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
	static INT8U   SKeyRole       = 0;
	static INT8U   FKeyPressCnt   = 0;
	static INT8U   SKeyPressCnt   = 0;
	if (FUNCT_BUTTON_DW){
		if (++FKeyPressCnt >= L_PRESS_INTVL){
			FKeyPressCnt = 0;
			SET_FLG(bNeedCalibrate);
			TIMER_A_STOP;
		}
	}
	else if (START_BUTTON_DW){
		if (++SKeyPressCnt >= L_PRESS_INTVL){
			SKeyPressCnt = 0;
			SET_FLG(bNeedConfig);
			channel_getted = 0;
			disable_wdt();
			wakeUp_from_wdt = 0;
			TIMER_A_STOP;
		}
	}
	else if (IsClr(ButtonIntSrc) && FUNCT_BUTTON_UP){
		if (S_PRESS_INTVL <= FKeyPressCnt && FKeyPressCnt < L_PRESS_INTVL){
			if(b_node_configured == 1){
				ContainerType_change = 1;
			}
			//FKeyPressCnt = 0;
		}// this is a signal jitter
		//{
			FKeyPressCnt = 0;
		//}
		TIMER_A_STOP;
	}
	else if (IsSet(ButtonIntSrc) && START_BUTTON_UP){
		if (S_PRESS_INTVL <= SKeyPressCnt && SKeyPressCnt < L_PRESS_INTVL){
			if(b_node_configured == 1){
				if (IsClr(SKeyRole)){
					SET_FLG(SKeyRole);
					SET_FLG(bStart);
				}
				else{
					CLR_FLG(SKeyRole);
					SET_FLG(bStop);
				}
			}
			//SKeyPressCnt = 0;
		}
		// this is a signal jitter
		//else{
			SKeyPressCnt = 0;
		//}
		TIMER_A_STOP;
	}
	else{
		enter_lp_mode();
		TIMER_A_STOP;
	}
}
