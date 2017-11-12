#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern long t;

FILE *init_out_file (const char *filename){
	
	//check for correct usage of the function
	if (filename == NULL || strlen(filename) < 1)
	{
		return NULL;
	}

	return fopen(filename, "w");
}


void write_int(FILE *out_file, int val){
	fprintf(out_file, "%ld\t%d\n", t, val );
}

void write_uint(FILE *out_file, unsigned int val){
	fprintf(out_file, "%ld\t%u\n", t, val );
}

void write_long(FILE *out_file, long val){
	fprintf(out_file, "%ld\t%ld\n", t, val );
}
