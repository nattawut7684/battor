#include <avr/io.h>
#include <stdio.h>
#include <string.h>

#include "spi.h"
#include "gpio.h"

#include "sram.h"

void sram_config_spi()
{
	SPIC.CTRL = SPI_ENABLE_bm | SPI_MASTER_bm | SPI_MODE_3_gc | SPI_PRESCALER_DIV4_gc; 
}

void sram_init() {
	PORTE.PIN3CTRL |= PORT_OPC_PULLUP_gc; // pull up on CS Hold Pin

	// NOTE: this happens two times, once here and once for the POT SPI
	// interface.  We think it's not a big deal but we will see when it's
	// not 10PM on a Thursday.
	PORTC.DIR |= SPI_SS_PIN_bm | SPI_MOSI_PIN_bm | SPI_SCK_PIN_bm;

	PORTE.DIR |= SRAM_CS_PIN_gm;
	gpio_on(&PORTE, SRAM_CS_PIN_gm);

 	sram_config_spi();
}

void* sram_write(void* addr, const void* src, size_t len)
{
	sram_hdr hdr = {SRAM_CMD_WRITE, (uint16_t)addr};

	// begin transaction by setting CS and sendng write header
	gpio_off(&PORTE, SRAM_CS_PIN_gm);
 	spi_txrx(&SPIC, &hdr, NULL, sizeof(hdr));

	// send bytes
 	spi_txrx(&SPIC, src, NULL, len);

	// unset CS to end transaction
	gpio_on(&PORTE, SRAM_CS_PIN_gm);

	return addr;
}

void* sram_read(void* dst, const void* addr, size_t len)
{
	sram_hdr hdr = {SRAM_CMD_READ, (uint16_t)addr};

	// begin transaction by setting CS and sendng read header
	gpio_off(&PORTE, SRAM_CS_PIN_gm);
 	spi_txrx(&SPIC, &hdr, NULL, sizeof(hdr));

	// recv bytes
 	spi_txrx(&SPIC, NULL, dst, len);

	// unset CS to end transaction
	gpio_on(&PORTE, SRAM_CS_PIN_gm);

	return dst;
}

int sram_self_test() {
		uint8_t test_src[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99};
		uint8_t buf[10];

    sram_config_spi();

    printf("sram self test\n");

    printf("sram write 1...");
		memset(buf, 0, sizeof(buf));
		sram_write(0x0000, test_src + 1, 1); // +1 to get non-zero src
		sram_read(buf, 0x0000, 1);
		if (memcmp(test_src + 1, buf, 1) != 0)
		{
			printf("FAILED (memcmp)\n");
			return 1;
		}
		printf("PASSED\n");

    printf("sram write 10...");
		memset(buf, 0, sizeof(buf));
		sram_write(0x0000, test_src, 10);
		sram_read(buf, 0x0000, 10);
		if (memcmp(test_src, buf, 10) != 0)
		{
			printf("FAILED (memcmp)\n");
			return 1;
		}
		printf("PASSED\n");

    printf("sram write 10 high address...");
		memset(buf, 0, sizeof(buf));
		sram_write(63000, test_src, 10);
		sram_read(buf, 63000, 10);
		if (memcmp(test_src, buf, 10) != 0)
		{
			printf("FAILED (memcmp)\n");
			return 1;
		}
		printf("PASSED\n");

		return 0;
}
