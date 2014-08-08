MEMORY {
  sfr              : ORIGIN = 0x0000, LENGTH = 0x0010 /* END=0x0010, size 16 */
  peripheral_8bit  : ORIGIN = 0x0010, LENGTH = 0x00f0 /* END=0x0100, size 240 */
  peripheral_16bit : ORIGIN = 0x0100, LENGTH = 0x0100 /* END=0x0200, size 256 */
  infomem          : ORIGIN = 0x1c00, LENGTH = 0x0060 /* END=0x1c60, size 96 as 1 96-byte segments */
  infoa            : ORIGIN = 0x1c00, LENGTH = 0x0060 /* END=0x1c60, size 96 */
  ram (wx)         : ORIGIN = 0x2380, LENGTH = 0x0080 /* END=0x2400, size 128 */
  rom (rx)         : ORIGIN = 0xfc80, LENGTH = 0x0360 /* END=0xffe0, size 864 */
  vectors          : ORIGIN = 0xffe0, LENGTH = 0x0020 /* END=0x10000, size 32 as 16 2-byte segments */
  /* Remaining banks are absent */
  bsl              : ORIGIN = 0x0000, LENGTH = 0x0000
  infob            : ORIGIN = 0x0000, LENGTH = 0x0000
  infoc            : ORIGIN = 0x0000, LENGTH = 0x0000
  infod            : ORIGIN = 0x0000, LENGTH = 0x0000
  ram2 (wx)        : ORIGIN = 0x0000, LENGTH = 0x0000
  ram_mirror (wx)  : ORIGIN = 0x0000, LENGTH = 0x0000
  usbram (wx)      : ORIGIN = 0x0000, LENGTH = 0x0000
  far_rom          : ORIGIN = 0x00000000, LENGTH = 0x00000000
}
REGION_ALIAS("REGION_TEXT", rom);
REGION_ALIAS("REGION_DATA", ram);
REGION_ALIAS("REGION_FAR_ROM", far_rom);
PROVIDE (__info_segment_size = 0x60);
PROVIDE (__infoa = 0x1c00);
