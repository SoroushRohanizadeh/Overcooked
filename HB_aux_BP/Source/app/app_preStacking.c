#include "app_preStacking.h"

#define DEFAULT_CLAW_ANGLE 30U // TODO determine
#define DEFAULT_LIFT_X 20U // TODO determine
#define SWEEPER_DELAY_TICKS 300 // todo determine this

void (*__preStackTick)(STACK_Handle *handle);

void app_preStacking_tickStacking(STACK_Handle *handle);
void app_preStacking_tickArrived(STACK_Handle *handle);

void app_preStacking_init(STACK_Handle *handle, uint16_t lastItemHeight) {
    app_claw_initFullyExtend(handle->claw);
    app_claw_setAngle(handle->claw, DEFAULT_CLAW_ANGLE);

    app_lift_setXForStack(handle->leftLift);
    app_lift_setXForStack(handle->rightLift);

    handle->__sweeperPreStackDelay = 0;


    __preStackTick = app_preStacking_tickStacking;
}

void app_preStacking_tick(STACK_Handle *handle) {
    __preStackTick(handle);
}

void app_preStacking_tickStacking(STACK_Handle *handle) {
    app_claw_tickMovePos(handle->claw);

    if (handle->__sweeperPreStackDelay == SWEEPER_DELAY_TICKS) {
        app_sweeper_initMoveToCenter(handle->sweeper);
    } else if (handle->__sweeperPreStackDelay > SWEEPER_DELAY_TICKS) {
        app_sweeper_tickMovePos(handle->sweeper);
        if (handle->claw->__state == CLAW_ARRIVED
            && handle->sweeper->__state == SWEEPER_ARRIVED) {

            __preStackTick = app_preStacking_tickArrived;
            handle->__preStackState == PRE_STACKING_DONE;
        }
    } else {
        handle->__sweeperPreStackDelay++;
    }

}

void app_preStacking_tickArrived(STACK_Handle *handle) {
    // Do nothing
}
