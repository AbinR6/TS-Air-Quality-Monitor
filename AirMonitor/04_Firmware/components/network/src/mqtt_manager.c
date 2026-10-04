#include "mqtt_manager.h"
#include <string.h>
#include "esp_log.h"
#include "mqtt_client.h"

static const char *TAG = "MQTT_MGR";

static esp_mqtt_client_handle_t s_client = NULL;
static bool s_connected = false;
static char s_topic[64] = "devices/airmonitor-001/telemetry";

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t)event_data;
    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT connected to broker.");
            s_connected = true;
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGW(TAG, "MQTT disconnected from broker.");
            s_connected = false;
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGD(TAG, "MQTT message published (msg_id=%d)", event->msg_id);
            break;
        default:
            break;
    }
}

esp_err_t mqtt_manager_init(const char *broker_uri, const char *client_id)
{
    ESP_LOGI(TAG, "Initializing MQTT client (Broker: %s)...", broker_uri ? broker_uri : "mqtt://127.0.0.1");

    if (client_id != NULL) {
        snprintf(s_topic, sizeof(s_topic), "devices/%s/telemetry", client_id);
    }

    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = broker_uri ? broker_uri : "mqtt://127.0.0.1",
        .credentials.client_id = client_id ? client_id : "airmonitor-001",
    };

    s_client = esp_mqtt_client_init(&mqtt_cfg);
    if (s_client == NULL) return ESP_FAIL;

    esp_mqtt_client_register_event(s_client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    return ESP_OK;
}

esp_err_t mqtt_manager_start(void)
{
    if (s_client == NULL) return ESP_ERR_INVALID_STATE;
    return esp_mqtt_client_start(s_client);
}

esp_err_t mqtt_manager_publish_telemetry(const char *json_payload)
{
    if (s_client == NULL || !s_connected || json_payload == NULL) {
        return ESP_FAIL;
    }
    int msg_id = esp_mqtt_client_publish(s_client, s_topic, json_payload, 0, 1, 0);
    return (msg_id >= 0) ? ESP_OK : ESP_FAIL;
}

bool mqtt_manager_is_connected(void)
{
    return s_connected;
}
