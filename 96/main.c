#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
struct termios orig_termios;
void reset_terminal(){
  tcsetattr(0, TCSANOW, &orig_termios);
}
int main(){
  //setup terminal
  struct termios new_termios;
  tcgetattr(0, &orig_termios);
  memcpy(&new_termios, &orig_termios, sizeof(new_termios));
  atexit(reset_terminal);
  cfmakeraw(&new_termios);
  new_termios.c_cflag &= ~ICANON;
  tcsetattr(0, TCSANOW, &new_termios);
  printf("\e[?1049h");
  printf("\e[2;1H");

  const char *text = "The quick brown fox jumps over the lazy dog.\n";
  printf("%s", text);
  printf("\e[1;1H");
  fflush(stdout);

  int pointer = 0;
  int mistakes = 0;
  int correct = 0;
  int a = 0;
  unsigned char c;
  while((a = read(0, &c, sizeof(c))) >= 0){
    //printf("%d", c);
    if(text[pointer]=='\n')
      break;
    if(c==48)
      break;
    if(c==127){
      // printf("\e[2D");
      printf("\x8");
      printf(" \e[1D");
      fflush(stdout);
      pointer-=2;
      correct--;
      continue;
    }
    if(c!=text[pointer]){
      printf("\e[97;101m");
      mistakes++;
      correct--;
    }
    putchar(c);
    printf("\e[0m");
    fflush(stdout);
    pointer++;
    correct++;
    printf("\e[s");
    printf("\e[3;1H");
    double rate = ((double)correct) / ((double)(correct+mistakes));
    printf("%2f", rate*100.0);
    printf("\e[u");
  }
  // printf("\e[?1049h");
  // printf("\e[1;1H");
  // fflush(stdout);
  // printf("Hello,World!\n");
  // //printf("\e[?1049h\e[1A\e[1m\e[33;42mHello, World!\e[0m\n");
  // sleep(5);
  printf("\e[?1049l");
  fflush(stdout);
  return 0;
}
