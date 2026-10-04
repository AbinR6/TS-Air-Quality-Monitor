#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

/* Checksum calculation logic matching Plantower PMS5003 */
static bool verify_pms_checksum(const uint8_t *buffer, int len)
{
    if (len != 32) return false;
    if (buffer[0] != 0x42 || buffer[1] != 0x4D) return false;

    uint16_t sum = 0;
    for (int i = 0; i < 30; i++) {
        sum += buffer[i];
    }
    uint16_t expected = ((uint16_t)buffer[30] << 8) | buffer[31];
    return (sum == expected);
}

void test_pms_checksum_validation(void)
{
    printf("[TEST] Running test_pms_checksum_validation...\n");

    /* Simulated valid 32-byte frame */
    uint8_t valid_frame[32] = {
        0x42, 0x4D, 0x00, 0x1C,
        0x00, 0x0A, /* PM1.0 std = 10 */
        0x00, 0x0F, /* PM2.5 std = 15 */
        0x00, 0x19, /* PM10 std = 25 */
        0x00, 0x0A, 0x00, 0x0F, 0x00, 0x19,
        0x03, 0xE8, 0x01, 0xF4, 0x00, 0x64, 0x00, 0x14, 0x00, 0x05, 0x00, 0x01,
        0x97, 0x00, /* Version, Error */
        0x00, 0x00  /* Placeholder checksum */
    };

    /* Compute valid checksum */
    uint16_t sum = 0;
    for (int i = 0; i < 30; i++) {
        sum += valid_frame[i];
    }
    valid_frame[30] = (uint8_t)(sum >> 8);
    valid_frame[31] = (uint8_t)(sum & 0xFF);

    assert(verify_pms_checksum(valid_frame, 32) == true);

    /* Corrupt one byte to simulate noise */
    valid_frame[6] ^= 0xFF;
    assert(verify_pms_checksum(valid_frame, 32) == false);

    printf("[TEST] test_pms_checksum_validation: PASS\n");
}

int main(void)
{
    test_pms_checksum_validation();
    printf("[ALL TESTS PASSED]\n");
    return 0;
}
