#include "stdbool.h"
#include "string.h"
#include "MKL46Z4.h"
#include "Srec.h"
#include "Boot.h"
#include "Gpio.h"
#include "Uart.h"
#include "Queue.h"
#include "Flash.h"


/* ----------------------------Private define --------------------------------*/

#define    ADDRESS_APP                (0xA000)
#define    ADDRESS_STACK_POINTER      (*(( volatile uint32_t *)ADDRESS_APP))
#define    ADDRESS_VECTOR_TABLE       (((((uint32_t) ADDRESS_APP) & 0xFFFFFF)<<7U)&SCB_VTOR_TBLOFF_Msk)
#define    ADDRESS_RESET_HANDLER      ((PtrResetHandler)(*(( volatile uint32_t *)(ADDRESS_APP+4U))))

/* ---------------------------------------------------------------------------*/

/* ----------------------Private function prototypes -------------------------*/

/* Check choose and password user */
static uint8_t CheckUserChoose(void);

/* ---------------------------------------------------------------------------*/


void BootRunBootLoader(void)
{
    /* Pointer return address of a line in file srec */
    char *PtrLine;
    uint8_t TranfferFinish=0;
    uint8_t CheckSrec=0;
    uint8_t ErrorSrec=0;

    UART0_PutString(" =======================================\n");
    UART0_PutString("|                   BOOT LOADER PROGRAM !              |\n");
    UART0_PutString("|                    Password: nguyenbaquan                   |\n");
    UART0_PutString("|   Notice: Tick +CR when send file and character   |\n");
    UART0_PutString(" =======================================\n");

    /* Check choose and password user */
    if(CheckUserChoose()==1)
    {
        /* Disable interrupt UART0 */
        NVIC_DisableIRQ(UART0_IRQn);

        /* Erase 2 sector in flash */
        FlashEraseMultiSector(ADDRESS_APP,2);

        /* Enable interrupt UART0 */
        NVIC_EnableIRQ(UART0_IRQn);

        while(TranfferFinish==0)
        {
            /* Wait receiver Srec file */
            while(Queue_Status()== QUEUE_EMPTY);

            /* Peek a line of file srec from Queue */
            PtrLine=Queue_Peek();

            /* Check a line file srec: check_sum, byte_count and data_line_count */
            CheckSrec=CheckFileSrec(PtrLine);

            /* Pop to next line */
            Queue_Pop();

            /* If Srec file no error */
            if(CheckSrec==true)
            {
                UART0_PutString(">");

                /* Write data of srec to address of flash */
                WriteSrecToFlash(PtrLine,&TranfferFinish);
            }
            /* If Srec file error */
            else
            {
                /* File srec is error */
                ErrorSrec=1;
                UART0_PutString("error\r\n");

                /* On red led */
                RED_LED_ON;
            }

            /* Check write srec file complete */
            if(TranfferFinish==1)
            {
                /* Srec file no error */
                if(ErrorSrec==0)
                {
                    UART0_PutString("100%\n");
                    UART0_PutString("\nBoot Loader complete!\n");
                    UART0_PutString("\nPlease press reset button to run App!\n");
                }
                /* Srec file error */
                else
                {
                    UART0_PutString("\nSrec file error !");
                    UART0_PutString(" Boot Loader fail!\n");

                    /* Disable interrupt UART0 */
                    NVIC_DisableIRQ(UART0_IRQn);

                    /* Erase 2 sector in flash */
                    FlashEraseMultiSector(ADDRESS_APP,2);

                    /* Enable interrupt UART0 */
                    NVIC_EnableIRQ(UART0_IRQn);

                    UART0_PutString("Please upload file srec!\n");

                    /* Return first while loop */
                    TranfferFinish=0;

                    /* Reset condition for run next loop */
                    ErrorSrec=0;
                }
            }
        }
    }
    /* If character not 'Y' or 'y' */
    else
    {
        UART0_PutString("\nYou cancel upload program!\n");
        UART0_PutString("\nPlease press reset button to run App!\n");
    }
}

void BootRunApplication(void)
{
    /* function pointer for run ResetHandler function */
    typedef void (*PtrResetHandler)(void);
    PtrResetHandler JumpToApp;

    /********************************************************
    * Move Vector Table to user application at 0xA000
    * Write 0xA000 to TBLOFF field of VTOR register
    ********************************************************/
    SCB->VTOR |= ADDRESS_VECTOR_TABLE;

    /* Change the main stack pointer */
    __set_MSP(ADDRESS_STACK_POINTER);

    /* Call ResetHandler function for run user application */
    JumpToApp= ADDRESS_RESET_HANDLER;
    JumpToApp();

}

uint8_t CheckUserChoose(void)
{
    char *PtrLine;
    uint8_t CheckChoose;
    uint8_t CheckPass;
    char PassWord[20]="nguyenbaquan\r\n";
    uint8_t CheckTotal=0;

    UART0_PutString("Are you want upload program?(Y/N?)\n");
    UART0_PutString("  Y: Upload     N: Exit and run app\n");

    /* Wait user confirm choose */
    while(Queue_Status()== QUEUE_EMPTY);
    PtrLine=Queue_Peek();
    Queue_Pop();

    if((PtrLine[0]=='Y') || (PtrLine[0]=='y'))
    {
        CheckChoose=1;

        /* Check password */
        do
        {
            UART0_PutString("\nPlease input Password:\n");

            /* Wait user confirm choose */
            while(Queue_Status()== QUEUE_EMPTY);
            PtrLine=Queue_Peek();

            /* Compare user's password with default password */
            CheckPass=strcmp(PtrLine,PassWord);

            Queue_Pop();

            /* if password correct */
            if(CheckPass==0)
            {
                UART0_PutString("Password Ok! Please upload file srec!\n");
            }
            /* if password wrong */
            else
            {
                UART0_PutString("Password Fail!\n");
                UART0_PutString("Press a character for continue or press reset for Exit!\n");

                /* Wait user confirm choose */
                while(Queue_Status()== QUEUE_EMPTY);
                Queue_Pop();
                CheckPass=1;
            }
        } while(CheckPass==1);

        /* Both choose user and PassWord correct */
        if((CheckChoose==1) && (CheckPass==0))
        {
            CheckTotal=1;
        }
    }
    /* user choose different 'Y' or 'y' */
    else
    {
        CheckTotal=0;
    }

    return CheckTotal;

}