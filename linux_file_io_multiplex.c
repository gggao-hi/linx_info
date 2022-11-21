
#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <malloc.h>
#include <unistd.h>

#define TIME_OUT 5
#define BUFF_LENGTH 1024

int mult_read()
{
    int file_fd = STDOUT_FILENO;
    struct timeval *time_out = (struct timeval *)malloc(sizeof(struct timeval));
    time_out->tv_sec = TIME_OUT;
    time_out->tv_usec = 0;

    fd_set read_fds;
    int ret;
    FD_ZERO(&read_fds);
    FD_SET(file_fd, &read_fds);
    ret = select(file_fd + 1, &read_fds, NULL, NULL, time_out);
    if (ret == -1)
    {
        free(time_out);
        perror("select\n");
        return 1;
    }
    else if (!ret)
    {
        free(time_out);
        printf("%d second elapsed\n", TIME_OUT);
        return 0;
    }

    if (FD_ISSET(file_fd, &read_fds))
    {
        char buff[BUFF_LENGTH];
        int len;
        len = read(file_fd, buff, BUFF_LENGTH);
        if (len == -1)
        {
            free(time_out);
            perror("read\n");
            return 1;
        }
        if (len)
        {
            buff[len] = '\0';
            printf("read:%s \n", buff);
        }
        free(time_out);
        return 0;
    }
    free(time_out);
    fprintf(stderr, "stdin and readfd not in here\n");
    return 1;
}

void sleep_by_mult(int sec, long usec)
{
    struct timeval time_out;
    time_out.tv_sec = sec;
    time_out.tv_usec = usec;
    select(0, NULL, NULL, NULL, &time_out);
    read(0, NULL, 0);
}
void print_now(const char *prefix)
{
    struct timeval now;
    gettimeofday(&now, NULL);
    char *time_second = (char *)malloc(20 * sizeof(char));
    int sec = (now.tv_sec % 60);
    int usec = (now.tv_usec);

    int len = sprintf(time_second, "%d: %d ", sec, usec);
    printf("%s-%s \n", prefix, time_second);
}

int mult_poll()
{
    return 0;
}
int main()
{
    printf("please input ...\n");
    int ret = mult_read();
    printf("mult read ret: %d\n", ret);
    printf("=============================\n");
    print_now("start sleep");
    sleep_by_mult(2, 500);
    print_now("end sleep");
    return 0;
}