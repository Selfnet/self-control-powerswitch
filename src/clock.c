/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : clock-arch.c
 *    Description : Implementation of architecture-specific clock functionality
 *
 *    History :
 *    1. Date        : October 4, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: #1 $
**************************************************************************/
#include "clock.h"

volatile clock_time_t Ticks;

/*************************************************************************
 * Function Name: clock_tick
 * Parameters: none
 *
 * Return: none
 *
 * Description: TIM2 interrupt handler
 *
 *************************************************************************/
void clock_tick(void)
{
  ++Ticks;
}

/*************************************************************************
 * Function Name: clock_time
 * Parameters: none
 *
 * Return: none
 *
 * Description: The current clock time, measured in system ticks
 *
 *************************************************************************/
clock_time_t clock_time(void)
{
  return(Ticks);
}
