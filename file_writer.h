#ifndef FILE_WRITER_H
#define FILE_WRITER_H

#include <stdio.h>

void write_int (FILE *out_file, int val);
void write_uint (FILE *out_file, unsigned int val);
void write_long (FILE *out_file, long val);
FILE *init_out_file (const char* filename);

#endif