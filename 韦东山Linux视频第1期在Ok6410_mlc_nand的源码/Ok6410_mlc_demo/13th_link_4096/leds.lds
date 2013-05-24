SECTIONS
{
	. = 0x1000;
	.text : {
		start.o
		* (.text)
	}
    		
	.data : {
		* (.data)
	}
	
	bss_start = .;  /* 0x1164 */
	.bss : {
		* (.bss)
	}
	bss_end  = .;	
}
