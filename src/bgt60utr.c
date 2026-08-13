/*
 * bgt60utr.c
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

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>
#include <gentyp.h>
#include "sysconf.h"
#include "msgconf.h"
#include "criterr.h"
#include "dlycnt.h"
#include "hwerr.h"
#include "bgt60utr.h"
#include <string.h>

#define SPI_HAL_SHORT_TRANS SPI_HAL_POLL
#define SPI_HAL_LONG_TRANS SPI_HAL_DMA
#define BGT60UTR_CHIP_VER_ID 0x070C
#define DLY_AFTER_SW_RST_US 100

/**
 * init_bgt60utr
 */
void init_bgt60utr(bgt60utr bgt)
{
	spi_hal_dev_init(&bgt->spi);
}

/**
 * bgt60utr_reg_read
 */
int bgt60utr_reg_read(bgt60utr bgt, enum bgt60utr_reg_adr adr, uint32_t *reg)
{
	int ret;
	uint8_t buf[4] = {0};

	buf[0] = adr << 1;
	if ((ret = spi_hal_xfer(&bgt->spi, SPI_HAL_SHORT_TRANS, buf, 1, buf + 1, 3))) {
		return (ret);
	}
	*reg = 0;
	*reg |= buf[1] << 16;
	*reg |= buf[2] << 8;
	*reg |= buf[3];
	return (0);
}

/**
 * bgt60utr_reg_write
 */
int bgt60utr_reg_write(bgt60utr bgt, enum bgt60utr_reg_adr adr, uint32_t *reg)
{
	uint8_t buf[4];

	buf[0] = (adr << 1) | 1;
	buf[1] = *reg >> 16;
	buf[2] = *reg >> 8;
	buf[3] = *reg;
	return (spi_hal_xfer(&bgt->spi, SPI_HAL_SHORT_TRANS, buf, 1, buf + 1, 3));
}

/**
 * bgt60utr_whoami_check
 */
int bgt60utr_whoami_check(bgt60utr bgt)
{
	uint32_t who;
	int ret;

	if ((ret = bgt60utr_reg_read(bgt, BGT60UTR_CHIP_VER_REG, &who))) {
		return (ret);
	}
	if (who != BGT60UTR_CHIP_VER_ID) {
		return (-EHW);
	}
	return (0);
}

/**
 * bgt60utr_soft_reset
 */
int bgt60utr_soft_reset(bgt60utr bgt)
{
	uint32_t rv;
	int ret;

	rv = BGT60UTR_MAIN_BIT_SW_RESET;
	if ((ret = bgt60utr_reg_write(bgt, BGT60UTR_MAIN_REG, &rv))) {
		return (ret);
	}
	delay_us(DLY_AFTER_SW_RST_US);
	rv = BGT60UTR_SFCTL_REG_DEFAUTL & ~BGT60UTR_SFCTL_BIT_MISO_HS_READ;
	if ((ret = bgt60utr_reg_write(bgt, BGT60UTR_SFCTL_REG, &rv))) {
		return (ret);
	}
	return (bgt60utr_whoami_check(bgt));
}
