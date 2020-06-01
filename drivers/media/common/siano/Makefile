# SPDX-License-Identifier: GPL-2.0
smsmdtv-objs := smscoreapi.o sms-cards.o smsendian.o
smsdvb-objs := smsdvb-main.o

obj-$(CONFIG_SMS_SIANO_MDTV) += smsmdtv.o smsdvb.o

smsmdtv-objs += ${CONFIG_SMS_SIANO_RC:y=smsir.o}

smsdvb-objs += ${CONFIG_SMS_SIANO_DEBUGFS:y=smsdvb-debugfs.o}
