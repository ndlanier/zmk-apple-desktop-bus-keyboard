/*
 *
 * Copyright (c) 2023 The ZMK Contributors
 * SPDX-License-Identifier: MIT
 *
 */

#include <zephyr/kernel.h>
#include "util.h"

LV_IMAGE_DECLARE(bolt);

void rotate_canvas(lv_obj_t *canvas, lv_color_t cbuf[]) {
    static lv_color_t cbuf_tmp[CANVAS_SIZE * CANVAS_SIZE];
    memcpy(cbuf_tmp, cbuf, sizeof(cbuf_tmp));

    lv_image_dsc_t img;
    img.header.magic = LV_IMAGE_HEADER_MAGIC;
    img.header.cf = LV_COLOR_FORMAT_NATIVE;
    img.header.w = CANVAS_SIZE;
    img.header.h = CANVAS_SIZE;
    img.header.stride = CANVAS_SIZE * sizeof(lv_color_t);
    img.data_size = sizeof(cbuf_tmp);
    img.data = (const uint8_t *)cbuf_tmp;

    lv_canvas_fill_bg(canvas, LVGL_BACKGROUND, LV_OPA_COVER);

    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);

    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);
    img_dsc.src = &img;
    img_dsc.rotation = 1800; /* 180 degrees in 0.1 degree units */
    img_dsc.pivot.x = CANVAS_SIZE / 2;
    img_dsc.pivot.y = CANVAS_SIZE / 2;

    lv_area_t area = {0, 0, CANVAS_SIZE - 1, CANVAS_SIZE - 1};
    lv_draw_image(&layer, &img_dsc, &area);

    lv_canvas_finish_layer(canvas, &layer);
}

void draw_battery(lv_obj_t *canvas, lv_layer_t *layer, const struct status_state *state) {
    lv_draw_rect_dsc_t rect_black_dsc;
    init_rect_dsc(&rect_black_dsc, LVGL_BACKGROUND);
    lv_draw_rect_dsc_t rect_white_dsc;
    init_rect_dsc(&rect_white_dsc, LVGL_FOREGROUND);

    /* Battery outline */
    lv_area_t area1 = {0, 2, 28, 13};
    lv_draw_rect(layer, &rect_white_dsc, &area1);

    /* Battery inner (black) */
    lv_area_t area2 = {1, 3, 27, 12};
    lv_draw_rect(layer, &rect_black_dsc, &area2);

    /* Battery level */
    int battery_width = (state->battery + 2) / 4;
    if (battery_width > 0) {
        lv_area_t area3 = {2, 4, 2 + battery_width - 1, 11};
        lv_draw_rect(layer, &rect_white_dsc, &area3);
    }

    /* Battery tip */
    lv_area_t area4 = {30, 5, 32, 10};
    lv_draw_rect(layer, &rect_white_dsc, &area4);

    lv_area_t area5 = {31, 6, 31, 9};
    lv_draw_rect(layer, &rect_black_dsc, &area5);

    if (state->charging) {
        lv_draw_image_dsc_t img_dsc;
        lv_draw_image_dsc_init(&img_dsc);
        img_dsc.src = &bolt;

        lv_area_t img_area = {9, -1, 9 + 10, -1 + 17};
        lv_draw_image(layer, &img_dsc, &img_area);
    }
}

void init_label_dsc(lv_draw_label_dsc_t *label_dsc, lv_color_t color, const lv_font_t *font,
                    lv_text_align_t align) {
    lv_draw_label_dsc_init(label_dsc);
    label_dsc->color = color;
    label_dsc->font = font;
    label_dsc->align = align;
}

void init_rect_dsc(lv_draw_rect_dsc_t *rect_dsc, lv_color_t bg_color) {
    lv_draw_rect_dsc_init(rect_dsc);
    rect_dsc->bg_color = bg_color;
    rect_dsc->bg_opa = LV_OPA_COVER;
}

void init_line_dsc(lv_draw_line_dsc_t *line_dsc, lv_color_t color, uint8_t width) {
    lv_draw_line_dsc_init(line_dsc);
    line_dsc->color = color;
    line_dsc->width = width;
}

void init_arc_dsc(lv_draw_arc_dsc_t *arc_dsc, lv_color_t color, uint8_t width) {
    lv_draw_arc_dsc_init(arc_dsc);
    arc_dsc->color = color;
    arc_dsc->width = width;
}
