# SPDX-License-Identifier: GPL-2.0
b2c2-flexcop-pci-objs += ${CONFIG_DVB_B2C2_FLEXCOP_PCI:y=flexcop-dma.o}

b2c2-flexcop-pci-objs += flexcop-pci.o
obj-$(CONFIG_DVB_B2C2_FLEXCOP_PCI) += b2c2-flexcop-pci.o

ccflags-y += -I $(srctree)/drivers/media/common/b2c2/
