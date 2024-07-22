#include "file_io.h"
#include <string.h>

int init_read_stream(FileStream *stream, const char *filename) {
    stream->file = fopen(filename, "rb");
    if (!stream->file) {
        perror("Error al abrir el archivo para lectura");
        return 0;
    }
    stream->buffer_pos = 0;
    stream->buffer_len = 0;
    stream->is_reading = 1;
    return 1;
}

int init_write_stream(FileStream *stream, const char *filename) {
    stream->file = fopen(filename, "wb");
    if (!stream->file) {
        perror("Error al abrir el archivo para escritura");
        return 0;
    }
    stream->buffer_pos = 0;
    stream->is_reading = 0;
    return 1;
}

int read_stream(FileStream *stream, unsigned char *data, int len) {
    int total_read = 0;
    while (len > 0) {
        if (stream->buffer_pos >= stream->buffer_len) {
            stream->buffer_len = fread(stream->buffer, 1, BUFFER_SIZE, stream->file);
            stream->buffer_pos = 0;
            if (stream->buffer_len == 0) break;
        }
        int available = stream->buffer_len - stream->buffer_pos;
        int to_copy = (len < available) ? len : available;
        memcpy(data + total_read, stream->buffer + stream->buffer_pos, to_copy);
        stream->buffer_pos += to_copy;
        total_read += to_copy;
        len -= to_copy;
    }
    return total_read;
}

int write_stream(FileStream *stream, const unsigned char *data, int len) {
    return fwrite(data, 1, len, stream->file);
}

void close_stream(FileStream *stream) {
    if (stream->file) {
        fclose(stream->file);
        stream->file = NULL;
    }
}
