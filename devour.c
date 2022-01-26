/*
 * devour
 * X11 window swallower
 */

#include <X11/Xlib.h>
#include <stdlib.h>
#include <string.h>

#define UNSAFE_CHARS "`\"'()[]& "

int shell_wrap = 0;

void run_command(char **argv) {
  char arg_char;
  char *arg;
  char cmd[1024] = {0};

  if(shell_wrap)
      strcat(cmd, "$SHELL -i -c '");
  while ((arg = *++argv)) {
    while ((arg_char = *arg++)) {
      if (strchr(UNSAFE_CHARS, arg_char))
        strcat(cmd, "\\");
      strncat(cmd, &arg_char, 1);
    }
    strcat(cmd, " ");
  }
  if(shell_wrap)
      strcat(cmd, "> /dev/null 2>&1; exit'");
  (void)!system(cmd);
}

int main(int argc, char **argv) {
  int rev;
  Window win;
  Display *dis = XOpenDisplay(0);

  if(strcmp("-s", argv[1]) == 0) {
      shell_wrap = 1;
      argv++;
  }

  XGetInputFocus(dis, &win, &rev);
  XUnmapWindow(dis, win);
  XFlush(dis);
  run_command(argv);
  XMapWindow(dis, win);
  XCloseDisplay(dis);

  (void)argc;
  return 0;
}
