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

extern void fih_mdss_panel_parse_dt(struct device_node *np,struct mdss_dsi_ctrl_pdata *ctrl_pdata);
extern int mdss_dsi_extra_panel_power_on(struct mdss_panel_data *pdata);
extern void mdss_dsi_extra_power_init(struct dsi_shared_data *sdata,struct mdss_panel_info *pinfo);
extern int mdss_dsi_extra_panel_power_on(struct mdss_panel_data *pdata);
extern int mdss_dsi_extra_panel_power_off(struct mdss_panel_data *pdata);
extern int mdss_dsi_panel_extra_power_lp(struct mdss_panel_data *pdata, int enable);
extern void fih_mdss_panel_ulps_pwr_parse_dt(struct device_node *np,struct mdss_dsi_ctrl_pdata *ctrl_pdata);
