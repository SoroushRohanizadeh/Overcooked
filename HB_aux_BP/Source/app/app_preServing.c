#include "app_preServing.h"

#define CLAW_CLEARANCE_ANGLE 30 // TODO determine
#define LIFT_CLEARANCE_HEIGHT 200 // TODO determine

void (*__tickClaw)(AUX_Handle *handle);
void (*__tickStack)(AUX_Handle *handle);

void app_preServing_tickClaw(AUX_Handle *handle);
void app_preServing_tickClawArrivedClearance(AUX_Handle *handle);
void app_preServing_tickClawDown(AUX_Handle *handle);

void app_preServing_tickLeftLiftDown(AUX_Handle *handle);
void app_preServing_tickSweeperLeft(AUX_Handle *handle);
void app_preServing_tickLeftLiftUp(AUX_Handle *handle);
void app_preServing_sweeperCenter(AUX_Handle *handle);
void app_preServing_tickStackWait(AUX_Handle *handle);


void app_preServing_init(AUX_Handle *handle) {
    app_claw_setAngle(handle->claw, CLAW_CLEARANCE_ANGLE); // TODO not ticking angle
    app_claw_initFullyRetracted(handle->claw);

    __tickClaw = app_claw_tickMovePos;

    app_lift_initMoveToHeight(handle->leftLift, LIFT_CLEARANCE_HEIGHT);

    __tickStack = app_preServing_tickLeftLiftDown;
}




void app_preServing_tick(AUX_Handle *handle) {
    __tickClaw(handle);
}

void app_preServing_tickClaw(AUX_Handle *handle) {
    app_claw_tickMovePos(handle->claw);

    if (handle->claw->__state == CLAW_ARRIVED) {
        __tickClaw = app_preServing_tickClawArrivedClearance;
    }
}

void app_preServing_tickClawArrivedClearance(AUX_Handle *handle) {
    UNUSED(handle);
    // do nothing
}

void app_preServing_tickClawDown(AUX_Handle *handle) {
}

void app_preServing_tickLeftLiftDown(AUX_Handle *handle) {
}

void app_preServing_tickSweeperLeft(AUX_Handle *handle) {
}

void app_preServing_tickLeftLiftUp(AUX_Handle *handle) {
}

void app_preServing_sweeperCenter(AUX_Handle *handle) {
}

void app_preServing_tickStackWait(AUX_Handle *handle) {
}
