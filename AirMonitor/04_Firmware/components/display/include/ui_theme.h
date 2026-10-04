#ifndef UI_THEME_H
#define UI_THEME_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* RGB565 Modern Theme Palette */
#define COLOR_BG_DARK       0x0841  /* Deep slate/navy background #0a0e17 */
#define COLOR_PANEL_BG      0x18E3  /* Card panel surface #1a2230 */
#define COLOR_TEXT_PRIMARY  0xFFFF  /* Crisp white #ffffff */
#define COLOR_TEXT_MUTED    0x8C71  /* Secondary gray #8b9bb4 */

#define COLOR_AQI_GREEN     0x2664  /* Clean air green #22c55e */
#define COLOR_AQI_YELLOW    0xEF62  /* Moderate yellow #eab308 */
#define COLOR_AQI_ORANGE    0xFBC0  /* Unhealthy orange #f97316 */
#define COLOR_AQI_RED       0xEA06  /* Severe red #ef4444 */
#define COLOR_AQI_PURPLE    0xA19B  /* Purple alert #a855f7 */

#define COLOR_CO2_CYAN      0x067F  /* Technical cyan #06b6d4 */
#define COLOR_TEMP_AMBER    0xFBE0  /* Temperature amber #f59e0b */
#define COLOR_RH_BLUE       0x3CDA  /* Humidity sky blue #38bdf8 */

#ifdef __cplusplus
}
#endif

#endif /* UI_THEME_H */
