//
// Created by kokolor on 13/05/25.
//

#ifndef TAR_H
#define TAR_H

#include <stdint.h>

struct tar_header
{
    char filename[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char checksum[8];
    char type_flag[1];
};

struct tar_file
{
    void* data;
    size_t size;
};

struct tar_handle
{
    void* data;
    size_t size;
    size_t offset;
};

uint32_t octal_to_uint32(const char* input);
int tar_find_file(const char* filename, struct tar_file* out);
int tar_open(const char* filename, struct tar_handle* handle);
size_t tar_read(struct tar_handle* handle, void* buf, const size_t count);
void tar_init(uint8_t* base);

#endif //TAR_H
