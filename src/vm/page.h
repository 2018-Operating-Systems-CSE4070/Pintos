#ifndef VM_PAGE_H
#define VM_PAGE_H
#include <hash.h>
#include "filesys/filesys.h"

#define FILE 0
#define SWAP 1
#define FILE_SWAP 2

struct spt_hash_entry
{
    void *uaddr;
    int type;
    bool load;

    struct file *f;
    off_t offset;
    uint32_t read_byte;
    uint32_t zero_byte;
    bool write;

    int swap_idx;
    bool swap_write;

    struct hash_elem elem;
};

unsigned spt_hash_hash (const struct hash_elem *, void *);
bool spt_hash_less (const struct hash_elem *, const struct hash_elem *, void *);
void spt_hash_remove(const struct hash_elem *, void *);

bool spt_insert_swap(struct hash *h, void* uaddr);
bool spt_insert_file(struct hash *h, struct file *f, off_t offset, void* uaddr, uint32_t read_byte, uint32_t zero_byte, bool write);
struct spt_hash_entry* spt_get_entry(struct hash *h, void* uaddr);
void spt_destroy_hash(struct hash *h);

bool load_page(struct spt_hash_entry *spte);
bool load_page_file(struct spt_hash_entry *spte);
bool load_page_swap(struct spt_hash_entry *spte);

#endif
