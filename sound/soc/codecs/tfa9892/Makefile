#
# Makefile for NXP TFA9892 smartamp driver.
#

# additional debugging
#TFA_DEBUG=1

# debugging support (also enables trace_printk)
EXTRA_CFLAGS += -DDEBUG

EXTRA_CFLAGS += -I$(src)/inc
EXTRA_CFLAGS += -Werror

snd-soc-tfa9892-objs += src/tfa98xx.o
snd-soc-tfa9892-objs += src/tfa_container.o
snd-soc-tfa9892-objs += src/tfa_dsp.o
snd-soc-tfa9892-objs += src/tfa_init.o

ifdef TFA_DEBUG
EXTRA_CFLAGS += -DTFA_DEBUG -DDEBUG
snd-soc-tfa9892-objs += src/tfa_debug.o
endif

obj-$(CONFIG_SND_SOC_TFA9892) := snd-soc-tfa9892.o
