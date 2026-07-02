#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <stdint.h>

// ピン配置の定義（左右各2ピン、計4ピン）
#define MOTOR_LEFT_PIN_A   25
#define MOTOR_LEFT_PIN_B   26
#define MOTOR_RIGHT_PIN_A  32
#define MOTOR_RIGHT_PIN_B  33

// 関数プロトタイプ宣言
void motor_driver_init(void);
void drive_motors(int16_t left_speed, int16_t right_speed);

#endif // MOTOR_DRIVER_H
