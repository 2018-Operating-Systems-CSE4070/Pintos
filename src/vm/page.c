#include "vm/page.h"
#include <hash.h>
#include <stdbool.h>
#include "filesys/file.h"
#include "vm/swap.h"
#include "vm/frame.h"

unsigned spt_hash_hash (const struct hash_elem *e, void *aux)
{
    struct spt_hash_entry *spte = hash_entry(e, struct spt_hash_entry, elem);
    return hash_bytes(&spte->uaddr, sizeof(void*));
}
bool spt_hash_less (const struct hash_elem *a, const struct hash_elem *b, void *aux)
{
    struct spt_hash_entry *spte_a = hash_entry(a, struct spt_hash_entry, elem);
    struct spt_hash_entry *spte_b = hash_entry(b, struct spt_hash_entry, elem);
    if(spte_a->uaddr < spte_b->uaddr) return true;
    else return false;
}
void spt_hash_remove(const struct hash_elem *e, void *aux)
{
    struct spt_hash_entry *spte = hash_entry(e, struct spt_hash_entry, elem);
    if(spte->type == SWAP) swap_remove(spte->swap_idx);
    free(spte);
}

bool spt_insert_swap(struct hash *h, void* uaddr)
{
    struct spt_hash_entry *spte = calloc(1, sizeof(struct spt_hash_entry));
    if(spte == NULL) return false;

    spte->type = SWAP;
    spte->uaddr = uaddr;
    spte->load = false;

    if(hash_insert(h, &spte->elem) != NULL) return false;
    else return true;
}
bool spt_insert_file(struct hash *h, struct file *f, off_t offset, void* uaddr, uint32_t read_byte, uint32_t zero_byte, bool write)
{
    struct spt_hash_entry *spte = calloc(1, sizeof(struct spt_hash_entry));
    if(spte == NULL) return false;

    spte->type = FILE;
    spte->f = f;
    spte->offset = offset;
    spte->uaddr = uaddr;
    spte->read_byte = read_byte;
    spte->zero_byte = zero_byte;
    spte->write = write;
    spte->load = false;
    
    if(hash_insert(h, &spte->elem) != NULL) return false;
    else return true;
}
struct spt_hash_entry* spt_get_entry(struct hash *h, void* uaddr)
{
    struct spt_hash_entry spte;
    struct hash_elem *e;
    spte.uaddr = uaddr;
    e = hash_find(h, &spte.elem);

    if(e != NULL) return hash_entry(e, struct spt_hash_entry, elem);
    else return NULL;
}
void spt_destroy_hash(struct hash *h)
{
    hash_destroy(h, spt_hash_remove);
}

bool load_page(struct spt_hash_entry *spte)
{
    if(spte->type == FILE) return load_page_file(spte);
    else return load_page_swap(spte);
}
bool load_page_file(struct spt_hash_entry *spte)
{
    file_seek(spte->f, spte->offset);
    uint8_t *kpage = alloc_frame(PAL_USER);
    if(kpage == NULL)
    {
        return false;
    }

    if(file_read (spte->f, kpage, spte->read_byte) != (off_t)spte->read_byte)
    {
        free_frame(kpage);
        return false;
    }
    memset(kpage + spte->read_byte, 0, spte->zero_byte);
    
    if(!pagedir_set_page(thread_current()->pagedir, spte->uaddr, kpage, spte->write))
    {
        free_frame(kpage);
        return false;
    }
    spte->load = true;
    return true;
}
bool load_page_swap(struct spt_hash_entry *spte)
{
    uint8_t *kpage = alloc_frame(PAL_USER);
    if(kpage == NULL) return false;

    if(!pagedir_set_page(thread_current()->pagedir, spte->uaddr, kpage, spte->write))
    {
        free_frame(kpage);
        return false;
    }

    swap_in(spte->swap_idx, spte->uaddr);
    if(spte->type == SWAP) hash_delete(&thread_current()->spt, &spte->elem);
    else spte->load = true;

    return true;
}
