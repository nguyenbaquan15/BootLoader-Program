#ifndef __SREC__
#define __SREC__


/* ----------------------------External functions ----------------------------*/

/*******************************************************************************
 * @brief: Function Check file srec: character, bytecount, checksum and datalinecount
 * @param: -str: string character of a line in srec file
 * @retval: pass or fail
 ******************************************************************************/
bool CheckFileSrec(char *str);


/*******************************************************************************
 * @brief: Function Check file srec: character, bytecount, checksum and datalinecount
 * @param: -str: string character of a line in srec file
 * @retval: pass or fail
 ******************************************************************************/
uint8_t WriteSrecToFlash(char *s, uint8_t *finish);

/* ---------------------------------------------------------------------------*/

#endif /* __SREC__ */
