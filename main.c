#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "bluetooth_control.h"

static const char *TAG = "PS4";

void app_main(void) {
    init_bluetooth_ps4();

    int8_t x = 0, y = 0;

    while (1) {
        bool connected = get_ps4_joystick_values(&x, &y);
        bool cross     = get_ps4_button_cross();
        bool circle    = get_ps4_button_circle();
        bool triangle  = get_ps4_button_triangle();
        bool square    = get_ps4_button_square();
        bool ps        = get_ps4_button_ps();
        bool l1        = get_ps4_button_l1();
        bool r1        = get_ps4_button_r1();
        bool l2        = get_ps4_button_l2();
        bool r2        = get_ps4_button_r2();
        bool share     = get_ps4_button_share();
        bool options   = get_ps4_button_options();
        bool l3        = get_ps4_button_l3();
        bool r3        = get_ps4_button_r3();
        bool up        = get_ps4_button_up();
        bool right     = get_ps4_button_right();
        bool down      = get_ps4_button_down();
        bool left      = get_ps4_button_left();

        if (connected) {
            ESP_LOGI(TAG, "\n"
              "  [STICKS]  LX: %4d | LY: %4d\n"
              "  [ACTION]  Cross: %-3s | Circle: %-3s | Triangle: %-3s | Square: %-3s | PS: %-3s\n"
              "  [BUMPER]  L1:    %-3s | R1:     %-3s | L2:       %-3s | R2:     %-3s\n"
              "  [SYSTEM]  Share: %-3s | Options:%-3s | L3:       %-3s | R3:     %-3s\n"
              "  [D-PAD ]  Up:    %-3s | Down:   %-3s | Left:     %-3s | Right:  %-3s",
             x, y, 
             cross ? "ON" : "off", circle ? "ON" : "off", triangle ? "ON" : "off", square ? "ON" : "off", ps ? "ON" : "off",
             l1 ? "ON" : "off",    r1 ? "ON" : "off",     l2 ? "ON" : "off",       r2 ? "ON" : "off",
             share ? "ON" : "off", options ? "ON" : "off",l3 ? "ON" : "off",      r3 ? "ON" : "off",
             up ? "ON" : "off",    down ? "ON" : "off",   left ? "ON" : "off",     right ? "ON" : "off");
        } else {
            ESP_LOGW(TAG, "コントローラー未接続...");
        }

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}