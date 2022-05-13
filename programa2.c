#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>
#include <pthread.h>

void *receive_file(void *param)
{

    char datos[20][21];

    while (1)
    {
        mqd_t mq1 = mq_open("/mq1", O_RDONLY);
        char buff[32];
        int order;

        for (int i = 0; i < 10; i++)
        {
            mq_receive(mq1, (datos[i]+i), 32, NULL);
        } 
        mq_receive(mq1, buff, 32, NULL);
        sscanf(buff, "%d", &order);
        switch (order)
        {
        case 1:
            for (int i = 0; i < 10; i++)
            {
                printf("%d:%s", i, (char *)(datos + i));
            }
            break;

        case 2:
            for (int i = 9; i > -1; i--)
            {
                printf("%d:%s", i, (char *)(datos + i));
            }
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    mqd_t mq = mq_open("/mq0", O_WRONLY);

    pthread_t threadID;
    pthread_create(&threadID, NULL, &receive_file, NULL);

    char buffer[64];
    char datos[20];

    while (1)
    {
        fgets(buffer, sizeof(buffer), stdin);
        if (buffer[strlen(buffer) - 1] == '\n')
            buffer[strlen(buffer) - 1] = 0;
        FILE *Text = fopen(buffer, "r");

        for (int i = 0; i < 10; i++)
        {
            fgets((char *)(datos + i), 20, Text);
            mq_send(mq, datos, strlen(datos) + 1, 0);
        }           
        fgets(buffer, sizeof(buffer), stdin);
        if (buffer[strlen(buffer) - 1] == '\n')
            buffer[strlen(buffer) - 1] = 0;
        mq_send(mq, buffer, strlen(buffer), 0);
    }
}