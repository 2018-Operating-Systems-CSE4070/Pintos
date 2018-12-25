#include "vm/frame.h"
#include "threads/synch.h"
#include "threads/malloc.h"

struct list frame_list;
struct lock frame_list_lock;

void frame_list_init()
{
    list_init(&frame_list);
    lock_init(&frame_list_lock);
}

void *alloc_frame(enum palloc_flags flag)
{
    void *paddr = palloc_get_page(flag);
    
    if(paddr != NULL)
    {
        struct frame_entry *fe;
        fe = calloc(1, sizeof(struct frame_entry));
        fe->paddr = paddr;
        fe->tid = thread_current()->tid;

        lock_acquire(&frame_list_lock);
        list_push_back(&frame_list, &fe->elem);
        lock_release(&frame_list_lock);
    }
    else // replacement algorithm
    {
        PANIC("replace failed.");
    }
    return paddr;
}

void free_frame(void *paddr)
{
    struct list_elem *e;

    lock_acquire(&frame_list_lock);
    for (e = list_begin (&frame_list); e != list_end (&frame_list); e = list_next (e))
    {
      struct frame_entry *fe = list_entry (e, struct frame_entry, elem);
      if(fe->paddr == paddr)
      {
          list_remove(e);
          free(fe);
          break;
      }
    }
    lock_release(&frame_list_lock);
    palloc_free_page(paddr);
}