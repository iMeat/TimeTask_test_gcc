#include <stdio.h>
#include <stdint.h>
#include <windows.h>

#define SLOT_DURATION_TIME	1000000	//us
#define TIME_TASK_LIST_SIZE 32
#define EXIT_VALUE          1
#define NO_EXIT_VALUE       0

#define TEST_TimeTask_SearchByTime          0
#define TEST_TimeTask_SearchNextIndexByTime 0
#define TEST_Other_tests                   1


#if TEST_TimeTask_SearchByTime == 1 || TEST_TimeTask_SearchNextIndexByTime == 1 || TEST_Other_tests == 1
#define ARRAY_STEP 50
#endif//TEST_TimeTask_SearchByTime == 1 || TEST_TimeTask_SearchNextIndexByTime == 1

//===============
#define TIME_TASK_ERROR 255

typedef uint8_t timeTaskNr_t;
typedef void(*FunctionCallback_t)(void);

typedef struct
{
	uint32_t time;//in us
//	FunctionCallback_t FunctionCallback;
}timeTask_t;


timeTask_t timeTaskList[TIME_TASK_LIST_SIZE];
uint8_t timeTaskCounter = 0;
uint32_t number = 0;

uint8_t isExit = 0;

//  uint8_t CheckList(t_list* list, uint8_t  size, uint16_t number);

//FUNCTION PROTOTYPES
void PrintTimeTaskList(uint8_t size);
void PrintMenu(void);
uint8_t menuParser(void);
timeTaskNr_t TimeTask_SearchByTime(timeTask_t *timeTaskList, uint8_t size, uint32_t time);
timeTaskNr_t TimeTask_SearchNextIndexByTime(timeTask_t *timeTaskList, uint8_t size, uint32_t time);
timeTaskNr_t TimeTask_Add(uint32_t time, FunctionCallback_t functionCallback);
uint8_t TimeTask_ListClear(void);
uint8_t TimeTask_DelByNr(timeTaskNr_t timeTaskNr);

//

int main(void)
{
#if TEST_TimeTask_SearchByTime == 1 || TEST_TimeTask_SearchNextIndexByTime == 1
  //init array
  for(uint8_t i = 0; i < TIME_TASK_LIST_SIZE; i++)
  {
    timeTaskList[i].time = i * ARRAY_STEP;
  }
#endif //TEST_TimeTask_SearchByTime == 1 || TEST_TimeTask_SearchNextIndexByTime == 1



#if TEST_TimeTask_SearchByTime == 1
  printf("Test TimeTask_SearchByTime()\r\n");
  PrintTimeTaskList(TIME_TASK_LIST_SIZE);
  printf("Enter number for search: ");
  scanf("%u", &number);
   printf("\r\n");
  uint8_t result = TimeTask_SearchByTime(list, TIME_TASK_LIST_SIZE, number);
  if(result != TIME_TASK_ERROR)
  {
    printf("Index: %02d\r\n", result);
  }
  else
  {
    printf("Error\r\n");
  }
#endif //TEST_SearchTimeTaskByTime

#if TEST_TimeTask_SearchNextIndexByTime == 1
  printf("Test TimeTask_SearchNextTime()\r\n");
  PrintTimeTaskList(TIME_TASK_LIST_SIZE);
  printf("Enter number for search: ");
  scanf("%u", &number);
   printf("\r\n");
  uint8_t result = TimeTask_SearchNextIndexByTime(timeTaskList, TIME_TASK_LIST_SIZE, number);
  if(result != TIME_TASK_ERROR)
  {
    printf("Index: %02d\r\n", result);
  }
  else
  {
    printf("Error\r\n");
  }
#endif //TEST_TimeTask_SearchNextIndexByTime

#if TEST_Other_tests == 1
  while(isExit != EXIT_VALUE)
    {
      PrintTimeTaskList(TIME_TASK_LIST_SIZE);      
      PrintMenu();     
      isExit = menuParser();
    }
    return 0;
}
#endif //TEST_Other_tests == 1


