#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>
#include <pthread.h>

void* receive_file(void *param)
{


    char datos[20][21];

    while (1)
    {
        mqd_t mq1 = mq_open("/mq1", O_RDONLY);
        char buff[32];

        mq_receive(mq1, buff, 32, NULL);
        FILE *Text = fopen(buff,"r");
        for(int i=0; i < 10; i++)
        {
            fgets((char*)(datos+i),20,Text);
        }
        for(int i=0; i < 10; i++)
        {
            printf("%d:%s",i,(char*)(datos+i));
        }
    }
}

int main(int argc, char *argv[])
{
    mqd_t mq = mq_open("/mq0", O_WRONLY);
    char buffer[64];

    pthread_t threadID;
    pthread_create(&threadID, NULL, &receive_file, NULL);

    while (1)
    {
        fgets(buffer, sizeof(buffer),stdin);
        if(buffer[strlen(buffer) - 1 ] == '\n') buffer[strlen(buffer) - 1] = 0;
        mq_send(mq, buffer, strlen(buffer) + 1, 0);
    }
}