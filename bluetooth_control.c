#include <stddef.h>
#include <stdbool.h>
#include "ps4.h"

static int8_t  current_x     = 0;
static int8_t  current_y     = 0;
static bool    current_cross = false;
static bool    is_connected  = false;
static bool    current_circle = false;
static bool    current_triangle = false;
static bool    current_square = false;
static bool    current_ps = false;
static bool    current_l1 = false;
static bool    current_r1 = false;
static bool    current_l2 = false;
static bool    current_r2 = false;
static bool    current_share = false;
static bool    current_options = false;
static bool    current_l3 = false;
static bool    current_r3 = false;
static bool    current_up = false;
static bool    current_right = false;
static bool    current_down = false;
static bool    current_left = false;


// ps4.h が定める正しいコールバック型: void (*)(ps4_t, ps4_event_t)
static void ps4_event_cb(ps4_t data, ps4_event_t event) {
    current_x     = data.analog.stick.lx;
    current_y     = data.analog.stick.ly;
    current_cross = (bool)data.button.cross;
    current_circle = (bool)data.button.circle;
    current_triangle = (bool)data.button.triangle;
    current_square = (bool)data.button.square;
    current_ps = (bool)data.button.ps;
    current_l1 = (bool)data.button.l1;
    current_r1 = (bool)data.button.r1;
    current_l2 = (bool)data.button.l2;
    current_r2 = (bool)data.button.r2;
    current_share = (bool)data.button.share;
    current_options = (bool)data.button.options;
    current_l3 = (bool)data.button.l3;
    current_r3 = (bool)data.button.r3;
    current_up = (bool)data.button.up;
    current_right = (bool)data.button.right;
    current_down = (bool)data.button.down;
    current_left = (bool)data.button.left;

}

void init_bluetooth_ps4(void) {
    ps4SetEventCallback(ps4_event_cb);
    ps4Init();
}

bool get_ps4_joystick_values(int8_t *out_x, int8_t *out_y) {
    if (out_x == NULL || out_y == NULL) return false;
    *out_x = current_x;
    *out_y = current_y;
    is_connected = ps4IsConnected();
    return is_connected;
}

bool get_ps4_button_cross(void) {
    return current_cross;
}

bool get_ps4_button_circle(void) {
    return current_circle;
}

bool get_ps4_button_triangle(void) {
    return current_triangle;
}

bool get_ps4_button_square(void) {
    return current_square;
}

bool get_ps4_button_ps(void) {
    return current_ps;
}

bool get_ps4_button_l1(void) {
    return current_l1;
}

bool get_ps4_button_r1(void) {
    return current_r1;
}

bool get_ps4_button_l2(void) {
    return current_l2;
}

bool get_ps4_button_r2(void) {
    return current_r2;
}

bool get_ps4_button_share(void) {
    return current_share;
}

bool get_ps4_button_options(void) {
    return current_options;
}

bool get_ps4_button_l3(void) {
    return current_l3;
}

bool get_ps4_button_r3(void) {
    return current_r3;
}

bool get_ps4_button_up(void) {
    return current_up;
}

bool get_ps4_button_right(void) {
    return current_right;
}

bool get_ps4_button_down(void) {
    return current_down;
}

bool get_ps4_button_left(void) {
    return current_left;
}
