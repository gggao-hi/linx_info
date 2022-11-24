#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void create_file(const char *file_name) {
  int fd = creat(file_name, 0644);
  printf("create fd:%d \n", fd);
}
int open_file(const char *file_name, int flag, int mode) {
  int fd = open(file_name, flag, mode);
  printf("open fd:%d \n", fd);
  return fd;
}
void write_file(int fd_open, const char *buff) {
  size_t length = strlen(buff);
  printf("length:%ld \n", length);
  printf("fd_open:%d \n", fd_open);
  //将文件的fd更新成文件末尾的position,实现在open函数中使用O_APPEND的效果
  // lseek(fd_open, 0, SEEK_END);
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
    buff[fd] = '\0';
    printf("buff: %s \n", buff);
  }
  printf("read fd:%d \n", fd);
  close(fd_open);
}
void pread_file(int fd_open) {
  char buff[100];
  int pread_fd = pread(fd_open, &buff, 100, 26);
  printf("pread fd: %d \n buff:%s \n", pread_fd, buff);
  close(fd_open);
}
void pwrite_file(int fd_open, const char *buff) {
  size_t length = strlen(buff);
  int pwrite_fd = pwrite(fd_open, buff, length, 30);
  printf("pwrite fd:%d \n", pwrite_fd);
  close(fd_open);
}
void truncate_file(const char *path, size_t length) {
  int fd_open = open_file(path, O_RDONLY | O_WRONLY, S_IRWXU);
  int result = ftruncate(fd_open, length);
  printf("truncate:%d \n", result);
  fd_open = open_file(path, O_RDONLY, S_IRWXU);

  read_file(fd_open);
}
int main() {
  const char *name = "./files/b.txt";

  const char *buff =
      "But nearly one third of this has to be spent in sleep; some years have "
      "to be spent over our meals; some in making journeys on land and voyages "
      "by sea; some in merry-making; some in watching over the sick-beds of "
      "our nearest and dearest relatives.\n";
  write_file(open_file(name, O_WRONLY | O_CREAT, S_IRWXU), buff);
  read_file(open_file(name, O_RDONLY, S_IRWXU));
  pwrite_file(open_file(name, O_WRONLY, S_IRWXU),
              "Your life can be enhanced, and your happiness enriched, when "
              "you choose to change your perspective. ");
  pread_file(open_file(name, O_RDONLY, S_IRWXU));
  printf("======================================\n");
  truncate_file(name, sizeof(char) * 32);
  return 0;
}