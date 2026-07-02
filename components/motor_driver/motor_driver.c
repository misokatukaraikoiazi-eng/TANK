#include "motor_driver.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "MOTOR_DRIVER";

void motor_driver_init(void) {
    // 左モーターのピンを出力モードに設定
    gpio_reset_pin(MOTOR_LEFT_PIN_A);
    gpio_set_direction(MOTOR_LEFT_PIN_A, GPIO_MODE_OUTPUT);
    gpio_reset_pin(MOTOR_LEFT_PIN_B);
    gpio_set_direction(MOTOR_LEFT_PIN_B, GPIO_MODE_OUTPUT);

    // 右モーターのピンを出力モードに設定
    gpio_reset_pin(MOTOR_RIGHT_PIN_A);
    gpio_set_direction(MOTOR_RIGHT_PIN_A, GPIO_MODE_OUTPUT);
    gpio_reset_pin(MOTOR_RIGHT_PIN_B);
    gpio_set_direction(MOTOR_RIGHT_PIN_B, GPIO_MODE_OUTPUT);
}

void drive_motors(int16_t left_speed, int16_t right_speed) 
{
    // 左モーターの処理 (left_speed > 0 なら正転、< 0 なら反転)

    if (left_speed > 0) {
        gpio_set_level(MOTOR_LEFT_PIN_A, 1);
        gpio_set_level(MOTOR_LEFT_PIN_B, 0);
    } else if (left_speed < 0) {
        gpio_set_level(MOTOR_LEFT_PIN_A, 0);
        gpio_set_level(MOTOR_LEFT_PIN_B, 1);
    } else {
        gpio_set_level(MOTOR_LEFT_PIN_A, 0);
        gpio_set_level(MOTOR_LEFT_PIN_B, 0);
    }
    // 右モーターの処理 (right_speed > 0 なら正転、< 0 なら反転)
    if (right_speed > 0) {
        gpio_set_level(MOTOR_RIGHT_PIN_A, 1);
        gpio_set_level(MOTOR_RIGHT_PIN_B, 0);
    } else if (right_speed < 0) {
        gpio_set_level(MOTOR_RIGHT_PIN_A, 0);
        gpio_set_level(MOTOR_RIGHT_PIN_B, 1);
    } else {
        gpio_set_level(MOTOR_RIGHT_PIN_A, 0);
        gpio_set_level(MOTOR_RIGHT_PIN_B, 0);
    }
}