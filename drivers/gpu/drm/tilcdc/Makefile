# SPDX-License-Identifier: GPL-2.0
extra_cflags_w != echo "${EXTRA_CFLAGS}" | grep -e "-W" && echo y || true
ccflags-${extra_cflags_w} += -Werror

tilcdc-y := \
	tilcdc_plane.o \
	tilcdc_crtc.o \
	tilcdc_panel.o \
	tilcdc_external.o \
	tilcdc_drv.o

obj-$(CONFIG_DRM_TILCDC)	+= tilcdc.o
