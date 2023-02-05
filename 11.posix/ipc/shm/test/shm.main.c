#include <fcntl.h> /* For O_* constants */
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void help(const char *name) {
    printf("usage:%s <c|p|d>\n", name);
    exit(EXIT_FAILURE);
}

void *initialize(void);
void consume(void *buff);
void product(void *buff);
void destroy(void);
void finish(void *buff);

int main(int argc, char *argv[]) {
    // param check
    if (argc != 2) {
        help(argv[0]);
    }
    int is_consume = !strcmp(argv[1], "c");
    int is_product = !strcmp(argv[1], "p");
    int is_destroy = !strcmp(argv[1], "d");
    if (!is_consume && !is_product && !is_destroy) {
        help(argv[0]);
    }
    // map
    void *buff = initialize();
    if (is_consume) {
        consume(buff);
    } else if (is_product) {
        product(buff);
    } else if (is_destroy) {
        destroy();
        return 0;
    } else {

    }
    finish(buff);
error:
    return 0;
}
