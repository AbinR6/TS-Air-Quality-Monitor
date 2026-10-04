#include "device_config.h"
#include "nvs_manager.h"
#include <string.h>

esp_err_t device_config_load(runtime_config_t *config)
{
    if (config == NULL) return ESP_ERR_INVALID_ARG;

    /* Populate safe factory defaults */
    strncpy(config->device_id, "airmonitor-001", sizeof(config->device_id));
    strncpy(config->wifi_ssid, "AirMonitor_Net", sizeof(config->wifi_ssid));
    config->wifi_password[0] = '\0';
    strncpy(config->mqtt_broker_uri, "mqtt://192.168.1.50:1883", sizeof(config->mqtt_broker_uri));
    config->reporting_interval_s = 60;
    config->display_brightness_pct = 80;
    config->simulation_mode = false;

    /* Attempt NVS overrides */
    nvs_manager_get_str("device_id", config->device_id, sizeof(config->device_id));
    nvs_manager_get_str("wifi_ssid", config->wifi_ssid, sizeof(config->wifi_ssid));
    nvs_manager_get_str("wifi_pass", config->wifi_password, sizeof(config->wifi_password));
    nvs_manager_get_str("mqtt_uri", config->mqtt_broker_uri, sizeof(config->mqtt_broker_uri));

    uint32_t val32 = 0;
    if (nvs_manager_get_u32("report_sec", &val32) == ESP_OK) {
        config->reporting_interval_s = (uint16_t)val32;
    }
    if (nvs_manager_get_u32("bright_pct", &val32) == ESP_OK) {
        config->display_brightness_pct = (uint8_t)val32;
    }

    return ESP_OK;
}

esp_err_t device_config_save(const runtime_config_t *config)
{
    if (config == NULL) return ESP_ERR_INVALID_ARG;

    nvs_manager_set_str("device_id", config->device_id);
    nvs_manager_set_str("wifi_ssid", config->wifi_ssid);
    nvs_manager_set_str("wifi_pass", config->wifi_password);
    nvs_manager_set_str("mqtt_uri", config->mqtt_broker_uri);
    nvs_manager_set_u32("report_sec", config->reporting_interval_s);
    nvs_manager_set_u32("bright_pct", config->display_brightness_pct);

    return ESP_OK;
}
