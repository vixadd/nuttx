////////////////////////////////////////////////
////////////////////////////////////////////////
//                                            //
//  This is the driver software that runs     //
// for the SI4713 unit, and FM Transmitter    //
//                                            //
//            Author: David Kroell           //
//           Version: 0.0.1                   //
//                                            //
////////////////////////////////////////////////
////////////////////////////////////////////////

//#include <linux/completion.h>
//#include <linux/delay.h>
//#include <linux/err.h>
//#include <linux/interrupt.h>
//#include <linux/i2c.h>
//#include <linux/slab.h>
//#include <linux/gpio.h>
//#include <linux/module.h>
//#include <media/v4l2-device.h>
//#include <media/v4l2-ioctl.h>
//#include <media/v4l2-common.h>

#include "SI4713.h"

/* module parameters */
static int debug;

module_param(debug, int, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(debug, "Debug level (0 - 2)");

MODULE_LICENSE("MIT");
MODULE_AUTHOR("David Kroell <kroelld.96@gmail.com");
MODULE_DESCRIPTION("NuttX I2C driver for Si4713 FM Radio Transmitter");
MODULE_VERSION("0.0.1");

#define DEFAULT_RDS_PI			0x00
#define DEFAULT_RDS_PTY			0x00
#define DEFAULT_RDS_DEVIATION		0x00C8
#define DEFAULT_RDS_PS_REPEAT_COUNT	0x0003
#define DEFAULT_LIMITER_RTIME		0x1392
#define DEFAULT_LIMITER_DEV		0x102CA
#define DEFAULT_PILOT_FREQUENCY		0x4A38
#define DEFAULT_PILOT_DEVIATION		0x1A5E
#define DEFAULT_ACOMP_ATIME		0x0000
#define DEFAULT_ACOMP_RTIME		0xF4240L
#define DEFAULT_ACOMP_GAIN		0x0F
#define DEFAULT_ACOMP_THRESHOLD		(-0x28)
#define DEFAULT_MUTE			0x01
#define DEFAULT_POWER_LEVEL		88
#define DEFAULT_FREQUENCY		8800
#define DEFAULT_PREEMPHASIS		FMPE_EU
#define DEFAULT_TUNE_RNL		0xFF

#define to_si4713_device(sd)	        container_of(sd, struct si4713_device, sd)

/* frequency domain transformation (using times 10 to avoid floats) */
#define FREQDEV_UNIT	                100000
#define FREQV4L2_MULTI	                625
#define si4713_to_v4l2(f)	        ((f * FREQDEV_UNIT) / FREQV4L2_MULTI)
#define v4l2_to_si4713(f)	        ((f * FREQV4L2_MULTI) / FREQDEV_UNIT)

#define FREQ_RANGE_LOW			7600
#define FREQ_RANGE_HIGH			10800

#define MAX_ARGS                        7

#define RDS_BLOCK			8
#define RDS_BLOCK_CLEAR			0x03
#define RDS_BLOCK_LOAD			0x04
#define RDS_RADIOTEXT_2A		0x20

#define RDS_RADIOTEXT_BLK_SIZE		4

#define RDS_RADIOTEXT_INDEX_MAX		0x0F
#define RDS_CARRIAGE_RETURN		0x0D

#define rds_ps_nblocks(len)	        ((len / RDS_BLOCK) + (len % RDS_BLOCK ? 1 : 0))

#define get_status_bit(p, b, m)	        (((p) & (m)) >> (b))
#define set_bits(p, v, b, m)	        (((p) & ~(m)) | ((v) << (b)))

#define ATTACK_TIME_UNIT	        500

#define POWER_OFF			0x00
#define POWER_ON			0x01

#define msb(x)                          ((u8)((u16) x >> 8))
#define lsb(x)                          ((u8)((u16) x &  0x00FF))
#define compose_u16(msb, lsb)	        (((u16)msb << 8) | lsb)
#define check_command_failed(status)	(!(status & SI4713_CTS) || (status & SI4713_ERR))
