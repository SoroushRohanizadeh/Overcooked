#include "app_stacking.h"

const CLAW_Stack_State* currentClawState;
const CLAW_Stack_State* nextClawState;

const STACK_State* currentStackState;
const STACK_State* nextStackState;

#define NUM_TICKS_CLAW_DESCEND 300 // the number of ticks it takes for the claw to descend onto the TODO determine
#define CLEARANCE_POS_TICKS 100 // TODO determine

#define CLAW_CLEARANCE_ANGLE 30 // TODO determine

#define CLAW_OUTSIDE_SWEEPER_DISTANCE 100 // in encoder ticks TODO determine

#define SWEEPER_RIGHT_LIFT_POS 800 // in encoder ticks TODO determine
#define SWEEPER_CENTER_POS 400 // in encoder ticks TODO determine
#define SWEEPER_LEFT_LIFT_POS 0 // in encoder ticks TODO determine

// --- State Machine ---
void runStackTickFunction(STACK_Handle* handle,
    void (*tickClaw)(STACK_Handle* handler),
    void (*tickStack)(STACK_Handle* handler)) {

    if (tickClaw != NULL) {
        tickClaw(handle);
    }

    if (nextClawState != currentClawState) {
        if (currentClawState->run_on_exit != NULL) {
            currentClawState->run_on_exit(handle);
        }

        currentClawState = nextClawState;

        if (currentClawState->run_on_entry != NULL) {
            currentClawState->run_on_entry(handle);
        }
    }

    if (tickStack != NULL) {
        tickStack(handle);
    }

    if (nextStackState != currentStackState) {
        if (currentStackState->run_on_exit != NULL) {
            currentStackState->run_on_exit(handle);
        }

        currentStackState = nextStackState;

        if (currentStackState->run_on_entry != NULL) {
            currentStackState->run_on_entry(handle);
        }
    }

    nextClawState = currentClawState;
    nextStackState = currentStackState;
}

void app_stackStateMachine_init(STACK_Handle *handle,
                                const CLAW_Stack_State *start_claw,
                                const STACK_State *start_stack) {

    currentClawState = start_claw;
    nextClawState    = start_claw;

    currentStackState = start_stack;
    nextStackState = start_stack;

    if (currentClawState->run_on_entry != NULL) {
        currentClawState->run_on_entry(handle);
    }

    if (currentStackState->run_on_entry != NULL) {
        currentStackState->run_on_entry(handle);
    }
}

void app_stackStateMachine_setNextClawState(const CLAW_Stack_State* state) {
    nextClawState = state;
}

void app_stackStateMachine_setNextStackState(const STACK_State* state) {
    nextStackState = state;
}

void app_stackStateMachine_tick100Hz(STACK_Handle *handle) {
    runStackTickFunction(handle, currentClawState->run_on_100Hz, currentStackState->run_on_100Hz);
}

// --- Sweeper State Functions ---
void initSweeperLeft(STACK_Handle *handle);
void tickSweeperLeft(STACK_Handle *handle);

void initLeftLiftDown(STACK_Handle *handle);
void tickLeftLiftDown(STACK_Handle *handle);

void initSweeperCenter(STACK_Handle *handle);
void tickSweeperCenter(STACK_Handle *handle);

void initSweeperRight(STACK_Handle *handle);
void tickSweeperRight(STACK_Handle *handle);

void initRightLiftDown(STACK_Handle *handle);
void tickRightLiftDown(STACK_Handle *handle);

void tickStackWaiting(STACK_Handle *handle);

// --- Claw State Functions ---
void initGrabFirst(STACK_Handle *handle);
void tickGrabFirst(STACK_Handle *handle);

void initRetractFirst(STACK_Handle *handle);
void tickRetractFirst(STACK_Handle *handle);

void initClearancePos1(STACK_Handle *handle);
void tickClearancePos1(STACK_Handle *handle);

void initClearancePos2(STACK_Handle * handle);
void tickClearancePos2(STACK_Handle * handle);

void initExtendForSecond(STACK_Handle *handle);
void tickExtendForSecond(STACK_Handle *handle);

void initGrabSecond(STACK_Handle *handle);
void tickGrabSecond(STACK_Handle *handle);

void initRetractSecond(STACK_Handle *handle);
void tickRetractSecond(STACK_Handle *handle);

void initInsertSecond(STACK_Handle *handle);
void tickInsertSecond(STACK_Handle *handle);

void tickClawWaiting(STACK_Handle *handle);

STACK_State sweeperLeft = {
    .name = "sweeperLeft",
    .run_on_entry = initSweeperLeft,
    .run_on_100Hz = tickSweeperLeft
};

