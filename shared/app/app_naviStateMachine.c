#include "app_naviStateMachine.h"

#include <stddef.h>

const NAVI_State* currentState;
const NAVI_State* nextState;

void runTickFunction(void (*tick_function)()) {
    if (tick_function != NULL) {
        tick_function();
    }

    if (nextState != currentState) {
        if (currentState->run_on_exit != NULL) {
            currentState->run_on_exit();
        }

        currentState = nextState;

        if (currentState->run_on_entry != NULL) {
            currentState->run_on_entry();
        }
    }
    nextState = currentState;
}

void app_naviStateMachine_init(NAVI_Handle* handle, const NAVI_State* start_state) {
    currentState = start_state;
    nextState    = start_state;

    if (currentState->run_on_entry != NULL)
    {
        currentState->run_on_entry();
    }
}

void app_naviStateMachine_tick100Hz(NAVI_Handle* handle) {
    runTickFunction(currentState->run_on_100Hz);
}

void app_naviStateMachine_setNextState(NAVI_Handle* handle, const NAVI_State* state) {
    nextState = state;
}