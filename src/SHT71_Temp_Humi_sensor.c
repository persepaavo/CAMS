/*
 * SHT71_Temp_Humi_sensor.c
 *
 * Created: 26.10.2017 17.51.09
 *  Author: henri
 */ 

// Example of user space C program to read a humidity and temperature
// sensor Sensirion SHT71 (http://www.acmesystems.it/?id=89)

// Version improved by by Douglas Gilbert

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <util/delay.h>

#ifdef __CRIS__
#include "linux/gpio_syscalls.h"
#endif

// -----------------------------------------------------------------------
// SHT71 sensor define
// -----------------------------------------------------------------------

#define CLOCK_BIT         25    // IO line on port G (was OG25)
#define DATA_BIT          24    // IO line on port G

#define CLOCK_MSK         (1 << CLOCK_BIT)
#define DATA_MSK          (1 << DATA_BIT)

#ifdef __CRIS__
// Macro to set the data line direction
#define DATA_LINE_IN      gpiosetdir(PORTG,DIRIN,DATA_MSK)
#define DATA_LINE_OUT     gpiosetdir(PORTG,DIROUT,DATA_MSK)

#define DATA_LINE_LOW     gpiosetdir(PORTG,DIROUT,DATA_MSK);gpioclearbits(PORTG,DATA_MSK)
#define DATA_LINE_HIGH    gpiosetdir(PORTG,DIRIN,DATA_MSK)
#define DATA_LINE_READ    gpiogetbits(PORTG,DATA_MSK)?(1):(0)

#define CLOCK_LINE_LOW    gpioclearbits(PORTG,CLOCK_MSK)
#define CLOCK_LINE_HIGH   gpiosetbits(PORTG,CLOCK_MSK)
#endif

#define CMD_READ_TEMP     0x03
#define CMD_READ_HUM      0x05


static int verbose = 0;
static const char * version_str = "1.02 20090128";


static void
usage(void)
{
	fprintf(stderr, "Usage: "
	"sht_test [-h] [-r] [-v] [-V]\n"
	"  where:\n"
	"    -h       print usage message\n"
	"    -r       send reset to SHT before fetching info\n"
	"    -v       increase verbosity (more written to log)\n"
	"    -V       print version string then exit\n\n"
	"Test Sensirion SHT-71, CLK=(I)OG%d DATA=IOG%d\n", CLOCK_BIT,
	DATA_BIT);
}

#ifdef __CRIS__
// -----------------------------------------------------------------------
// Send the start sequence
// -----------------------------------------------------------------------
static void
SendStart(void)
{
	DATA_LINE_OUT;

	CLOCK_LINE_HIGH;
	_delay_us(1000);
//	usleep(1000);
	DATA_LINE_IN;
	DATA_LINE_LOW;
	CLOCK_LINE_LOW;
	CLOCK_LINE_HIGH;
	_delay_us(1000);
//	usleep(1000);
	DATA_LINE_HIGH;
	CLOCK_LINE_LOW;
	DATA_LINE_IN;
}

// -----------------------------------------------------------------------
// Sensor reset
// -----------------------------------------------------------------------
static void
SendReset (void)
{
	int k;

	for (k = 0; k < 12; k++) {
		CLOCK_LINE_HIGH;
		_delay_us(1000);
//		usleep(1000);
		CLOCK_LINE_LOW;
	}
}

// -----------------------------------------------------------------------
// Send a byte to the sensor
// -----------------------------------------------------------------------
static int
SendByte(unsigned char byte)
{
	unsigned char tempbyte;
	int k;

	DATA_LINE_OUT;
	tempbyte = byte;
	for (k = 0x80; k > 0; k /= 2) {

		if (tempbyte & k)
		DATA_LINE_HIGH;
		else
		DATA_LINE_LOW;

		CLOCK_LINE_HIGH;
		_delay_us(1000);
//		usleep(1000);
		CLOCK_LINE_LOW;
	}
	DATA_LINE_IN;
	CLOCK_LINE_HIGH;
	CLOCK_LINE_LOW;
	return 1;
}

// -----------------------------------------------------------------------
// Read a byte from the sensor
// withack
//   1 = send the ACK
//   0 = don't send the ACK
// -----------------------------------------------------------------------
static unsigned char
ReadByte(int withack)
{
	unsigned char tempbyte;
	int k;

	tempbyte = 0;
	DATA_LINE_IN;
	for (k = 0x80; k > 0; k /= 2) {
		CLOCK_LINE_HIGH;
		if (DATA_LINE_READ)
		tempbyte |= k;
		CLOCK_LINE_LOW;
	}
	if (withack) {
		// Acknowledge del byte
		DATA_LINE_OUT;
		DATA_LINE_LOW;
		CLOCK_LINE_HIGH;
		CLOCK_LINE_LOW;
		DATA_LINE_IN;
		} else {
		// Senza acknowledge
		DATA_LINE_OUT;
		DATA_LINE_HIGH;
		CLOCK_LINE_HIGH;
		CLOCK_LINE_LOW;
		DATA_LINE_IN;
	}
	return tempbyte;
}

