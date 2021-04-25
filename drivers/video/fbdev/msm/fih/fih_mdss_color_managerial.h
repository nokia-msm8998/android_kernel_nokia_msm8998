/* Copyright (c) 2017, FIH Mobile Limited Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include "../mdss.h"
#include "../mdss_panel.h"
#include "../mdss_dsi.h"
#include "../mdss_fb.h"

extern int fih_mdss_color_config(unsigned char type);
extern int fih_mdss_high_brightness_configure(bool enable);
extern void mdss_dsi_color_mode_restore(struct mdss_dsi_ctrl_pdata *ctrl);
extern void mdss_dsi_colormode_init(struct mdss_dsi_ctrl_pdata *ctrl);
