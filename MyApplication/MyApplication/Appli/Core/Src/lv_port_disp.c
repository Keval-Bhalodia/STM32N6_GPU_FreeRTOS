/*
 * lv_port_disp.c
 *
 *  Created on: Mar 13, 2026
 *      Author: Silicon-Signals
 */
#include "lvgl.h"
#include "lv_port_disp.h"

/* draw buffer */
static lv_color_t buf1[800 * 40];

static lv_display_t *disp;

/* flush function (temporary version) */
static void my_flush_cb(lv_display_t *disp_drv, const lv_area_t *area, uint8_t *px_map)
{
    uint16_t x, y;
    uint16_t *fb = (uint16_t *)0x34146000;

    for(y = area->y1; y <= area->y2; y++)
    {
        for(x = area->x1; x <= area->x2; x++)
        {
            fb[y * 800 + x] = *((uint16_t *)px_map);
            px_map += 2;
        }
    }

    lv_display_flush_ready(disp_drv);
}

void lv_port_disp_init(void)
{
    disp = lv_display_create(800, 480);

    lv_display_set_buffers(disp, buf1, NULL, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);

    lv_display_set_flush_cb(disp, my_flush_cb);
}



