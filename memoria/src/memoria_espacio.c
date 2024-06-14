#include "memoria_espacio.h"


memory_t* initialize_memory(size_t total_size, size_t page_size) {
    memory_t* mem = malloc(sizeof(memory_t));
    mem->total_size = total_size;
    mem->page_size = page_size;
    mem->num_frames = total_size / page_size;
    mem->memory_space = malloc(total_size);
    mem->frame_usage = bitarray_create(mem->num_frames);
    mem->page_tables = NULL;
    mem->num_processes = 0;
    return mem;
}

void create_process(memory_t* mem, size_t num_pages) {
    mem->page_tables = realloc(mem->page_tables, sizeof(page_table_t*) * (mem->num_processes + 1));
    mem->page_tables[mem->num_processes] = create_page_table(num_pages);
    mem->num_processes++;
}

void finalize_process(memory_t* mem, size_t process_id) {
    if (process_id >= mem->num_processes) return;

    page_table_t* pt = mem->page_tables[process_id];
    for (size_t i = 0; i < pt->num_entries; ++i) {
        if (pt->entries[i].valid) {
            bitarray_clear(mem->frame_usage, pt->entries[i].frame_number);
        }
    }
    destroy_page_table(pt);
    mem->page_tables[process_id] = NULL;
}

void* get_memory_address(memory_t* mem, uint32_t frame_number, size_t offset) {
    return (char*)mem->memory_space + frame_number * mem->page_size + offset;
}

int read_data(memory_t* mem, size_t process_id, uint32_t logical_address, void* buffer, size_t size) {
    if (process_id >= mem->num_processes) return -1;

    page_table_t* pt = mem->page_tables[process_id];
    size_t page_number = logical_address / mem->page_size;
    size_t offset = logical_address % mem->page_size;

    uint32_t frame_number;
    if (get_page_frame(pt, page_number, &frame_number) == -1) {
        return -1;
    }

    void* src = get_memory_address(mem, frame_number, offset);
    memcpy(buffer, src, size);
    return 0;
}

int write_data(memory_t* mem, size_t process_id, uint32_t logical_address, const void* data, size_t size) {
    if (process_id >= mem->num_processes) return -1;

    page_table_t* pt = mem->page_tables[process_id];
    size_t page_number = logical_address / mem->page_size;
    size_t offset = logical_address % mem->page_size;

    uint32_t frame_number;
    if (get_page_frame(pt, page_number, &frame_number) == -1) {
        return -1;
    }

    void* dest = get_memory_address(mem, frame_number, offset);
    memcpy(dest, data, size);
    return 0;
}

int read_page(memory_t* mem, size_t process_id, size_t page_number, void* buffer) {
    if (process_id >= mem->num_processes) return -1;

    page_table_t* pt = mem->page_tables[process_id];
    uint32_t frame_number;
    if (get_page_frame(pt, page_number, &frame_number) == -1) {
        return -1;
    }

    void* src = get_memory_address(mem, frame_number, 0);
    memcpy(buffer, src, mem->page_size);
    return 0;
}

int write_page(memory_t* mem, size_t process_id, size_t page_number, const void* data) {
    if (process_id >= mem->num_processes) return -1;

    page_table_t* pt = mem->page_tables[process_id];
    uint32_t frame_number;
    if (get_page_frame(pt, page_number, &frame_number) == -1) {
        return -1;
    }

    void* dest = get_memory_address(mem, frame_number, 0);
    memcpy(dest, data, mem->page_size);
    return 0;
}

void destroy_memory(memory_t* mem) {
    for (size_t i = 0; i < mem->num_processes; ++i) {
        if (mem->page_tables[i] != NULL) {
            finalize_process(mem, i);
        }
    }
    free(mem->page_tables);
    free(mem->memory_space);
    bitarray_destroy(mem->frame_usage);
    free(mem);
}