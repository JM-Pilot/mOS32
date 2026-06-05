#include "../arch/x86/asm.h"
#ifndef SCHED_H
#define SCHED_H

enum task_state {
	TASK_DEAD = 0,
	TASK_ALIVE = 1,
	TASK_WAITING = 2,
};
struct task {
	struct registers reg;
	enum task_state state;
	int id;
};


void sched_init();
struct task* sched_next();
void task_start(struct task *t);
struct task *task_add(void (*entry)(void), void *stack);
void sched_remove_dead();
void sched_add_task(struct task *t);
struct task* sched_next();
#endif