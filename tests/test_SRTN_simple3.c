#include"../lib/scheduler.h"
#include"../lib/process.h"
#include "helpers.h"
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

#define PROCESS_COUNT   4
#define RR_QUANTUM      2
#define STRATEGY        SRTN

int main()
{
    printf("Checks if SRTN(P) works for simple process.");
    /*Defining the processes                Arrival Dur.    Prio    ID
                                            uint    uint    uint    char*/
    process processes[PROCESS_COUNT] = {{   0,      7,      9,      'A'},
                                        {    1,      5,      8,      'B'},
                                        {    2,      3,      7,      'C'},
                                        {    3,      1,      6,      'Z'}};

    
    char* expected_result = "ABCZCCBBBBAAAAAA";


    print_schedule_info(PROCESS_COUNT, STRATEGY, 0, processes);

    char* resulting_schedule = scheduler(processes,PROCESS_COUNT,STRATEGY,RR_QUANTUM);

    int err = compare_result(resulting_schedule, expected_result);
    free(resulting_schedule);
    exit(err);
}
