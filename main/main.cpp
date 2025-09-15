#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "services/wifi-service/wifi_service.h"

#define WIFI_SSID "TAF"
#define WIFI_PASS "semangatsemangatokok"
#define PIN_ETH GPIO_NUM_26

EventGroupHandle_t event_group;

extern "C" void app_main(void)
{
    // Setup WiFi
    WifiDriver wifi_driver;
    wifi_driver.initWifiDriver(WIFI_SSID, WIFI_PASS);

    LedDriver led_driver(PIN_ETH);

    // Buat service untuk hubungkan wifi dengan led
    WifiService wifi_service(wifi_driver, led_driver);
    
    while (true) {
        wifi_service.wifiIndicatorStatus();
        vTaskDelay(pdMS_TO_TICKS(100));
    }   
}