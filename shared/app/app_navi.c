#include "app_navi.h"
#include <math.h>

#define DEFAULT_THROTTLE 100U
#define ROTATION_SPEED 100U

#define BOTTOM_HEIGHT 762U
#define MIDDLE_HEIGHT BOTTOM_HEIGHT / 2


// --- State Machine ---
const NAVI_State* currentState;
const NAVI_State* nextState;
void runTickFunction(NAVI_Handle* handle, void (*tick_function)(NAVI_Handle* handler)) {
    if (tick_function != NULL) {
        tick_function(handle);
    }

    if (nextState != currentState) {
        if (currentState->run_on_exit != NULL) {
            currentState->run_on_exit(handle);
        }

        currentState = nextState;

        if (currentState->run_on_entry != NULL) {
            currentState->run_on_entry(handle);
        }
    }
    nextState = currentState;
}

void app_naviStateMachine_init(NAVI_Handle* handle, const NAVI_State* start_state) {
    currentState = start_state;
    nextState    = start_state;

    if (currentState->run_on_entry != NULL)
    {
        currentState->run_on_entry(handle);
    }
}

void app_naviStateMachine_tick100Hz(NAVI_Handle* handle) {
    runTickFunction(handle, currentState->run_on_100Hz);
}

void app_naviStateMachine_setNextState(const NAVI_State* state) {
    nextState = state;
}


// --- Source Code ---
void app_initDriveVert(NAVI_Handle * handle);
void app_tickDriveVert(NAVI_Handle * handle);
void app_exitDriveVert(NAVI_Handle * handle);

void app_initDriveHor(NAVI_Handle * handle);
void app_tickDriveHor(NAVI_Handle * handle);
void app_exitDriveHor(NAVI_Handle * handle);

void app_initAlignVert(NAVI_Handle * handle);
void app_tickAlignVert(NAVI_Handle * handle);
void app_exitAlignVert(NAVI_Handle * handle);

void app_initAlignHor(NAVI_Handle * handle);
void app_tickAlignHor(NAVI_Handle * handle);
void app_exitAlignHor(NAVI_Handle * handle);

void app_initRotate(NAVI_Handle * handle);
void app_tickRotate(NAVI_Handle * handle);
void app_exitRotate(NAVI_Handle * handle);

NAVI_State driveVertState = {
    .name = DRIVE_VERT,
    .run_on_entry = app_initDriveVert,
    .run_on_100Hz = app_tickDriveVert,
    .run_on_exit = app_exitDriveVert
};

NAVI_State driveHorState = {
    .name = DRIVE_HOR,
    .run_on_entry = app_initDriveHor,
    .run_on_100Hz = app_tickDriveHor,
    .run_on_exit = app_exitDriveHor
};

NAVI_State alignVertState = {
    .name = ALIGN_VERT,
    .run_on_entry = app_initAlignVert,
    .run_on_100Hz = app_tickAlignVert,
    .run_on_exit = app_exitAlignVert
};

NAVI_State alignHorState = {
    .name = ALIGN_HOR,
    .run_on_entry = app_initAlignHor,
    .run_on_100Hz = app_tickAlignHor,
    .run_on_exit = app_exitAlignHor
};

NAVI_State rotateState = {
    .name = ROTATE,
    .run_on_entry = app_initRotate,
    .run_on_100Hz = app_tickRotate,
    .run_on_exit = app_exitRotate
};

void app_navi_initDriveToNode(NAVI_Handle *handle, Node *current, Node *destination) {
    handle->__currentNode = current;
    handle->__destinationNode = destination;

    if (handle->__currentNode->type == handle->__destinationNode->type) {
        app_naviStateMachine_init(handle, &driveHorState);
    } else if (handle->__currentNode->type == LEFT_BOUND || handle->__currentNode->type == RIGHT_BOUND) {
        app_naviStateMachine_init(handle, &rotateState);
    } else {
        app_naviStateMachine_init(handle, &driveVertState);
    }
}



void app_navi_tickDriveToNode(NAVI_Handle *handle) {
    app_naviStateMachine_tick100Hz(handle);
}

void app_navi_endDriveToNode(NAVI_Handle *handle) {}

// --- DIAGONAL NAVIGATION ---
// double_t app_navi_drivingAngle(NAVI_Handle *handle);
// uint16_t app_navi_mapNodeHeight(Node_Type type);
//
// void app_navi_initDriveToNodeDiagonal(NAVI_Handle* handle, Node* current, Node* destination) {
//     handle->__currentNode = current;
//     handle->__destinationNode = destination;
//     app_drivetrain_driveVect(handle->dtHandle, DEFAULT_THROTTLE, app_navi_drivingAngle(handle), 0);
// }
//
// // Origin is Top Left
// double_t app_navi_drivingAngle(NAVI_Handle *handle) {
//     uint16_t xDist = handle->__destinationNode->xLocation - handle->__currentNode->xLocation;
//     uint16_t yDist = app_navi_mapNodeHeight(handle->__destinationNode->type)
//         - app_navi_mapNodeHeight(handle->__currentNode->type);
//     return atan(yDist / xDist);
// }
//
// uint16_t app_navi_mapNodeHeight(Node_Type type) {
//     if (type == TOP_NODE) {
//         return 0;
//     }
//     if (type == BOTTOM_NODE) {
//         return BOTTOM_HEIGHT;
//     }
//     return MIDDLE_HEIGHT;
// }
//
// void app_navi_tickDriveToNodeDiagonal(NAVI_Handle *handle) {
//     handle->tickFunction();
//
//     // drive until "close"
//     // start looking for line
//     // stop once line found
//     // center on line & go until counter
//     // stop
// }