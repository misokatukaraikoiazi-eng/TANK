// インクルードガード追加
#ifndef LED_H
#define LED_H   
#include <stdbool.h>
#include <stdint.h>
#include "bluetooth_control.h" // 構造体の定義を読み込む

// led.c プロトタイプ宣言追加
void led_init(void);
void led_update(const ps4_state_t *state);
// 💡 状態表示LED（GPIO2）を制御する関数を定義
// connected: 真なら点灯、偽なら内部タイマーで200msごとにパタパタ点滅させる
void led_status_monitor(bool connected);

// （前回のled_updateなどがあればそのままでOKです）
#endif // LED_H