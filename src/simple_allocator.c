#include "type.h"  // for size_t

#define HEAP_SIZE 4096  // 4KB 簡易堆積大小

static char simple_heap[HEAP_SIZE];  // 簡易 heap 區域
static size_t heap_offset = 0;       // 指向下一個可用區域

void* simple_malloc(size_t size) {
    // 記憶體對齊，4 或 8 bytes 可調整
    size = (size + 7) & (~0x7);  // 向上對齊至 8 的倍數

    if (heap_offset + size > HEAP_SIZE) {
        return NULL;  // 記憶體不夠，回傳 NULL
    }

    void* ptr = &simple_heap[heap_offset];
    heap_offset += size;
    return ptr;
}
