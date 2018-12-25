#ifndef VM_SWAP_H
#define VM_SWAP_H

void swap_bitmap_init();
int swap_out(void* uaddr);
void swap_in(int swap_idx, void* uaddr);
void swap_remove(int swap_idx);

#endif