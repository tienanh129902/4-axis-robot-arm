#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "string.h"
#include "ledc_app.h"
#include <sys/param.h>
#include "http_server_app.h"
#include "esp_netif.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#define ESP_WIFI_SSID "ESP32 wifi"
#define ESP_WIFI_PASS "12091999"
#define MAX_STA_CONN 2

static const char *TAG = "wifi softAP";

static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED)
    {
        wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *)event_data;
        ESP_LOGI(TAG, "station " MACSTR " join, AID=%d",
                 MAC2STR(event->mac), event->aid);
    }
    else if (event_id == WIFI_EVENT_AP_STADISCONNECTED)
    {
        wifi_event_ap_stadisconnected_t *event = (wifi_event_ap_stadisconnected_t *)event_data;
        ESP_LOGI(TAG, "station " MACSTR " leave, AID=%d",
                 MAC2STR(event->mac), event->aid);
    }
}

esp_err_t wifi_init_softap(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = ESP_WIFI_SSID,
            .ssid_len = strlen(ESP_WIFI_SSID),
            .password = ESP_WIFI_PASS,
            .max_connection = MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK},
    };
    if (strlen(ESP_WIFI_PASS) == 0)
    {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s with default address", ESP_WIFI_SSID, ESP_WIFI_PASS);
    ESP_ERROR_CHECK(esp_wifi_start());
    return ESP_OK;
}

void slider1_data_callback(char *data, int len)
{
    char number_str[10];
    memcpy(number_str, data, len + 1);
    int duty1 = atoi(number_str);
    printf("Servo 1 get duty %d\n", duty1);
    LedC_Set_Duty(1,duty1);
}

void slider2_data_callback(char *data, int len)
{
    char number_str[10];
    memcpy(number_str, data, len + 1);
    int duty2 = atoi(number_str);
    printf("Servo 2 get duty %d\n", duty2);
    LedC_Set_Duty(2,duty2);
}

void slider3_data_callback(char *data, int len)
{
    char number_str[10];
    memcpy(number_str, data, len + 1);
    int duty3 = atoi(number_str);
    printf("Servo 3 get duty %d\n", duty3);
    // LedC_Set_Duty(3,duty3);
}

void slider4_data_callback(char *data, int len)
{
    char number_str[10];
    memcpy(number_str, data, len + 1);
    int duty4 = atoi(number_str);
    printf("Servo 4 get duty %d\n", duty4);
    LedC_Set_Duty(4,duty4);
}

void page_load_call_back()
{
    // don't know what to do :)))
}

void app_main(void)
{
    http_set_callback_slider1(slider1_data_callback);
    http_set_callback_slider2(slider2_data_callback);
    http_set_callback_slider3(slider3_data_callback);
    http_set_callback_slider4(slider4_data_callback);
    http_set_page_load_callback(page_load_call_back);

    LedC_Init(50);
    LedC_Add_Pin(13,1);
    LedC_Add_Pin(12,2);
    // LedC_Add_Pin(1,3);
    LedC_Add_Pin(27,4);

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
    ESP_ERROR_CHECK(wifi_init_softap());
    start_webserver();
}
