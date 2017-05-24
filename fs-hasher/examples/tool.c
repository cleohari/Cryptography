#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <sys/types.h>
#include <errno.h>
#include <time.h>

#include "../libhashfile.h"
#include "uthash.h"

#define MAXLINE	4096

struct hashfile_handle *handle;
const struct chunk_info *ci;

struct node{
    uint64_t hash;
    UT_hash_handle hh;
};

struct node *table = NULL;

static void print_chunk_hash(uint64_t chunk_count, const uint8_t *hash,
        int hash_size_in_bytes)
{
    int j;

    printf("Chunk %06"PRIu64 ": ", chunk_count);

    printf("%.2hhx", hash[0]);
    for (j = 1; j < hash_size_in_bytes; j++)
        printf(":%.2hhx", hash[j]);
    printf("\n");
}

int open_hashfile(char* name){
    handle = hashfile_open(name);
    if (!handle) return -1;
    return 1;
}

int main(int argc, char *argv[])
{

    int user = 4;
    int month = 1;
    int day = 22;
    long long t_size = 0;
    long long t_unique = 0;
    while(1){
        char filename[255];
        printf("%02d/%02d\t", month, day);
        
        day++;
        if (day > 31) {
            month++;
            day = 1;
        }
        if (month == 6 && day > 17) return 1;

        int ret = open_hashfile(filename);
        if (ret < 0) continue;
        
        sprintf(filename, "%02d_%d%02d",user,month,day);

        long long size = hashfile_numbytes(handle);
        t_size += size;
        long long unique = 0;

        while(1){
            int ret = hashfile_next_file(handle);
            if(ret == 0) break;

            while(1){
                ci = hashfile_next_chunk(handle);
                if(!ci) break;

                uint64_t* temp_hash = (uint64_t*)(ci->hash);

                struct node* s;

                HASH_FIND_INT(table, temp_hash, s);
                if(s==NULL){
                    unique += ci->size;
                    s = (struct node*)malloc(sizeof(struct node));
                    s->hash = *temp_hash;
                    HASH_ADD_INT(table, hash, s);
                }

            }
        }
        t_unique += unique;

        double rate = (double)(size-unique)/size*100;
        double t_rate = (double)(t_size - t_unique)/t_size*100;
        printf("%lld\t%lld\t%lld\t%lld\t%lf\t%lf\n", size, t_size, unique, t_unique, rate, t_rate);

    }
    return 1;
}
