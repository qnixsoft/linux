# SPDX-License-Identifier: GPL-2.0
obj-y += time.o timer.o hrtimer.o
obj-y += timekeeping.o ntp.o clocksource.o jiffies.o timer_list.o
obj-y += timeconv.o timecounter.o alarmtimer.o

posix-timers != test "${CONFIG_POSIX_TIMERS}" == y && \
	echo posix-timers.o posix-cpu-timers.o posix-clock.o itimer.o || \
	echo posix-stubs.o

obj-y += ${posix-timers}

obj-$(CONFIG_GENERIC_CLOCKEVENTS)		+= clockevents.o tick-common.o

GCB := ${CONFIG_GENERIC_CLOCKEVENTS_BROADCAST}
obj-y						+= ${GCB:y=tick-broadcast.o}
obj-$(CONFIG_TICK_ONESHOT)			+= ${GCB:y=tick-broadcast-hrtimer.o}

obj-$(CONFIG_GENERIC_SCHED_CLOCK)		+= sched_clock.o
obj-$(CONFIG_TICK_ONESHOT)			+= tick-oneshot.o tick-sched.o
obj-$(CONFIG_DEBUG_FS)				+= timekeeping_debug.o
obj-$(CONFIG_TEST_UDELAY)			+= test_udelay.o