STACK_State leftLiftDown = {
    .name = "leftLiftDown",
    .run_on_entry = initLeftLiftDown,
    .run_on_100Hz = tickLeftLiftDown
};

STACK_State sweeperCenter = {
    .name = "sweeperCenter",
    .run_on_entry = initSweeperCenter,
    .run_on_100Hz = tickSweeperCenter
};

STACK_State sweeperRight = {
    .name = "sweeperRight",
    .run_on_entry = initSweeperRight,
    .run_on_100Hz = tickSweeperRight
};

STACK_State rightLiftDown = {
    .name = "rightLiftDown",
    .run_on_entry = initRightLiftDown,
    .run_on_100Hz = tickRightLiftDown
};

STACK_State stackWaiting = {
    .name = "stackWaiting",
    .run_on_100Hz = tickStackWaiting
};

// --- Claw State ---
CLAW_Stack_State grabFirst = {
    .name = "grabFirst",
    .run_on_entry = initGrabFirst,
    .run_on_100Hz = tickGrabFirst
};

CLAW_Stack_State retractFirst = {
    .name = "retractFirst",
    .run_on_entry = initRetractFirst,
    .run_on_100Hz = tickRetractFirst
};

CLAW_Stack_State clearancePos1 = {
    .name = "clearancePos1",
    .run_on_entry = initClearancePos1,
    .run_on_100Hz = tickClearancePos1
};

CLAW_Stack_State clearancePos2 = {
    .name = "clearancePos2",
    .run_on_entry = initClearancePos2,
    .run_on_100Hz = tickClearancePos2
};

CLAW_Stack_State extendForSecond = {
    .name = "extendForSecond",
    .run_on_entry = initExtendForSecond,
    .run_on_100Hz = tickExtendForSecond
};

CLAW_Stack_State grabSecond = {
    .name = "grabSecond",
    .run_on_entry = initGrabSecond,
    .run_on_100Hz = tickGrabSecond
};

CLAW_Stack_State retractSecond = {
    .name = "retractSecond",
    .run_on_entry = initRetractSecond,
    .run_on_100Hz = tickRetractSecond
};

CLAW_Stack_State insertSecond = {
    .name = "insertSecond",
    .run_on_entry = initInsertSecond,
    .run_on_100Hz = tickInsertSecond
};

CLAW_Stack_State clawWaiting = {
    .name = "clawWaiting",
    .run_on_100Hz = tickClawWaiting
};

// --- Public Functions ---
void app_stacking_initStack(STACK_Handle *handle, STACK_Item *item) {
    handle->item = item;
    app_stackStateMachine_init(handle, &grabFirst, &stackWaiting);
}

void app_stacking_tickStack(STACK_Handle *handle) {
    app_stackStateMachine_tick100Hz(handle);
}


// --- State Function Definitions ---
void initSweeperLeft(STACK_Handle *handle) {
    app_sweeper_initMoveToPos(handle->sweeper, SWEEPER_LEFT_LIFT_POS);
}

void tickSweeperLeft(STACK_Handle *handle) {
    app_sweeper_tickMovePos(handle->sweeper);
    if (handle->sweeper->__state == SWEEPER_ARRIVED) {
        app_stackStateMachine_setNextStackState(&leftLiftDown);
    }
}

void initLeftLiftDown(STACK_Handle *handle) {
    app_lift_initMoveByHeight(handle->leftLift, -handle->item->height);
}

void tickLeftLiftDown(STACK_Handle *handle) {
    app_lift_tickMoveHeight(handle->leftLift);
    if (handle->leftLift->__state == LIFT_ARRIVED) {
        app_stackStateMachine_setNextStackState(&sweeperCenter);
    }
}

void initSweeperCenter(STACK_Handle *handle) {
    app_sweeper_initMoveToPos(handle->sweeper, SWEEPER_CENTER_POS);
}

void tickSweeperCenter(STACK_Handle *handle) {
    app_sweeper_tickMovePos(handle->sweeper);
    if (handle->sweeper->__state == SWEEPER_ARRIVED) {
        app_stackStateMachine_setNextStackState(&stackWaiting);
        if (currentClawState->name == clawWaiting.name) {
            app_stackStateMachine_setNextClawState(&insertSecond);
        }
    }
}

void initSweeperRight(STACK_Handle *handle) {
    app_sweeper_initMoveToPos(handle->sweeper, SWEEPER_RIGHT_LIFT_POS);
}

