#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_log.h"
#include <stdlib.h>
#include "led.h"

#define STATUS_LED_PIN GPIO_NUM_2

// GPIO25(左スティックPWM), GPIO26(右スティックPWM)をLEDCで初期化
// GPIO27(〇ボタン), GPIO14(×ボタン)を通常の出力として初期化する関数
void led_init(void) {
    // LEDCのタイマーを設定
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .freq_hz = 5000,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    // GPIO25とGPIO26をLEDCチャネルに割り当て
    ledc_channel_config_t ledc_channel_left = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = GPIO_NUM_25,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&ledc_channel_left);

    ledc_channel_config_t ledc_channel_right = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_1,
        .timer_sel = LEDC_TIMER_0,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = GPIO_NUM_26,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&ledc_channel_right);

    // GPIO27とGPIO14を通常の出力として設定
    gpio_reset_pin(GPIO_NUM_27);
    gpio_set_direction(GPIO_NUM_27, GPIO_MODE_OUTPUT);
    
    gpio_reset_pin(GPIO_NUM_14);
    gpio_set_direction(GPIO_NUM_14, GPIO_MODE_OUTPUT);

    // 状態表示LED（GPIO2）を通常の出力として設定
    gpio_reset_pin(STATUS_LED_PIN);
    gpio_set_direction(STATUS_LED_PIN, GPIO_MODE_OUTPUT);
}
// ジョイスティックとボタンの状態を受け取り、各LEDを制御する関数
// 左右スティック(lx, ly, rx,  ry: -128〜127)の絶対値をPWMのデューティ比(0〜255)に変換して明暗させる
// circle, crossが真なら対応するLEDを点灯、triangleが真ならcircleのLEDを消灯、squareが真ならcrossのLEDを消灯する
void led_update(const ps4_state_t *state) {
    if (state == NULL) {
        return;
    }
    // 1. 先にint32_tにキャストして、-128を受け止められる箱にする
    int32_t raw_lx = (int32_t)state->lx;

    // 2. デッドゾーン（不感帯15）の処理
    if (abs(raw_lx) < 15) {
        raw_lx = 0;
    }

    // 3. 安全に絶対値を取り、最大値が「128」になっても溢れないように計算
    // 分母を 127 ではなく 128（左端の最大値）で割ることで、完全に255にクランプさせます
    uint32_t duty_left = (uint32_t)(abs(raw_lx) * 255 / 128);

    // 4. 万が一、計算誤差で255を超えた場合のガード処理
    if (duty_left > 255) {
        duty_left = 255;
    }

    // 左スティックの絶対値をPWMのデューティ比に変換
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty_left);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

    // 右スティックの絶対値をPWMのデューティ比に変換
    int32_t raw_rx = (int32_t)state->rx;
    if (abs(raw_rx) < 15) {
        raw_rx = 0;
    }
    uint32_t duty_right = (uint32_t)(abs(raw_rx) * 255 / 128);
    if (duty_right > 255) {
        duty_right = 255;
    }
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, duty_right);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);

    // ボタンの状態に応じてLEDを制御
    if (state->circle) {
        gpio_set_level(GPIO_NUM_27, 1); // circleが真なら〇ボタンのLEDを点灯
    }
    if (state->cross) {
        gpio_set_level(GPIO_NUM_14, 1); // crossが真なら×ボタンのLEDを点灯
    }
    if (state->triangle) {
        gpio_set_level(GPIO_NUM_27, 0); // triangleが真なら〇ボタンのLEDを消灯
    }
    if (state->square) {
        gpio_set_level(GPIO_NUM_14, 0); // squareが真なら×ボタンのLEDを消灯
    }
}
// 20ms周期で呼ばれることを想定し、内部カウンターで点滅周期（例：200ms）を作ります
void led_status_monitor(bool connected) {
    static int counter = 0;
    static bool prev_connected = false;
    static bool blink_on = false;

    if (connected != prev_connected) {
        counter = 0;
        if (!connected) {
            blink_on = false;
            gpio_set_level(STATUS_LED_PIN, 0);
        }
        prev_connected = connected;
    }

    if (connected) {
        gpio_set_level(STATUS_LED_PIN, 1); // 接続されている場合は常に点灯
    } else {
        counter++;
        if (counter >= 10) { // 200msごとに点滅（20ms * 10 = 200ms）
            blink_on = !blink_on;
            gpio_set_level(STATUS_LED_PIN, blink_on ? 1 : 0);
            counter = 0; // カウンターをリセット
        }
    }
}