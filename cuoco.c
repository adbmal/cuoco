

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

void saybye()
{
  printf("Bye!\n");

}
void yield();
void b()
{
  void (*foo_ptr)() = yield;
  foo_ptr();
}
void sayhi()
{
  printf("Hi!\n");
  b();
  printf("Hi2!\n");
}
char stack[1024 * 128];
void *regs[5];
void __attribute__ ((noinline)) yield()
{
  void *bp = (void *)stack + 1024 * 128 - 1;
  void *func = saybye;
  asm ("\t movq %0, 0(%%rbp)" : "=r"(bp));
  asm ("\t movq %0, 8(%%rbp)" : "=r"(func));
}

int main(int argc, char *argv[])
{
  printf("Hi, main!\n");
  printf("Hi, create %d\n", cuoco_create());

  sayhi();

  printf("Hi, create %d\n", cuoco_create());
  printf("Hi, create %d\n", cuoco_create());
  return 0;
}
