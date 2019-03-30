#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "bcm2079x.h"

#define DEV_NAME "/dev/bcm2079x-i2c"

int main(int argc, char **argv)
{
    int fd, ret;
    unsigned long arg;
    unsigned char buf[32];
    unsigned char *p;
    fd_set fds;

    fd = open(DEV_NAME, O_RDWR);
    if (fd < 0) {
        perror("open");
        exit(1);
    }

    arg = 0;
    ret = ioctl(fd, BCMXXC_POWER_CTL, &arg);
    if (ret) {
        perror("ioctl");
        close(fd);
        exit(1);
    }
    sleep(1);
    arg = 1;
    ret = ioctl(fd, BCMXXC_POWER_CTL, &arg);
    if (ret) {
        perror("ioctl");
        close(fd);
        exit(1);
    }

    printf("waiting...\n");
    do {
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        ret = select(fd+1, &fds, NULL, NULL, NULL);
        if (ret < 0) {
            perror("select");
            close(fd);
            exit(1);
        }
        ret = read(fd, buf, 32);
        p=buf;
        while (ret-- > 0) {
            printf("%02x ", *p++);
        }
    } while (ret >= 0);

    printf("read < 0\n");

    arg = 0;
    ret = ioctl(fd, BCMXXC_POWER_CTL, &arg);
    if (ret) {
        perror("ioctl");
        close(fd);
        exit(1);
    }

    close(fd);

return 0;
}
