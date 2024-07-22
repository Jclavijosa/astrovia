#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdio.h>

#define BUFFER_SIZE 1024

typedef struct {
    FILE *file;
    unsigned char buffer[BUFFER_SIZE];
    int buffer_pos;
    int buffer_len;
    int is_reading;
} FileStream;

int init_read_stream(FileStream *stream, const char *filename);
int init_write_stream(FileStream *stream, const char *filename);
int read_stream(FileStream *stream, unsigned char *data, int len);
int write_stream(FileStream *stream, const unsigned char *data, int len);
void close_stream(FileStream *stream);

#endif
