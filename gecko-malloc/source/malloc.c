#include "malloc.h"
#include <stdatomic.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <assert.h>

// List for free HugePages
struct free_page page_list[PAGELIST_SIZE] = {{.pos = ATOMIC_VAR_INIT(0),.pages={ATOMIC_VAR_INIT(0)}}};
// occupied smalls
struct malloc_page small = {.page_size = (size_t)ALLOC_SIZE, .level = ATOMIC_VAR_INIT(0), .next = NULL,.pageref = NULL};
struct malloc_page medium = {.page_size = (size_t)ALLOC_SIZE, .level = ATOMIC_VAR_INIT(0), .next = NULL,.pageref = NULL};
struct malloc_page large = {.page_size = (size_t)ALLOC_SIZE, .level = ATOMIC_VAR_INIT(0), .next = NULL,.pageref = NULL};
// list for free small ptr
struct malloc_header* _Atomic small_free[129];

atomic_flag small_heap_locked = ATOMIC_FLAG_INIT,
            medium_heap_locked = ATOMIC_FLAG_INIT,
            large_heap_locked = ATOMIC_FLAG_INIT,
            page_list_lock = ATOMIC_FLAG_INIT;

__attribute__((constructor)) static void malloc_init(void)
{
    for (int i = 0; i < 129; ++i)
        atomic_init(&small_free[i], NULL);

    atomic_store(&small.pageref, ALLOC(ALLOC_SIZE));
    if (atomic_load(&small.pageref) == MAP_FAILED)
        _exit(-1);
    posix_madvise(atomic_load(&small.pageref),ALLOC_SIZE,MADV_WILLNEED);
	small.page_size = ALLOC_SIZE;
    atomic_init(&small.next, NULL);
    atomic_init(&small.level, 0);
    
    atomic_store(&medium.pageref, ALLOC(ALLOC_SIZE));
    if (atomic_load(&medium.pageref) == MAP_FAILED)
        _exit(-1);
    posix_madvise(atomic_load(&medium.pageref),ALLOC_SIZE,MADV_WILLNEED);
    medium.page_size = ALLOC_SIZE;
    atomic_init(&medium.next, NULL);
    atomic_init(&medium.level, sizeof(struct malloc_page));
    
    atomic_store(&large.pageref, ALLOC(ALLOC_SIZE));
    if (atomic_load(&large.pageref) == MAP_FAILED)
        _exit(-1);
    posix_madvise(atomic_load(&large.pageref),ALLOC_SIZE,MADV_WILLNEED);
    large.page_size = ALLOC_SIZE;
    atomic_init(&large.next, NULL);
    atomic_init(&large.level, sizeof(struct malloc_page));

    atomic_flag_clear(&small_heap_locked);
    atomic_flag_clear(&medium_heap_locked);
    atomic_flag_clear(&large_heap_locked);
    atomic_flag_clear(&page_list_lock);
}

static inline size_t indexCatch(size_t pos)
{
    --pos;
    pos |= pos >> 1;
    pos |= pos >> 2;
    pos |= pos >> 4;
    pos |= pos >> 8;
    pos |= pos >> 16;
    pos |= pos >> 32;
    ++pos;
    pos = __builtin_ctzll(pos) - 21;
    return pos;
}

