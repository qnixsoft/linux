# SPDX-License-Identifier: GPL-2.0
clk-common				= dpll.o composite.o divider.o gate.o \
					  fixed-factor.o mux.o apll.o \
					  clkt_dpll.o clkt_iclk.o clkt_dflt.o \
					  clkctrl.o

SOC_AM33XX-$(CONFIG_SOC_AM33XX)		+= $(clk-common) clk-33xx.o dpll3xxx.o \
					  clk-33xx-compat.o
SOC_TI81XX-$(CONFIG_SOC_TI81XX)		+= $(clk-common) fapll.o clk-814x.o clk-816x.o
ARCH_OMAP2-$(CONFIG_ARCH_OMAP2)		+= $(clk-common) interface.o clk-2xxx.o
ARCH_OMAP3-$(CONFIG_ARCH_OMAP3)		+= $(clk-common) interface.o \
					   clk-3xxx.o dpll3xxx.o
ARCH_OMAP4-$(CONFIG_ARCH_OMAP4)		+= $(clk-common) clk-44xx.o \
					   dpll3xxx.o dpll44xx.o
SOC_OMAP5-$(CONFIG_SOC_OMAP5)			+= $(clk-common) clk-54xx.o \
					   dpll3xxx.o dpll44xx.o
SOC_DRA7XX-$(CONFIG_SOC_DRA7XX)		+= $(clk-common) clk-7xx.o \
					   clk-dra7-atl.o dpll3xxx.o \
					   dpll44xx.o clk-7xx-compat.o
SOC_AM43XX-$(CONFIG_SOC_AM43XX)		+= $(clk-common) dpll3xxx.o clk-43xx.o \
					   clk-43xx-compat.o

ARCH_OMAP2PLUS-${CONFIG_ARCH_OMAP2PLUS}	+= clk.o autoidle.o clockdomain.o \
					   ${SOC_AM33XX-y} ${SOC_TI81XX-y} \
					   ${ARCH_OMAP2-y} ${ARCH_OMAP3-y} \
					   ${ARCH_OMAP4-y} ${SOC_OMAP5-y} \
					   ${SOC_DRA7XX-y} ${SOC_AM43XX-y}

obj-$(CONFIG_COMMON_CLK_TI_ADPLL)	+= adpll.o
