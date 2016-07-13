/*
 * TS-7300 SD card bootloader replacement
 *
 * Copyright (C) 2016, Florian Fainelli <f.fainelli@gmail.com>
 *
 * Note: this must fit within 446 bytes of a MBR
 *
 * See LICENSE for details
 */

/*
 * Here is the state in which we start:
 *
 * R0 points to the sd_read function
 * R1 points to the ser_puts function
 *
 * The stack pointer is initialized in SRAM
 */

int main(void(*sr_read)(unsigned int start_sector,
			char *buffer, int num_sectors),
	 void(*ser_puts)(char *buffer))
{
	ser_puts("Hello there!\n");

	return 0;
}
