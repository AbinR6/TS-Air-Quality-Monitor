#ifndef NVS_MANAGER_H
#define NVS_MANAGER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize NVS configuration namespace.
 */
esp_err_t nvs_manager_init(void);

/**
 * @brief Save a string parameter to NVS.
 */
esp_err_t nvs_manager_set_str(const char *key, const char *value);

/**
 * @brief Read a string parameter from NVS.
 */
esp_err_t nvs_manager_get_str(const char *key, char *out_val, size_t max_len);

/**
 * @brief Save an unsigned 32-bit integer to NVS.
 */
esp_err_t nvs_manager_set_u32(const char *key, uint32_t value);

/**
 * @brief Read an unsigned 32-bit integer from NVS.
 */
esp_err_t nvs_manager_get_u32(const char *key, uint32_t *out_val);

#ifdef __cplusplus
}
#endif

#endif /* NVS_MANAGER_H */
