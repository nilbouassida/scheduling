
#include "../lib/MLF.h"
#include <stdio.h>
#include <stdlib.h>

static queue_object **MLF_queues;
static int quantums[] = {1, 2, 4, 8};
static int time_in_quantum;
static int current_level;

int append_to_queue(process *new_process, queue_object *queue)
{
    if (!queue || !new_process) {
        return 1;
    }
    queue_object *new_node = malloc(sizeof(queue_object));
    if (new_node == NULL) {
        return 1;
    }

    new_node->object = new_process;
    new_node->next = NULL;

    queue_object *current = queue;
    while (current->next && ((process *)current->next->object)->priority >= new_process->priority) {
        current = current->next;
    }
    new_node->next = current->next;
    current->next = new_node;

    return 0;
}

process *MLF_tick(process *running_process)
{
    if (!running_process || running_process->time_left == 0 || time_in_quantum >= quantums[current_level]) {
        time_in_quantum = 0;

        if (running_process && running_process->time_left > 0) {
            if (current_level < 3) {
                append_to_queue(running_process, MLF_queues[current_level + 1]);
            } else {
                append_to_queue(running_process, MLF_queues[current_level]);
            }
        }

        running_process = NULL;
        for (int i = 0; i < 4; i++) {
            running_process = queue_poll(MLF_queues[i]);
            if (running_process) {
                current_level = i;
                break;
            }
        }
    }

    if (running_process) {
        time_in_quantum++;
        running_process->time_left--;
    }

    return running_process;
}

int MLF_startup()
{
    MLF_queues = malloc(4 * sizeof(queue_object*));
    if (!MLF_queues) {
        return 1;
    }

    for (int i = 0; i < 4; i++) {
        MLF_queues[i] = new_queue();
        if (!MLF_queues[i]) {
            for (int j = 0; j < i; j++) {
                free_queue(MLF_queues[j]);
            }
            free(MLF_queues);
            return 1;
        }
    }

    time_in_quantum = 0;
    current_level = 0;

    return 0;
}

process *MLF_new_arrival(process *arriving_process, process *running_process)
{
    if (arriving_process != NULL) {
        append_to_queue(arriving_process, MLF_queues[0]);
    }

    if (running_process == NULL || (arriving_process && arriving_process->priority < running_process->priority)) {
        if (running_process && running_process->time_left > 0) {
            append_to_queue(running_process, MLF_queues[current_level]);
        }

        running_process = NULL;
        for (int i = 0; i < 4; i++) {
            running_process = queue_poll(MLF_queues[i]);
            if (running_process) {
                current_level = i;
                break;
            }
        }
    }

    return running_process;
}

void MLF_finish()
{
    for (int i = 0; i < 4; i++) {
        free_queue(MLF_queues[i]);
    }
    free(MLF_queues);
}