//==========================================================================
void PrintTimeTaskList(uint8_t size)
{
  printf("===============================================\r\n");
  printf("TIME TASK LIST\r\n");  
  printf("===============================================\r\n");
  printf("Index:\t\t");  
  for(uint8_t i = 0; i< size; i++)
  {
    printf("%04d ", i);
  }
  printf("\r\n");       
  printf("Time:\t\t");  
  for(uint8_t i = 0; i< size; i++)
  {
    printf("%04d ", timeTaskList[i].time);
  }
  printf("\r\n");
  // printf("FunctionCb:\t");  
  // for(uint8_t i = 0; i< size; i++)
  // {
  //   printf("%04d ", timeTaskList[i].FunctionCallback);
  // }
  // printf("\r\n");
  printf("===============================================\r\n");
}
//==========================================================================
void PrintMenu(void)
{
  printf("MENU:\r\n");
  printf("1 - TimeTask_Add()\r\n");
  printf("2 - TimeTask_ListClear()\r\n");
  printf("3 - TimeTask_DelByNr()\r\n");     
  printf("E - exit\r\n");
  printf("===============================================\r\n"); 
}
//==========================================================================
uint8_t menuParser(void)
{
  char letter;
  printf("Enter letter:\r\n"); 
  scanf(" %c", &letter);
  switch(letter)
  {
  //============   
  case '1':
  printf("TimeTask_Add()\r\n");
  uint32_t time = 0;
  FunctionCallback_t *cb = NULL;
  while(time == 0)
  {
    printf("Enter time valie:\r\n");
    scanf("%u", &time);
    if(time > SLOT_DURATION_TIME)
    {
      time = 0;
      printf("Invalid time valie!\r\n");  
    } 
  }
uint8_t status = TimeTask_Add(time, &PrintMenu);
if(status == TIME_TASK_ERROR)
{
printf("TimeTask_Add() return ERROR\r\n");  
}
else
{
  printf("TimeTask_Add() retrun code %03d\r\n",status);  
}

  return NO_EXIT_VALUE;     
  break;
  //============
  case '2':
  TimeTask_ListClear();
  break;
  //============  
  case '3':
  {
  uint8_t taskNumber = 0;
  uint8_t status = 0;
  printf("Enter Task number valie: \r\n");
  scanf("%u", &taskNumber);
  status = TimeTask_DelByNr(taskNumber);
  if(status == TIME_TASK_ERROR)
  {
   printf("TimeTask_DelByNr() return ERROR\r\n"); 
  }
  else
  {
   printf("TimeTask_DelByNr() return %03d\r\n", status);  
  }
  break;
  //============
  } 
  case 'E':
  return EXIT_VALUE;
  //============ 
  default:
  printf("Invalid letter value!\r\n");
  return NO_EXIT_VALUE;
  }
  
}
//==========================================================================
/*Binary search task by time value.
 * IN: timeTask_t *timeTaskList - array for search
 * IN: uint8_t size - size of array: 1...254
 * OUT: timeTaskNr_t:
 * 0...254 - number of task
 * 255 - error: no such index in array
*/
timeTaskNr_t TimeTask_SearchByTime(timeTask_t *timeTaskList, uint8_t size, uint32_t time)
{
#define MAX_ITERATIONS 8//for an array of 255 elements 8 iterations are enough
	if(size == 0 || size > 255)
	{
    return TIME_TASK_ERROR;
  }
	if(time > SLOT_DURATION_TIME)
	{
    return TIME_TASK_ERROR;
  }
	uint8_t start = 0;
	uint8_t mid  = 0;
	uint8_t end = size;
	//start---mid---end
	for(uint8_t i = 0; i < MAX_ITERATIONS; i++)
	{
		mid = start + (end - start) / 2;
		if(time > timeTaskList[mid].time)
		//mid-*-end
		{
			start = mid + 1;
		}
		else
		//start-*-mid
		{
			end = mid;
		}
		if(start == end)
		{
			if(timeTaskList[start].time == time)
			{
				return start; //start value is 	desired index
			}
			else
			{
				return TIME_TASK_ERROR;
			}
		}
	}
	return TIME_TASK_ERROR; //no such index in array
}

