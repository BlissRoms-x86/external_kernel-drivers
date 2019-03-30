/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#define I2C_BUS		"/dev/i2c-0"
#define I2C_BAT_DEV	0x70


unsigned char bat_ec_rd(int fd, char arg0)
{
    unsigned char bufo[8];
    unsigned char bufi[8];
    struct i2c_rdwr_ioctl_data work_queue;
    struct i2c_msg msg[2];

    memset(bufo, 0, 8);
    memset(bufi, 0, 8);

    work_queue.nmsgs = 1;
    work_queue.msgs = msg;

    bufo[0] = 0x02;	// len
    bufo[1] = 0x80;	// ECNR
    bufo[2] = arg0;

    work_queue.msgs[0].addr = I2C_BAT_DEV;
    work_queue.msgs[0].len = 5;
    work_queue.msgs[0].flags = 0;
    work_queue.msgs[0].buf = bufo;

    if (ioctl(fd, I2C_RDWR, &work_queue) < 0) {
        fprintf(stderr, "ioctl() failed\n");
        exit(3);
    }

    work_queue.msgs[0].addr = I2C_BAT_DEV;
    work_queue.msgs[0].len = 6;
    work_queue.msgs[0].flags = I2C_M_RD;
    work_queue.msgs[0].buf = bufi;

    if (ioctl(fd, I2C_RDWR, &work_queue) < 0) {
        fprintf(stderr, "ioctl() failed\n");
        exit(3);
    }

#ifdef DEBUG
    // print result buffer
    fprintf(stderr, "0x%02x: ", (unsigned char)arg0);
    for (int i=0; i<8; i++)
        fprintf(stderr, "0x%02x ", (unsigned char)bufi[i]);
    fprintf(stderr, "\n");
#endif

    return bufi[0];
}

void get_status(int fd)
{
    unsigned int Local0, Local1, Local2, Local3, Local4, Local5, Local6, Local7;
    unsigned int PBST[4];
    double rateW, remWh, volt, tbat;
#ifdef DEBUG
    int i;
#endif

    // this is basically a direct copy of the ACPI DSDT battery status method
    PBST[0] = 0x00;
    PBST[1] = 0xFFFFFFFF;
    PBST[2] = 0xFFFFFFFF;
    PBST[3] = 0xFFFFFFFF;

    Local0 = bat_ec_rd(fd, 0xc1);
    PBST[0] = Local0;
    Local1 = bat_ec_rd(fd, 0xd1);
    Local2 = bat_ec_rd(fd, 0xd0) | (Local1 << 8);
    if (Local2 > 0x7FFF) {
        Local2 = (0x00010000 - Local2);
    }
    Local5 = bat_ec_rd(fd, 0xc7);
    Local6 = bat_ec_rd(fd, 0xc6) | (Local5 << 8);
    Local2 *= Local6;
    Local7 = Local2 % 0x03E8; // ??? Divide (Local2, 0x03E8, Local7, Local2)
    PBST[1] = Local2;
    Local3 = bat_ec_rd(fd, 0xc3);
    Local4 = bat_ec_rd(fd, 0xc2) | (Local3 << 8);
    Local4 *= 0x0a;
    PBST[2] = Local4;
    PBST[3] = Local6;

#ifdef DEBUG
    for (i=0; i<=3; i++) {
        fprintf(stderr, "%1d:%9u   ", i, PBST[i]);
    }
    fprintf(stderr, "\n");
#endif
    switch (PBST[0]) {
      case 0:
        fprintf(stderr, "AC online   ");
        break;
      case 1:
        fprintf(stderr, "discharging ");
        break;
      case 2:
        fprintf(stderr, "charging    ");
        break;
      default:
        fprintf(stderr, "unkknown %2d ", PBST[0]);
        break;
    }

    rateW = (double)PBST[1] / 1000000.;
    remWh = (double)PBST[2] / 1000.;
    volt = (double)PBST[3] / 1000.;
    if (PBST[0] == 1) { // discharging, time till empty
        tbat = (remWh / rateW) * 60; // minutes
    } else
        tbat = 0.;
    fprintf(stderr, "@ %2.2fW | Cap-remain %2.2fWh | Voltage %1.3fV | Remain %dh %dm\n", rateW, remWh, volt, (int)tbat / 60, (int)tbat % 60);
}

int main(int argc, char **argv)
{
    int fd, addr;
    unsigned int l, cnt;

    fd = open(I2C_BUS, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "failed to open I2C bus %s\n", I2C_BUS);
        exit(1);
    }
    addr = I2C_BAT_DEV;
    if (ioctl(fd, I2C_SLAVE, addr) < 0) {
        fprintf(stderr, "failed to select I2C device 0x%02x\n", I2C_BAT_DEV);
        exit(1);
    }

    if (argc >= 2) {
        cnt = atoi(argv[1]);
        l = 0;
    } else {
        cnt = 0;
        l = 1;
    };

    while (l | cnt--) {
        get_status(fd);
        if (l || cnt>0)
            sleep(1);
    }

    close(fd);
}
