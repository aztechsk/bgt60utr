/*
 * bgt60utr.h
 *
 * Copyright (c) 2025 Jan Rusnak <jan@rusnak.sk>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef BGT60UTR_H
#define BGT60UTR_H

#include "spi_hal.h"

#define BGT60UTR_MAIN_BIT_SW_RESET (1 << 1)
#define BGT60UTR_SFCTL_REG_DEFAUTL 0x796000
#define BGT60UTR_SFCTL_BIT_MISO_HS_READ (1 << 16)

enum bgt60utr_reg_adr {
	BGT60UTR_MAIN_REG,
	BGT60UTR_CHIP_VER_REG = 0x02,
	BGT60UTR_SFCTL_REG = 0x06
};

typedef struct bgt60utr_dsc *bgt60utr;

struct bgt60utr_dsc {
	struct spi_hal_dev spi;
};

/**
 * @brief Initialize the BGT60UTR descriptor.
 *
 * @param bgt Device handle.
 */
void init_bgt60utr(bgt60utr bgt);

/**
 * bgt60utr_reg_read
 */
int bgt60utr_reg_read(bgt60utr bgt, enum bgt60utr_reg_adr adr, uint32_t *reg);

/**
 * bgt60utr_reg_write
 */
int bgt60utr_reg_write(bgt60utr bgt, enum bgt60utr_reg_adr adr, uint32_t *reg);

/**
 * bgt60utr_whoami_check
 */
int bgt60utr_whoami_check(bgt60utr bgt);

/**
 * bgt60utr_soft_reset
 */
int bgt60utr_soft_reset(bgt60utr bgt);

#endif
