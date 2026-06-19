#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "bluetooth_control.h"
#include "led.h"

static const char *TAG = "PS4";

void app_main(void) {
    init_bluetooth_ps4();
    led_init();

    int8_t x = 0, y = 0;
    int8_t rx = 0, ry = 0;
    ps4_state_t state = {0};
    (void)TAG;
    while (1) {
        bool connected = get_ps4_joystick_values(&x, &y, &rx, &ry);
        bool cross = get_ps4_button_cross();
        bool circle = get_ps4_button_circle();
        bool triangle = get_ps4_button_triangle();
        bool square = get_ps4_button_square();
        bool ps = get_ps4_button_ps();
        bool l1 = get_ps4_button_l1();
        bool r1 = get_ps4_button_r1();
        bool l2 = get_ps4_button_l2();
        bool r2 = get_ps4_button_r2();
        bool share = get_ps4_button_share();
        bool options = get_ps4_button_options();
        bool l3 = get_ps4_button_l3();
        bool r3 = get_ps4_button_r3();
        bool up = get_ps4_button_up();
        bool right = get_ps4_button_right();
        bool down = get_ps4_button_down();
        bool left = get_ps4_button_left();

        // 未使用変数のコンパイル警告(Unused Variable)を防止するキャスト処理
        // 変数構造やロジックを崩さずに保持できます
        (void)connected; (void)ps; (void)l1; (void)r1; (void)l2; (void)r2;
        (void)share; (void)options; (void)l3; (void)r3;
        (void)up; (void)right; (void)down; (void)left;

#if 0  // 💡 1 から 0 に変更してログ出力を完全に無効化
        if (connected) {
            ESP_LOGI(TAG, "\n"
            " [STICKS] LX: %4d | LY: %4d\n"
            " [ACTION] Cross: %-3s | Circle: %-3s | Triangle: %-3s | Square: %-3s | PS: %-3s\n"
            " [BUMPER] L1: %-3s | R1: %-3s | L2: %-3s | R2: %-3s\n"
            " [SYSTEM] Share: %-3s | Options:%-3s | L3: %-3s | R3: %-3s\n"
            " [D-PAD ] Up: %-3s | Down: %-3s | Left: %-3s | Right: %-3s",
            x, y, 
            cross ? "ON" : "off", circle ? "ON" : "off", triangle ? "ON" : "off", square ? "ON" : "off", ps ? "ON" : "off",
            l1 ? "ON" : "off", r1 ? "ON" : "off", l2 ? "ON" : "off", r2 ? "ON" : "off",
            share ? "ON" : "off", options ? "ON" : "off",l3 ? "ON" : "off", r3 ? "ON" : "off",
            up ? "ON" : "off", down ? "ON" : "off", left ? "ON" : "off", right ? "ON" : "off");
        } else {
            ESP_LOGW(TAG, "コントローラー未接続...");
        }
#endif

        // LEDの制御は20ms周期で爆速処理
        ps4_state_t state = {
            .lx = x,
            .ly = y,
            .rx = rx,
            .ry = ry,
            .cross = cross,
            .circle = circle,
            .triangle = triangle,
            .square = square,
            .ps = ps,
            .l1 = l1,
            .r1 = r1,
            .l2 = l2,
            .r2 = r2,
            .share = share,
            .options = options,
            .l3 = l3,
            .r3 = r3,
            .up = up,
            .right = right,
            .down = down,
            .left = left
        };
        led_update(&state);

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}
