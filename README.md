# ts7300-sdboot

TS-7300 replacement bootloader in MBR licensed under BSD 3-clause.

Based on documentation information from:

https://wiki.embeddedarm.com/wiki/TS-7300

The amount of storage is limited (446 bytes) for code + data, as such UART
prints are limited to what is useful:

* S: could not read a valid signature from the MBR (error)
* P: skipped a partition with a type different from 0xda (4 P is an error)
* L: found and started loading Linux from a partition, terminated by "."
* I: found and started loading an initrd from a partition, terminated by "."
* G: jumping to the kernel (not seeing G is an error)

Other useful resources:

http://infocenter.arm.com/help/topic/com.arm.doc.ddi0151c/ARM920T_TRM1_S.pdf
