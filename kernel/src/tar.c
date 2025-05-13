//
// Created by kokolor on 13/05/25.
//

#include "string.h"
#include "tar.h"

uint8_t* g_tar_base;
size_t g_tar_entries_num;

uint32_t octal_to_uint32(const char* input)
{
    uint32_t result = 0;

    for (size_t i = 0; i < 11; i++)
    {
        result = (result << 3) + (input[i] - '0');
    }

    return result;
}

int tar_find_file(const char* filename, struct tar_file* out)
{
    uint8_t* base = g_tar_base;

    for (size_t i = 0; i < g_tar_entries_num; i++)
    {
        const struct tar_header* header = (const struct tar_header*)base;
        const uint32_t size = octal_to_uint32(header->size);

        if (strncmp(header->filename, filename, 100) == 0)
        {
            out->data = base + 512;
            out->size = size;

            return 0;
        }

        base += ((size + 511) / 512 + 1) * 512;
    }

    return -1;
}

int tar_open(const char* filename, struct tar_handle* handle)
{
    struct tar_file file;

    if (tar_find_file(filename, &file) != 0)
    {
        return -1;
    }

    handle->data = file.data;
    handle->size = file.size;
    handle->offset = 0;

    return 0;
}

size_t tar_read(struct tar_handle* handle, void* buf, const size_t count)
{
    const size_t remaining = handle->size - handle->offset;
    const size_t to_read = count < remaining ? count : remaining;

    memcpy(buf, (uint8_t*)handle->data + handle->offset, to_read);
    handle->offset += to_read;

    return to_read;
}


void tar_init(uint8_t* base)
{
    g_tar_base = base;
    const uint8_t* address = g_tar_base;
    g_tar_entries_num = 0;

    while (1)
    {
        const struct tar_header* header = (const struct tar_header*)address;

        if (header->filename[0] == '\0')
        {
            break;
        }

        const uint32_t size = octal_to_uint32(header->size);
        address += ((size + 511) / 512 + 1) * 512;
        g_tar_entries_num++;
    }
}
