#ifndef APP_STATE_MACHINE_H
#define APP_STATE_MACHINE_H
#include <FreeRTOS.h>

#endif //APP_STATE_MACHINE_H

#define STATE_NAME_LIMIT

typedef struct {
    char name[STATE_NAME_LIMIT];

    void (*run_on_entry)();
    void (*run_on_1Hz)();
    void (*run_on_100Hz)();
    void (*run_on_exit)();
} State;

void app_stateMachine_init(State *start_state);

const State* app_stateMachine_getCurrentState(void);

void app_stateMachine_setNextState(const State* state);

void app_stateMachine_tick1Hz(void);

void app_stateMachine_tick100Hz(void);