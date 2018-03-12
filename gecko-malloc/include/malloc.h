#pragma once

#ifdef _WIN32
#error This malloc implementation is not designed to work with Windows
#endif

#include <stdatomic.h>
#include <unistd.h>

#define ALLOC_SIZE 0x400000
#define MAX_SMALL 512
#define MAX_MEDIUM 1024
#define MAX_LARGE 2048
#define ALIGN 8
//should be ideomic otherwise we must replace this with bithacks
#define ALIGN_SIZE(X,SIZE) (((X-1)|(SIZE-1))+1)

#define LOOKUP_8(X) (X / 8 - 1)
#define LOOKUP_16(X) (X / 16 +32)
#define LOOKUP_32(X) (X / 32 +64)

//maximum index for page_list array
#define PAGELIST_MAX 1000

#define PAGELIST_SIZE 44
//allocator macro for better feature useage of memory mappings
#ifdef __linux__
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,38)
#define ALLOC(size) ({void *tmp = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS , -1, 0); \
      				madvise(tmp,size,MADV_HUGEPAGE);                                            \
                      tmp;})
#endif
#elif defined(__FreeBSD__)
#define ALLOC(size) ({void *tmp = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_ALIGNED_SUPER, -1, 0); \
                  if (tmp == MAP_FAILED)                                                                                            \
                      tmp = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);                           \
                  tmp;})
#elif defined(__unix__) || defined(__linux__)   //this will probably waste alot of pages
#define ALLOC(size) mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#else
#error Operating System not supported
#endif

//macro for uncommiting free if possible
//ptr points to page start
#if defined(__linux__)
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,5,0)
#define FREE(ptr,size) madvise((ptr),size,MADV_FREE)
#else
#define FREE(ptr,size) madvise((ptr),size,MADV_DONTNEED)
#endif
#elif defined(__FreeBSD__)
#define FREE(ptr,size) madvise((ptr),size,MADV_FREE)
#elif defined(__unix__) || defined(__linux__)   //this is uneffective and possibly wastes alot of space
#define FREE(ptr,size) posix_madvise((ptr),size,MADV_DONTNEED)
#else
#error Operating System not supported
#endif

struct __attribute__((aligned)) malloc_header {
    struct malloc_header *_Atomic next;
	struct malloc_page * _Atomic pageref;
    size_t size;
};
//TODO:refactor
struct __attribute__((aligned)) malloc_page {
    size_t page_size;
    atomic_size_t level;       //allocated can be used as offset to free memory on page
    struct malloc_page * _Atomic next;
    void * _Atomic pageref;
};

struct __attribute__((aligned)) free_page {
	atomic_size_t pos;
	void *pages[PAGELIST_MAX];
};

extern struct free_page page_list[];
extern struct malloc_page small;
extern struct malloc_page medium;
extern struct malloc_page large;
extern struct malloc_header * _Atomic small_free[];

void *malloc(size_t size) __attribute__((malloc));
void *calloc(size_t nmemb, size_t size)  __attribute__((malloc));
void *realloc(void *ptr, size_t size)__attribute__((malloc));
void free(void *restrict ptr);
