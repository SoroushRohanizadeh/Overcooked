#ifndef APP_NAVI_H
#define APP_NAVI_H
#endif //APP_NAVI_H

#include <app_drivetrain.h>


typedef enum __Node_Type {
    TOP_NODE,
    BOTTOM_NODE,
    RIGHT_BOUND,
    LEFT_BOUND
} Node_Type;

typedef struct __Node {
    Node_Type type;
    uint16_t xLocation; // in milimeters with respect to LEFT_BOUND, rough understimate
} Node;

typedef struct __NAVI_Handle {
    DT_Handle* dtHandle;
    Node* __currentNode;
    Node* __destinationNode;
} NAVI_Handle;

typedef enum __NAVI_STATE_NAME {
    DRIVE_VERT,
    DRIVE_HOR,
    ALIGN_VERT,
    ALIGN_HOR,
    ROTATE
} NAVI_STATE_NAME;

typedef struct __NAVI_State {
    NAVI_STATE_NAME name;

    void (*run_on_entry)(NAVI_Handle* handle);
    void (*run_on_100Hz)(NAVI_Handle* handle);
    void (*run_on_exit) (NAVI_Handle* handle);
} NAVI_State;

void app_navi_initDriveToNode(NAVI_Handle* handle, Node* current, Node* destination);

void app_navi_tickDriveToNode(NAVI_Handle* handle);

void app_navi_endDriveToNode(NAVI_Handle* handle);

// void app_navi_initDriveToNodeDiagonal(NAVI_Handle* handle, Node* current, Node* destination);
//
// void app_navi_tickDriveToNodeDiagonal(NAVI_Handle *handle);