void* malloc(size_t size)
{
    unsigned char index;
    // First check if whole pages are required
    // if yes check for cached pages will be done
    // new map called if not available
    // no further changes required during structure change
    if (size == 0)
    	return NULL;
    if (size <= MAX_LARGE) {
        if (size <= MAX_SMALL) {
            size = ALIGN_SIZE(size, 8);
            index = LOOKUP_8(size);
            while (atomic_flag_test_and_set(&small_heap_locked))
           		;
            // lookup freelist atomically hoping the ABA-Problem does not occure
            struct malloc_header *ret = NULL;
            struct malloc_header * _Atomic next = ATOMIC_VAR_INIT(NULL);
            do {
                ret = atomic_load(&small_free[index]);
                if (ret == NULL)
                    break;
                atomic_store(&next, atomic_load(&atomic_load(&small_free[index])->next));
            } while (!atomic_compare_exchange_weak(&small_free[index], &ret,
                next));
            // get new page, if none in freelist
            if (ret == NULL) {
                const unsigned char page_index = indexCatch(ALLOC_SIZE);

                while (atomic_flag_test_and_set(&page_list_lock))
                    ;
                if (page_list[page_index].pos > 0) {
                    atomic_store(&small.pageref, page_list[page_index].pages[--page_list[page_index].pos]);
                    atomic_store(&small.level, 0);
                }
                atomic_flag_clear(&page_list_lock);

                // get new page, if not enough cached
                if (atomic_load(&small.pageref) == NULL || small.page_size - atomic_load(&small.level) <= size + sizeof(struct malloc_header)) {
                    atomic_store(&small.pageref, ALLOC(ALLOC_SIZE));
                    if (atomic_load(&small.pageref) == MAP_FAILED) {
                        atomic_store(&small.pageref, NULL);
                        atomic_flag_clear(&small_heap_locked);
                        return NULL;
                    }
                    atomic_store(&small.level, 0);
                }
                // alloc from page is not atomic
                size_t level = atomic_load(&small.level);
                atomic_fetch_add(&small.level, sizeof(struct malloc_header) + size);
                ret = (struct malloc_header*)((char*)(atomic_load(&small.pageref)) + level);
                atomic_store(&ret->pageref, atomic_load(&small.pageref));
                ret->size = size;
                atomic_init(&ret->next, NULL);
            }
            atomic_flag_clear(&small_heap_locked);
            assert(ret->size >= size);
            return ret + 1;
        } else if (size <= MAX_MEDIUM) {
            size = ALIGN_SIZE(size, 16);
            index = LOOKUP_16(size);
            while (atomic_flag_test_and_set(&medium_heap_locked))
                ;
            // lookup freelist atomically hoping the ABA-Problem does not occure
            struct malloc_header *ret = NULL;
            struct malloc_header * _Atomic next = ATOMIC_VAR_INIT(NULL);
            do {
                ret = atomic_load(&small_free[index]);
                if (ret == NULL)
                    break;
                atomic_store(&next, atomic_load(&atomic_load(&small_free[index])->next));
            } while (!atomic_compare_exchange_weak(&small_free[index], &ret,
                next));
            // get new page, if none in freelist
            if (ret == NULL) {
                const unsigned char page_index = indexCatch(ALLOC_SIZE);

                while (atomic_flag_test_and_set(&page_list_lock))
                    ;
                if (page_list[page_index].pos > 0) {
                    atomic_store(&medium.pageref ,page_list[page_index].pages[--page_list[page_index].pos]);
                    atomic_store(&medium.level, 0);
                }
                atomic_flag_clear(&page_list_lock);
                // get new page, if not enough cached
                if (atomic_load(&medium.pageref) == NULL || medium.page_size - atomic_load(&medium.level) <= size + sizeof(struct malloc_header)) {
                    atomic_store(&medium.pageref, ALLOC(ALLOC_SIZE));
                    if (atomic_load(&medium.pageref) == MAP_FAILED) {
                        atomic_store(&medium.pageref, NULL);
                        atomic_flag_clear(&medium_heap_locked);
                        return NULL;
                    }
                    atomic_store(&medium.level, 0);
                }
                // alloc from page is not atomic
                size_t level = atomic_load(&medium.level);
                atomic_fetch_add(&medium.level, sizeof(struct malloc_header) + size);
                ret = (struct malloc_header*)((char*)(atomic_load(&medium.pageref)) + level);
                atomic_store(&ret->pageref, atomic_load(&medium.pageref));
                ret->size = size;
                atomic_init(&ret->next, NULL);
            }
            atomic_flag_clear(&medium_heap_locked);
            assert(ret->size >= size);
            return ret + 1;
        } else {
            size = ALIGN_SIZE(size, 32);
            index = LOOKUP_32(size);
            while (atomic_flag_test_and_set(&large_heap_locked))
                ;
            // lookup freelist atomically hoping the ABA-Problem does not occure
            struct malloc_header *ret = NULL, *next = NULL;
            do {
                ret = atomic_load(&small_free[index]);
                if (ret == NULL)
                    break;
                next = atomic_load(&atomic_load(&small_free[index])->next);
            } while (!atomic_compare_exchange_weak(&small_free[index], &ret,
                small_free[index]->next));
            // get new page, if none in freelist
                        if (ret == NULL) {
                const unsigned char page_index = indexCatch(ALLOC_SIZE);

                while (atomic_flag_test_and_set(&page_list_lock))
                    ;
                if (page_list[page_index].pos > 0) {
                    atomic_store(&large.pageref, page_list[page_index].pages[--page_list[page_index].pos]);
                    atomic_store(&large.level, 0);
                }
                atomic_flag_clear(&page_list_lock);

                // get new page, if not enough cached
                if (atomic_load(&large.pageref) == NULL || large.page_size - atomic_load(&large.level) <= size + sizeof(struct malloc_header)) {
                    atomic_store(&large.pageref, ALLOC(ALLOC_SIZE));
                    if (atomic_load(&large.pageref) == MAP_FAILED) {
                        atomic_store(&large.pageref, NULL);
                        atomic_flag_clear(&large_heap_locked);
                        return NULL;
                    }
                    atomic_store(&large.level, 0);
                }
                // alloc from page is not atomic
                size_t level = atomic_load(&large.level);
                atomic_fetch_add(&large.level, sizeof(struct malloc_header) + size);
                ret = (struct malloc_header*)((char*)(atomic_load(&large.pageref)) + level);
                atomic_store(&ret->pageref, atomic_load(&large.pageref));
                ret->size = size;
                atomic_init(&ret->next, NULL);
            }
            atomic_flag_clear(&large_heap_locked);
            assert(ret->size >= size);
            return ret + 1;
        }
    }
    // HugePage needed completely
    // can we design the following branch lockfree?
    else {
        struct malloc_header* restrict ret;

        size = ALIGN_SIZE(size + sizeof(struct malloc_header), ALLOC_SIZE);
        size_t page_index = indexCatch(size);
        
        // acquire lock on page_list
        while (atomic_flag_test_and_set(&page_list_lock))
            ;
        // check for empty list
        if (atomic_load(&page_list[page_index].pos) == 0) {
		    atomic_flag_clear(&page_list_lock);
			ret = ALLOC(size);
        }
        else {
            ret = (struct malloc_header * restrict) page_list[page_index].pages[--page_list[page_index].pos];
            // release lock
	        atomic_flag_clear(&page_list_lock);
        }

        if (ret == MAP_FAILED)
            return NULL;
        else {
            ret->size = size;
            atomic_store(&ret->pageref, (struct malloc_page *)ret);
            ret->next = NULL;
            return ret + 1;
        }
    }
}

