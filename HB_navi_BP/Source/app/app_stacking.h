#ifndef APP_STACKING_H
#define APP_STACKING_H
#include <app_claw.h>
#include <app_lift.h>
#include <app_sweeper.h>

typedef struct __AUX_Handle {
    LIFT_Handle* leftLift;
    LIFT_Handle* rightLift;
    CLAW_Handle* claw;
    SWEEP_Handle* sweeper;
} AUX_Handle;

#endif //APP_STACKING_H
