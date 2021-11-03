#ifndef __QUEUE__
#define __QUEUE__


/* ----------------------------Private define --------------------------------*/

/* Number max element of queue */
#define    MAXQUEUE       4U

/* Number max character of element */
#define    ELEMENT_MAX    80U

/* ---------------------------------------------------------------------------*/

/* ----------------------------Private typedef -------------------------------*/

/* Data type element of queue */
typedef struct Queue
{
	char Element[ELEMENT_MAX];
	uint8_t Full;

} Queue_t;

/* Status of queue */
typedef enum queuestatus
{
    PASS,
    QUEUE_FULL,
    QUEUE_EMPTY

} QueueStatus_t;

/* ---------------------------------------------------------------------------*/

/* ----------------------------External functions ----------------------------*/

/*******************************************************************************
 * @brief: Function init queue
 * @param: None
 * @retval: None
 ******************************************************************************/
void Queue_Init(void);


/*******************************************************************************
 * @brief: Function push data to queue
 * @param: -c: character need push into queue
 * @retval: None
 ******************************************************************************/
void Queue_Push(char c);


/*******************************************************************************
 * @brief: Function get address data from queue
 * @param: None
 * @retval: Address of a element in queue
 ******************************************************************************/
char *Queue_Peek(void);


/*******************************************************************************
 * @brief: Function pop data from queue
 * @param: None
 * @retval: None
 ******************************************************************************/
void Queue_Pop(void);


/*******************************************************************************
 * @brief: Function check status of queue
 * @param: None
 * @retval: Status of queue: empty, ok, full
 ******************************************************************************/
QueueStatus_t Queue_Status(void);

/* ---------------------------------------------------------------------------*/

#endif /* __QUEUE__ */
