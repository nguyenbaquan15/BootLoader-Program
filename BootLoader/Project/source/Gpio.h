#ifndef __GPIO__
#define __GPIO__


/* ----------------------------Public define ---------------------------------*/

/* Position of red led */
#define RED_LED_Msk         0x20000000U

/* Position of SW1 button */
#define SW_Msk              0x00000008U

/* Control led */
#define RED_LED_ON          (GPIOE->PCOR |= GPIO_PCOR_PTCO(RED_LED_Msk))
#define RED_LED_OFF         (GPIOE->PSOR |= GPIO_PSOR_PTSO(RED_LED_Msk))
#define SW_BOOT_LOADER      (GPIOC->PDIR & SW_Msk)

/* ---------------------------------------------------------------------------*/

/* ----------------------------External functions ----------------------------*/
/*******************************************************************************
 * @brief: Function init for GPIO
 * @param: None
 * @retval: None
 ******************************************************************************/
void GPIO_Init(void);


/*******************************************************************************
 * @brief: Function init for SW1 button
 * @param: None
 * @retval: None
 ******************************************************************************/
void SwitchInit(void);

/* ---------------------------------------------------------------------------*/

#endif /* __GPIO__ */