void tickSweeperRight(STACK_Handle *handle) {
    app_sweeper_tickMovePos(handle->sweeper);
    if (handle->sweeper->__state == SWEEPER_ARRIVED) {
        app_stackStateMachine_setNextStackState(&rightLiftDown);
    }
}

void initRightLiftDown(STACK_Handle *handle) {
    app_lift_initMoveByHeight(handle->rightLift, -handle->item->height);
}

void tickRightLiftDown(STACK_Handle *handle) {
    app_lift_tickMoveHeight(handle->rightLift);
    if (handle->rightLift->__state == LIFT_ARRIVED) {
        app_stackStateMachine_setNextStackState(&stackWaiting);
    }
}

void tickStackWaiting(STACK_Handle *handle) {
    UNUSED(handle);
    // do nothing
}

void initGrabFirst(STACK_Handle *handle) {
    app_claw_setAngle(handle->claw, 0);
    handle->__clawAngleCounter = 0;
}

void tickGrabFirst(STACK_Handle *handle) {
    if (handle->__clawAngleCounter >= NUM_TICKS_CLAW_DESCEND) {
        app_stackStateMachine_setNextClawState(&retractFirst);
    }
    handle->__clawAngleCounter++;
}

void initRetractFirst(STACK_Handle *handle) {
    app_claw_initMoveToPos(handle->claw, handle->item->clawPos);
}

void tickRetractFirst(STACK_Handle *handle) {
    app_claw_tickMovePos(handle->claw);
    if (handle->claw->__state == CLAW_ARRIVED) {
        app_stackStateMachine_setNextClawState(&clearancePos1);
    }
}

void initClearancePos1(STACK_Handle *handle) {
    app_claw_setAngle(handle->claw, CLAW_CLEARANCE_ANGLE);
    handle->__clawAngleCounter = 0;
}

void tickClearancePos1(STACK_Handle *handle) {
    if (handle->__clawAngleCounter >= CLEARANCE_POS_TICKS) {
        app_stackStateMachine_setNextClawState(&extendForSecond);
        app_stackStateMachine_setNextStackState(&sweeperLeft);
    }
    handle->__clawAngleCounter++;
}

void initExtendForSecond(STACK_Handle *handle) {
    app_claw_initFullyExtend(handle->claw);
}

void tickExtendForSecond(STACK_Handle *handle) {
    app_claw_tickMovePos(handle->claw);
    if (handle->claw->__state == CLAW_ARRIVED) {
        app_stackStateMachine_setNextClawState(&grabSecond);
    }
}

void initGrabSecond(STACK_Handle *handle) {
    app_claw_setAngle(handle->claw, 0);
    handle->__clawAngleCounter = 0;
}

void tickGrabSecond(STACK_Handle *handle) {
    if (handle->__clawAngleCounter >= NUM_TICKS_CLAW_DESCEND) {
        app_stackStateMachine_setNextClawState(&retractSecond);
    }
    handle->__clawAngleCounter++;
}

void initRetractSecond(STACK_Handle *handle) {
    app_claw_initMoveToPos(handle->claw, CLAW_OUTSIDE_SWEEPER_DISTANCE + handle->item->clawPos);
}

void tickRetractSecond(STACK_Handle *handle) {
    app_claw_tickMovePos(handle->claw);
    if (handle->claw->__state == CLAW_ARRIVED) {
        if (currentStackState->name == stackWaiting.name) {
            app_stackStateMachine_setNextClawState(&insertSecond);
        } else {
            app_stackStateMachine_setNextClawState(&clawWaiting);
        }
    }
}

void initInsertSecond(STACK_Handle *handle) {
    app_claw_initMoveToPos(handle->claw, handle->item->clawPos);
}

void tickInsertSecond(STACK_Handle *handle) {
    app_claw_tickMovePos(handle->claw);
    if (handle->claw->__state == CLAW_ARRIVED) {
        app_stackStateMachine_setNextClawState(&clearancePos2);
    }
}

void initClearancePos2(STACK_Handle *handle) {
    app_claw_setAngle(handle->claw, CLAW_CLEARANCE_ANGLE);
    handle->__clawAngleCounter = 0;
}

void tickClearancePos2(STACK_Handle *handle) {
    if (handle->__clawAngleCounter >= CLEARANCE_POS_TICKS) {
        app_stackStateMachine_setNextClawState(&clawWaiting);
        app_stackStateMachine_setNextStackState(&sweeperRight);
    }
    handle->__clawAngleCounter++;
}

void tickClawWaiting(STACK_Handle *handle) {
    UNUSED(handle);
    // do nothing
}
