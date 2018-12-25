#include "vm/swap.h"
#include <bitmap.h>
#include <stdbool.h>
#include "devices/block.h"
#include "threads/vaddr.h"

#define SECTOR_PER_PAGE PGSIZE / BLOCK_SECTOR_SIZE

struct block *swap_blocks;
struct bitmap *swap_bitmap;

void swap_bitmap_init()
{
    swap_blocks = block_get_role(BLOCK_SWAP);
    swap_bitmap = bitmap_create(block_size(swap_blocks) / SECTOR_PER_PAGE);
    bitmap_set_all(swap_bitmap, true);
}

int swap_out(void* uaddr)
{
    int swap_idx = bitmap_scan_and_flip(swap_bitmap, 0, 1, true);

    if(swap_idx == BITMAP_ERROR) return -1;

    int sector_idx = 0;
    while(sector_idx < SECTOR_PER_PAGE)
    {
        block_write(swap_blocks, swap_idx * SECTOR_PER_PAGE + sector_idx, uaddr + sector_idx * BLOCK_SECTOR_SIZE);
        sector_idx++;
    }
    return swap_idx;
}

void swap_in(int swap_idx, void* uaddr)
{
    int sector_idx = 0;
    while(sector_idx < SECTOR_PER_PAGE)
    {
        block_read(swap_blocks, swap_idx * SECTOR_PER_PAGE + sector_idx, uaddr + sector_idx * BLOCK_SECTOR_SIZE);
        sector_idx++;
    }
    bitmap_flip(swap_bitmap, swap_idx);
}

void swap_remove(int swap_idx)
{
    bitmap_flip(swap_bitmap, swap_idx);
}