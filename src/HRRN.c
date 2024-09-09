#include "../lib/HRRN.h"
#include "../lib/queue.h"
#include <stdlib.h>
#include <stdio.h>

static queue_object *HRRN_queue;
static unsigned int current_time = 0;

process *find_next(queue_object *queue) {
    if (queue == NULL || queue->next == NULL) {
        return NULL;
    }

    queue_object *current = queue->next;
    queue_object *previous = queue;
    queue_object *highest_priority_prev = NULL;
    queue_object *highest_priority_node = NULL;
    double highest_rr = -1.0;

    while (current != NULL) {
        process *proc = (process *)current->object;
        unsigned int wait_time = current_time - proc->start_time;
        double rr = (double)(wait_time + proc->time_left) / proc->time_left;

        if (rr > highest_rr) {
            highest_rr = rr;
            highest_priority_prev = previous;
            highest_priority_node = current;
        }

        previous = current;
        current = current->next;
    }

    if (highest_priority_node != NULL) {
        highest_priority_prev->next = highest_priority_node->next;
        process *next_process = (process *)highest_priority_node->object;
        free(highest_priority_node);
        return next_process;
    }

    return NULL;
}

process *HRRN_tick(process *running_process)
{
    if (running_process == NULL || running_process->time_left == 0) {
        running_process = find_next(HRRN_queue);
    }
    if (running_process != NULL) {
        running_process->time_left--;
    }
    current_time++;

    return running_process;
}

int HRRN_startup()
{
    HRRN_queue = new_queue();
    if (HRRN_queue == NULL) {
        return 1;
    }
    current_time = 0;
    return 0;
}

process *HRRN_new_arrival(process *arriving_process, process *running_process)
{
    if (arriving_process != NULL) {
        queue_add(arriving_process, HRRN_queue);
    }
    return running_process;
}

void HRRN_finish()
{
    free_queue(HRRN_queue);
}
