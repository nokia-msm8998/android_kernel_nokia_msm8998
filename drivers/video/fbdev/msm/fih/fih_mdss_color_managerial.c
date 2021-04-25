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
#include "fih_mdss_global.h"
#include "fih_msm_mdss_aod.h"

static char color_reg[2] = {0x58, 0x00};
static struct dsi_cmd_desc color_manager_cmd = {
	{DTYPE_DCS_WRITE1, 1, 0, 0, 1, sizeof(color_reg)},
	color_reg
};

static int ColoeMode=0;
#define LGD_BRIGHTNESS_ENABLE 0xFF
#define BRIGHTNESS_RANGE_LEVEL 10
#define HBM_ENABLE 1
#define HBM_DISABLE 0

#define COLOR_NORMAL	0x0
#define COLOR_INITIAL	0x1
#define COLOR_DCIP3		0x04
#define COLOR_SRGB		0x08
#define COLOR_ADOBE		0x0c

#define COLOR_MODE_UNSET 0x0
#define COLOR_MODE_COLDBOOT 0x1
#define COLOR_MODE_INIT_FINISH 0x2

void mdss_dsi_panel_color_manager(struct mdss_dsi_ctrl_pdata *ctrl, unsigned char type)
{
	struct dcs_cmd_req cmdreq;
	struct mdss_panel_info *pinfo;

	pinfo = &(ctrl->panel_data.panel_info);
	if (pinfo->dcs_cmd_by_left) {
		if (ctrl->ndx != DSI_CTRL_LEFT)
			return;
	}

	color_reg[1] = (unsigned char)type;

	memset(&cmdreq, 0, sizeof(cmdreq));
	cmdreq.cmds = &color_manager_cmd;
	cmdreq.cmds_cnt = 1;
	cmdreq.flags = CMD_REQ_COMMIT | CMD_CLK_CTRL;
	cmdreq.rlen = 0;
	cmdreq.cb = NULL;

	mdss_dsi_cmdlist_put(ctrl, &cmdreq);
}

void mdss_dsi_color_mode_restore(struct mdss_dsi_ctrl_pdata *ctrl)
{

	switch(ColoeMode){
		case COLOR_NORMAL:
		case COLOR_DCIP3:
		case COLOR_SRGB:
		case COLOR_ADOBE:
			pr_err("%s, Restore Color type %x\n",__func__,(int) ColoeMode);
			mdss_dsi_panel_color_manager(ctrl,ColoeMode);
		break;
		default:
			break;
	}
	return;
}

void mdss_dsi_colormode_init(struct mdss_dsi_ctrl_pdata *ctrl)
{
	struct mdss_panel_info *pinfo = &(ctrl->panel_data.panel_info);

	if(pinfo==NULL){
		return;
	}
	if(!pinfo->color_managerial){
		pr_err("%s, Not Support\n",__func__);
		return;
	}
	mdss_dsi_color_mode_restore(ctrl);
	return;
}

