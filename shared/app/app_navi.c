#include "app_navi.h"
#include <math.h>

#define DEFAULT_THROTTLE 100U

#define MAX_HEIGHT 762U
#define MIDDLE_HEIGHT MAX_HEIGHT / 2

double_t app_navi_drivingAngle(NAVI_Handle *handle);
uint16_t app_navi_mapNodeHeight(Node_Type type);

void app_navi_initDriveToNode(NAVI_Handle* handle, Node* current, Node* destination) {
    handle->__currentNode = current;
    handle->__destinationNode = destination;
    app_drivetrain_driveVect(handle->dtHandle, DEFAULT_THROTTLE, app_navi_drivingAngle(handle));
}

// Origin is Bottom Left
double_t app_navi_drivingAngle(NAVI_Handle *handle) {
    uint16_t xDist = handle->__destinationNode->xLocation - handle->__currentNode->xLocation;
    uint16_t yDist = app_navi_mapNodeHeight(handle->__destinationNode->type)
        - app_navi_mapNodeHeight(handle->__currentNode->type);
    return atan(yDist / xDist);
}

uint16_t app_navi_mapNodeHeight(Node_Type type) {
    if (type == TOP_NODE) {
        return MAX_HEIGHT;
    }
    if (type == BOTTOM_NODE) {
        return 0;
    }
    return MIDDLE_HEIGHT;
}

void app_navi_tickDriveToNode(NAVI_Handle *handle) {
    handle->tickFunction();

    // drive until "close"
    // start looking for line
    // stop once line found
    // center on line & go until counter
    // stop
}

void app_navi_endDriveToNode(NAVI_Handle *handle) {
}
