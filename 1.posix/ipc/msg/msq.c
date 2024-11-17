#include <errno.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef struct msgbuf {
    long mtype;
    char mtext[80];
} msgbuf_t;

#define msg_type 2046

static int message_send(int id, int type, const char *message) {
    int retval = 0;
    msgbuf_t sendbuf;
    memset(&sendbuf, 0, sizeof(sendbuf));

    sendbuf.mtype = msg_type;
    snprintf(sendbuf.mtext, sizeof(sendbuf.mtext), "message: %s", message);
    retval = msgsnd(id, (void *)&sendbuf, sizeof(sendbuf.mtext), IPC_NOWAIT);
    if (retval < 0) {
        printf("msgsnd failed %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

static int message_recv(int id, int type) {
    int retval = 0;
    msgbuf_t recvbuf;
    memset(&recvbuf, 0, sizeof(recvbuf));

    retval = msgrcv(id, (void *)&recvbuf, sizeof(recvbuf.mtext), msg_type, 0);
    if (retval < 0) {
        printf("msgrcv failed %s\n", strerror(errno));
        return -1;
    }
    printf("msgrcv: type %ld, text %s\n", recvbuf.mtype, recvbuf.mtext);
    return 0;
}

int main( int argc, char **argv ) {
    key_t msg_key = ftok("/tmp/tmp.wLRD6C9EFP", 0x1024);
    int msg_id = msgget(msg_key, IPC_CREAT | 0644 );
    if (msg_id < 0) {
        printf("msgget failed %s\n", strerror(errno));
        goto error;
    }
    if (message_send(msg_id, msg_type, "hello world")) {
        goto error;
    }
    if (message_recv(msg_id, msg_type)) {
        goto error;
    }
error:
    if (msg_id > 0) {
        msgctl(msg_id, IPC_RMID, NULL);
    }
    return 0;
}