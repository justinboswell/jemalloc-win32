
#include <vld.h>

#include <assert.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <errno.h>

#include "jemalloc_test.h"
#include "memory_pool_test.h"

class AA
{
public:
    int a, b;
};

void Hello_World()
{
    printf("\n");
    printf("Hello world !\n");
    printf("\n");
}

void Memory_Pool_Test()
{
    mem_pool_test_linstener *tester = new mem_pool_test();
    if (tester != NULL) {
        tester->Begin();
        tester->Malloc(0);
        tester->End();
        delete tester;
    }
}

void MemoryPool_Test()
{
    int i;
#ifdef _DEBUG
    const int nMaxLoop  = 100000;
    const int nMaxAlloc = 16384;
#else
    const int nMaxLoop  = 1000000;
    const int nMaxAlloc = 16384;
    //const int nMaxAlloc = 10000;
#endif

    unsigned int dwUsedTime1, dwUsedTime2;
    unsigned int dwStartTickCount = GetTickCount();
    for (i = 0; i < nMaxLoop; i++) {
        char *p = (char *)malloc((i % nMaxAlloc) + 1);
        if (p)
            free(p);
    }

    dwUsedTime1 = GetTickCount() - dwStartTickCount;
    printf("Alloc Func : malloc()     Alloc Size: 1-%d\n", nMaxAlloc);
    printf("Alloc Count: %d\n",nMaxLoop);
    printf("Total Cost : %d ms.\n", dwUsedTime1);

    //system("pause");
    printf("\n");

    set_errno(0);
    je_init();

    dwStartTickCount = GetTickCount();
    for (i = 0; i < nMaxLoop; i++) {
        void *p = je_malloc((i % nMaxAlloc) + 1);
        if (p)
            je_free(p);
    }

    je_uninit();

    dwUsedTime2 = GetTickCount() - dwStartTickCount;
    printf("Alloc Func : je_malloc()  Alloc Size: 1-%d\n", nMaxAlloc);
    printf("Alloc Count: %d\n", nMaxLoop);
    printf("Total Cost : %d ms.\t  Speed up: %0.2f %%\n", dwUsedTime2, ((double)(dwUsedTime1 * 10000 / dwUsedTime2) / 100.0));

    //system("pause");
    //printf("\n");
}

/*
 *
 * 顺序大小, 顺序释放, 随机大小(分不同的块), 乱序释放
 * 关于分块: 1-4096, 4096-16384, 16K-64K, 16K-84K, 128K-1M, 1M-64M
 *
 */

int main(int argc, char *argv[])
{
#if 1
    AA a;
    a.a = 0;
    a.b = 9;
    printf("A.a = %d, A.b = %d\n\n", a.a, a.b);
#endif

    do {
        Hello_World();

        printf("-----------------------------------------------------\n");
        MemoryPool_Test();
        printf("-----------------------------------------------------\n");

    } while (0);

    printf("\n");

#ifdef _WIN32
    system("pause");
#endif
    return 0;
}
