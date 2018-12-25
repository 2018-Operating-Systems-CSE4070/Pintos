#ifndef VM_FRAME_H
#define VM_FRAME_H

#include "threads/thread.h"
#include "threads/palloc.h"
#include <list.h>

struct frame_entry
{
    void *paddr;
    tid_t tid;
    struct list_elem elem;
};

void frame_list_init();
void *alloc_frame(enum palloc_flags flag);
void free_frame(void *paddr);

#endif
