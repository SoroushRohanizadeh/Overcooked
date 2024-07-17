#include "app_stateMachine.h"

#include <semphr.h>
#include <portmacro.h>

static const State* next_state;
static const State* current_state;

static StaticSemaphore_t state_tick_mutex_storage;
static SemaphoreHandle_t state_tick_mutex;

void runTickFunction(void (*tick_function)()) {
    xSemaphoreTake(state_tick_mutex, portMAX_DELAY);

    if (tick_function != NULL) {
        tick_function();
    }

    if (next_state != current_state) {
        if (current_state->run_on_exit != NULL) {
            current_state->run_on_exit();
        }

        current_state = next_state;

        if (current_state->run_on_entry != NULL) {
            current_state->run_on_entry();
        }
    }

    // We assume the next time we tick we will continue in the current state,
    // unless told otherwise.
    next_state = current_state;

    xSemaphoreGive(state_tick_mutex);
}

void app_stateMachine_init(State *start_state) {
    current_state = start_state;
    next_state    = start_state;

    if (current_state->run_on_entry != NULL)
    {
        current_state->run_on_entry();
    }

    state_tick_mutex = xSemaphoreCreateMutexStatic(&(state_tick_mutex_storage));
}

void app_stateMachine_tick1Hz(void) {
    runTickFunction(current_state->run_on_1Hz);
}

void app_stateMachine_tick100Hz(void) {
    runTickFunction(current_state->run_on_100Hz);
}

const State* app_stateMachine_getCurrentState(void) {
    return current_state;
}

void app_stateMachine_setNextState(const State *state) {
    next_state = state;
}