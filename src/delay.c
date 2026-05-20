#include <systick.h>
#include <delay.h>

void delay_ms(int ms)
{
    *STK_CTLR = 0;
    *STK_SR = 0;
    *STK_CMPLR = 144000*ms;
    *STK_CTLR = STK_B_INIT|STK_B_CLK|STK_B_EN;
    while( (*STK_SR & STK_B_CNT )==0);
    *STK_CTLR = 0;
}
