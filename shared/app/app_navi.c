#include "app_navi.h"

#define DEFAULT_THROTTLE 50U
#define ROTATION_SPEED 50U
#define CORRECTION_THROTTLE 20U
#define INTO_WALL_THROTTLE 20U

#define BOTTOM_HEIGHT 762U
#define MIDDLE_HEIGHT BOTTOM_HEIGHT / 2


// --- State Machine ---
const NAVI_State* currentState;
const NAVI_State* nextState;

void runNaviTickFunction(NAVI_Handle* handle, void (*tick_function)(NAVI_Handle* handler)) {
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
    runNaviTickFunction(handle, currentState->run_on_100Hz);
}

void app_naviStateMachine_setNextState(const NAVI_State* state) {
    nextState = state;
}


// --- Source Code ---
void app_initDriveVertToLine(NAVI_Handle * handle);
void app_tickDriveVertToLine(NAVI_Handle * handle);
void app_exitDriveVertToLine(NAVI_Handle * handle);

void app_tickDriveVertToBench(NAVI_Handle * handle);
void app_exitDriveVertToBench(NAVI_Handle * handle);
void app_initDriveVertToBench(NAVI_Handle * handle);

void app_initDriveHor(NAVI_Handle * handle);
void app_tickDriveHor(NAVI_Handle * handle);
void app_exitDriveHor(NAVI_Handle * handle);

void app_initAlignVert(NAVI_Handle * handle);
void app_tickAlignVert(NAVI_Handle * handle);

void app_tickAlignHor(NAVI_Handle * handle);

void app_initRotate(NAVI_Handle * handle);
void app_tickRotate(NAVI_Handle * handle);
void app_exitRotate(NAVI_Handle * handle);

NAVI_State driveVertToLineState = {
    .name = DRIVE_VERT,
    .run_on_entry = app_initDriveVertToLine,
    .run_on_100Hz = app_tickDriveVertToLine,
    .run_on_exit = app_exitDriveVertToLine
};

NAVI_State driveVertToBenchState = {
    .name = DRIVE_VERT,
    .run_on_entry = app_initDriveVertToBench,
    .run_on_100Hz = app_tickDriveVertToBench,
    .run_on_exit = app_exitDriveVertToBench
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
    .run_on_100Hz = app_tickAlignVert
};

NAVI_State alignHorState = {
    .name = ALIGN_HOR,
    .run_on_100Hz = app_tickAlignHor
};

NAVI_State rotateState = {
    .name = ROTATE,
    .run_on_entry = app_initRotate,
    .run_on_100Hz = app_tickRotate,
    .run_on_exit = app_exitRotate
};

NAVI_State arrivedState = {
    .name = ARRIVED
};

void app_navi_initDriveToNode(NAVI_Handle *handle, const Node *current, const Node *destination) {
    handle->__currentNode = current;
    handle->__destinationNode = destination;

    if (handle->__currentNode->type == handle->__destinationNode->type) {
        app_naviStateMachine_init(handle, &driveHorState);
    } else if (handle->__currentNode->type == LEFT_BOUND || handle->__currentNode->type == RIGHT_BOUND) {
        app_naviStateMachine_init(handle, &rotateState);
    } else {
        app_naviStateMachine_init(handle, &driveVertToLineState);
    }
}

// --- State Machine Functions ---
void app_initDriveVertToLine(NAVI_Handle *handle) {
    if (handle->__destinationNode->type == TOP_NODE) {
        app_drivetrain_drive(handle->dtHandle, DEFAULT_THROTTLE, DRIVE_UP);
    } else {
        app_drivetrain_drive(handle->dtHandle, DEFAULT_THROTTLE, DRIVE_DOWN);
    }
}

void app_tickDriveVertToLine(NAVI_Handle *handle) {
    uint8_t sns1 = 0;
    uint8_t sns2 = 0;

    if (handle->dtHandle->state == DRIVE_UP) {
        sns1 = L1;
        sns2 = R1;
    } else {
        sns1 = L2;
        sns2 = R2;
    }

    if (hw_reflectance_lineDetected(handle->sns, sns1) ||  hw_reflectance_lineDetected(handle->sns, sns2)) {
        app_naviStateMachine_setNextState(&alignVertState);
    }
}

void app_exitDriveVertToLine(NAVI_Handle *handle) {
    app_drivetrain_brake(handle->dtHandle);
}

void app_initAlignVert(NAVI_Handle *handle) {
    if (handle->__destinationNode->type == TOP_NODE) {
        app_drivetrain_drive(handle->dtHandle, CORRECTION_THROTTLE, DRIVE_DOWN);
    } else {
        app_drivetrain_drive(handle->dtHandle, CORRECTION_THROTTLE, DRIVE_UP);
    }
}

void app_tickAlignVert(NAVI_Handle *handle) {
    uint8_t sns1 = 0;
    uint8_t sns2 = 0;

    if (handle->dtHandle->state == DRIVE_UP) {
        sns1 = L1;
        sns2 = R1;
    } else {
        sns1 = L2;
        sns2 = R2;
    }

    // TODO ASSUMES WE DO NOT SHOOT PAST THE TAPE COMPLETELY
    if (!hw_reflectance_lineDetected(handle->sns, sns1) || !hw_reflectance_lineDetected(handle->sns, sns2)) {
        app_drivetrain_stop(handle->dtHandle);
        app_naviStateMachine_setNextState(&rotateState);
    }
}

