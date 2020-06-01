# SPDX-License-Identifier: GPL-2.0
obj-$(CONFIG_VGASTATE)            += vgastate.o
obj-$(CONFIG_HDMI)                += hdmi.o

obj-$(CONFIG_VT)		  += console/
obj-$(CONFIG_FB_STI)		  += console/
obj-$(CONFIG_LOGO)		  += logo/
obj-y				  += backlight/

obj-y				  += fbdev/

of-${CONFIG_OF}			  += of_display_timing.o of_videomode.o
obj-$(CONFIG_VIDEOMODE_HELPERS)	  += display_timing.o videomode.o ${of-y}
