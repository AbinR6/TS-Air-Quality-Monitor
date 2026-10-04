#include "nvs_manager.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_log.h"

static const char *TAG = "NVS_MGR";
#define NVS_NAMESPACE   "air_config"

esp_err_t nvs_manager_init(void)
{
    ESP_LOGI(TAG, "Initializing NVS configuration storage...");
    return ESP_OK;
}

esp_err_t nvs_manager_set_str(const char *key, const char *value)
{
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) return err;

    err = nvs_set_str(handle, key, value);
    if (err == ESP_OK) err = nvs_commit(handle);
    nvs_close(handle);
    return err;
}

esp_err_t nvs_manager_get_str(const char *key, char *out_val, size_t max_len)
{
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READONLY, &handle);
    if (err != ESP_OK) return err;

    size_t required_size = max_len;
    err = nvs_get_str(handle, key, out_val, &required_size);
    nvs_close(handle);
    return err;
}

esp_err_t nvs_manager_set_u32(const char *key, uint32_t value)
{
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) return err;

    err = nvs_set_u32(handle, key, value);
    if (err == ESP_OK) err = nvs_commit(handle);
    nvs_close(handle);
    return err;
}

esp_err_t nvs_manager_get_u32(const char *key, uint32_t *out_val)
{
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READONLY, &handle);
    if (err != ESP_OK) return err;

    err = nvs_get_u32(handle, key, out_val);
    nvs_close(handle);
    return err;
}