int fih_mdss_color_config(unsigned char type)
{
	struct mdss_panel_info *pinfo;
	struct mdss_dsi_ctrl_pdata *ctrl = NULL;
	struct msm_fb_data_type *mfd;

	int i =0;
	pr_err("%s, type %x\n",__func__,(int) type);
	mfd = fih_mdss_primary_fb_get_ctrl();

	if(mfd==NULL)
	{
		pr_err("%s, mfd is NULL\n",__func__);
		return 0;
	}

	if(fih_get_blank_mode()!=FB_BLANK_UNBLANK&&!mdss_fb_is_power_on_interactive(mfd)){
		pr_err("%s, The power state is not allow set up Color %d\n",__func__,type);
		return 0;
	}

	for(i=0;i<DSI_CTRL_MAX;i++)
	{
		if(fih_mdss_dsi_get_ctrl(i)==NULL){
			pr_debug("%s, NULL BUS%d\n",__func__,i);
			continue;
		}
		ctrl = fih_mdss_dsi_get_ctrl(i);
		pinfo = &ctrl->panel_data.panel_info;
		if (pinfo->dcs_cmd_by_left) {
			if (ctrl->ndx != DSI_CTRL_LEFT)
				continue;
		}

		switch(type){
			case COLOR_NORMAL:
			case COLOR_DCIP3:
			case COLOR_SRGB:
			case COLOR_ADOBE:
				pr_debug("%s,%d  %x, init =%d\n",__func__,__LINE__,(int) type,pinfo->color_managerial_init);
				if(pinfo->color_managerial_init==COLOR_MODE_UNSET)
				{
					pr_err("%s, property color type is %x\n",__func__,(int) type);
					pinfo->color_managerial_init=COLOR_MODE_COLDBOOT;
				}else{
					pr_err("%s, set up type %x\n",__func__,(int) type);
					mdss_dsi_panel_color_manager(ctrl,type);
				}
				ColoeMode = type;
			break;
			case COLOR_INITIAL:
				if(pinfo->color_managerial_init==COLOR_MODE_COLDBOOT){
					pr_err("%s,%d Initial color typ to 0x%x\n",__func__,__LINE__,(int) ColoeMode);
					mdss_dsi_colormode_init(ctrl);
					pinfo->color_managerial_init=COLOR_MODE_INIT_FINISH;
				}
				break;
			default:
				pr_err("%s, set up fail type %x\n",__func__,(int) type);
			break;
		}
	}


	return 0;

}
EXPORT_SYMBOL(fih_mdss_color_config);
int fih_mdss_high_brightness_configure(bool enable)
{
	struct msm_fb_data_type *mfd;
	struct mdss_panel_data *pdata;
	// u32 bl_level=0;
	mfd = fih_mdss_primary_fb_get_ctrl();
	if(mfd==NULL)
	{
		pr_err("%s, mfd is NULL\n",__func__);
		return 0;
	}
	mutex_lock(&mfd->bl_lock);
	if(fih_get_blank_mode()!=FB_BLANK_UNBLANK&& !mdss_fb_is_power_on_interactive(mfd))
	{
		pr_err("%s, The power state is not allow set up brightness %d\n",__func__,enable);
		mutex_unlock(&mfd->bl_lock);
		return 0;
	}
	pdata = dev_get_platdata(&mfd->pdev->dev);

	//Verify the same configure in the property
	if(pdata->high_brightness_mode==enable)
	{
		pr_err("%s, Double setup the HBM ready %s\n",__func__,enable?"Enable":"Disable");
		mutex_unlock(&mfd->bl_lock);
		return 0;
	}
	if(enable){
		if(enable!=HBM_ENABLE)
		{
			pr_err("%s, The HBM setting(%d) is not legitimate \n",__func__,enable);
			mutex_unlock(&mfd->bl_lock);
			return 0;
		}
		pdata->high_brightness_mode=1;
	}else{
		pdata->high_brightness_mode=0;
	}
	if(mfd->bl_level>=mfd->panel_info->bl_max){
		mfd->allow_bl_update=true;
		pdata->set_backlight(pdata,mfd->panel_info->bl_max);
		mfd->allow_bl_update=false;
	}

	mutex_unlock(&mfd->bl_lock);
	return 0;
}
EXPORT_SYMBOL(fih_mdss_high_brightness_configure);

bool fih_mdss_high_brightness_get(void)
{
	struct msm_fb_data_type *mfd;
	struct mdss_panel_data *pdata;

	mfd = fih_mdss_primary_fb_get_ctrl();
	if(mfd==NULL)
	{
		pr_err("%s, mfd is NULL\n",__func__);
		return 0;
	}
	pdata = dev_get_platdata(&mfd->pdev->dev);

	return pdata->high_brightness_mode;
}
EXPORT_SYMBOL(fih_mdss_high_brightness_get);


