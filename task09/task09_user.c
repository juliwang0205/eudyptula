#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define PAGE_SIZE getpagesize()

typedef struct foo_struct {
    int fd;
    char *data;
} foo_struct;

void* writer(void *arg)
{
    foo_struct *write_data = arg;

    int fd = write_data->fd;
    char *data = write_data->data; 
    
    if (write(fd, data, strlen(data)) < 0) {
        perror("Write failed");
    } else {
        printf("Wrote %lu bytes to device\n", strlen(data));
    }

    return NULL;

}

void* reader(void *arg)
{
    foo_struct *read_data = arg;
    int fd = read_data->fd;
    char buf_foo[PAGE_SIZE];

    int ret_foo = read(fd, buf_foo, PAGE_SIZE);
    if (ret_foo < 0) {
        perror("Read failed");
    } else {
        buf_foo[ret_foo] = '\0'; // Null-terminate
        printf("Read from device: %s length = %ld\n", buf_foo, sizeof(buf_foo));
    }
    return NULL;
}


int main(int argc, char **argv) 
{
    int fd_id = open("/sys/kernel/debug/eudyptula/id", O_RDWR);
    if (fd_id < 0) {
        perror("Failed to open device");
        return 1;
    }

    const char id_msg[] = "7c1caf2f50d1";
    if (write(fd_id, id_msg, sizeof(id_msg)) < 0) {
        perror("Write failed");
    } else {
        printf("Wrote %ld bytes to device\n", sizeof(id_msg));
    }

    char id_buf[13];
    int ret_id = read(fd_id, id_buf, sizeof(id_buf));
    if (ret_id < 0) {
        perror("Read failed");
    } else {
        id_buf[ret_id] = '\0'; // Null-terminate
        printf("Read from device: %s\n", id_buf);
    }

    close(fd_id);
     
    int jiffies_fd = open("/sys/kernel/debug/eudyptula/jiffies", O_RDONLY);
    if (jiffies_fd < 0) {
        perror("jiffies Failed to open device jiffies");
        return 1;
    }

    unsigned long jiffies_buf;
    int ret = read(jiffies_fd, &jiffies_buf, sizeof(jiffies_buf));
    if (ret < 0) {
        perror("jiffies Read failed");
    } else {
        printf("jiffies Read from device: %ld\n", jiffies_buf);
    }

    close(jiffies_fd);

    int foo_fd = open("/sys/kernel/debug/eudyptula/foo", O_RDWR);
    if (foo_fd < 0) {
        perror("Failed to open device");
        return 1;
    }

    pthread_t tid1;
    char *str1 = "wangf23 Hello world";
    foo_struct data_thread1;
    data_thread1.fd = foo_fd;
    data_thread1.data = str1;
    pthread_create(&tid1, NULL, writer, &data_thread1);
    
    pthread_t tid3;
    foo_struct data_thread3;
    data_thread3.fd = foo_fd;
    pthread_create(&tid3, NULL, reader, &data_thread3);

    pthread_t tid2;
    char *str2 = "ewngjlx Hello world";
    foo_struct data_thread2;
    data_thread2.fd = foo_fd;
    data_thread2.data = str2;
    pthread_create(&tid2, NULL, writer, &data_thread2);
    
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);    
    pthread_join(tid3, NULL);

    close(foo_fd);
    return 0;
}
