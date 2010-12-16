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

#define TCC_ENABLE_BIT(X) (1 << (X))

extern unsigned int camera_daemon_pid;
void set_gpio_interrupt(void);
void set_drm_interrupt(void);

struct my_struct {
	struct delayed_work camera_wq;
	atomic_t data;
} my;

void bc_do_tasklet(struct work_struct *work)
{
	printk(KERN_ALERT "Back camera is %s\n", (gpio_get_value(GPIO_VIDEO1_DET) == 0 ? "removed" : "inserted"));
	if (gpio_get_value(GPIO_VIDEO1_DET) == 1) {	/* insert */
		struct task_struct *p = find_task_by_vpid(camera_daemon_pid);
		send_sig(SIGUSR1, p, 0);
	} else {
		int err;
		char path[256] = "/system/bin/pkill";
		char *argv[] = { path, "com.android.camera", NULL };
		static char *envp[] = { "PATH=/system/bin:/sbin:/usr/sbin:/bin:/usr/bin", NULL };

		err = call_usermodehelper(path, argv, envp, 1);
		if (err < 0) {
			printk("call_usermodehelper %d", err);
		}
	}
}

static irqreturn_t bc_irq_handler(int irq, void *data)
{
	if (!work_pending(&my.camera_wq.work))
		if (schedule_delayed_work(&my.camera_wq, 50) == 0)
			printk(KERN_ALERT "Can't schedule work!\n");
	return IRQ_HANDLED;
}

static irqreturn_t drm_irq_handler(int irq, void *data)
{
	printk(KERN_ALERT "Got an interrupt from drm, %d\n", gpio_get_value(GPIO_DRM_ID));
	return IRQ_HANDLED;
}

static int hello_init(void)
{
	printk(KERN_ALERT "========= Welcome to Hello World @ %s ==========\n", __TIMESTAMP__);
	printk(KERN_ALERT "GPIO: %d\n", gpio_get_value(GPIO_VIDEO1_DET));

	INIT_DELAYED_WORK(&my.camera_wq, bc_do_tasklet);
//	set_gpio_interrupt();
	set_drm_interrupt();

	return 0;
}

void set_gpio_interrupt(void)
{
	int ret;
	ret = gpio_request(GPIO_VIDEO1_DET, "Video1 Detect");
	if (ret) {
		printk(KERN_ALERT "Failed to request video1 detect\n");
	}

	ret = gpio_direction_input(GPIO_VIDEO1_DET);
	if (ret) {
		printk(KERN_ALERT "Failed to set direction of video1 detect\n");
	}

        BITCLR(HwGPIOD->GPEN, Hw10);              //set GPIOA2 as input mode
	BITCSET(HwGPIOD->GPPD0, 0x300000, 0x10);      //0xF0401000 , set GPIOA2 pull-high (01)
        BITCSET(HwEINTSEL->EINTSEL2, HwEINTSEL2_EINT9_MASK, HwEINTSEL2_EINT9(SEL_GPIOD10)); //select EI10 as GPIOA2
	BITSET(HwPIC->IEN0, TCC_ENABLE_BIT(IRQ_EI9));      //0xF0401000 , set enable EI10 interrupt
	BITSET(HwPIC->SEL0, TCC_ENABLE_BIT(IRQ_EI9));      //0xF0401018 , set EI10 as a nIRQ (0=FIQ)
        BITSET(HwPIC->POL0, TCC_ENABLE_BIT(IRQ_EI9));    //0xF0401038 , set EI10 as 0=low-active
	BITCLR(HwPIC->MODE0, TCC_ENABLE_BIT(IRQ_EI9));    //0xF0401060 , set EI10 act as 0=edge-triggered (1=level-triggered)
	BITCLR(HwPIC->MODEA0, TCC_ENABLE_BIT(IRQ_EI9));    //0xF0401078 , set EI10 act as 0=single-edge-triggered (1=both-edge)
	BITSET(HwPIC->INTMSK0, TCC_ENABLE_BIT(IRQ_EI9));   //0xF0401100 , set EI10 passes interrupt to IRQ or FIQ

	ret = request_irq(IRQ_EI9, bc_irq_handler,
		IRQF_TRIGGER_FALLING | IRQF_DISABLED, "BACK CAMERA", NULL);
	if (ret) {
		printk(KERN_ALERT "Can't request irq for back camera\n");
	}
}

void set_drm_interrupt(void)
{
        int ret;
        ret = gpio_request(GPIO_DRM_ID, "DRM Detect");
        if (ret) {
                printk(KERN_ALERT "Failed to request drm detect\n");
        }

        ret = gpio_direction_input(GPIO_DRM_ID);
        if (ret) {
                printk(KERN_ALERT "Failed to set direction of drm detect\n");
        }

        BITCLR(HwGPIOF->GPEN, Hw8);              //set GPIOA2 as input mode
        BITCSET(HwGPIOF->GPPD0, 0x30000, 0x10);      //0xF0401000 , set GPIOA2 pull-high (01)
        BITCSET(HwEINTSEL->EINTSEL1, HwEINTSEL1_EINT5_MASK, HwEINTSEL1_EINT5(SEL_GPIOF8)); //select EI10 as GPIOA2
        BITSET(HwPIC->IEN0, TCC_ENABLE_BIT(IRQ_EI5));      //0xF0401000 , set enable EI10 interrupt
        BITSET(HwPIC->SEL0, TCC_ENABLE_BIT(IRQ_EI5));      //0xF0401018 , set EI10 as a nIRQ (0=FIQ)
        BITSET(HwPIC->POL0, TCC_ENABLE_BIT(IRQ_EI5));    //0xF0401038 , set EI10 as 0=low-active
        BITCLR(HwPIC->MODE0, TCC_ENABLE_BIT(IRQ_EI5));    //0xF0401060 , set EI10 act as 0=edge-triggered (1=level-triggered)
        BITCLR(HwPIC->MODEA0, TCC_ENABLE_BIT(IRQ_EI5));    //0xF0401078 , set EI10 act as 0=single-edge-triggered (1=both-edge)
        BITSET(HwPIC->INTMSK0, TCC_ENABLE_BIT(IRQ_EI5));   //0xF0401100 , set EI10 passes interrupt to IRQ or FIQ

        ret = request_irq(IRQ_EI5, drm_irq_handler,
                IRQF_TRIGGER_FALLING | IRQF_DISABLED, "DRM CAMERA", NULL);
        if (ret) {
                printk(KERN_ALERT "Can't request irq for back camera\n");
        }
}

static void hello_exit(void)
{
	gpio_free(GPIO_VIDEO1_DET);
	gpio_free(GPIO_DRM_ID);
	free_irq(IRQ_EI9, NULL);
	free_irq(IRQ_EI5, NULL);
	printk(KERN_ALERT "Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
