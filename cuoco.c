

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CUOCO 1024

#define ESP 0
#define EIP 1
// -----------
#define RSP 0
#define RIP 1
#define RBX 2
#define RDI 3
#define RSI 4

typedef int (*cuoco_ctx_pfn_t)( void *s);
struct cuoco_ctx_t {
  void *regs[ 5 ];
  void *param;
	cuoco_ctx_pfn_t routine;
	size_t ss_size;
	char sRunStack[ 1024 * 128 ];
};
extern void cuoco_ctx_swap( void *, void *) asm("cuoco_ctx_swap");

struct cuoco_t {
  int status;
  struct cuoco_ctx_t *ctx;
};

static struct cuoco_t env[MAX_CUOCO] = {{0}};
static int current_co = 0;

void cuoco_routine_func(void *func)
{
  printf("routine!\n");
}
// int cuoco_create(cuoco_ctx_pfn_t *pfn)
int cuoco_create()
{
  int index = 0;
  while(index < MAX_CUOCO && env[index].status != 0)
    ++index;
  env[index].status = 1;
  struct cuoco_ctx_t *ctx = malloc(sizeof(struct cuoco_ctx_t));
  memset(ctx, 0, sizeof(*ctx));
  // ctx->routine = pfn;
  current_co = index;
  return index;
}

char stack[1024 * 128];
void *regs[5];
void *regs2[5];

void saybye();
void yield()
{
  printf("Yield!\n");
  regs2[0] = stack + 1024*128 - 1;
  regs2[1] = saybye;
  cuoco_ctx_swap(regs, regs2);
}
void yield2()
{
  printf("Yield2!\n");
  cuoco_ctx_swap(regs2, regs);
}
void saybye()
{
  printf("Bye!\n");
  yield2();
}

void sayhi()
{
  printf("Hi1\n");
  yield();
  printf("Hi2!\n");
}
// void *regs[5];
void *a, *b, *c;

int main(int argc, char *argv[])
{
  printf("Hi, main!\n");
  // printf("Hi, create %d\n", cuoco_create());

  sayhi();

  printf("Hi, create %d\n", cuoco_create());
  printf("Hi, create %d\n", cuoco_create());
  return 0;
}
