#include "malloc.h"

int main(){
	// printf("here\n");
	// int *p=(int)my_malloc(10*sizeof(int));
	// // printf("here\n");
	// char *q=(char)my_malloc(2*sizeof(char));
	// // printf("here\n");
	// int *r=(int)my_malloc(100*sizeof(int));
	// // printf("here\n");
	// my_free(p);
	// // printf("here\n");
	// char *w=(char)my_malloc(70);
	// // printf("here\n");
	// my_free(r);
	// // printf("here\n");
	// int *k=(int)my_malloc(50*sizeof(int));

	void *p1 = my_malloc((size_t)150);
	// Debug();
	void *p2 = my_malloc((size_t)200);
	void *p3 = my_malloc((size_t)300);
	// Debug();
	// my_free(p2);
	// Debug();
	void *p4 = my_realloc(p1,(size_t)165);
	Debug();
	p4 = my_realloc(p4, (size_t)140);
	Debug();
	// my_free(p1);
	void *p5 = my_malloc((size_t)140);
	Debug();
	printf("Allocation and deallocation is done successfully!\n");
	return 0;
}