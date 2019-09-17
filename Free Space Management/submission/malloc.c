#include<stdio.h>
#include<stddef.h>
#include <sys/mman.h>
#include <string.h>
#include "malloc.h"

// struct seg{
// 	size_t size;
// 	int free;
// 	struct seg *next;
// 	struct seg *prev;
// };

// struct seg *mem_list;
// size_t segz;

void initialize(){
	// printf("initialize\n");
	size_t tot = 4096;
	segz = sizeof(struct seg);
	mem_list = mmap( 0, tot, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0 );
	// struct seg *mem_list = mem;
	// printf("Start: %p\n", mem_list);
	mem_list->size = tot - segz;
	mem_list->free = 1;
	mem_list->prev = mem_list-> next = NULL;
}	

void Debug(){
	// freopen("stderr.t")
	// printf("segz: %lu\n", segz);
	if(mem_list == NULL){
		fprintf(stderr, "No memory yet !");
		return;
	}
	struct seg* cur = mem_list;
	int x =0 ;
	while(1){
		if(cur==NULL)break;
		struct seg* strt = cur; ++strt ;
		fprintf(stderr, "Block %d :- Ptr:%p Size:%lu  State: ", x++, strt, cur->size);
		// --cur ; 
		if(cur->free == 1) fprintf(stderr, "Free\n");
		else fprintf(stderr, "Used\n");
		cur = cur-> next;
	}
	fprintf(stderr, "\n\n");
}

void SplitSeg(struct seg *cur, size_t req){
	// printf("split req:%lu \n",req);
	struct seg *new_block = (void*)((void*)cur +  segz + req);
	new_block->free = 1;
	new_block->size = cur->size - (req + segz);
	// printf("new block size: %lu \n", new_block->size);
	struct seg *nbst = new_block; ++nbst;
	// printf("newblock seg start: %p  new block start: %p \n",new_block, nbst );
	new_block->prev = cur;
	new_block->next = cur->next;
	cur->next = new_block;
	cur->free = 0; 
	cur->size = req;
	// printf("%d\n", );
}

void *my_malloc(size_t sz){
	// printf("in malloc\n");
	size_t req = sz;
	if(mem_list == NULL){
		initialize();
	}
	struct seg* cur = mem_list;
	int type = -1;
	while(1){
		if( cur->free == 0){cur = cur->next; continue;}
		if( cur->size == req){type = 0; break;}
		if(cur->size >= req + segz){type = 1; break;}
		if(cur->next == NULL)break;
		cur = cur->next;
		// break;
	}
	// printf("type: %d \n", type);
	if(type == -1){
		return NULL;
		// printf("Memory not available!");
	}
	else if (type == 0){
		cur-> free = 0;
		// cur++;
		return (void*)(++cur);
	}
	else {
		SplitSeg(cur, req);
		// void * nx = ++cur; --cur;
		// --cur;

		// void * nx = ++cur;--cur;
		// printf("A> %p B> %p \n",cur, nx);
		return (void *)(++cur);
	}
}

void *my_calloc(size_t sz){
	void *ptr = my_malloc(sz);
	if(!ptr) return NULL;
	memset(ptr , 0, sz);
	return ptr;
}

void my_free(void* ptr);
int *my_realloc(void* ptr, size_t sz){
	struct seg *ptr1 = (struct seg*)ptr;
	--ptr1;
	struct seg *cur = mem_list;
	while(cur!=NULL && cur!=ptr1){
		cur = cur->next;
	}
	if(cur != ptr1)return NULL;

	if(cur->size == sz)return ptr;

	void *newm = my_malloc(sz);

	size_t tocopy =  cur->size<sz ? cur->size : sz;
	if(newm){
		memcpy(newm, ptr, tocopy);
		my_free(ptr);
		return newm;
	}
	if(cur->size < sz){
		if(cur->size > sz + segz){
			SplitSeg(cur, sz);
			return (void *)(++cur);
		}
		else{
			return ptr;
		}
	}
	else{
		return NULL;
	}
}	

void merge(struct seg *cur){
	if(cur->prev!=NULL){
		struct seg *prev = cur->prev;
		if(prev->free == 1){
			prev->size += cur->size + segz;
			prev->next = cur->next;
			cur = prev;
		}
	}
	if(cur->next != NULL){
		struct seg *nxt = cur->next;
		if(nxt->free == 1){
			cur->size += nxt->size + segz;
			cur->next = cur->next->next;
		}
	}
}

void my_free(void* ptr){
	struct seg* cur = mem_list;
	if(ptr< (void*)mem_list || ptr >= (void *)(mem_list + 4096-segz)) {
		printf("Invalid pointer1!\n");
		return;
	}
	int okay = 0;
	struct seg* ptr1 = (struct seg *) ptr ;
	ptr1--;
	while(cur != ptr1){
		// printf("Cur:%p , ptr:%p \n", cur, ptr1);
		if(cur->next ==NULL)break;
		cur = cur->next;
	}
	if(cur != ptr1){
		printf("Invalid pointer2!\n");
		return;
	}
	// --cur;
	cur->free = 1;
	merge(cur);
}

// int main(){
// 	// printf("here\n");
// 	// int *p=(int)my_malloc(10*sizeof(int));
// 	// // printf("here\n");
// 	// char *q=(char)my_malloc(2*sizeof(char));
// 	// // printf("here\n");
// 	// int *r=(int)my_malloc(100*sizeof(int));
// 	// // printf("here\n");
// 	// my_free(p);
// 	// // printf("here\n");
// 	// char *w=(char)my_malloc(70);
// 	// // printf("here\n");
// 	// my_free(r);
// 	// // printf("here\n");
// 	// int *k=(int)my_malloc(50*sizeof(int));

// 	void *p1 = my_malloc((size_t)150);
// 	// Debug();
// 	void *p2 = my_malloc((size_t)200);
// 	void *p3 = my_malloc((size_t)300);
// 	// Debug();
// 	// my_free(p2);
// 	// Debug();
// 	void *p4 = my_realloc(p1,(size_t)165);
// 	Debug();
// 	p4 = my_realloc(p4, (size_t)140);
// 	Debug();
// 	// my_free(p1);
// 	void *p5 = my_malloc((size_t)140);
// 	Debug();
// 	printf("Allocation and deallocation is done successfully!\n");
// 	return 0;
// }