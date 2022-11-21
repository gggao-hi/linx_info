#include <stdio.h>
#include <string.h>
#include <sys/poll.h>
#include <unistd.h>
#define TIME_OUT 5
#define FD_NUMBER 2

int multi_poll() {
  struct pollfd fds[FD_NUMBER];
  fds[0].events = POLLIN;
  fds[0].fd = STDIN_FILENO;
  fds[1].events = POLLOUT;
  fds[1].fd = STDOUT_FILENO;
  // 只要有一个 fd ready了,就不会触发 timeout
  int ret = poll(fds, FD_NUMBER, TIME_OUT * 1000);
  if (ret == -1) {
    perror("poll");
    return 1;
  }
  if (!ret) {
    printf("%d seconds elapsed\n", TIME_OUT);
    return 0;
  }
  if (fds[0].revents & POLLIN) {
    printf("read is ready\n");
  }
  if (fds[1].revents & POLLOUT) {
    printf("write is ready\n");
  }

  char buff[30];
  read(fds[0].fd, buff, 30);
  int len = strlen(buff);
  buff[len] = '\0';
  write(fds[1].fd, buff, len);
  return 0;
}

int main(int argc, char const *argv[]) {
  multi_poll();

  return 0;
}
