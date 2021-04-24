/*
 *  stmvl53l0-cci.h - Linux kernel modules for STM VL53L0 FlightSense TOF sensor
 *
 *  Copyright (C) 2016 STMicroelectronics Imaging Division
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */
/*
 * Defines
 */
#ifndef STMVL53L0_CCI_H
#define STMVL53L0_CCI_H
#include <linux/types.h>

#ifdef CAMERA_CCI
#include <soc/qcom/camera2.h>
#include "msm_camera_i2c.h"
#include "msm_camera_dt_util.h"
#include "msm_camera_io_util.h"
#include "msm_cci.h"
#include <linux/io.h>
#include <linux/pinctrl/consumer.h>

#define	MSM_TOF_MAX_VREGS (10)

#define VL53L0_PINCTRL_STATE_DEFAULT "vl53l0_default"
#define VL53L0_PINCTRL_STATE_SLEEP "vl53l0_sleep"
struct msm_tof_vreg {
	struct camera_vreg_t *cam_vreg;
	void *data[MSM_TOF_MAX_VREGS];
	int num_vreg;
};

struct vl53l0_pinctrl_res {
	struct pinctrl *pinctrl;
	struct pinctrl_state *gpio_state_active;
	struct pinctrl_state *gpio_state_suspend;
};


struct cci_data {
	struct msm_camera_i2c_client g_client;
	struct msm_camera_i2c_client *client;
	struct platform_device *pdev;
	enum msm_camera_device_type_t device_type;
	enum cci_i2c_master_t cci_master;
	struct msm_tof_vreg vreg_cfg;
	struct msm_sd_subdev msm_sd;
	struct v4l2_subdev sdev;
	struct v4l2_subdev_ops *subdev_ops;
	struct msm_pinctrl_info cci_pinctrl;
	char subdev_initialized;
	uint32_t subdev_id;
	uint8_t power_up;
	int xshut_en_gpio;
	uint8_t cci_pinctrl_status;
	struct vl53l0_pinctrl_res pin_res;
};

int stmvl53l0_init_cci(void);
void stmvl53l0_exit_cci(void *);
int stmvl53l0_power_down_cci(void *);
int stmvl53l0_power_up_cci(void *, unsigned int *);
int stmvl53l0_cci_power_status(void *);
#endif /* CAMERA_CCI */
#endif /* STMVL53L0_CCI_H */
