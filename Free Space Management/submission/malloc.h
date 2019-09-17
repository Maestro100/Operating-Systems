#include<stdio.h>
#include<stddef.h>
#include <sys/mman.h>
#include <string.h>

struct seg{
	size_t size;
	int free;
	struct seg *next;
	struct seg *prev;
};

struct seg *mem_list;
size_t segz;

void initialize();	

void Debug();

void SplitSeg(struct seg *cur, size_t req);

void *my_malloc(size_t sz);

void *my_calloc(size_t sz);

void my_free(void* ptr);

int *my_realloc(void* ptr, size_t sz);

void merge(struct seg *cur); 
