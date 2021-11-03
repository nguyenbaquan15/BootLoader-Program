#include "MKL46Z4.h"
#include "Uart.h"


/* ----------------------------Private define --------------------------------*/

/* Baudrate for UART */
#define    BAURDRATE         115200U

/* Update clock system to 24 Mhz */
#define    CLOCK_SYSTEM      24000000U

/* ---------------------------------------------------------------------------*/

/* ------------------------------Private macro -------------------------------*/

/* Calculate absolute value */
#define    ABS(a,b)    ((a>b)?(a-b):(b-a))

/* ---------------------------------------------------------------------------*/

/* -----------------------------Private variables ----------------------------*/

/* Function pointer for call Push() */
PtrQueuePush CallBackQueuePush;

/* ---------------------------------------------------------------------------*/

/* ----------------------Private function prototypes -------------------------*/

/* Calculate Baudrate auto for UART0 */
static void BaudrateAuto(uint8_t *OSR, uint16_t *SBR);

/* ---------------------------------------------------------------------------*/

void UART_Init(void)
{
    uint8_t ValueOSR;
    uint16_t ValueSBR;
    uint8_t RegisterTemp;

    /* Clock for UART0 from MCGFLLCLK clock */
    SIM->SOPT2 &= ~( SIM_SOPT2_PLLFLLSEL_MASK );

    /* UART0 Clock Source Select: MCGFLLCLK clock */
    SIM->SOPT2 &= ~SIM_SOPT2_UART0SRC_MASK;
    SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1U);

    /* Enable clock for UART0 */
    SIM->SCGC4 |= ( SIM_SCGC4_UART0_MASK );

    /* Disabled transmitter and receiver */
    UART0->C2 &= ~( UART0_C2_TE_MASK | UART0_C2_RE_MASK );

    /* Calculator auto OSR and SBR */
    BaudrateAuto(&ValueOSR,&ValueSBR);

    /* OSR Register: load value OSR */
    /* keep bit: 5,6,7 of register */
    RegisterTemp=(UART0->C4)&0xE0;
    /* Load value OSR */
    UART0->C4 = RegisterTemp |((UART0->C4)&ValueOSR);

    /* Baud Rate Register: load value SBR */
    /* Write 5 bit high */
    UART0->BDH &= ~UART0_BDH_SBR_MASK;
    UART0->BDH |= UART0_BDH_SBR(( ValueSBR & 0x1F00)>>8U );
    /* Write 8 bit low */
    UART0->BDL &= ~UART0_BDL_SBR_MASK;
    UART0->BDL |= UART0_BDL_SBR( ValueSBR & 0xFF );           

    /* 8-bit data characters */
    UART0->C1 &= ~( UART0_C1_M_MASK );
    UART0->C4 &= ~( UART0_C4_M10_MASK );

    /* Disable parity */
    UART0->C1 &= ~( UART0_C1_PE_MASK );

    /* One stop bit */
    UART0->BDH &= ~( UART0_BDH_SBNS_MASK );

    /* MSB First: LSB */
    UART0->S2 &= ~( UART0_S2_MSBF_MASK );

    /* Receive Data Inversion: not inverted */
    UART0->S2 &= ~( UART0_S2_RXINV_MASK );

    /* Transmit Interrupt Enable for TDRE: using polling */
    UART0->C2 &= ~( UART0_C2_TIE_MASK );

    /* Receiver Interrupt Enable for RDRF */
    UART0->C2 |= ( UART0_C2_RIE_MASK );

    /* Enabled transmitter and receiver */
    UART0->C2 |= ( UART0_C2_TE_MASK | UART0_C2_RE_MASK );

}

