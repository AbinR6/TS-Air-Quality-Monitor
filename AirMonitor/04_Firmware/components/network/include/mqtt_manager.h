#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <stdbool.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize MQTT client with broker URI.
 */
esp_err_t mqtt_manager_init(const char *broker_uri, const char *client_id);

/**
 * @brief Start MQTT client daemon.
 */
esp_err_t mqtt_manager_start(void);

/**
 * @brief Publish telemetry payload to device topic.
 */
esp_err_t mqtt_manager_publish_telemetry(const char *json_payload);

/**
 * @brief Return true if MQTT client is connected to broker.
 */
bool mqtt_manager_is_connected(void);

#ifdef __cplusplus
}
#endif

#endif /* MQTT_MANAGER_H */
