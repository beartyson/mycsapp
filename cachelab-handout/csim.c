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

info* get_info(char* address){
    unsigned int tmp = 0;
    sscanf(address,"%x",&tmp);
    printf("%x\n",tmp);
    info* result = (info*)malloc(sizeof(info));
    int zero = ((1<<(b+s)) - (1<<b));
    result->set_number = (zero & tmp)>>b;
    result->tag = tmp >> (b+s);
    printf("set:%d,tag:%d\n",result->set_number ,result->tag);
    return result;
}

typedef struct blocks{
    
    unsigned long tag;
    struct blocks* next;
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
    cache_head->set_num = NULL;
 
    set* head = cache_head->set_num;
    for (size_t i = 0; i < 1<<s; i++)
    {
        /* code */
        set* tmp = (set*)malloc(sizeof(set));
        tmp->block_head = NULL;
        tmp->block_tail = NULL;
        tmp->total = 0;
        tmp->capacity = 1>>E;
        tmp->next_set = NULL;
        if(head == NULL){
            head = tmp;
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
            while (block_h != NULL)
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


int is_hit(info* inf,cache* cache_head){
    
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

        if(*line=='I') continue;

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
        printf("op is %c, size is %d, address is %s\n", op, atoi(line+comma+1), address);
        inf = get_info(address);
        switch (is_hit(inf,cache_head))
        {
        case 0:
            /* code */
            m
            if(v){
                printf("%s miss");
            }
            if(op == 'M'){
                printf(" hit\n");
            }else{
                printf("\n");
            }
            break;
        case 1:
            break;
        case 2:
            break;
        default:
            break;
        }
        
        
    }

    fclose(fp);
    if (line)
        free(line);
    
    // printf("b is %d, E is %d, s is %d, t is %s\n", b, E, s, t);
    
    // printSummary(0, 0, 0);
    return 0;
}
