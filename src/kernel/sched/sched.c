#include "../arch/x86/asm.h"
#include "sched.h"
#include <stdint.h>
#include <libk/stdlib.h>
#include <stddef.h>
#include <libk/string.h>

#define MAX_TASKS 16
struct task tasks[MAX_TASKS];
int tasks_count;

void sched_init(){
	tasks_count = 0;
}
void sched_add_task(struct task *t){
	if (tasks_count < MAX_TASKS){
		tasks[tasks_count++] =  *t;
	}
}
void sched_remove_dead(){
	for (int i = 0; i < tasks_count; i++){
		if (tasks[i].state == TASK_DEAD){
			for (int j = i; j < tasks_count - 1; j++){
				tasks[j] = tasks[j + 1];
			}
			tasks_count--;
			i--;
		}
	}
}
struct task *task_add(void (*entry)(void), void *stack){
	struct task *t = kmalloc(sizeof(struct task));
	t->state = TASK_ALIVE;
	k_memset(&t->reg, 0, sizeof(t->reg));
	t->reg.eip = (uint32_t)entry;
	t->reg.esp = (uint32_t)stack;
	t->reg.eflags = 0x202;
	return t;
}

void task_start(struct task *t){
	sched_add_task(t);
}

int current_task_index = 0;

struct task* sched_next(){
	int tries = 0;
	
	while (tries < tasks_count){
		struct task *t = &tasks[current_task_index];
		current_task_index = (current_task_index + 1) % tasks_count;
		
		if (t->state == TASK_ALIVE){
			return t;
		}
		tries++;
	}
	
	return NULL;
}