cppflags-$(CONFIG_LPM_DM_STUB) += -I$(srctree)/lpm/include
cppflags-$(CONFIG_LPM_DM_STUB) += -I$(srctree)/pm/include
cppflags-$(CONFIG_LPM_DM_STUB) += -I$(srctree)/lpm/include/soc/$(TARGET_SOC)
cppflags-$(CONFIG_LPM_DM_STUB) += -I$(srctree)/lpm
cppflags-$(CONFIG_LPM_32_BIT_DDR) += -I$(srctree)/lpm/cdns_generated_defines

obj-y += soc/

obj-y += main.o
obj-y += psc_raw.o
obj-y += pll_16fft_raw.o
obj-y += ctrlmmr_raw.o
obj-$(CONFIG_LPM_DM_TRACE_UART) += 8250.o
obj-$(CONFIG_LPM_DM_TRACE_BUFFER) += lpm_trace_buffer.o
obj-y += timeout.o
obj-y += sec_proxy.o
obj-y += ddr.o
obj-y += vim_raw.o
obj-y += lpm_string.o
obj-y += lpm_trace.o
obj-$(CONFIG_LPM_32_BIT_DDR) += cdns_ddr_reg_config.o
