#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "Srec.h"
#include "Flash.h"


/* ----------------------Private function prototypes -------------------------*/

/* Check byte count file srec */
static bool CheckByteCount(char *s);

/* Check sum file srec */
static bool CheckSum(char *s);

/* Check data line count file srec */
static bool CheckDataLineCount(char *s);

/* Convert from character to hexa */
static char HexaConvert(char hexa);

/* Convert n byte from character to hexa */
static uint32_t HexaConvertDec(char *s, uint8_t Position, uint8_t Number);

/* ---------------------------------------------------------------------------*/


bool CheckByteCount(char *s)
{
    uint8_t ByteCount=0;
    uint8_t Count=0;
    uint8_t index;
    bool Check=false;

    /* Convert byte count from hexa to dec */
    ByteCount=(HexaConvert(s[2])<<4)+HexaConvert(s[3]);

    /* Count number character after ByteCount */
    for(index=4;s[index+1]!='\n';index++)
    {
        Count++;
    }

    /* Check Byte Count */
    if((ByteCount*2)==Count)
    {
        Check=true;
    }
    return Check;

}

bool CheckSum(char *s)
{
    bool Check=true;
    uint32_t CheckSum=0;
    uint8_t index;

    for(index=2;s[index+1]!='\n';index+=2)
    {
        CheckSum+=(HexaConvert(s[index])<<4)+HexaConvert(s[index+1]);
    }

    if((CheckSum & 0xFF) != 0xFF)
    {
        Check = false;
    }

    return Check;
}

bool CheckDataLineCount(char *s)
{
    bool check=true;
    static uint16_t ValueDataLineCount=0;
    static uint16_t LineCount=0;
    char RecordType;
    uint16_t ByteCount;
    uint8_t ByteData;
    uint8_t IsCheckS5=0;

    RecordType =HexaConvert(s[1]);

    /* Find S5 */
    if(RecordType==5)
    {
        /* Have S5 in srec file */
        IsCheckS5=1;

        /* Calculate byte count of S5 */
        ByteCount=(HexaConvert(s[2])<<4)+HexaConvert(s[3]);

        /* Calculate data in S5 */
        ByteData=(ByteCount-1);

        /* Calculate Value Data Line Count in S5 */
        ValueDataLineCount= HexaConvertDec(s,4,ByteData);
    }

    /* Find S1 or S2 or S3 */
    if(RecordType==1 || RecordType==2 || RecordType==3)
    {
        LineCount++;
    }

    /* End line in srec file */
    if(RecordType==7 || RecordType==8 || RecordType==9)
    {
        /* Compare LineCount with value in S5 */
        if((IsCheckS5==1)&&(ValueDataLineCount!=LineCount))
        {
            check=false;
        }
        else
        {
            check=true;
        }

        /* Reset value for send next file srec */
        ValueDataLineCount=0;
        LineCount=0;
    }

    return check;
}

char HexaConvert(char hexa)
{
    uint8_t result;

    /*Convert character 0-9 => hexa 0-9 */
    if((hexa>='0')&&(hexa<='9'))
    {
        result=hexa-'0';
    }
    /*Convert character A-Z => hexa A-Z */
    else
    {
        result=hexa-'A'+10;
    }

    return result;
}

uint32_t HexaConvertDec(char *s, uint8_t Position, uint8_t ByteData)
{
    uint32_t result=0;
	uint8_t index;
    uint8_t Number=ByteData*2;

	for(index=0;index<Number;index++)
	{
        /* Convert hexa to decimal */
		result=result+(((uint32_t)HexaConvert(s[Position+index]))<<((Number-1-index)<<2));
	}

	return result;
}

bool CheckFileSrec(char *str)
{
    bool Check[4];
    bool TotalCheck=false;

    /* Check Byte Count */
    Check[0] = CheckByteCount(str);

    /* Check sum */
    Check[1] = CheckSum(str);

    /* Check Data Line Count */
    Check[2] = CheckDataLineCount(str);

    if(Check[0]&Check[1]&Check[2])
    {
        TotalCheck=true;
    }
    else
    {
        TotalCheck=false;
    }

    return TotalCheck;
}

uint8_t WriteSrecToFlash(char *s, uint8_t *finish)
{
    uint8_t ByteCount=0;
    uint8_t idata=0;
    uint8_t jdata=0;
    uint8_t index=0;
    uint8_t RecordType;
    uint8_t check=0;
    uint32_t Address;
    char Buffer[4];
    uint8_t ByteData;

    /* Calculate Byte Count in a line */
    ByteCount=(HexaConvert(s[2])<<4)+HexaConvert(s[3]);

    /* Calculate record type of line */
    RecordType =HexaConvert(s[1]);

    /* Calculate byte data of line */
    ByteData= (ByteCount-3);

    /* End line in Srec file */
    if(RecordType==7 || RecordType==8 || RecordType==9)
    {
        *finish=1;
    }

    /* If record type: S1 or S3 */
    if((RecordType==1)||(RecordType==3))
    {
        /* Calculate address */
        Address =(((uint32_t)HexaConvert(s[4]))<<12) | (((uint32_t)HexaConvert(s[5]))<<8)\
                |(((uint32_t)HexaConvert(s[6]))<<4) | ((uint32_t)HexaConvert(s[7]));

        /* Write data to Flash */
        while(idata<ByteData)
        {
            /* Delete Buffer */
            for(index=0;index<4;index++)
            {
                Buffer[index]=0xFF;
            }

            /* Get 4 byte data */
            for(index=0;((index<4)&&(idata<ByteData));index++,jdata+=2)
            {
                Buffer[index]=((HexaConvert(s[jdata+8])<<4) | (HexaConvert(s[jdata+9])));
                idata++;
            }

            /* Write 4 byte data to Flash */
            FlashProgramLongword(Address,Buffer);
            Address=Address+4U;
        }
        check=1;
    }

    return check;

}



