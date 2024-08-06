#ifndef APP_STACKING_H
#define APP_STACKING_H
#include <app_claw.h>
#include <app_lift.h>
#include <app_sweeper.h>

#define MAX_NAME_LEN 16

typedef enum __PreStacking_State {
    PRE_STACKING,
    PRE_STACKING_DONE
} PreStacking_State;

typedef struct __STACK_ITEM {
    char name[MAX_NAME_LEN];
    uint16_t height; // in encoder ticks TODO measure
    uint16_t clawPos; // how far in the claw goes before stacking item TODO measure for each food + plate
} STACK_Item;

typedef struct __AUX_Handle {
    LIFT_Handle* leftLift;
    LIFT_Handle* rightLift;
    CLAW_Handle* claw;
    SWEEP_Handle* sweeper;
    STACK_Item* item;

    PreStacking_State __preStackState;
    uint16_t __clawAngleCounter; // used to stall until claw reaches correct angle
} AUX_Handle;

typedef struct __STACK_State {
    char name[MAX_NAME_LEN];

    void (*run_on_entry)(AUX_Handle* handle);
    void (*run_on_100Hz)(AUX_Handle* handle);
    void (*run_on_exit) (AUX_Handle* handle);
} STACK_State;

typedef struct __CLAW_Stack_State {
    char name[MAX_NAME_LEN];

    void (*run_on_entry)(AUX_Handle* handle);
    void (*run_on_100Hz)(AUX_Handle* handle);
    void (*run_on_exit) (AUX_Handle* handle);
} CLAW_Stack_State;

#endif //APP_STACKING_H

void app_stacking_initStack(AUX_Handle* handle, STACK_Item* item);
void app_stacking_tickStack(AUX_Handle* handle);


void app_stackStateMachine_init(AUX_Handle* handle, const CLAW_Stack_State* start_claw, const STACK_State* start_stack);
void app_stackStateMachine_tick100Hz(AUX_Handle* handle);