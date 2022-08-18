#include <linux/string.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
/*modify by shenwenbin for open double tap wakeup 20190516 begin*/
#include <linux/lct_tp_fm_info.h>
/*modify by shenwenbin for open double tap wakeup 20190516 end*/

static struct kobject *msm_tp_device;
static struct kobject *tp_gesture_device;
static u16 tp_ver_show = 0;
static char tp_ver_show_str[80] = {0x00};
static char module_name[80] = {0x00};
static int gesture_wakeup = 0;
static ssize_t msm_tp_module_id_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	ssize_t rc = 0;

    char tp_version[60] = {0}; //liyong2 2014.1.9

    if((0 == tp_ver_show) && (0 == strlen(tp_ver_show_str)))
        strcpy(tp_version,"no tp");
    else
    {
        sprintf(tp_version, "[Vendor]%s,%s\n", (strlen(module_name) ? module_name : "Unknown"),
				(strlen(tp_ver_show_str) ? tp_ver_show_str : "Unknown product"));
    }
	
	sprintf(buf, "%s\n", tp_version);
	rc = strlen(buf) + 1;

	return rc;
}
static ssize_t msm_tp_gesture_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	ssize_t rc = 0;

	sprintf(buf, "%d\n", gesture_wakeup);
	rc = strlen(buf) + 1;

	return rc;
}
static ssize_t msm_tp_gesture_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t len)
{
	if(1!=sscanf(buf,"%d",&gesture_wakeup))
		pr_info("%s: failed\n", __func__);
	
	return len;

}

static DEVICE_ATTR(wakeup_gesture_enable, 0644, msm_tp_gesture_show, msm_tp_gesture_store);

static DEVICE_ATTR(tp_info, 0444, msm_tp_module_id_show, NULL);

static int tp_fm_creat_sys_entry(void)
{
    int32_t rc = 0;

	msm_tp_device = kobject_create_and_add("android_tp", NULL);
	if (msm_tp_device == NULL) {
		pr_info("%s: subsystem_register failed\n", __func__);
		rc = -ENOMEM;
		return rc ;
	}

	rc = sysfs_create_file(msm_tp_device, &dev_attr_tp_info.attr);
	if (rc) {
		pr_info("%s: sysfs_create_file failed\n", __func__);
		kobject_del(msm_tp_device);
	}


	tp_gesture_device= kobject_create_and_add("android_touch", NULL);
	if (tp_gesture_device == NULL) {
		pr_info("%s: subsystem_register failed\n", __func__);
		rc = -ENOMEM;
		return rc ;
	}
	rc = sysfs_create_file(tp_gesture_device, &dev_attr_wakeup_gesture_enable.attr);
	if (rc) {
		pr_info("%s: sysfs_create_file failed\n", __func__);
		kobject_del(tp_gesture_device);
	}

	return 0 ;
}

static ssize_t tp_proc_tp_info_read(struct file *file, char __user *buf, size_t size, loff_t *ppos)
{
    int cnt=0;
    char *page = NULL;

	page = kzalloc(128, GFP_KERNEL);

    if((0 == strlen(module_name)) && (0 == tp_ver_show) && (0 == strlen(tp_ver_show_str)))
        cnt = sprintf(page, "no tp\n");
	else
	{
		// xuke @ 20140811
		cnt = sprintf(page, "[Vendor]%s,%s\n", (strlen(module_name) ? module_name : "Unknown"),
				(strlen(tp_ver_show_str) ? tp_ver_show_str : "Unknown product"));
	}

	cnt = simple_read_from_buffer(buf, size, ppos, page, cnt);
//	printk("%s, page=%s, cnt=%d\n", __func__, page, cnt);
	
	kfree(page);
    return cnt;
}
static ssize_t tp_proc_gesture_wakeup_read(struct file *file, char __user *buf, size_t size, loff_t *ppos)
{
	int cnt=0;
	char buff[12] = {0};
	cnt=sprintf(buff,"%d\n",gesture_wakeup);
	cnt += sprintf(buff + cnt, "\n");
	if(copy_to_user(buf, buff,sizeof(buff)))
		pr_err("%s %d copy_to_user \n",__func__,__LINE__);
	//printk("%s,%d,gesture_wakeup =%d\n",__func__,__LINE__,gesture_wakeup);
	return cnt;

}

static ssize_t tp_proc_gesture_wakeup_write(struct file *file, const char *buff,size_t len, loff_t *pos)
{
	char buf[12] = {0};
	if(len > 12)
		len =12;
	if(copy_from_user(buf, buff, len))
		pr_err("%s %d copy_from_user \n",__func__,__LINE__);
	if(buf[0]=='0'||buf[0]==0)
		gesture_wakeup = 0;
	else
		gesture_wakeup = 1;

	//printk("%s,%d,gesture_wakeup=%d\n",__func__,__LINE__,gesture_wakeup);
	return len;
}


static const struct file_operations tp_proc_tp_info_fops = {
	.read		= tp_proc_tp_info_read,
};

static const struct file_operations tp_proc_gesture_wakeup_fops = {
	.read		= tp_proc_gesture_wakeup_read,
	.write		= tp_proc_gesture_wakeup_write,	
};

static int tp_fm_creat_proc_entry(void)
{
    struct proc_dir_entry *proc_entry_tp;

	proc_entry_tp = proc_create_data("tp_info", 0444, NULL, &tp_proc_tp_info_fops, NULL);
	if (IS_ERR_OR_NULL(proc_entry_tp))
	{
		pr_err("add /proc/tp_info error \n");
	}

	proc_entry_tp = proc_create_data("gesture_wakeup", 0666, NULL, &tp_proc_gesture_wakeup_fops, NULL);
	if (IS_ERR_OR_NULL(proc_entry_tp))
	{
		pr_err("add /proc/gesture_wakeup error \n");
	}

    return 0;
}

int tp_gesture_wakeup(void)
{
	return gesture_wakeup;
}


int init_tp_fm_info(u16 version_info_num, char* version_info_str, char *name)
{
    tp_ver_show = version_info_num;

    if (NULL != version_info_str)
        strcpy(tp_ver_show_str, version_info_str);
    if (NULL != name)
        strcpy(module_name, name);

    tp_fm_creat_sys_entry();
    tp_fm_creat_proc_entry();		// xuke @ 20140811		Add proc entry for checklist.

    return 0;
}

void update_tp_fm_info(char* version_info_str)
{
    if (NULL != version_info_str) {
		memset(tp_ver_show_str, 0, sizeof(tp_ver_show_str));
        strcpy(tp_ver_show_str, version_info_str);
    }
}

MODULE_DESCRIPTION("GTP Series Driver");
MODULE_LICENSE("GPL");
