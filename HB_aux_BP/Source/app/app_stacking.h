#ifndef APP_STACKING_H
#define APP_STACKING_H
#include <app_claw.h>
#include <app_lift.h>
#include <app_preStacking.h>
#include <app_sweeper.h>

#define MAX_NAME_LEN 16

typedef struct __STACK_ITEM {
    char name[MAX_NAME_LEN];
    uint16_t height; // in encoder ticks TODO measure
} STACK_Item;

typedef struct __AUX_Handle {
    LIFT_Handle* leftLift;
    LIFT_Handle* rightLift;
    CLAW_Handle* claw;
    SWEEP_Handle* sweeper;
    STACK_Item* item;

    PreStacking_State __preStackState;
    uint16_t __sweeperPreStackDelay; // TODO might overflow
} STACK_Handle;

// State Machine
typedef enum __STACK_STATE_NAME {
    SWEEPER_LEFT,
    LEFT_LIFT_DOWN,
    SWEEPER_CENTRAL,

    SWEEPER_RIGHT,
    RIGHT_LIFT_DOWN,

    STACK_WAITING
} STACK_STATE_NAME;

typedef enum __CLAW_STATE_NAME {
    CLAW_GRAB_FIRST,
    CLAW_RETRACT_FIRST,
    CLAW_CLEARANCE_ANGLE,

    CLAW_EXTEND_FOR_SECOND,
    CLAW_GRAB_SECOND,
    CLAW_RETRACT_SECOND, // Waiting outside of sweepr
    CLAW_INSERT_SECOND,

    CLAW_WAITING
} CLAW_STATE_NAME;

typedef struct __STACK_State {
    void (*run_on_entry)(STACK_Handle* handle);
    void (*run_on_100Hz)(STACK_Handle* handle);
    void (*run_on_exit) (STACK_Handle* handle);
} STACK_State;

typedef struct __CLAW_Stack_State {
    void (*run_on_entry)(STACK_Handle* handle);
    void (*run_on_100Hz)(STACK_Handle* handle);
    void (*run_on_exit) (STACK_Handle* handle);
} CLAW_Stack_State;

#endif //APP_STACKING_H

void app_stacking_initStack(STACK_Handle* handle, STACK_Item* item);
void app_stacking_tickStack(STACK_Handle* handle);


void app_stackStateMachine_init(STACK_Handle* handle, const CLAW_Stack_State* start_claw, const STACK_State* start_stack);
void app_stackStateMachine_tick100Hz(STACK_Handle* handle);