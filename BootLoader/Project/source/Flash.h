#ifndef __FLASH__
#define __FLASH__


/* ----------------------------Private typedef -------------------------------*/

/* Status of Flash */
typedef enum flashstatus
{
    FLASH_OK=0,
    FLASH_ERROR_READ,
    FLASH_ERROR_ACCERR,
    FLASH_ERROR_PROTECTION

} FlashStatus;

/* ---------------------------------------------------------------------------*/


/* ----------------------------External functions ----------------------------*/

/*******************************************************************************
 * @brief: Function Erase a sector in flash
 * @param: Address: start address need erase
 * @retval: Status of flash: OK, ERROR_READ, ERROR_ACCERR and ERROR_PROTECTION
 ******************************************************************************/
FlashStatus  FlashEraseSector(uint32_t Address);


/*******************************************************************************
 * @brief: Function Erase many sector in flash
 * @param: Address: start address need erase
 * @param: Number: Number sector need erase
 * @retval: Status of flash: OK, ERROR_READ, ERROR_ACCERR and ERROR_PROTECTION
 ******************************************************************************/
FlashStatus  FlashEraseMultiSector(uint32_t Address, uint8_t Number);


/*******************************************************************************
 * @brief: Function Erase write data to flash
 * @param: Address: start address need write data
 * @param: *data: Buffer 4 byte data
 * @retval: Status of flash: OK, ERROR_READ, ERROR_ACCERR and ERROR_PROTECTION
 ******************************************************************************/
FlashStatus FlashProgramLongword(uint32_t Address, char *data);

/* ---------------------------------------------------------------------------*/

#endif /* __FLASH__ */
