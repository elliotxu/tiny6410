#ifndef _NAND_8K_H_BAB
#define _NAND_8K_H_BAB

#include "basetype.h"

void nand_read(u32 nand_start, u32 ddr_start, u32 len);
void nand_erase_block(u32 addr);
void nand_write(u32 nand_start, u8 * buf, u32 len);
void nand_info(void);
void nand_read_page (u8 *buf, u32 PageAddr);
void nand_write_page(u8 *buf, u32 PageAddr);

#endif

