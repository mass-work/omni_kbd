BOOTMAGIC_ENABLE = yes      # Enable Bootmagic Lite
MOUSEKEY_ENABLE = yes       # Mouse keys
EXTRAKEY_ENABLE = yes       # Audio control and System control
CONSOLE_ENABLE = yes         # Console for debug
COMMAND_ENABLE = no         # Commands for debug and configuration
NKRO_ENABLE = yes           # Enable N-Key Rollover
DYNAMIC_MACRO_ENABLE = yes
EEPROM_ENABLE = yes
# TAP_DANCE_ENABLE = yes




CUSTOM_MATRIX = lite		# duplex matrix
SRC += matrix.c

POINTING_DEVICE_ENABLE = yes
POINTING_DEVICE_DRIVER = custom
SRC += drivers/pmw3360.c
SRC += drivers/pmw33xx_common.c
QUANTUM_LIB_SRC += spi_master.c # Optical sensor use SPI to communicate

QUANTUM_PAINTER_ENABLE = yes

QUANTUM_PAINTER_DRIVERS += gc9a01_spi
# QUANTUM_PAINTER_DRIVERS += st7789_spi

SRC += customfunc/draw_custom.c


SRC += font/noto11.qff.c

SRC += icon\omni_image_loader.c
SRC += icon/generated/omni_logo.qgf.c
SRC += icon/generated/save.qgf.c

SRC += icon/generated/layer_00.qgf.c
SRC += icon/generated/layer_01.qgf.c
SRC += icon/generated/layer_02.qgf.c
SRC += icon/generated/layer_03.qgf.c
SRC += icon/generated/layer_04.qgf.c
SRC += icon/generated/layer_05.qgf.c
SRC += icon/generated/layer_06.qgf.c
SRC += icon/generated/layer_07.qgf.c
SRC += icon/generated/layer_08.qgf.c
SRC += icon/generated/layer_09.qgf.c
SRC += icon/generated/layer_10.qgf.c
SRC += icon/generated/layer_11.qgf.c

SRC += icon/generated/000.qgf.c
SRC += icon/generated/001.qgf.c
SRC += icon/generated/002.qgf.c
SRC += icon/generated/003.qgf.c
SRC += icon/generated/004.qgf.c
SRC += icon/generated/005.qgf.c
SRC += icon/generated/006.qgf.c
SRC += icon/generated/007.qgf.c
SRC += icon/generated/008.qgf.c
SRC += icon/generated/009.qgf.c
SRC += icon/generated/010.qgf.c
SRC += icon/generated/011.qgf.c
SRC += icon/generated/012.qgf.c
SRC += icon/generated/013.qgf.c
SRC += icon/generated/014.qgf.c
SRC += icon/generated/015.qgf.c
SRC += icon/generated/016.qgf.c
SRC += icon/generated/017.qgf.c
SRC += icon/generated/018.qgf.c
SRC += icon/generated/019.qgf.c
SRC += icon/generated/020.qgf.c
SRC += icon/generated/021.qgf.c
SRC += icon/generated/022.qgf.c
SRC += icon/generated/023.qgf.c
SRC += icon/generated/024.qgf.c
SRC += icon/generated/025.qgf.c
SRC += icon/generated/026.qgf.c
SRC += icon/generated/027.qgf.c
SRC += icon/generated/028.qgf.c
SRC += icon/generated/029.qgf.c
SRC += icon/generated/030.qgf.c
SRC += icon/generated/031.qgf.c
SRC += icon/generated/032.qgf.c
SRC += icon/generated/033.qgf.c
SRC += icon/generated/034.qgf.c
SRC += icon/generated/035.qgf.c
SRC += icon/generated/036.qgf.c
SRC += icon/generated/037.qgf.c
SRC += icon/generated/038.qgf.c
SRC += icon/generated/039.qgf.c
SRC += icon/generated/040.qgf.c
SRC += icon/generated/041.qgf.c
SRC += icon/generated/042.qgf.c
SRC += icon/generated/043.qgf.c
SRC += icon/generated/044.qgf.c
SRC += icon/generated/045.qgf.c
SRC += icon/generated/046.qgf.c
SRC += icon/generated/047.qgf.c
SRC += icon/generated/048.qgf.c
SRC += icon/generated/049.qgf.c
SRC += icon/generated/050.qgf.c
SRC += icon/generated/051.qgf.c
SRC += icon/generated/052.qgf.c
SRC += icon/generated/053.qgf.c
SRC += icon/generated/054.qgf.c
SRC += icon/generated/055.qgf.c
SRC += icon/generated/056.qgf.c
SRC += icon/generated/057.qgf.c
SRC += icon/generated/058.qgf.c
SRC += icon/generated/059.qgf.c
SRC += icon/generated/060.qgf.c
SRC += icon/generated/061.qgf.c
SRC += icon/generated/062.qgf.c
SRC += icon/generated/063.qgf.c
SRC += icon/generated/064.qgf.c
SRC += icon/generated/065.qgf.c
SRC += icon/generated/066.qgf.c
SRC += icon/generated/067.qgf.c
SRC += icon/generated/068.qgf.c
SRC += icon/generated/069.qgf.c
SRC += icon/generated/070.qgf.c
SRC += icon/generated/071.qgf.c
SRC += icon/generated/072.qgf.c
SRC += icon/generated/073.qgf.c
SRC += icon/generated/074.qgf.c
SRC += icon/generated/075.qgf.c
SRC += icon/generated/076.qgf.c
SRC += icon/generated/077.qgf.c
SRC += icon/generated/078.qgf.c
SRC += icon/generated/079.qgf.c
SRC += icon/generated/080.qgf.c

I2C_ENABLE = yes
QUANTUM_LIB_SRC += i2c_master.c
SRC += drivers/cst816t.c
