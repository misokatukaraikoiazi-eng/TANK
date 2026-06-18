#ifndef BLUETOOTH_CONTROL_H
#define BLUETOOTH_CONTROL_H

#include <stdint.h>
#include <stdbool.h>

// Bluetooth（PS4コントローラー）の初期化
void init_bluetooth_ps4(void);

// 左スティック値を取得（戻り値：接続中なら true）
bool get_ps4_joystick_values(int8_t *out_x, int8_t *out_y);

// クロスボタンの押下状態を取得
bool get_ps4_button_cross(void);
bool get_ps4_button_circle(void);
bool get_ps4_button_triangle(void);
bool get_ps4_button_square(void);
bool get_ps4_button_ps(void);
bool get_ps4_button_l1(void);
bool get_ps4_button_r1(void);
bool get_ps4_button_l2(void);
bool get_ps4_button_r2(void);
bool get_ps4_button_share(void);
bool get_ps4_button_options(void);
bool get_ps4_button_l3(void);
bool get_ps4_button_r3(void);
bool get_ps4_button_up(void);
bool get_ps4_button_right(void);
bool get_ps4_button_down(void);
bool get_ps4_button_left(void);

#endif // BLUETOOTH_CONTROL_H
