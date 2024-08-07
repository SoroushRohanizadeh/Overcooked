#ifndef APP_NAVI_H
#define APP_NAVI_H
#endif //APP_NAVI_H

#include "app_drivetrain.h"
#include "hw_reflectance.h"

#define MAX_NAME_LENGTH 16

typedef enum __Node_Type {
    TOP_NODE,
    BOTTOM_NODE,
    RIGHT_BOUND,
    LEFT_BOUND
} Node_Type;

typedef enum __Node_Name {
    LETTUCE,
    CHEESE,
    TOMATO,
    PASSING,
    COOKTOP,
    PLATING,
    START
} Node_Name;

typedef struct __Node {
    Node_Name name;
    Node_Type type;
    uint16_t xLocation; // in milimeters with respect to LEFT_BOUND, rough understimate
} Node;

typedef struct __NAVI_Handle {
    DT_Handle* dtHandle;
    ADC_Handler* sns;
    GPIO_TypeDef* leftBumperDef;
    uint16_t leftBumperPin;
    GPIO_TypeDef* rightBumperDef;
    uint16_t rightBumperPin;
    UART_HandleTypeDef* huart;

    const Node* __currentNode;
    const Node* __destinationNode;
    uint8_t __numSkipsHorizontal;
    bool __lineSeenToSkip;
    uint8_t numNodes;
    Node* nodes;
} NAVI_Handle;

void app_navi_initDriveToNode(NAVI_Handle* handle, const Node* current, const Node* destination);

void app_navi_tickDriveToNode(NAVI_Handle* handle);

// void app_navi_endDriveToNode(NAVI_Handle* handle);

// --- State Machine ---
typedef enum __NAVI_STATE_NAME {
    DRIVE_VERT,
    DRIVE_HOR,
    ALIGN_VERT,
    ALIGN_HOR,
    ROTATE,
    ARRIVED
} NAVI_STATE_NAME;

typedef struct __NAVI_State {
    NAVI_STATE_NAME name;

    void (*run_on_entry)(NAVI_Handle* handle);
    void (*run_on_100Hz)(NAVI_Handle* handle);
    void (*run_on_exit) (NAVI_Handle* handle);
} NAVI_State;

void app_naviStateMachine_init(NAVI_Handle* handle, const NAVI_State* start_state);
void app_naviStateMachine_tick100Hz(NAVI_Handle* handle);

// void app_navi_initDriveToNodeDiagonal(NAVI_Handle* handle, Node* current, Node* destination);
//
// void app_navi_tickDriveToNodeDiagonal(NAVI_Handle *handle);