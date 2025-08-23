#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int fd = open("/dev/eudyptula", O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }

    // Write to the device
    const char msg[] = "7c1caf2f50d1";
    //const char *msg = "7c1caf2f50d1";

    if (write(fd, msg, sizeof(msg)) < 0) {
        perror("Write failed");
    } else {
        printf("Wrote %ld bytes to device\n", sizeof(msg));
    }

    // Read from the device
    char buf[13];
    int ret = read(fd, buf, sizeof(buf));
    if (ret < 0) {
        perror("Read failed");
    } else {
        buf[ret] = '\0'; // Null-terminate
        printf("Read from device: %s\n", buf);
    }

    close(fd);
    return 0;
}
