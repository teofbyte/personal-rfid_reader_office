#include "wifi_driver.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "string.h"

static const char *TAG = "WifiDriver";

WifiDriver::WifiDriver() {}

void WifiDriver::initWifiDriver(const char *ssid, const char *password)
{
    // Inisialisasi NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Inisialisasi TCP/IP stack & event loop
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    // Inisialisasi WiFi
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // Konfigurasi WiFi STA
    wifi_config_t wifi_config = {};
    strcpy((char *)wifi_config.sta.ssid, ssid);
    strcpy((char *)wifi_config.sta.password, password);
    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));

    // Registrasi event handler
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &WifiDriver::wifi_event_handler_static,
                                                        this,
                                                        nullptr));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &WifiDriver::wifi_event_handler_static,
                                                        this,
                                                        nullptr));

    // Start WiFi
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "WiFi driver initialization completed");
}

void WifiDriver::wifi_event_handler(esp_event_base_t event_base,
                                    int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        ESP_LOGI(TAG, "WiFi STA start, connecting...");
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        connected = false;
        ESP_LOGW(TAG, "WiFi disconnected, retrying...");
        esp_wifi_connect(); // selalu retry
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "Got IP:" IPSTR, IP2STR(&event->ip_info.ip));
        connected = true;
    }
}

void WifiDriver::wifi_event_handler_static(void *arg, esp_event_base_t event_base,
                                          int32_t event_id, void *event_data)
{
    WifiDriver *instance = static_cast<WifiDriver *>(arg);
    instance->wifi_event_handler(event_base, event_id, event_data);
}

bool WifiDriver::isconnectedWifi()
{
    return connected;
}

// void WifiDriver::reconnectWifi()
// {
//     ESP_LOGI(TAG, "Manual reconnect triggered");
//     esp_wifi_disconnect();
//     esp_wifi_connect();
// }

WifiDriver::~WifiDriver()
{
    esp_wifi_stop();
    esp_wifi_deinit();
    esp_event_loop_delete_default();
}