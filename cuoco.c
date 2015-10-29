

#include <stdio.h>

#define MAX_CUOCO 1024

struct cuoco_t {
  int pid;
};

static struct cuoco_t env[MAX_CUOCO] = {0};
static int current_co = 0;

int cuoco_create()
{
  int index = 0;
  while(index < MAX_CUOCO && env[index].pid != 0)
    ++index;
  env[index].pid = 1;
  return index;
}

int main(int argc, char *argv[])
{
  printf("Hi, there!\n");
  printf("Hi, create %d\n", cuoco_create());
  printf("Hi, create %d\n", cuoco_create());
  printf("Hi, create %d\n", cuoco_create());
  return 0;
}
