#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/mfd/compass_pmic.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/workqueue.h>
#include <asm/io.h>
#include <asm/atomic.h>
#include <mach/hardware.h>
#include <mach/bsp.h>
#include <mach/board-tccleo.h>

MODULE_LICENSE("Dual BSD/GPL");

void fuckyou_release(struct device *dev)
{
}

static struct platform_device camera_core_device = {
	.name = "fuckyou",
	.dev = {
		.release = fuckyou_release,
		},
	.id = 0,
};

unsigned int camera_daemon_pid = 0;
EXPORT_SYMBOL(camera_daemon_pid);

ssize_t camera_pid_show(struct device *kobj, struct device_attribute *attr, char *buffer)
{
        return sprintf(buffer, "pid set to: %u\n", camera_daemon_pid);
}

ssize_t camera_pid_store(struct device *kobj, struct device_attribute *attr, const char *buffer, size_t size)
{
        sscanf(buffer, "%d", &camera_daemon_pid);
        return strlen(buffer);
}

static struct device_attribute camera_attr_pid = {
        .attr = { .name = "pid", .mode = S_IRUGO | S_IWUSR },
        .show = camera_pid_show,
        .store = camera_pid_store,
};

static int fuck_camera_init(void)
{
	printk(KERN_ALERT "========= Welcome to Fuck World @ %s ==========\n", __TIMESTAMP__);

	platform_device_register(&camera_core_device);

	if (sysfs_create_file(&camera_core_device.dev.kobj, &camera_attr_pid.attr) < 0) {
		printk(KERN_ALERT "daemon pid can't create sysfs\n");
	} else {
		printk(KERN_ALERT "daemon pid create sysfs!\n");
	}
	return 0;
}

static void fuck_camera_exit(void)
{
	platform_device_unregister(&camera_core_device);
	printk(KERN_ALERT "Goodbye, fuck world\n");
}

module_init(fuck_camera_init);
module_exit(fuck_camera_exit);
