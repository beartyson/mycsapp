#define  _GNU_SOURCE
#include "cachelab.h"
#include <stdio.h>  /* printf */
#include <getopt.h> /* getopt */
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int opt;
int s = 1;
int E = 1;
int b = 1;
int v = 0;
char* t = NULL;
int miss = 0;
int hit = 0;
int eviction = 0;



typedef struct INFO
{
    int set_number;
    int tag;
    /* data */
}info;

info* get_info(unsigned long address){
    // unsigned int tmp = 0;
    // sscanf(address,"%x",&tmp);
    info* result = (info*)malloc(sizeof(info));
    int zero = ((1<<(b+s))-1);
    result->set_number = (zero & address)>>b;
    result->tag = address >> (b+s);
    printf("set:%d,tag:%d\n",result->set_number ,result->tag);
    return result;
}

typedef struct blocks{
    
    unsigned long tag;
    struct blocks* next;
    struct blocks* prev;
}block;

typedef struct sets{
    int capacity;
    int total;
    block* block_head;
    block* block_tail;
    struct sets* next_set;
}set;

typedef struct caches
{
    /* data */
    set* set_num;
}cache;

cache* inint_cache(){
    cache* cache_head = (cache*)malloc(sizeof(cache));
    
    
    if(cache_head == NULL){
        fprintf(stderr,"can't assign cache space");
        return NULL;
    }
  
 
    set* head = NULL;
    cache_head->set_num = head;
    
    for (size_t i = 0; i < 1<<s; i++)
    {
        /* code */
        set* tmp = (set*)malloc(sizeof(set));
        tmp->block_head = (block*)malloc(sizeof(block));
        tmp->block_tail = (block*)malloc(sizeof(block));
        tmp->block_head->tag = 0;
        tmp->block_tail->tag = 0;
        tmp->block_head->next = tmp->block_tail;
        tmp->block_tail->prev = tmp->block_head;
        tmp->total = 0;
        tmp->capacity = 1<<E;
        tmp->next_set = NULL;
        if(head == NULL){
            cache_head->set_num = tmp;
            head = cache_head->set_num;
        }else{
            head->next_set = tmp;
            head = head->next_set;
        }
        
    }
    return cache_head;
}

void destroy_cache(cache* cache_head){
    if(cache_head != NULL){
        set* head = cache_head->set_num;
        set* head_n = NULL;
        while(head){
            block* block_h = head->block_head;
            block* block_n = NULL;
            while (head->total--)
            {
                /* code */
                block_n = block_h->next;
                free(block_h);
                block_h = block_n;
            }
            head_n = head->next_set;
            free(head);
            head = head_n;
        }
    }
}
void appendTohead(set* start,block* b_s){
    b_s->prev = start->block_head;
    b_s->next = start->block_head->next;
    start->block_head->next->prev = b_s;
    start->block_head->next = b_s;
}

void removeNode(set* start,block* b_s){
    b_s->prev->next = b_s->next;
    b_s->next->prev = b_s->prev;
}
void moveToHead(set* start,block* b_s){
    removeNode(start,b_s);
    appendTohead(start,b_s);

}
void removeTail(set* start){
    removeNode(start,start->block_tail->prev);
}

int is_hit(info* inf,cache* cache_head){

    int set_number = inf->set_number;
    
    int tag = inf->tag;
    set* start = cache_head->set_num;

    while(set_number--){
    
        start = start->next_set;
    }
    block* b_s = start->block_head->next;
    while (b_s != start->block_tail)
    {
        /* code */
        if(b_s->tag == tag){
            moveToHead(start,b_s);
            return 1;
        }
        b_s = b_s->next;
    }
    block* new_block = (block*)malloc(sizeof(block));
    new_block->tag = tag;
    appendTohead(start,new_block);
    start->total += 1;

    if(start->total == start->capacity){
        removeTail(start);
        start->total -= 1;
        return 2;
    }
  
    return 0;
}

int main(int argc, char *argv[])
{
    

    while ((opt = getopt(argc, argv, "-:s:E:b:t:vh"))!= -1)
    {
        switch (opt)
        {
        case 's':
            /* code */
            sscanf(optarg,"%d",&s);
            break;
        case 'E':
            sscanf(optarg,"%d",&E);
            break;
        case 'b':
            sscanf(optarg,"%d",&b);
                break;
        case 'v':
            v = 1;
            break;
        case 'h':
            v = 1; 
            break;      
        case 't':
            t = optarg;
                break;
        case '?':
            printf("Unknown option: %c\n", optopt);
            break;
        case ':':
            printf("Missing arg for %c\n", optopt);
        break;
        case 1:
            printf("Non-option arg: %s\n", optarg);
            break;
        default:
            break;
        }
    }
    
    
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    FILE *fp;
    fp = fopen(t, "r");
    if (fp == NULL){
        fprintf(stderr,"can't open trace file");
        return 1;
    }
    
    char op = 0;
    char* address = (char*)malloc(100);
    int comma = 0;
    int address_l = 0;
    cache* cache_head = inint_cache();
    info* inf = NULL;
    while ((read = getline(&line, &len, fp)) != -1) {
        // printf("Retrieved line of length %zu:\n", read);
        // printf("%s", line);

        if(*line=='I' || read == 1) continue;

        for (size_t i = 0; i < read; i++)
        {
            if(*(line+i) == ','){
                comma = i;
                address_l = i-3;
                break;
            }
            /* code */
        }
        
        op = *(line+1);
        strncpy(address,line+3,address_l);
        char *addressStr = strtok(line + 3, ",");
        char *end;
        unsigned long address = strtoul(addressStr, &end, 16);
        inf = get_info(address);
        switch (is_hit(inf,cache_head))
        {
        case 0:
            /* code */
            miss += 1;
            if(v){
                printf("%c %s,%d miss", op, addressStr , atoi(line+comma+1));
                if(op == 'M'){
                    printf(" hit\n");
                }else{
                    printf("\n");
                }
            }
            
            break;
        case 1:
            hit += 1;
            
            if(v){
                printf("%c %s,%d hit", op, addressStr , atoi(line+comma+1));
                if(op == 'M'){
                    printf(" hit\n");
                }else{
                    printf("\n");
            }
            
            }
            
            break;
        case 2:
            eviction += 1;
            miss += 1;
            
            if(v){
                printf("%c %s,%d miss eviction", op, addressStr , atoi(line+comma+1));
                if(op == 'M'){
                    printf(" hit\n");
                }else{
                    printf("\n");
                }
            }
            
            break;
        default:
            break;
        }
        if(op == 'M'){
            hit += 1;
            }
        
    }

    fclose(fp);
    if (line)
        free(line);
    
    // printf("b is %d, E is %d, s is %d, t is %s\n", b, E, s, t);
    destroy_cache(cache_head);
    printSummary(hit, miss, eviction);
    return 0;
}
