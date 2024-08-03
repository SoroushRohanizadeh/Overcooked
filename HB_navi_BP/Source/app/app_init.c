#include "app_init.h"

#define NUM_NODES 6

PWM_Handle cw_pwmHandler_w1 = {
  .htim = &htim4,
  .channel = TIM_CHANNEL_1,
  .TIM_start = HAL_TIM_Base_Start,
  .PWM_start = HAL_TIM_PWM_Start,
  .TIM_stop = HAL_TIM_Base_Stop,
  .PWM_stop = HAL_TIM_PWM_Stop
};

PWM_Handle ccw_pwmHandler_w1 = {
  .htim = &htim4,
  .channel = TIM_CHANNEL_2,
  .TIM_start = HAL_TIM_Base_Start,
  .PWM_start = HAL_TIM_PWM_Start,
  .TIM_stop = HAL_TIM_Base_Stop,
  .PWM_stop = HAL_TIM_PWM_Stop
};

PWM_Handle cw_pwmHandler_w2 = {
  .htim = &htim3,
  .channel = TIM_CHANNEL_4,
  .TIM_start = HAL_TIM_Base_Start,
  .PWM_start = HAL_TIM_PWM_Start,
  .TIM_stop = HAL_TIM_Base_Stop,
  .PWM_stop = HAL_TIM_PWM_Stop
};

PWM_Handle ccw_pwmHandler_w2 = {
  .htim = &htim3,
  .channel = TIM_CHANNEL_3,
  .TIM_start = HAL_TIM_Base_Start,
  .PWM_start = HAL_TIM_PWM_Start,
  .TIM_stop = HAL_TIM_Base_Stop,
  .PWM_stop = HAL_TIM_PWM_Stop
};

PWM_Handle cw_pwmHandler_w3 = {
  .htim = &htim1,
  .channel = TIM_CHANNEL_3,
  .TIM_start = HAL_TIM_Base_Start,
  .PWM_start = HAL_TIM_PWM_Start,
  .TIM_stop = HAL_TIM_Base_Stop,
  .PWM_stop = HAL_TIM_PWM_Stop
};

PWM_Handle ccw_pwmHandler_w3 = {
  .htim = &htim1,
  .channel = TIM_CHANNEL_2,
  .TIM_start = HAL_TIM_Base_Start,
  .PWM_start = HAL_TIM_PWM_Start,
  .TIM_stop = HAL_TIM_Base_Stop,
  .PWM_stop = HAL_TIM_PWM_Stop
};

PWM_Handle cw_pwmHandler_w4 = {
  .htim = &htim4,
  .channel = TIM_CHANNEL_4,
  .TIM_start = HAL_TIM_Base_Start,
  .PWM_start = HAL_TIM_PWM_Start,
  .TIM_stop = HAL_TIM_Base_Stop,
  .PWM_stop = HAL_TIM_PWM_Stop
};

PWM_Handle ccw_pwmHandler_w4 = {
  .htim = &htim4,
  .channel = TIM_CHANNEL_3,
  .TIM_start = HAL_TIM_Base_Start,
  .PWM_start = HAL_TIM_PWM_Start,
  .TIM_stop = HAL_TIM_Base_Stop,
  .PWM_stop = HAL_TIM_PWM_Stop
};

Motor_Handle wheel1 = {
  .cw_handle = &cw_pwmHandler_w1,
  .ccw_handle = &ccw_pwmHandler_w1,
  .state = MOTOR_STOP,
  // .rotary_handle = &rotary_handle_w1,
  .maxSpeed = 260
};

Motor_Handle wheel2 = {
  .cw_handle = &cw_pwmHandler_w2,
  .ccw_handle = &ccw_pwmHandler_w2,
  .state = MOTOR_STOP,
  // .rotary_handle = &rotary_handle_w2,
  .maxSpeed = 260
};

Motor_Handle wheel3 = {
  .cw_handle = &cw_pwmHandler_w3,
  .ccw_handle = &ccw_pwmHandler_w3,
  .state = MOTOR_STOP,
  // .rotary_handle = &rotary_handle_w3,
  .maxSpeed = 260
};

Motor_Handle wheel4 = {
  .cw_handle = &cw_pwmHandler_w4,
  .ccw_handle = &ccw_pwmHandler_w4,
  .state = MOTOR_STOP,
  // .rotary_handle = &rotary_handle_w4,
  .maxSpeed = 260
};

DT_Handle driveHandle = {
  .wheel_1 = &wheel1,
  .wheel_2 = &wheel2,
  .wheel_3 = &wheel3,
  .wheel_4 = &wheel4,
  .state = DRIVE_STOP
};

const Node lettuce = {
  .name = "Lettuce",
  .type = BOTTOM_NODE,
  .xLocation = 2426
};

const Node cheese = {
  .name = "Cheese",
  .type = BOTTOM_NODE,
  .xLocation = 152
};

const Node tomato = {
  .name = "Tomato",
  .type = TOP_NODE,
  .xLocation = 152
};

// Nominally the "cutting" square
const Node passing = {
  .name = "Passing",
  .type = TOP_NODE,
  .xLocation = 610
};

const Node cooktop = {
  .name = "Cooking",
  .type = TOP_NODE,
  .xLocation = 1829
};

const Node plating = {
  .name = "Plating",
  .type = TOP_NODE,
  .xLocation = 2286
};

Node nodes[7] = {lettuce, cheese, tomato, passing, cooktop, plating};

NAVI_Handle naviHandle = {
  .dtHandle = &driveHandle,
  .leftBumperDef = GPIOB,
  .leftBumperPin = LEFT_BUMP_Pin,
  .rightBumperDef = GPIOB,
  .rightBumperPin = RIGHT_BUMP_Pin,
  .numNodes = NUM_NODES,
  .nodes = nodes
};

void app_init_init(ADC_Handler* adcHandler) {
    naviHandle.sns = adcHandler;
}

NAVI_Handle* app_init_getNaviHandle() {
    return &naviHandle;
}

const Node* app_init_getNode(char name[]) {
    for (uint8_t i = 0; i < NUM_NODES; i++) {
      if (nodes[i].name == name) {
        return &nodes[i];
      }
    }

    return &lettuce; // should never get here
}
