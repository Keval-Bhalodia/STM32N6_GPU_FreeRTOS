/*
 * lv_port_disp.c
 *
 *  Created on: Mar 13, 2026
 *      Author: Silicon-Signals
 */

#include "lvgl.h"
#include "lv_port_disp.h"
#include "stm32n6xx_hal.h"
#include "stm32n6570_discovery_lcd.h"

/* Display resolution */
#define MY_DISP_HOR_RES   800
#define MY_DISP_VER_RES   480

/* LTDC framebuffer address */
#define FB1_ADDR  0x34146000
#define FB2_ADDR  0x342BD000

static lv_color_t *fb1 = (lv_color_t *)FB1_ADDR;
static lv_color_t *fb2 = (lv_color_t *)FB2_ADDR;

/* LVGL display object */
static lv_display_t *disp;

/* Flush function (very simple in DIRECT mode) */
static void disp_flush(lv_display_t *disp_drv,
                       const lv_area_t *area,
                       uint8_t *px_map)
{
    uint32_t addr = 0x34146000 +
                    ((area->y1 * 800) + area->x1) * sizeof(lv_color_t);

    uint32_t size =
        (area->x2 - area->x1 + 1) *
        (area->y2 - area->y1 + 1) *
        sizeof(lv_color_t);

    SCB_CleanDCache_by_Addr((uint32_t *)addr, size);

    lv_display_flush_ready(disp_drv);
}

/* Initialize LVGL display driver */
void lv_port_disp_init(void)
{
	lv_init();
    /* Create LVGL display */
    disp = lv_display_create(MY_DISP_HOR_RES, MY_DISP_VER_RES);

    /* Use LTDC framebuffer directly */
    lv_display_set_buffers(disp,
                           fb1,
                           fb2,
                           MY_DISP_HOR_RES * MY_DISP_VER_RES * sizeof(lv_color_t),
                           LV_DISPLAY_RENDER_MODE_DIRECT);

    /* Register flush callback */
    lv_display_set_flush_cb(disp, disp_flush);
}
