/*
 * custom linker map for rfloader, rom is reserved at the high end
 * of the flash and is preserved between user code loads
 * make sure the values here match memconfig.h
 */
MEMORY {
  sfr              : ORIGIN = 0x0000, LENGTH = 0x0010 /* END=0x0010, size 16 */
  peripheral_8bit  : ORIGIN = 0x0010, LENGTH = 0x00f0 /* END=0x0100, size 240 */
  peripheral_16bit : ORIGIN = 0x0100, LENGTH = 0x0100 /* END=0x0200, size 256 */
  bsl              : ORIGIN = 0x1000, LENGTH = 0x0800 /* END=0x1800, size 2K as 4 512-byte segments */
  infomem          : ORIGIN = 0x1800, LENGTH = 0x0200 /* END=0x1a00, size 512 as 4 128-byte segments */
  infod            : ORIGIN = 0x1800, LENGTH = 0x0080 /* END=0x1880, size 128 */
  infoc            : ORIGIN = 0x1880, LENGTH = 0x0080 /* END=0x1900, size 128 */
  infob            : ORIGIN = 0x1900, LENGTH = 0x0080 /* END=0x1980, size 128 */
  infoa            : ORIGIN = 0x1980, LENGTH = 0x0080 /* END=0x1a00, size 128 */
  ram (wx)         : ORIGIN = 0x1c00, LENGTH = 0x0ffe /* END=0x2bfe, size 4094 */
  rom (rx)         : ORIGIN = 0x1000, LENGTH = 0x0600 /* END=0xfe00, size 1536 */
  urom (rx)        : ORIGIN = 0x8000, LENGTH = 0x7f80 /* END=0xff80, size 32640 */
  vectors          : ORIGIN = 0xff80, LENGTH = 0x0080 /* END=0x10000, size 128 as 64 2-byte segments */
  /* Remaining banks are absent */
  ram2 (wx)        : ORIGIN = 0x0000, LENGTH = 0x0000
  ram_mirror (wx)  : ORIGIN = 0x0000, LENGTH = 0x0000
  usbram (wx)      : ORIGIN = 0x0000, LENGTH = 0x0000
  far_rom          : ORIGIN = 0x00000000, LENGTH = 0x00000000
}
REGION_ALIAS("REGION_TEXT", urom);
REGION_ALIAS("REGION_DATA", ram);
REGION_ALIAS("REGION_FAR_ROM", far_rom);
PROVIDE (__info_segment_size = 0x80);
PROVIDE (__infod = 0x1800);
PROVIDE (__infoc = 0x1880);
PROVIDE (__infob = 0x1900);
PROVIDE (__infoa = 0x1980);
PROVIDE (__userrom = 0x8000);
/*
  rom (rx)         : ORIGIN = 0x1000, LENGTH = 0x0800 /* END=0x1800, size 2048 * /
  urom (rx)        : ORIGIN = 0x8000, LENGTH = 0x7f80 /* END=0xff80, size 32640 * /
*/
