#include "../lib/RR.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * RR ist als Beispiel vorgegeben
 */

static queue_object *RR_queue;
static int quant;
static int timeinquant;



process *RR_tick(process *running_process)
{
    if (running_process == NULL || running_process->time_left == 0 || timeinquant >= quant) {
        timeinquant = 0; 
        if (running_process != NULL && running_process->time_left > 0) {
            queue_add(running_process, RR_queue); 
        }
        running_process = queue_poll(RR_queue);
    }
    if (running_process != NULL) {
        timeinquant++;
        running_process->time_left--;
    }

    return running_process;
}

int RR_startup(int quantum)
{
    quant = quantum;
    timeinquant = 0;
    RR_queue = new_queue();
    if (RR_queue == NULL) {
        return 1;
    }
    return 0;
}

process *RR_new_arrival(process *arriving_process, process *running_process)
{
    if (arriving_process != NULL) {
        queue_add(arriving_process, RR_queue);
    }
    return running_process;
}

void RR_finish()
{
    free_queue(RR_queue);
}