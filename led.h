// インクルードガード追加
#ifndef LED_H
#define LED_H   
#include <stdbool.h>
#include <stdint.h>
#include "bluetooth_control.h" // 構造体の定義を読み込む

// led.c プロトタイプ宣言追加
void led_init(void);
void led_update(const ps4_state_t *state);
#endif // LED_H