//==========================================================================
/*Binary search next time task.
 * IN: timeTask_t *timeTaskList - array for search
 * IN: uint8_t size - size of array: 1...254
 * OUT: timeTaskNr_t:
 * 0...254 - number of task next by time
 * 255 - error: no such index in array
*/
timeTaskNr_t TimeTask_SearchNextIndexByTime(timeTask_t *timeTaskList, uint8_t size, uint32_t time)
{
#define MAX_ITERATIONS 8//for an array of 255 elements 8 iterations are enough
//	if(size == 0 || size > 255)
 	if(size > 255) 
	{
    return TIME_TASK_ERROR;
  }
	if(time > SLOT_DURATION_TIME)
	{
    return TIME_TASK_ERROR;
  }
  if(size == 0)
  {
    return 0;
  }
	uint8_t start = 0;
	uint8_t mid  = 0;
	uint8_t end = size;
	//start---mid---end
	for(uint8_t i = 0; i < MAX_ITERATIONS; i++)
	{
		mid = start + (end - start) / 2;
		if(time > timeTaskList[mid].time)
		//mid-*-end
		{
			start = mid + 1;
		}
		else
		//start-*-mid
		{
			end = mid;
		}
		if(start == end)
		{
      return start; //start value is 	desired index
		}
	}
	return TIME_TASK_ERROR; //no such index in array
}

//==========================================================================
/* Add task to the end of TimeTask array
 * IN: uint32_t time - time: (0 ... SLOT_DURATION_TIME) in us
 * IN: FunctionCallback_t FunctionCallback - function that will be callbacked when time elapsed
 * OUT: timeTaskNr_t - Returned task number in the list
 * 0...254 - task number
 * 255 - error
*/
timeTaskNr_t TimeTask_Add(uint32_t time, FunctionCallback_t functionCallback)
{
	//check time
	if(time > SLOT_DURATION_TIME)
	{
		return TIME_TASK_ERROR;
	}
	//check list overflow
	if(timeTaskCounter == TIME_TASK_LIST_SIZE)
	{
		return TIME_TASK_ERROR;
	}
	//check callback
	if(functionCallback == NULL)
	{
		return TIME_TASK_ERROR;
	}
	//search next time index
	uint8_t i = TimeTask_SearchNextIndexByTime(timeTaskList, timeTaskCounter, time);
  printf("TimeTask_SearchNextIndexByTime() return code %03d\r\n", i);
  if(i == TIME_TASK_ERROR)
  {
    return TIME_TASK_ERROR;
  }
  //check time
  if(timeTaskList[i].time == time)
  {
    return TIME_TASK_ERROR;
  }
	//shift right tasks
  	timeTaskCounter++;
    for(uint8_t j = timeTaskCounter; j > i; j--)
    {
      timeTaskList[j] = timeTaskList[j - 1];
    }
	//add new task
	timeTaskList[i].time = time;
//	timeTaskList[i].FunctionCallback = functionCallback;
	return timeTaskCounter;
}
//==========================================================================
uint8_t TimeTask_ListClear(void)
{
	for(uint8_t i = 0;i < timeTaskCounter; i++)
	{
		timeTaskList[i].time = 0;
//		timeTaskList[i].FunctionCallback = NULL;
//		timeTaskList[i].isActive = 0;
	}
	timeTaskCounter = 0;
	return 0;
}
//==========================================================================
/*Delite task by number by shifting all lefter indexes
 * IN: timeTaskNr_t timeTaskNr - 0...254
 * OUT: uint
*/
uint8_t TimeTask_DelByNr(timeTaskNr_t timeTaskNr)
{
  //check timeTaskNr
  if(timeTaskNr > timeTaskCounter || timeTaskNr > TIME_TASK_LIST_SIZE)
  {
    return TIME_TASK_ERROR;
  }
  //left shift from timeTaskCounter index to timeTaskNr index
	for(uint8_t i = timeTaskNr; i < timeTaskCounter; i++)
	{
		timeTaskList[i] = timeTaskList[i+1];
	}
	timeTaskCounter--;
	return 0;//ok
}
//==========================================================================
