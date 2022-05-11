#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>
#include <pthread.h>

mqd_t mq1;

void *send_file(void *param)
{
    mqd_t mq1 = mq_open("/mq1", O_WRONLY);
    char buffer[64];

    while (1)
    {
        fgets(buffer, sizeof(buffer), stdin);
        if (buffer[strlen(buffer) - 1] == '\n')
            buffer[strlen(buffer) - 1] = 0;
        mq_send(mq1, buffer, strlen(buffer) + 1, 0);
    }
}

int main(int argc, char *argv[])
{
    mqd_t mq;

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 20;
    attr.mq_msgsize = 32;
    attr.mq_curmsgs = 0;

    struct mq_attr attr1;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 20;
    attr.mq_msgsize = 32;
    attr.mq_curmsgs = 0;

    mq = mq_open("/mq0", O_RDONLY | O_CREAT, 0644, &attr);

    mq1 = mq_open("/mq1", O_RDONLY | O_CREAT, 0644, &attr1);

    char buff[32];

    pthread_t thread_ID;
    pthread_create(&thread_ID, NULL, &send_file, NULL);

    char datos[20][21];

    while (1)
    {
        mq_receive(mq, buff, 32, NULL);
        FILE *Text = fopen(buff, "r");
        for (int i = 0; i < 10; i++)
        {
            fgets((char *)(datos + i), 20, Text);
        }
        for (int i = 0; i < 10; i++)
        {
            printf("%d:%s", i, (char *)(datos + i));
        }

        //    for (int i = 10; i > -1; i--)
        //   {
        //        printf("%d:%s", i, (char *)(datos - i));
        //    }
    }
}

