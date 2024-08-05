#include "app_stacking.h"

const CLAW_Stack_State* currentClawState;
const CLAW_Stack_State* nextClawState;

const STACK_State* currentStackState;
const STACK_State* nextStackState;

// --- State Machine ---
void runNaviTickFunction(STACK_Handle* handle,
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
    runNaviTickFunction(handle, currentClawState->run_on_100Hz, currentStackState->run_on_100Hz);
}

// --- Sweeper State Functions ---
void initSweeperLeft(STACK_Handle *handle);
void tickSweeperLeft(STACK_Handle *handle);
void endSweeperLeft(STACK_Handle *handle);

void initLeftLiftDown(STACK_Handle *handle);
void tickLeftLiftDown(STACK_Handle *handle);
void endLeftLiftDown(STACK_Handle *handle);

void initSweeperCenter(STACK_Handle *handle);
void tickSweeperCenter(STACK_Handle *handle);
void endSweeperCenter(STACK_Handle *handle);

void initSweeperRight(STACK_Handle *handle);
void tickSweeperRight(STACK_Handle *handle);
void endSweeperRight(STACK_Handle *handle);

void initRightLiftDown(STACK_Handle *handle);
void tickRightLiftDown(STACK_Handle *handle);
void endRightLiftDown(STACK_Handle *handle);

void initStackWaiting(STACK_Handle *handle);
void tickStackWaiting(STACK_Handle *handle);
void endStackWaiting(STACK_Handle *handle);

// --- Claw State Functions ---
void initGrabFirst(STACK_Handle *handle);
void tickGrabFirst(STACK_Handle *handle);
void endGrabFirst(STACK_Handle *handle);

void initRetractFirst(STACK_Handle *handle);
void tickRetractFirst(STACK_Handle *handle);
void endRetractFirst(STACK_Handle *handle);

void initClearanceAngle(STACK_Handle *handle);
void tickClearanceAngle(STACK_Handle *handle);
void endClearanceAngle(STACK_Handle *handle);

void initExtendForSecond(STACK_Handle *handle);
void tickExtendForSecond(STACK_Handle *handle);
void endExtendForSecond(STACK_Handle *handle);

void initGrabSecond(STACK_Handle *handle);
void tickGrabSecond(STACK_Handle *handle);
void endGrabSecond(STACK_Handle *handle);

void initRetractSecond(STACK_Handle *handle);
void tickRetractSecond(STACK_Handle *handle);
void endRetractSecond(STACK_Handle *handle);

void initInsertSecond(STACK_Handle *handle);
void tickInsertSecond(STACK_Handle *handle);
void endInsertSecond(STACK_Handle *handle);

void initClawWaiting(STACK_Handle *handle);
void tickClawWaiting(STACK_Handle *handle);
void endClawWaiting(STACK_Handle *handle);

STACK_State sweeperLeft = {
    .run_on_entry = initSweeperLeft,
    .run_on_100Hz = tickSweeperLeft,
    .run_on_exit = endSweeperLeft
};

STACK_State leftLiftDown = {
    .run_on_entry = initLeftLiftDown,
    .run_on_100Hz = tickLeftLiftDown,
    .run_on_exit = endLeftLiftDown
};

STACK_State sweeperCenter = {
    .run_on_entry = initSweeperCenter,
    .run_on_100Hz = tickSweeperCenter,
    .run_on_exit = endSweeperCenter
};

STACK_State sweeperRight = {
    .run_on_entry = initSweeperRight,
    .run_on_100Hz = tickSweeperRight,
    .run_on_exit = endSweeperRight
};

STACK_State rightLiftDown = {
    .run_on_entry = initRightLiftDown,
    .run_on_100Hz = tickRightLiftDown,
    .run_on_exit = endRightLiftDown
};

STACK_State stackWaiting = {
    .run_on_entry = initStackWaiting,
    .run_on_100Hz = tickStackWaiting,
    .run_on_exit = endStackWaiting
};

// --- Claw State ---
CLAW_Stack_State grabFirst = {
    .run_on_entry = initGrabFirst,
    .run_on_100Hz = tickGrabFirst,
    .run_on_exit = endGrabFirst
};

CLAW_Stack_State retractFirst = {
    .run_on_entry = initRetractFirst,
    .run_on_100Hz = tickRetractFirst,
    .run_on_exit = endRetractFirst
};

