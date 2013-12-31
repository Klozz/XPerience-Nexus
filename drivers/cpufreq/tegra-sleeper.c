/*
 * Oxydo tegra-sleeper by sgt. meow <fayad.chowdhury@gmail.com>
 * Ported over from ElementalX. Originally flar2's work.
 * 
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/earlysuspend.h>
#include <linux/workqueue.h>
#include <linux/cpu.h>
#include <linux/module.h>
#include <linux/cpufreq.h>
#include <linux/init.h>
#include <linux/tegra_minmax_cpufreq.h>


#define TEGRA_SLEEPER_MAJOR_VERSION	1
#define TEGRA_SLEEPER_MINOR_VERSION	1

#ifdef CONFIG_HAS_EARLYSUSPEND
static void __cpuinit tegra_sleeper_early_suspend(struct early_suspend *h)
{
	int cpu;
	for_each_possible_cpu(cpu) {
		if (cpu_online(cpu)) {	
			per_cpu(tegra_cpu_max_freq, cpu) = 475000;
			pr_info("Earlysuspend: set max freq to 475MHz\n");
		}
	}		
	return; 

}

static void __cpuinit tegra_sleeper_late_resume(struct early_suspend *h)
{
	int cpu;
	for_each_possible_cpu(cpu) {
		if (cpu_online(cpu)) {
			per_cpu(tegra_cpu_max_freq, cpu) = 1300000;	
			pr_info("Lateresume: set max freq to 1300MHz\n");	
		}
	}
	return; 

}

static struct early_suspend tegra_sleeper_early_suspend_driver = {
	.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 10,
	.suspend = tegra_sleeper_early_suspend,
	.resume = tegra_sleeper_late_resume,
};
#endif

static int __init tegra_sleeper_init(void)
{
	pr_info("tegra-sleeper version %d.%d\n",
		 TEGRA_SLEEPER_MAJOR_VERSION,
		 TEGRA_SLEEPER_MINOR_VERSION);

#ifdef CONFIG_HAS_EARLYSUSPEND
		register_early_suspend(&tegra_sleeper_early_suspend_driver);
#endif
	return 0;
}

MODULE_AUTHOR("sgt. meow <fayad.chowdhury@gmail.com>");
MODULE_DESCRIPTION("'tegra-sleeper' - Limit max frequency while screen is off");
MODULE_LICENSE("GPL");

late_initcall(tegra_sleeper_init);



