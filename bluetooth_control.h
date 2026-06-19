
#ifndef BLUETOOTH_CONTROL_H
#define BLUETOOTH_CONTROL_H

#include <stdint.h>
#include <stdbool.h>

// コントローラーの全入力を管理する構造体
typedef struct {
    int8_t lx;
    int8_t ly;
    int8_t rx;
    int8_t ry;
    bool cross;
    bool circle;
    bool triangle;
    bool square;
    bool ps;
    bool l1;
    bool r1;
    bool l2;
    bool r2;
    bool share;
    bool options;
    bool l3;
    bool r3;
    bool up;
    bool right;
    bool down;
    bool left;
} ps4_state_t;

// 既存の初期化関数
void init_bluetooth_ps4(void);
// Bluetooth（PS4コントローラー）の初期化
void init_bluetooth_ps4(void);

// 左スティック値を取得（戻り値：接続中なら true）
bool get_ps4_joystick_values(int8_t *out_x, int8_t *out_y, int8_t *out_rx, int8_t *out_ry);

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
