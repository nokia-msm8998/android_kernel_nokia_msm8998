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

extern void fih_mdss_panel_feature_parse_dt(struct device_node *np,struct mdss_dsi_ctrl_pdata *ctrl_pdata);
extern void fih_mdss_dsi_global_init(struct platform_device *pdev,struct mdss_dsi_ctrl_pdata *ctrl_pdata,u32 ctrl_id);
extern struct mdss_dsi_ctrl_pdata *fih_mdss_dsi_get_ctrl(u32 ctrl_id);
extern void fih_mdss_fb_global_init(struct platform_device *pdev,struct msm_fb_data_type *mfd,u32 index);
extern struct msm_fb_data_type *fih_mdss_primary_fb_get_ctrl(void);
