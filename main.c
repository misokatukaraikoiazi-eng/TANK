#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "bluetooth_control.h"
#include "led.h"

static const char *TAG = "PS4";

void app_main(void) {
    init_bluetooth_ps4();
    led_init();

    ps4_state_t state = {0};
    (void)TAG;
    while (1) {
        (void)get_ps4_state(&state);

        // LEDの制御は20ms周期で爆速処理
        led_update(&state);

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}