// this is only an alias for malloc, because mmapped pages are 0 by default an
// our free allways sets memory to zero
inline void* calloc(size_t nmemb, size_t size) { 
	void *tmp = malloc(nmemb * size);
	memset(tmp,0,size);
	return tmp;
}

void* realloc(void* restrict ptr, size_t size)
{
    if (size == 0) {
        free(ptr);
        return NULL;
    }
    if (ptr == NULL)
        return malloc(size);

    // check wether we have enough space from padding and if new size i reasonable
    // small for a reallocation
    if (size > (((struct malloc_header*)ptr - 1)->size >> 1) && size < ((struct malloc_header*)ptr - 1)->size)
        return ptr;

    void* restrict tmp = malloc(size);

    if (tmp == NULL)
        return NULL;

    memcpy(tmp, ptr, size);
    return tmp;
}

void free(void* restrict ptr)
{
    if (ptr == NULL)
        return;

    size_t size = (((struct malloc_header*)ptr) - 1)->size;

    // set used memory chunk to zero
    

    // set used amount of page to size of header
    //((struct malloc_page*)ptr)->level = sizeof(struct malloc_page);

    if (size <= MAX_SMALL) {
        void* tmp;
        // set next element only once the other times our exchange does it for us
        (((struct malloc_header*)ptr) - 1)->next = small_free[LOOKUP_8(size)];
        struct malloc_header* next = atomic_load(&(((struct malloc_header*)ptr) - 1)->next);
        do {
            tmp = ptr;
            atomic_store(&(((struct malloc_header*)ptr) - 1)->next, next);
            // try to set new free element beginning of list in loop
        } while (!atomic_compare_exchange_weak(&small_free[LOOKUP_8(size)], &next,
            ((struct malloc_header*)tmp) - 1));
    } else if (size <= MAX_MEDIUM) {
        void* tmp;
        // set next element only once the other times our exchange does it for us
        (((struct malloc_header*)ptr) - 1)->next = small_free[LOOKUP_16(size)];
        struct malloc_header* next = atomic_load(&(((struct malloc_header*)ptr) - 1)->next);
        do {
            tmp = ptr;
            atomic_store(&(((struct malloc_header*)ptr) - 1)->next, next);
            // try to set new free element beginning of list in loop
        } while (!atomic_compare_exchange_weak(&small_free[LOOKUP_16(size)], &next,
            ((struct malloc_header*)tmp) - 1));
    } else if (size <= MAX_LARGE) {
        void* tmp;
        // set next element only once the other times our exchange does it for us
        (((struct malloc_header*)ptr) - 1)->next = small_free[LOOKUP_32(size)];
        struct malloc_header* next = atomic_load(&(((struct malloc_header*)ptr) - 1)->next);
        do {
            tmp = ptr;
            atomic_store(&(((struct malloc_header*)ptr) - 1)->next, next);
            // try to set new free element beginning of list in loop
        } while (!atomic_compare_exchange_weak(&small_free[LOOKUP_32(size)], &next,
            ((struct malloc_header*)tmp) - 1));
    }
    // once again can we make this lockfree?
    else {
        size_t pos = indexCatch(size);

        // acquire lock
        while (atomic_flag_test_and_set(&page_list_lock))
            ;
		//this leads to virtual addressspace fragmentation
        if (page_list[pos].pos == PAGELIST_MAX) {
            munmap((((struct malloc_header*)ptr) - 1),size);
        }
        // else just append to existing list
        else{
            page_list[pos].pages[page_list[pos].pos++] = atomic_load(&(((struct malloc_header*)ptr) - 1)->pageref);
        	FREE(ptr - 1, (((struct malloc_header*)ptr) - 1)->size);
		}
		atomic_flag_clear(&page_list_lock);
    }
}
