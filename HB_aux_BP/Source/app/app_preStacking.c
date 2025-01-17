#include "app_preStacking.h"

#define DEFAULT_CLAW_ANGLE 30U // TODO determine
#define DEFAULT_LIFT_X 20U // TODO determine
#define SWEEPER_DELAY_TICKS 300 // todo determine this

void (*__preStackTick)(AUX_Handle *handle);

void app_preStacking_tickStacking(AUX_Handle *handle);
void app_preStacking_tickArrived(AUX_Handle *handle);

void app_preStacking_init(AUX_Handle *handle, uint16_t lastItemHeight) {
    app_claw_initFullyExtend(handle->claw);
    app_claw_setAngle(handle->claw, DEFAULT_CLAW_ANGLE);

    app_lift_setXForStack(handle->leftLift);
    app_lift_setXForStack(handle->rightLift);

    app_sweeper_initMoveToCenter(handle->sweeper);

    __preStackTick = app_preStacking_tickStacking;
}

void app_preStacking_tick(AUX_Handle *handle) {
    __preStackTick(handle);
}

void app_preStacking_tickStacking(AUX_Handle *handle) {
    app_claw_tickMovePos(handle->claw);
    app_sweeper_tickMovePos(handle->sweeper);

    if (handle->claw->__state == CLAW_ARRIVED && handle->sweeper->__state == SWEEPER_ARRIVED) {
        __preStackTick = app_preStacking_tickArrived;
        handle->__preStackState == PRE_STACKING_DONE;
    }
}

void app_preStacking_tickArrived(AUX_Handle *handle) {
    // Do nothing
}
