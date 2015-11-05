

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACKSIZE (1024*128)

typedef void (*cuoco_ctx_pfn_t)( void *s);
typedef enum cuoco_status_enum {
  INIT, END
} cuoco_status;
typedef struct cuoco_ctx_s {
  void *regs[ 2 ];
  void *param;
	cuoco_ctx_pfn_t routine;
  struct cuoco_ctx_s *next;
  cuoco_status status;
	char run_stack[ STACKSIZE ];
} cuoco_ctx_t;
extern void cuoco_ctx_swap( void *, void *) asm("cuoco_ctx_swap");

cuoco_ctx_t *cuoco_ctx_head = NULL;
cuoco_ctx_t *cuoco_ctx_tail = NULL;
void *cuoco_main[2];

void cuoco_yield()
{
  cuoco_ctx_t *ctx = cuoco_ctx_head;
  if(ctx == cuoco_ctx_tail)
    return;
  cuoco_ctx_head = cuoco_ctx_head->next;
  cuoco_ctx_tail->next = ctx;
  cuoco_ctx_tail = ctx;
  ctx-> next = NULL;
  cuoco_ctx_swap(ctx, cuoco_ctx_head);
}
void cuoco_routine_init()
{
  cuoco_ctx_t *current_ctx = cuoco_ctx_head;
  current_ctx->routine(current_ctx->param);
  current_ctx->status = END;
  cuoco_ctx_swap(cuoco_ctx_head, cuoco_main);
}

cuoco_ctx_t *cuoco_create(cuoco_ctx_pfn_t pfn, void *param)
{
  cuoco_ctx_t *ctx = malloc(sizeof(cuoco_ctx_t));
  memset(ctx, 0, sizeof(*ctx));
  ctx->regs[0] = ctx->run_stack + STACKSIZE - 1;
  ctx->regs[1] = cuoco_routine_init;
  ctx->routine = pfn;
  ctx->param = param;
  ctx->next = cuoco_ctx_head;
  ctx->status = INIT;
  cuoco_ctx_head = ctx;
  if (!cuoco_ctx_tail) {
    cuoco_ctx_tail = cuoco_ctx_head;
  }
  return ctx;
}

void cuoco_start()
{
  cuoco_ctx_t *ctx;
  while ( (ctx=cuoco_ctx_head) != NULL ) {
    if(ctx->status==INIT){
      cuoco_ctx_swap(cuoco_main, ctx);
    }
    if(ctx->status==END){
      cuoco_ctx_head = cuoco_ctx_head->next;
      free(ctx);
    }
  }
}

void sayhi(int times)
{
  for(int a=0; a<times; ++a)
  {
    printf("Hi  %d\n", a);
    cuoco_yield();
  }
}

void saybye(int times)
{
  for(int a=0; a<times; ++a)
  {
    printf("Bye %d\n", a);
    cuoco_yield();
  }
}

int main(int argc, char *argv[])
{
  printf("Hi, main!\n");
  printf("routine create %p!\n", cuoco_create((cuoco_ctx_pfn_t)sayhi, (void *)10));
  printf("routine create %p!\n", cuoco_create((cuoco_ctx_pfn_t)saybye, (void *)10));

  cuoco_start();

  return 0;
}
