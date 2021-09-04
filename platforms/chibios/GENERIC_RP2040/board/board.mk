include $(CHIBIOS)/os/various/pico_bindings/pico-sdk.mk
include $(CHIBIOS)/os/hal/ports/RP/RP2040/platform.mk
include $(CHIBIOS)/os/hal/boards/RP_PICO_RP2040/board.mk
include $(CHIBIOS_CONTRIB)/os/hal/ports/RP/RP2040/platform.mk

EXTRAINCDIRS += $(PICOSDKINC)
BOARDSRC += $(PICOSDKSRC)

CSTANDARD = -std=gnu11

CFLAGS += -DPICO_NO_FPGA_CHECK -DCRT0_VTOR_INIT=1 -DCRT0_EXTRA_CORES_NUMBER=0 -DNO_USB_STARTUP_CHECK