CLAW_Stack_State clearanceAngle = {
    .run_on_entry = initClearanceAngle,
    .run_on_100Hz = tickClearanceAngle,
    .run_on_exit = endClearanceAngle
};

CLAW_Stack_State extendForSecond = {
    .run_on_entry = initExtendForSecond,
    .run_on_100Hz = tickExtendForSecond,
    .run_on_exit = endExtendForSecond
};

CLAW_Stack_State grabSecond = {
    .run_on_entry = initGrabSecond,
    .run_on_100Hz = tickGrabSecond,
    .run_on_exit = endGrabSecond
};

CLAW_Stack_State retractSecond = {
    .run_on_entry = initRetractSecond,
    .run_on_100Hz = tickRetractSecond,
    .run_on_exit = endRetractSecond
};

CLAW_Stack_State insertSecond = {
    .run_on_entry = initInsertSecond,
    .run_on_100Hz = tickInsertSecond,
    .run_on_exit = endInsertSecond
};

CLAW_Stack_State clawWaiting = {
    .run_on_entry = initClawWaiting,
    .run_on_100Hz = tickClawWaiting,
    .run_on_exit = endClawWaiting
};

// --- Public Functions ---
void app_stacking_initStack(STACK_Handle *handle, STACK_Item *item) {
    handle->item = item;
    app_stackStateMachine_init(handle, grabFirst, stackWaiting);
}

void app_stacking_tickStack(STACK_Handle *handle) {
    app_stackStateMachine_tick100Hz(handle);
}


// --- State Function Definitions ---
void initSweeperLeft(STACK_Handle *handle) {
}

void tickSweeperLeft(STACK_Handle *handle) {
}

void endSweeperLeft(STACK_Handle *handle) {
}

void initLeftLiftDown(STACK_Handle *handle) {
}

void tickLeftLiftDown(STACK_Handle *handle) {
}

void endLeftLiftDown(STACK_Handle *handle) {
}

void initSweeperCenter(STACK_Handle *handle) {
}

void tickSweeperCenter(STACK_Handle *handle) {
}

void endSweeperCenter(STACK_Handle *handle) {
}

void initSweeperRight(STACK_Handle *handle) {
}

void tickSweeperRight(STACK_Handle *handle) {
}

void endSweeperRight(STACK_Handle *handle) {
}

void initRightLiftDown(STACK_Handle *handle) {
}

void tickRightLiftDown(STACK_Handle *handle) {
}

void endRightLiftDown(STACK_Handle *handle) {
}

void initStackWaiting(STACK_Handle *handle) {
}

void tickStackWaiting(STACK_Handle *handle) {
}

void endStackWaiting(STACK_Handle *handle) {
}

void initGrabFirst(STACK_Handle *handle) {
}

void tickGrabFirst(STACK_Handle *handle) {
}

void endGrabFirst(STACK_Handle *handle) {
}

void initRetractFirst(STACK_Handle *handle) {
}

void tickRetractFirst(STACK_Handle *handle) {
}

void endRetractFirst(STACK_Handle *handle) {
}

void initClearanceAngle(STACK_Handle *handle) {
}

void tickClearanceAngle(STACK_Handle *handle) {
}

void endClearanceAngle(STACK_Handle *handle) {
}

void initExtendForSecond(STACK_Handle *handle) {
}

void tickExtendForSecond(STACK_Handle *handle) {
}

void endExtendForSecond(STACK_Handle *handle) {
}

void initGrabSecond(STACK_Handle *handle) {
}

void tickGrabSecond(STACK_Handle *handle) {
}

void endGrabSecond(STACK_Handle *handle) {
}

void initRetractSecond(STACK_Handle *handle) {
}

void tickRetractSecond(STACK_Handle *handle) {
}

void endRetractSecond(STACK_Handle *handle) {
}

void initInsertSecond(STACK_Handle *handle) {
}

void tickInsertSecond(STACK_Handle *handle) {
}

void endInsertSecond(STACK_Handle *handle) {
}

void initClawWaiting(STACK_Handle *handle) {
}

void tickClawWaiting(STACK_Handle *handle) {
}

void endClawWaiting(STACK_Handle *handle) {
}
