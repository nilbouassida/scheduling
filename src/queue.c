#include "../lib/queue.h"
#include <stdlib.h>
#include <stdio.h>

int queue_add(void *new_object, queue_object *queue)
{
    if (queue == NULL) {
        return 1;
    }

    queue_object *new_node = (queue_object *)malloc(sizeof(queue_object));
    if (new_node == NULL) {
        return 1;
    }

    new_node->object = new_object;
    new_node->next = NULL;

    queue_object *current = queue;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = new_node;

    return 0;
}


void *queue_poll(queue_object *queue)
{
	if (queue == NULL || queue->next == NULL) {
        return NULL;
    }

    queue_object *old = queue->next;
	void *obj = old->object;

    queue->next = old->next;
	free(old);

    return obj;
	// TODO
}

queue_object *new_queue()
{
	queue_object* queue = (queue_object*)malloc(sizeof(queue_object));
    if (queue == NULL) {
        return NULL;
    }

    queue->object = NULL;
    queue->next = NULL;

    return queue;

}

void free_queue(queue_object *queue)
{
	if (queue == NULL) {
        return;
    }

    queue_object *current = queue;
    while (current != NULL) {
        queue_object *next = current->next;
        free(current);
        current = next;
    }
	// TODO
}

void *queue_peek(queue_object *queue)
{
	if (queue == NULL || queue->next == NULL) {
        return NULL;
    }

    return queue->next->object;
}
