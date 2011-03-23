#include <stdio.h>
#include <string.h>

#include <assert.h>

#include <mpp/shmem.h>

static long pSync[_SHMEM_BCAST_SYNC_SIZE];

static long src[4] = { 11, 12, 13, 14 };

#define DST_SIZE 4

static long dst[DST_SIZE];

int npes;
int me;

int
main(void)
{
  int i;

  start_pes(0);
  npes = _num_pes();
  me = _my_pe();

  for (i = 0; i < DST_SIZE; i++) {
    dst[i] = -1;
  }

  for (i = 0; i < _SHMEM_BCAST_SYNC_SIZE; i += 1) {
    pSync[i] = _SHMEM_SYNC_VALUE;
  }

  shmem_barrier_all();

  shmem_fcollect64(dst, src, 2,
                   0, 0, 4,
                   pSync);

  show_dst("AFTER");

  return 0;
}

show_dst(char *tag)
{
  int i;
  printf("%8s: dst[%d/%d] = ", tag, me, npes);
  for (i = 0; i < DST_SIZE; i+= 1) {
    printf("%d ", dst[i]);
  }
  printf("\n");
}