void UART0_ClockUpdate(void)
{
    /* SystemClock= 24Mhz */
    #if (CLOCK_SYSTEM == 24000000U)
    {
        /* DCO Range Select: Low range */
        MCG->C4 &= ~MCG_C4_DRST_DRS_MASK;

        /* DCO Maximum Frequency with 32.768 kHz Reference */
        MCG->C4 |= MCG_C4_DMX32_MASK;    /* Clock: 24 Mhz */
    }
    /* SystemClock= 48Mhz */
    #elif (CLOCK_SYSTEM  == 48000000U)
    {
        /* DCO Range Select: Mid range */
        MCG->C4 &= ~MCG_C4_DRST_DRS_MASK;
        MCG->C4 |= MCG_C4_DRST_DRS(1U);

        /* DCO Maximum Frequency with 32.768 kHz Reference */
        MCG->C4 |= MCG_C4_DMX32_MASK;    /* Clock: 48 Mhz */
    }
    /* SystemClock= 72Mhz */
    #elif (CLOCK_SYSTEM  == 72000000U)
    {
        /* DCO Range Select: Mid-high range */
        MCG->C4 &= ~MCG_C4_DRST_DRS_MASK;
        MCG->C4 |= MCG_C4_DRST_DRS(2U);

        /* DCO Maximum Frequency with 32.768 kHz Reference */
        MCG->C4 |= MCG_C4_DMX32_MASK;    /* Clock: 48 Mhz */
    }
    /* SystemClock= DEFAULT_SYSTEM_CLOCK = 20971520 Hz */
    #else
    {
        /* do nothing */
    }
    #endif /* __SYSTEM_CLOCK__ */
}

void UART0_ClockApp(void)
{
    /* Set Clock as default system clock for run with clock of app */
    MCG->C4 &= ~MCG_C4_DRST_DRS_MASK;
    MCG->C4 &= ~MCG_C4_DMX32_MASK;
}

void BaudrateAuto(uint8_t *OSR, uint16_t *SBR)
{
	uint8_t iOsr;
	uint32_t BaudrateDown;
	uint32_t BaudrateUp;
	uint16_t SbrTemp;
	uint16_t Delta1;
	uint16_t Delta2;
	uint16_t MinDelta;

	for(iOsr=4;iOsr<=31;iOsr++)
	{
        /* Calculator value SBR
        *  Baudrate = baud clock / ((OSR+1) × BR)
        *  => BR= baud clock/(Baudrate*(OSR+1))
        */
		SbrTemp= CLOCK_SYSTEM/(BAURDRATE*(iOsr+1));

        /* Calculate baudrate when SbrTemp round down */
		BaudrateDown= CLOCK_SYSTEM/((iOsr+1)*SbrTemp);

        /* Calculate baudrate when SbrTemp round up */
		BaudrateUp= CLOCK_SYSTEM/((iOsr+1)*(SbrTemp+1));

        /* Calculate tolerance between 2 baudrate */
		Delta1= ABS(BAURDRATE,BaudrateDown);
		Delta2= ABS(BAURDRATE,BaudrateUp);

        /* Find tolerance min firstly */
		if(iOsr==4)
		{
			if(Delta1<Delta2)
			{
				MinDelta=Delta1;
				*OSR=iOsr;
				*SBR=SbrTemp;
			}
			else
			{
				MinDelta=Delta2;
				*OSR=iOsr;
				*SBR=SbrTemp+1;
			}
		}

        /* Find tolerance min */
		if(Delta1<MinDelta)
		{
			MinDelta=Delta1;
			*OSR=iOsr;
			*SBR=SbrTemp;
		}

        /* Find tolerance min */
		if(Delta2<MinDelta)
		{
			MinDelta=Delta2;
			*OSR=iOsr;
			*SBR=SbrTemp+1;
		}
	}
}

char UART0_Getchar(void)
{
    while((UART0->S1 & UART0_S1_RDRF_MASK)==0);
    return UART0->D;
}

void UART0_GetString(char *str)
{
    uint8_t index=0;
    do
    {
        str[index]=UART0_Getchar();
        index++;
    } while(str[index-1]!='\n');
}

void UART0_Putchar(char c)
{
    while((UART0->S1 & UART0_S1_TDRE_MASK)==0);
    UART0->D = c;
}

void UART0_PutString(char *str)
{
    uint8_t index=0;

    while(*(str+index)!='\0')
    {
        while((UART0->S1 & UART0_S1_TDRE_MASK)==0);
        UART0->D = *(str+index);
        index++;
    }
}

void UARTCallPushQueue(PtrQueuePush Address)
{
    CallBackQueuePush=Address;
}

void UART0_IRQHandler(void)
{
    /* Check receiver flag */
    if((UART0->S1 & UART0_S1_RDRF_MASK)!=0)
    {
        /* Tranffer 1 character to Queue */
        CallBackQueuePush(UART0->D);
    }

    /* Check over run flag */
    if(UART0->S1 & UART_S1_OR_MASK)
    {
        UART0->S1 |= UART_S1_OR_MASK;
        UART0_PutString("Over Run\n");
    }
}
