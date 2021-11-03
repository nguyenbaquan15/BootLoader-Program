#ifndef __UART__
#define __UART__


/* ----------------------------Private typedef -------------------------------*/

typedef void (*PtrQueuePush)(char);

/* ---------------------------------------------------------------------------*/


/* ----------------------------External functions ----------------------------*/

/*******************************************************************************
 * @brief: Function init for UART 
 * @param:-Clock: clock supply for module UART, uint: Hz
 *        -Baudrate: speed tranffer data of UART
 * @retval: None
 ******************************************************************************/
void UART_Init(void);


/*******************************************************************************
 * @brief: Function update clock system
 * @param: SystemClock: Clock setup for system: default, 24Mhz, 48Mhz
 * @retval: None
 ******************************************************************************/
void UART0_ClockUpdate(void);


/*******************************************************************************
 * @brief: Function update clock as clock of App
 * @param: None
 * @retval: None
 ******************************************************************************/
void UART0_ClockApp(void);


/*******************************************************************************
 * @brief: Function transmitter a character
 * @param:-c: character need transmitter
 * @retval: None
 ******************************************************************************/
void UART0_Putchar(char c);


/*******************************************************************************
 * @brief: Function transmitter a string
 * @param:-str: character string need transmitter
 * @retval: None
 ******************************************************************************/
void UART0_PutString(char *str);


/*******************************************************************************
 * @brief: Function callback Push() function in Queue.c
 * @param:-Address: Address of function
 * @retval: None
 ******************************************************************************/
void UARTCallPushQueue(PtrQueuePush Address);


/*******************************************************************************
 * @brief: Function receiver a character from UART
 * @param: None
 * @retval: Character receivered from UART
 ******************************************************************************/
char UART0_Getchar(void);


/*******************************************************************************
 * @brief: Function receiver a character string from UART
 * @param: *str: address of string
 * @retval: None
 ******************************************************************************/
void UART0_GetString(char *str);

/* ---------------------------------------------------------------------------*/

#endif /* __UART__ */