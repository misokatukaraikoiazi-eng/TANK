#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sdkconfig.h"
#include "nvs.h"

static bool is_valid_mac_string(const char *mac)
{
    unsigned int b0 = 0;
    unsigned int b1 = 0;
    unsigned int b2 = 0;
    unsigned int b3 = 0;
    unsigned int b4 = 0;
    unsigned int b5 = 0;

    if (mac == NULL) {
        return false;
    }

    return sscanf(mac, "%2x:%2x:%2x:%2x:%2x:%2x", &b0, &b1, &b2, &b3, &b4, &b5) == 6;
}

// Priority:
// 1) CONFIG_PAIRING_DEVICE_MAC (menuconfig)
// 2) NVS namespace "pairing", key "device_mac"
// 3) PAIRING_DEVICE_MAC env var (host-side tests only)
__attribute__((weak)) const char *get_paired_device_mac(void)
{
#if defined(CONFIG_PAIRING_DEVICE_MAC)
    if (CONFIG_PAIRING_DEVICE_MAC[0] != '\0' && is_valid_mac_string(CONFIG_PAIRING_DEVICE_MAC)) {
        return CONFIG_PAIRING_DEVICE_MAC;
    }
#endif

    static char nvs_mac[18] = {0};
    nvs_handle_t handle = 0;
    size_t required = sizeof(nvs_mac);

    if (nvs_open("pairing", NVS_READONLY, &handle) == ESP_OK) {
        esp_err_t nvs_ret = nvs_get_str(handle, "device_mac", nvs_mac, &required);
        nvs_close(handle);

        if (nvs_ret == ESP_OK && required > 0 && required <= sizeof(nvs_mac) && is_valid_mac_string(nvs_mac)) {
            return nvs_mac;
        }
    }

    const char *env_mac = getenv("PAIRING_DEVICE_MAC");
    if (env_mac != NULL && env_mac[0] != '\0' && is_valid_mac_string(env_mac)) {
        return env_mac;
    }

    return NULL;
}
