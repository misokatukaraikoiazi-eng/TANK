#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_mac.h"
#include "nvs_flash.h"
#include "bluetooth_control.h"
#include "led.h"

static const char *TAG = "PS4";
static const TickType_t MAC_RETRY_INTERVAL_TICKS = pdMS_TO_TICKS(1000);
static const TickType_t MAC_WAIT_TIMEOUT_TICKS = pdMS_TO_TICKS(10000);

// ペアリング処理側が実装する MAC 取得フック。
const char *get_paired_device_mac(void);

static esp_err_t init_nvs(void) {
    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    return ret;
}

static void log_local_bt_mac_for_pairing(void) {
    uint8_t bt_mac[6] = {0};
    esp_err_t ret = esp_read_mac(bt_mac, ESP_MAC_BT);

    if (ret == ESP_OK) {
        ESP_LOGW(TAG,
                 "Paired MAC is unknown. Using device default BT MAC: %02X:%02X:%02X:%02X:%02X:%02X",
                 bt_mac[0],
                 bt_mac[1],
                 bt_mac[2],
                 bt_mac[3],
                 bt_mac[4],
                 bt_mac[5]);
        ESP_LOGW(TAG, "Pair the controller to this MAC (or set CONFIG_PAIRING_DEVICE_MAC in menuconfig) and reboot.");
    } else {
        ESP_LOGW(TAG, "Paired MAC is unknown and local BT MAC could not be read (err=0x%x).", ret);
    }
}

void app_main(void) {
    ESP_ERROR_CHECK(init_nvs());

    TickType_t wait_start_tick = xTaskGetTickCount();

    while (!is_paired_ps4_mac_registered()) {
        const char *paired_device_mac = get_paired_device_mac();

        if (paired_device_mac != NULL) {
            bool register_ok = register_paired_ps4_mac(paired_device_mac);
            if (register_ok) {
                ESP_LOGI(TAG, "Registered paired MAC for PS4: %s", paired_device_mac);
                break;
            }

            ESP_LOGW(TAG, "Ignoring invalid paired MAC format: %s", paired_device_mac);
        } else {
            ESP_LOGW(TAG, "Waiting for paired MAC from pairing module...");
        }

        if ((xTaskGetTickCount() - wait_start_tick) >= MAC_WAIT_TIMEOUT_TICKS) {
            log_local_bt_mac_for_pairing();
            break;
        }

        vTaskDelay(MAC_RETRY_INTERVAL_TICKS);
    }

    init_bluetooth_ps4();

    led_init();

    ps4_state_t state = {0};
    (void)TAG;
    bool concheck = false;
    while (1) {
        concheck = get_ps4_state(&state);
    #if 0
        if (concheck) {
            ESP_LOGI(TAG, "PS4 connected");
        } else {
            ESP_LOGI(TAG, "PS4 not connected");
        }
    #endif
        // stateの内容をログに出力
    #if 0
        ESP_LOGI(TAG, "LX: %d, LY: %d, RX: %d, RY: %d", state.lx, state.ly, state.rx, state.ry);
        ESP_LOGI(TAG, "Buttons: Cross: %d, Circle: %d, Triangle: %d, Square: %d, PS: %d, L1: %d, R1: %d, L2: %d, R2: %d", state.cross, state.circle, state.triangle, state.square, state.ps, state.l1, state.r1, state.l2, state.r2);
    #endif
        
        // main.cのwhileループ内です。
        // 💡この直前に、先ほど作った led_status_monitor(connected); を割り込ませるコードを書いて。
        led_status_monitor(concheck);
        // LEDの制御は20ms周期で爆速処理
        led_update(&state);
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}
