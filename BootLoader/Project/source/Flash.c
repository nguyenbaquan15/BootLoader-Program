#include "stdint.h"
#include "MKL46Z4.h"
#include "Flash.h"


/* ----------------------------Private define --------------------------------*/

#define    BYTE_PER_SECTOR    1024

/* ---------------------------------------------------------------------------*/


FlashStatus  FlashEraseSector(uint32_t Address)
{
    FlashStatus Status=FLASH_OK;

    /* Previous command complete? */
    while(((FTFA->FSTAT) & FTFA_FSTAT_CCIF_MASK)==0 );

    /* Results from previous command: Access Error and Protection Violation Check */
    if((((FTFA->FSTAT) & FTFA_FSTAT_ACCERR_MASK)!=0) || (((FTFA->FSTAT) & FTFA_FSTAT_FPVIOL_MASK)!=0) )
    {
        /* Clear the old errors */
        FTFA->FSTAT |= FTFA_FSTAT_ACCERR_MASK | FTFA_FSTAT_FPVIOL_MASK;
    }

    /* Key Command Erase: 0x09 */
    FTFA->FCCOB0=0x09;

    /* Address need Erase: type big endian */
    FTFA->FCCOB1= (uint8_t)(Address>>16);
    FTFA->FCCOB2= (uint8_t)(Address>>8);
    FTFA->FCCOB3= (uint8_t)Address;

    /* Clear the CCIF to launch the command */
    FTFA->FSTAT |= FTFA_FSTAT_CCIF_MASK;

    /* Wait command complete */
    while(((FTFA->FSTAT) & FTFA_FSTAT_CCIF_MASK)==0 );

    /* Check Error */
    if(((FTFA->FSTAT) & FTFA_FSTAT_ACCERR_MASK)!=0)
    {
        Status=FLASH_ERROR_ACCERR;
    }
    else if(((FTFA->FSTAT) & FTFA_FSTAT_FPVIOL_MASK)!=0)
    {
        Status=FLASH_ERROR_PROTECTION;
    }
    else if(((FTFA->FSTAT) & FTFA_FSTAT_RDCOLERR_MASK)!=0)
    {
        Status=FLASH_ERROR_READ;
    }
    else
    {
        Status=FLASH_OK;
    }

    return Status;
}

FlashStatus  FlashEraseMultiSector(uint32_t Address, uint8_t Number)
{
    uint8_t index;
    FlashStatus Status=FLASH_OK;

    for(index=0;index<Number;index++)
    {
        Status=FlashEraseSector(Address+(index * BYTE_PER_SECTOR));
    }

    return Status;
}

FlashStatus FlashProgramLongword(uint32_t Address, char *data)
{
    FlashStatus Status=FLASH_OK;

    /* Previous command complete? */
    while(((FTFA->FSTAT) & FTFA_FSTAT_CCIF_MASK)==0 );

    /* Results from previous command: Access Error and Protection Violation Check */
    if((((FTFA->FSTAT) & FTFA_FSTAT_ACCERR_MASK)!=0) || (((FTFA->FSTAT) & FTFA_FSTAT_FPVIOL_MASK)!=0) )
    {
        /* Clear the old errors */
        FTFA->FSTAT |= (FTFA_FSTAT_ACCERR_MASK|FTFA_FSTAT_FPVIOL_MASK);
    }

    /* Key Command Write: 0x06 */
    FTFA->FCCOB0=0x06;

    /* Address need Write: type big endian */
    FTFA->FCCOB1= (uint8_t)(Address>>16);
    FTFA->FCCOB2= (uint8_t)(Address>>8);
    FTFA->FCCOB3= (uint8_t)Address;

    /* Data need Write: type big endian */
    FTFA->FCCOB4=data[3];    /* Byte 3 data */
    FTFA->FCCOB5=data[2];    /* Byte 2 data */
    FTFA->FCCOB6=data[1];    /* Byte 1 data */
    FTFA->FCCOB7=data[0];    /* Byte 0 data */

    /* Disable interrupt UART0 */
    NVIC_DisableIRQ(UART0_IRQn);

    /* Clear the CCIF to launch the command */
    FTFA->FSTAT = FTFA_FSTAT_CCIF_MASK;

    /* Wait command complete */
    while(((FTFA->FSTAT)==0x00 ));

    /* Enable interrupt UART0 */
    NVIC_EnableIRQ(UART0_IRQn);

    /* Check Error */
    if(((FTFA->FSTAT) & FTFA_FSTAT_ACCERR_MASK)!=0)
    {
        Status=FLASH_ERROR_ACCERR;
    }
    else if(((FTFA->FSTAT) & FTFA_FSTAT_FPVIOL_MASK)!=0)
    {
        Status=FLASH_ERROR_PROTECTION;
    }
    else if(((FTFA->FSTAT) & FTFA_FSTAT_RDCOLERR_MASK)!=0)
    {
        Status=FLASH_ERROR_READ;
    }
    else
    {
        Status=FLASH_OK;
    }

    return Status;
}