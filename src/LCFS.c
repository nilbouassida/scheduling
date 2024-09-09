#include "../lib/LCFS.h"

static queue_object *LCFS_queue;


process *LCFS_tick(process *running_process)
{
	if (running_process == NULL || running_process->time_left == 0) {
		running_process = queue_poll(LCFS_queue);
	}
	if (running_process != NULL) {
		running_process->time_left--;
	}

	return running_process;
}

int LCFS_startup()
{
	LCFS_queue = new_queue();
    if (LCFS_queue == NULL) {
        return 1;
    }
    return 0;
}

process *LCFS_new_arrival(process *arriving_process, process *running_process)
{
	if (arriving_process != NULL) {
		// queue_add(arriving_process, LCFS_queue);
		// queue_object* new_obj = (queue_object*)malloc(sizeof(queue_object));
		// if (new_obj != NULL) {
		// 	new_obj->object = arriving_process;
		// 	new_obj->next = NULL;
		// }


		queue_object* new = malloc(sizeof(queue_object));
		if(new){
			new->object = arriving_process;
			new->next = LCFS_queue->next;
			LCFS_queue->next = new;
		}

	}
	return running_process;
}

void LCFS_finish()
{
	// TODO
	free_queue(LCFS_queue);
}
