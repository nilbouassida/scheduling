#include "../lib/PRIOP.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * PRIOP ist als Beispiel vorgegeben
 */

static queue_object *PRIOP_queue;

process *PRIOP_tick(process *running_process)
{
	if (running_process == NULL || running_process->time_left == 0) {
		running_process = queue_poll(PRIOP_queue);
	}
	if (running_process != NULL) {
		running_process->time_left--;
	}

	return running_process;
}

int PRIOP_startup()
{
	PRIOP_queue = new_queue();
	if (PRIOP_queue == NULL) {
		return 1;
	}
	return 0;
}

int append(process *new_process, queue_object *queue) {
    if (queue == NULL || new_process == NULL) {
        return 1;
    }
    queue_object* new = malloc(sizeof(queue_object));
	new->object = new_process;
	new->next = NULL;
    queue_object* current = queue;
    while (current->next != NULL && ((process*)current->next->object)->priority >= new_process->priority) {
        current = current->next;
    }
    new->next = current->next;
    current->next = new;

    return 0;
}

process *PRIOP_new_arrival(process *arriving_process, process *running_process)
{
    if (arriving_process != NULL) {
        append(arriving_process, PRIOP_queue);
    }
    if (running_process == NULL || (arriving_process != NULL && arriving_process->priority > running_process->priority)) {
        if (running_process != NULL && running_process->time_left > 0) {
            append(running_process, PRIOP_queue);
        }
        running_process = queue_poll(PRIOP_queue);
    }
    return running_process;
}

void PRIOP_finish()
{
	free_queue(PRIOP_queue);
}
