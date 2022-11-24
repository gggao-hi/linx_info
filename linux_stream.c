#include <limits.h>
#include <stdio.h>
#include <string.h>

FILE *open_file(const char *path, const char *mode) {
  FILE *stream = fopen(path, mode);
  if (!stream) {
    perror("open failed");
    return NULL;
  }
  return stream;
}
void use_stdin_stdout() {
  fputs("stdin and stdout\n", stdout);
  char buff[LINE_MAX];

  fgets(buff, LINE_MAX, stdin);

  fflush(stdin);
  fclose(stdin);
  printf("out:%s", buff);
}
int close_file(FILE *stream) {
  int ret = fclose(stream);
  if (ret == EOF) {
    perror("close failed");
  }
  return ret;
}
void read_file(FILE *stream) {
  int ret = fgetc(stream);
  if (ret == EOF) {
    perror("read error");
  } else {
    printf("%c \n", (char)ret);
  }

  char buff[LINE_MAX];
  if (fgets(buff, LINE_MAX, stream)) {
    printf("%s \n", buff);
  }

  close_file(stream);
}
void fread_file(FILE *stream) {
  char buff[LINE_MAX];
  size_t nr = fread(buff, LINE_MAX, 1, stream);
  printf("nr: %ld\n", nr);
  printf("buff: %s\n", buff);
}
void fputs_file(FILE *stream) {
  const char *source = "lllllsss";
  int ret = fputs(source, stream);
  printf("ret : %d \n", ret);
  fflush(stream);
  fclose(stream);
}
void fwrite_file(FILE *stream) {
  const char *source = "writesssss \n sdasdasdsa \n ssswrite000";
  size_t ret = fwrite(source, strlen(source), 1, stream);
  printf("fwrite ret: %ld", ret);
  fflush(stream);
  fclose(stream);
}

int main(int argc, char const *argv[]) {
  use_stdin_stdout();
  printf("=======read file=========\n");
  read_file(open_file("./files/stream_test.txt", "r"));
  printf("======================================\n");
  fread_file(open_file("./files/stream_test.txt", "r"));
  printf("============fputs==============\n");
  fputs_file(open_file("./files/stream_test.txt", "a"));
  fread_file(open_file("./files/stream_test.txt", "r"));
  printf("============fwrite==============\n");
  fwrite_file(open_file("./files/stream_test.txt", "a"));
  fread_file(open_file("./files/stream_test.txt", "r"));

  return 0;
}
