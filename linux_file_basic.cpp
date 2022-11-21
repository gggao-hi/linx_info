#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void create_file(const char* file_name) {
  int fd = creat(file_name, 0644);
  printf("create fd:%d \n", fd);
}
int open_file(const char* file_name, int flag, int mode) {
  int fd = open(file_name, flag, mode);
  printf("open fd:%d \n", fd);
  return fd;
}
void write_file(int fd_open, const char* buff) {
  size_t length = strlen(buff);
  printf("length:%ld \n", length);
  printf("fd_open:%d \n", fd_open);
  //将文件的fd更新成文件末尾的position,实现在open函数中使用O_APPEND的效果
  lseek(fd_open, 0, SEEK_END);
  int fd = write(fd_open, buff, length);
  printf("write fd:%d \n", fd);
  // open 中加上O_SYNC 就会自动sync
  int sync_fd = fsync(fd_open);
  printf("fsync fd : %d \n", sync_fd);

  int close_fd = close(fd_open);
  printf("close fd:%d \n", close_fd);
}
void read_file(int fd_open) {
  size_t length = 1024;
  char buff[length];
  int fd = -2;
  while ((fd = read(fd_open, &buff, length)) > 0) {
    printf("read fd:%d \n", fd);
    printf("buff: %s \n", buff);
  }
  printf("read fd:%d \n", fd);
  close(fd_open);
}
int main() {
  const char* name = "/mnt/d/code/clearn/file_operator/b.txt";

  const char* buff =
      "adsadsadsadsadasdasdsadsadsadsadsadsadsadsadsadsadsadsanadsadsads"
      "adsada\n adsadsadsadsadsadsadsadsadsadsadsadsrgfdg\n";
  write_file(open_file(name, O_WRONLY | O_CREAT, S_IRWXU), buff);
  read_file(open_file(name, O_RDONLY, S_IRWXU));
  return 0;
}