#include <fcntl.h> /* For O_* constants */
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static const char *shm_name = "message_center";
static const size_t buff_size = 4096;
void *initialize(void) {
    int shm_fd = shm_open(shm_name, O_RDWR | O_CREAT, 0644);  // 1. 调用 open(/dev/shm/message_center);
    if (shm_fd < 0) {
        printf("shm_open error=[%s]\n", strerror(errno));
        return NULL;
    }
    ftruncate(shm_fd, buff_size);                               // 2. 形同 文件扩容
    // mmap MAP_PRIVATE 仅线程共享; MAP_SHARED 进程间共享     // 3. 映射到进程空间
    return (void *)mmap(NULL, buff_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
}

void consume(void *buff) {
    printf("do consume\n");
    char message[1024] = { 0 };
    strncpy(message, (char *)buff, 64);
    printf("consume message=[%s]\n", message);
}

void product(void *buff) {
    printf("do product\n");
    const char *message = "hello world";
    strcpy((char *)buff, message);
    printf("product message=[%s]\n", message);
}

void destroy(void) {
    printf("do destroy\n");
    shm_unlink(shm_name); // 调用 unlink("/dev/shm/$shm_name")
    // 5. 没有unlink /dev/shm/message_center 将一直存在
}

void finish(void *buff) {
    getchar();
    printf("end\n");
    munmap(buff, buff_size);
}
