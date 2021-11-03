#include "MKL46Z4.h"
#include "Gpio.h"


void GPIO_Init(void)
{
    /* Enable clock for PORTE */
    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

    /* Configure multiplex of PTE29 as GPIO */
    PORTE->PCR[29] &= ~( PORT_PCR_MUX_MASK );
    PORTE->PCR[29] |= PORT_PCR_MUX(1U);

    /* Configure PTE29 as output */
    GPIOE->PDDR |= GPIO_PDDR_PDD(RED_LED_Msk);

    /* Clear PTE29 */
    RED_LED_OFF;

    /* Enable clock for PORTA */
    SIM->SCGC5 |= ( SIM_SCGC5_PORTA_MASK );

    /* Configure multiplex of PTA1 and PTA2 as UART0 */
    PORTA->PCR[1] &= ~( PORT_PCR_MUX_MASK );
    PORTA->PCR[1] |= PORT_PCR_MUX(2U);
    PORTA->PCR[2] &= ~( PORT_PCR_MUX_MASK );
    PORTA->PCR[2] |= PORT_PCR_MUX(2U);

}

void SwitchInit(void)
{
    /* Enable clock for PORTC */
    SIM->SCGC5 |= ( SIM_SCGC5_PORTC_MASK );

    /* Configure multiplex of PTC3 as GPIO */
    PORTC->PCR[3] |= PORT_PCR_MUX(1U);

    /* Configure PTC3 as input */
    GPIOC->PDDR &= ~( SW_Msk );

    /* Enable internal pull resistor for PTC3 */
    PORTC->PCR[3] |= PORT_PCR_PE_MASK;

    /* Enable internal pullup resistor for PTC3 and PTC12 */
    PORTC->PCR[3] |= PORT_PCR_PS_MASK;

}
