OUTPUT_FORMAT(elf32-littlearm)
OUTPUT_ARCH(arm)

PHDRS {
	text PT_LOAD FLAGS(7);
}

ENTRY(main)
SECTIONS {
	.text : { * (.text_main); * (.text*); }:text = 0
	.data : { * (.data); }
	.rodata : { * (.rodata); }
	.bss :
	{
		* (.bss);
	}

	/DISCARD/ : {
		*(.comment)
		*(.note)
		*(.comment.*)
		*(.note.*)
		*(.ARM.*)
	}
}
