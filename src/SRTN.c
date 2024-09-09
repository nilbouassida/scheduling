#include "../lib/SRTN.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * SRTN ist als Beispiel vorgegeben
 */

static queue_object *SRTN_queue;

process *SRTN_tick(process *running_process)
{
	if (running_process == NULL || running_process->time_left == 0) {
		running_process = queue_poll(SRTN_queue);
	}
	if (running_process != NULL) {
		running_process->time_left--;
	}

	return running_process;
}

int SRTN_startup()
{
	SRTN_queue = new_queue();
	if (SRTN_queue == NULL) {
		return 1;
	}
	return 0;
}

static int append1(process *new_process, queue_object *queue) {
    if (queue == NULL || new_process == NULL) {
        return 1;
    }
    queue_object* new = malloc(sizeof(queue_object));
	new->object = new_process;
	new->next = NULL;
    queue_object* current = queue;
    while (current->next != NULL && ((process*)current->next->object)->time_left <= new_process->time_left) {
        current = current->next;
    }
    new->next = current->next;
    current->next = new;

    return 0;
}

process *SRTN_new_arrival(process *arriving_process, process *running_process)
{
    if (arriving_process != NULL) {
        append1(arriving_process, SRTN_queue);
    }
    if (running_process == NULL || (arriving_process != NULL && arriving_process->time_left < running_process->time_left)) {
        if (running_process != NULL && running_process->time_left > 0) {
            append1(running_process, SRTN_queue);
        }
        running_process = queue_poll(SRTN_queue);
    }
    return running_process;
}

void SRTN_finish()
{
	free_queue(SRTN_queue);
}
