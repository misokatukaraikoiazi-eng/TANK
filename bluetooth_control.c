#include <stddef.h>
#include <stdbool.h>
#include "bluetooth_control.h"
#include "ps4.h"

static ps4_state_t current_state = {0};
static bool is_connected = false;

typedef bool (*ps4_button_reader_t)(void);

static bool read_cross(void) { return current_state.cross; }
static bool read_circle(void) { return current_state.circle; }
static bool read_triangle(void) { return current_state.triangle; }
static bool read_square(void) { return current_state.square; }
static bool read_ps(void) { return current_state.ps; }
static bool read_l1(void) { return current_state.l1; }
static bool read_r1(void) { return current_state.r1; }
static bool read_l2(void) { return current_state.l2; }
static bool read_r2(void) { return current_state.r2; }
static bool read_share(void) { return current_state.share; }
static bool read_options(void) { return current_state.options; }
static bool read_l3(void) { return current_state.l3; }
static bool read_r3(void) { return current_state.r3; }
static bool read_up(void) { return current_state.up; }
static bool read_right(void) { return current_state.right; }
static bool read_down(void) { return current_state.down; }
static bool read_left(void) { return current_state.left; }

static bool read_button_state(ps4_button_reader_t reader) {
    return reader();
}


// ps4.h が定める正しいコールバック型: void (*)(ps4_t, ps4_event_t)
static void ps4_event_cb(ps4_t data, ps4_event_t event) {
    (void)event;

    current_state.lx = data.analog.stick.lx;
    current_state.ly = data.analog.stick.ly;
    current_state.rx = data.analog.stick.rx;
    current_state.ry = data.analog.stick.ry;
    current_state.cross = (bool)data.button.cross;
    current_state.circle = (bool)data.button.circle;
    current_state.triangle = (bool)data.button.triangle;
    current_state.square = (bool)data.button.square;
    current_state.ps = (bool)data.button.ps;
    current_state.l1 = (bool)data.button.l1;
    current_state.r1 = (bool)data.button.r1;
    current_state.l2 = (bool)data.button.l2;
    current_state.r2 = (bool)data.button.r2;
    current_state.share = (bool)data.button.share;
    current_state.options = (bool)data.button.options;
    current_state.l3 = (bool)data.button.l3;
    current_state.r3 = (bool)data.button.r3;
    current_state.up = (bool)data.button.up;
    current_state.right = (bool)data.button.right;
    current_state.down = (bool)data.button.down;
    current_state.left = (bool)data.button.left;

}

void init_bluetooth_ps4(void) {
    ps4SetEventCallback(ps4_event_cb);
    ps4Init();
}

bool get_ps4_state(ps4_state_t *out_state) {
    if (out_state == NULL) {
        return false;
    }

    *out_state = current_state;
    is_connected = ps4IsConnected();
    return is_connected;
}

bool get_ps4_joystick_values(int8_t *out_x, int8_t *out_y, int8_t *out_rx, int8_t *out_ry) {
    if (out_x == NULL || out_y == NULL || out_rx == NULL || out_ry == NULL) return false;
    *out_x = current_state.lx;
    *out_y = current_state.ly;
    *out_rx = current_state.rx;
    *out_ry = current_state.ry;
    is_connected = ps4IsConnected();
    return is_connected;
}


bool get_ps4_button_cross(void) {
    return read_button_state(read_cross);
}

bool get_ps4_button_circle(void) {
    return read_button_state(read_circle);
}

bool get_ps4_button_triangle(void) {
    return read_button_state(read_triangle);
}

bool get_ps4_button_square(void) {
    return read_button_state(read_square);
}

bool get_ps4_button_ps(void) {
    return read_button_state(read_ps);
}

bool get_ps4_button_l1(void) {
    return read_button_state(read_l1);
}

bool get_ps4_button_r1(void) {
    return read_button_state(read_r1);
}

bool get_ps4_button_l2(void) {
    return read_button_state(read_l2);
}

bool get_ps4_button_r2(void) {
    return read_button_state(read_r2);
}

bool get_ps4_button_share(void) {
    return read_button_state(read_share);
}

bool get_ps4_button_options(void) {
    return read_button_state(read_options);
}

bool get_ps4_button_l3(void) {
    return read_button_state(read_l3);
}

bool get_ps4_button_r3(void) {
    return read_button_state(read_r3);
}

bool get_ps4_button_up(void) {
    return read_button_state(read_up);
}

bool get_ps4_button_right(void) {
    return read_button_state(read_right);
}

bool get_ps4_button_down(void) {
    return read_button_state(read_down);
}

bool get_ps4_button_left(void) {
    return read_button_state(read_left);
}
