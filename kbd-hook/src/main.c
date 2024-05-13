#define _POSIX_C_SOURCE 199309L

#include <fcntl.h>
#include <linux/input.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define LEN 64
int32_t main() {
    int32_t fd = open("/dev/input/event3", O_RDONLY | O_NONBLOCK);
    if (fd == -1) {
        printf("Couldn't open /dev/input/event3 \n");
        return 1;
    }
    struct input_event event = {};
    while (true) {
        read(fd, &event, sizeof(event));
        if (event.type == 1) {
            switch (event.value) {
            case 1:
                printf("Key: %i, value: %i (pressed)\n", event.code,
                       event.value);
                break;
            case 0:
                printf("Key: %i, value: %i (released)\n", event.code,
                       event.value);
                break;
            case 2:
                printf("Key: %i, value: %i (hold)\n", event.code, event.value);
                break;
            default:
                printf("Key: %i\n", event.code);
            }
            printf("code: %i, value: %i\n", event.code, event.value);
        }
    }
    close(fd);
}
