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

#include <linux/of_platform.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <linux/qpnp/pin.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/leds.h>
#include <linux/qpnp/pwm.h>
#include <linux/err.h>
#include <linux/string.h>

#include "../mdss_dsi.h"
#include "../mdss_mdp.h"
#include "../mdss_fb.h"

static struct mdss_dsi_ctrl_pdata *globalpdata[2]  = {NULL,NULL};
static struct msm_fb_data_type *mfd_data=NULL;
extern struct mdss_dsi_ctrl_pdata *mdss_dsi_get_ctrl(u32 ctrl_id);
#define PRIMARY_FB0_NUM 0
void fih_mdss_panel_feature_parse_dt(struct device_node *np,struct mdss_dsi_ctrl_pdata *ctrl_pdata)
{
	struct mdss_panel_info *pinfo = &(ctrl_pdata->panel_data.panel_info);
	if(pinfo==NULL)
		return;
#ifdef CONFIG_PANEL_COLOR_MANAGERIAL
	pinfo->color_managerial=of_property_read_bool(np,
			"fih,color_managerial");
	pr_info("%s:Feature color managerial is %s\n", __func__, pinfo->color_managerial?"on":"off");
#endif
	return;
}
struct mdss_dsi_ctrl_pdata *fih_mdss_dsi_get_ctrl(u32 ctrl_id)
{
	if (ctrl_id >= DSI_CTRL_MAX)
		return NULL;
	if (globalpdata[ctrl_id]==NULL){
		return NULL;
	}

	return globalpdata[ctrl_id];
}
EXPORT_SYMBOL(fih_mdss_dsi_get_ctrl);
void fih_mdss_dsi_global_init(struct platform_device *pdev,struct mdss_dsi_ctrl_pdata *ctrl_pdata,u32 ctrl_id)
{
	if(ctrl_id >= DSI_CTRL_MAX)
	{
		pr_err("%s: FAILED: DSI%d out of bus \n",__func__,ctrl_id);
		return; 
	}
	if(ctrl_id==DSI_CTRL_0)
	{
		globalpdata[DSI_CTRL_0] = devm_kzalloc(&pdev->dev,
					  sizeof(struct mdss_dsi_ctrl_pdata),
					  GFP_KERNEL);
		if (!globalpdata[DSI_CTRL_0]) {
			pr_err("%s: FAILED: cannot alloc dsi ctr - gpdata\n",__func__);
			goto error_no_mem;
		}
		globalpdata[DSI_CTRL_0] = mdss_dsi_get_ctrl(ctrl_id);
		platform_set_drvdata(pdev, globalpdata[DSI_CTRL_0]);


	}else if(ctrl_id==DSI_CTRL_1)
	{
		globalpdata[DSI_CTRL_1] = devm_kzalloc(&pdev->dev,
					  sizeof(struct mdss_dsi_ctrl_pdata),
					  GFP_KERNEL);
		if (!globalpdata[DSI_CTRL_1]) {
			pr_err("%s: FAILED: cannot alloc dsi ctr - spdata\n",__func__);
			goto error_no_mem;
		}
		globalpdata[DSI_CTRL_1] = mdss_dsi_get_ctrl(ctrl_id);
		platform_set_drvdata(pdev, globalpdata[DSI_CTRL_1]);

	}else
	{
		pr_err("%s: Out of bus number(%d)\n",__func__,ctrl_id);
		return;
	}


	pr_info("\n\n***%s, probe pass return \n\n", __func__);
	return;
error_no_mem:
	if(ctrl_id==DSI_CTRL_0)
		devm_kfree(&pdev->dev, globalpdata[DSI_CTRL_0]);
	else if(ctrl_id==DSI_CTRL_1)
		devm_kfree(&pdev->dev, globalpdata[DSI_CTRL_1]);
	return;
}

void fih_mdss_fb_global_init(struct platform_device *pdev,struct msm_fb_data_type *mfd,u32 index)
{
	if(index > PRIMARY_FB0_NUM)
	{
		return;
	}
	if(index==PRIMARY_FB0_NUM)
	{
		mfd_data = devm_kzalloc(&pdev->dev,
					  sizeof(struct msm_fb_data_type),
					  GFP_KERNEL);
		if (!mfd_data) {
			pr_err("%s: FAILED: cannot alloc fb0 data\n",__func__);
			goto error_no_mem;
		}
		mfd_data = mfd;
		platform_set_drvdata(pdev, mfd_data);
	}


	pr_info("\n\n***%s, probe Done \n\n", __func__);
	return;
error_no_mem:
	if(index==PRIMARY_FB0_NUM)
		devm_kfree(&pdev->dev, mfd_data);
	return;
}
struct msm_fb_data_type *fih_mdss_primary_fb_get_ctrl(void)
{

	if(mfd_data==NULL)
		return NULL;
	else
		return mfd_data;
}
EXPORT_SYMBOL(fih_mdss_primary_fb_get_ctrl);


