#include "app_navi.h"
#include <math.h>

#define DEFAULT_THROTTLE 100U
#define ROTATION_SPEED 100U

#define BOTTOM_HEIGHT 762U
#define MIDDLE_HEIGHT BOTTOM_HEIGHT / 2

NAVI_State driveVertState = {
    .name = NAVI_DRIVE_VERT,
    .run_on_entry = app_initDriveVert,
    .run_on_100Hz = app_tickDriveVert,
    .run_on_exit = app_exitDriveVert
};

NAVI_State driveHorState = {
    .name = NAVI_DRIVE_HOR,
    .run_on_entry = app_initDriveHor,
    .run_on_100Hz = app_tickDriveHor,
    .run_on_exit = app_exitDriveHor
};

NAVI_State alignVertState = {
    .name = NAVI_ALIGN_VERT,
    .run_on_entry = app_initAlignVert,
    .run_on_100Hz = app_tickAlignVert,
    .run_on_exit = app_exitAlignVert
};

NAVI_State alignHorState = {
    .name = NAVI_ALIGN_HOR,
    .run_on_entry = app_initAlignHor,
    .run_on_100Hz = app_tickAlignHor,
    .run_on_exit = app_exitAlignHor
};

NAVI_State rotateState = {
    .name = NAVI_ROTATE,
    .run_on_entry = app_initRotate,
    .run_on_100Hz = app_tickRotate,
    .run_on_exit = app_exitRotate
};

void app_navi_initDriveVertToMiddleLine(NAVI_Handle *handle);
void app_navi_initRotate180(NAVI_Handle *handle);
void app_navi_initDriveToWall(NAVI_Handle *handle);
void app_navi_initDriveHorizontalToNode(NAVI_Handle *handle);


void app_navi_initVertDirection(NAVI_Handle *handle);

void app_navi_initHorizontalDirection(NAVI_Handle *handle);


void app_navi_initDriveToNode(NAVI_Handle *handle, Node *current, Node *destination) {
    handle->__currentNode = current;
    handle->__destinationNode = destination;


    if (handle->__currentNode->type == handle->__destinationNode->type) {
        app_simpleStateMachine_init(&stateMachine, driveHorState);
        app_navi_initHorizontalDirection(handle);
        return;
    }

    if (handle->__currentNode->type == LEFT_BOUND || handle->__currentNode->type == RIGHT_BOUND) {
        app_simpleStateMachine_init(&stateMachine, rotateState);
        app_navi_initRotate180(handle);
        return;
    }

    app_simpleStateMachine_init(&stateMachine, driveVertState);
    app_navi_initDriveToMiddleLine(handle);
}

void app_navi_tickDriveToNode(NAVI_Handle *handle) {
    app_simpleStateMachine_tick100Hz(&stateMachine);

    // if changing sides
    //      if middle
    //          rotate
    //          go horizontal -
    //      if side
    //          go to middle
    //          rotate
    //          go horizontal
    // drive and skip lines if necessary
    // drive until line found
    // center on line and go in
}

void app_navi_initVertDirection(NAVI_Handle *handle) {

}

void app_navi_initHorizontalDirection(NAVI_Handle *handle) {
    if (handle->__destinationNode->xLocation >= handle->__currentNode->xLocation) {
        app_drivetrain_drive(handle->dtHandle, DEFAULT_THROTTLE, RIGHT);
    } else {
        app_drivetrain_drive(handle->dtHandle, DEFAULT_THROTTLE, LEFT);
    }
}

void app_navi_initRotate180(NAVI_Handle *handle) {}

void app_navi_initDriveToMiddleLine(NAVI_Handle *handle) {
}

void app_navi_endDriveToNode(NAVI_Handle *handle) {
}

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