void app_initRotate(NAVI_Handle *handle) {
    app_drivetrain_driveVect(handle->dtHandle, 0, 0, ROTATION_SPEED);
}

void app_tickRotate(NAVI_Handle *handle) {
    if (hw_reflectance_lineDetected(handle->sns, L1)) {
        app_naviStateMachine_setNextState(&driveVertToBenchState);
    }
}

void app_exitRotate(NAVI_Handle *handle) {
    app_drivetrain_driveVect(handle->dtHandle, 0, 0, -ROTATION_SPEED);
    app_drivetrain_stop(handle->dtHandle);
}

void app_initDriveVertToBench(NAVI_Handle *handle) {
    if (handle->__destinationNode->type == TOP_NODE) {
        app_drivetrain_drive(handle->dtHandle, INTO_WALL_THROTTLE, DRIVE_UP);
    } else {
        app_drivetrain_drive(handle->dtHandle, INTO_WALL_THROTTLE, DRIVE_DOWN);
    }
}

void app_tickDriveVertToBench(NAVI_Handle *handle) {
    if (!HAL_GPIO_ReadPin(handle->leftBumperDef, handle->leftBumperPin)
        && !HAL_GPIO_ReadPin(handle->rightBumperDef, handle->rightBumperPin)) {
        if (handle->__currentNode->xLocation == handle->__destinationNode->xLocation) {
            app_naviStateMachine_setNextState(&alignHorState);
        } else {
            app_naviStateMachine_setNextState(&driveHorState);
        }
    }
}

void app_exitDriveVertToBench(NAVI_Handle *handle) {
    app_drivetrain_stop(handle->dtHandle);
}

void app_navi_numSkips(NAVI_Handle *handle, Drive_State state) {
    uint8_t numSkips = 0;

    for (uint8_t i = 0; i < handle->numNodes; i++) {
        if (handle->nodes[i].type != handle->__destinationNode->type) continue;

        if (state == DRIVE_RIGHT) {
            if (handle->nodes[i].xLocation > handle->__currentNode->xLocation) {
                numSkips++;
            }
        } else {
            if (handle->nodes[i].xLocation > handle->__currentNode->xLocation) {
                numSkips++;
            }
        }
    }
    handle->__numSkipsHorizontal = numSkips - 1; // accounting for __destinationNode being in nodes[]
}

void app_initDriveHor(NAVI_Handle *handle) {
    handle->__lineSeenToSkip = false;

    if (handle->__destinationNode->xLocation > handle->__currentNode->xLocation) {
        app_drivetrain_drive(handle->dtHandle, DEFAULT_THROTTLE, DRIVE_RIGHT); // TODO consider making this vector slightly into wall
        app_navi_numSkips(handle, DRIVE_RIGHT);
    } else {
        app_drivetrain_drive(handle->dtHandle, DEFAULT_THROTTLE, DRIVE_LEFT); // TODO consider making this vector slightly into wall
        app_navi_numSkips(handle, DRIVE_LEFT);
    }
}

void app_tickDriveHor(NAVI_Handle *handle) {
    uint8_t sns = 0;

    if (handle->dtHandle->state == DRIVE_RIGHT) {
        sns = T2;
    } else {
        sns = T1;
    }

    if (hw_reflectance_lineDetected(handle->sns, sns)) {
        if (handle->__numSkipsHorizontal == 0) {
            app_naviStateMachine_setNextState(&alignHorState);
        } else if (!handle->__lineSeenToSkip) {
            handle->__lineSeenToSkip = true;
            handle->__numSkipsHorizontal -= 1;
        }
    } else {
        handle->__lineSeenToSkip = false;
    }
}

void app_exitDriveHor(NAVI_Handle *handle) {
    Drive_State state = handle->dtHandle->state;
    app_drivetrain_brake(handle->dtHandle);

    if (state == DRIVE_RIGHT) {
        app_drivetrain_drive(handle->dtHandle, CORRECTION_THROTTLE, DRIVE_LEFT);
    } else {
        app_drivetrain_drive(handle->dtHandle, CORRECTION_THROTTLE, DRIVE_RIGHT);
    }
}

void app_tickAlignHor(NAVI_Handle *handle) {
    uint8_t sns = 0;

    if (handle->dtHandle->state == DRIVE_RIGHT) {
        sns = T1;
    } else {
        sns = T2;
    }

    if (!hw_reflectance_lineDetected(handle->sns, sns)) {
        app_drivetrain_stop(handle->dtHandle);
        app_naviStateMachine_setNextState(&arrivedState);
    }
}

// --- END State Machine Functions ---

void app_navi_tickDriveToNode(NAVI_Handle *handle) {
    app_naviStateMachine_tick100Hz(handle);
}

// void app_navi_endDriveToNode(NAVI_Handle *handle) {}

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