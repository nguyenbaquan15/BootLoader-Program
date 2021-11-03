#include "stdbool.h"
#include "string.h"
#include "MKL46Z4.h"
#include "Gpio.h"
#include "Uart.h"
#include "Queue.h"
#include "Srec.h"
#include "Boot.h"
#include "Flash.h"


void main()
{
    /* Init for GPIO */
    GPIO_Init();

    /* Init for SW1 */
    SwitchInit();

    /* Configure clock system */
    UART0_ClockUpdate();

    /* Init for UART */
    UART_Init();

    /* Init for Queue */
    Queue_Init();

    /* Enable interrupt UART0 */
    NVIC_EnableIRQ(UART0_IRQn);

    /* Callback function Push() */
    UARTCallPushQueue(&Queue_Push);

    /*******************************************************
    * Instruction to Boot Loader:
    *  + Press reset-> SW1-> release reset -> release SW1
    * 
    ********************************************************/

    /* Run Boot Loader */
    if(SW_BOOT_LOADER==0)
    {
        BootRunBootLoader();
    }
    else
    {
        /* Set Clock as default system clock for run with clock of app */
        UART0_ClockApp();

        /* Run User Application */
        BootRunApplication();
    }

    while(1);

}