// ----------------------
// Read the temperature
// ----------------------
static int
ReadTemperature(void)
{
	unsigned char Lsb, Msb, Chk;

	SendStart();
	if (! SendByte(CMD_READ_TEMP))
	return 0;
	while (DATA_LINE_READ)
	;
	Msb = ReadByte(1);
	if (verbose > 1)
	fprintf(stderr, "ReadTemperature: Msb=%d\n", Msb);
	Lsb = ReadByte(1);
	if (verbose > 1)
	fprintf(stderr, "ReadTemperature: Lsb=%d\n", Lsb);
	Chk = ReadByte(0);
	if (verbose > 1)
	fprintf(stderr, "ReadTemperature: Chk=%d\n", Chk);
	if (1 == verbose)
	fprintf(stderr, "ReadTemperature: Msb=%d Lsb=%d Chk=%d\n", Msb, Lsb,
	Chk);
	return (Msb << 8) + Lsb;
}

// ------------------
// Read the humidity
// ------------------

static int
ReadHumidity(void)
{
	unsigned char Lsb, Msb, Chk;

	SendStart();

	if (! SendByte(CMD_READ_HUM))
	return 0;
	while (DATA_LINE_READ)
	;
	Msb = ReadByte(1);
	if (verbose > 1)
	fprintf(stderr, "ReadHumidity: Msb=%d\n", Msb);
	Lsb = ReadByte(1);
	if (verbose > 1)
	fprintf(stderr, "ReadHumidity: Lsb=%d\n", Lsb);
	Chk = ReadByte(0);
	if (verbose > 1)
	fprintf(stderr, "ReadHumidity: Chk=%d\n", Chk);
	if (1 == verbose)
	fprintf(stderr, "ReadHumidity: Msb=%d Lsb=%d Chk=%d\n", Msb, Lsb,
	Chk);
	return (Msb << 8) + Lsb ;
}

#else

static int
ReadHumidity(void)
{
	return 0;
}

static int
ReadTemperature(void)
{
	return 0;
}

static void
SendReset(void)
{
}

#endif


/*

// ----------
// main code
// ----------
int
main(int argc, char ** argv)
{
	int opt, soh, sot;
	int do_reset = 0;
	float rel_humidity, temperature_c;

	while ((opt = getopt(argc, argv, "hrvV")) != -1) {
		switch (opt) {
			case 'h':
			usage();
			exit(EXIT_SUCCESS);
			case 'r':
			++do_reset;
			break;
			case 'v':
			++verbose;
			break;
			case 'V':
			printf("%s\n", version_str);
			exit(EXIT_SUCCESS);
			default: // '?' 
			usage();
			exit(EXIT_FAILURE);
		}
	}
	if (optind < argc) {
		if (optind < argc) {
			for (; optind < argc; ++optind)
			fprintf(stderr, "Unexpected extra argument: %s\n",
			argv[optind]);
			usage();
			exit(EXIT_FAILURE);
		}
	}

	if (verbose)
	fprintf(stderr, "%s: Clock line (I)OG%d, Data line IOG%d\n",
	__FILE__ ,CLOCK_BIT, DATA_BIT);
	if (do_reset > 0)
	SendReset();
	// assume 12 bit (rather than 8 bit) accuracy
	soh = ReadHumidity();
	sot = ReadTemperature();
	if (verbose)
	fprintf(stderr, "soh=%d [0x%x]  sot=%d [0x%x]\n", soh, soh,  sot, sot);
	#if 1
	// new formula from SHT7x version 4.1 datasheet
	rel_humidity = -2.0468 + (0.0367 * soh) + (-1.5955E-6 * soh * soh);
	#else
	// older formula
	rel_humidity = -4 + (0.0405 * soh) + -2.8E-6;
	#endif
	if (rel_humidity > 99.9)
	rel_humidity = 100.0;
	else if (rel_humidity < 0.1)
	rel_humidity = 0.0;
	temperature_c = -39.66 + (0.01 * sot);
	printf ("Rel. Humidity: %.2f %%\n", rel_humidity);
	printf ("Temperature  : %.2f C\n", temperature_c);
	return 0;
}

*/