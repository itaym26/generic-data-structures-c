#include <stdio.h>
#include <stdlib.h>
#include "gen_hash/gen_hash.h"

static size_t hash_int(const void* k){ return (size_t)(*(const int*)k); }   /* const-correct per new contract */
static int eq_int(const void* a, const void* b){ return *(const int*)a == *(const int*)b; }
static int kv_count(const void* k, void* v, void* c){ (void)k;(void)v; (*(int*)c)++; return 1; }
static int key_frees=0, val_frees=0;
static void kfree_cnt(void* k){ (void)k; key_frees++; }
static void vfree_cnt(void* v){ (void)v; val_frees++; }
static int fail=0;
#define CHECK(cond, msg) do{ if(!(cond)){ printf("FAIL: %s\n", msg); fail=1; } else printf("ok: %s\n", msg); }while(0)

int main(void){
    HashMap* m = HashMap_Create(8, hash_int, eq_int);
    CHECK(m!=NULL, "create");
    int keys[40], vals[40];
    for(int i=0;i<40;i++){ keys[i]=i; vals[i]=i*100; }

    CHECK(HashMap_Insert(NULL,&keys[0],&vals[0])==MAP_UNINITIALIZED_ERROR, "insert NULL map");
    CHECK(HashMap_Insert(m,NULL,&vals[0])==MAP_KEY_NULL_ERROR, "insert NULL key");
    for(int i=0;i<40;i++) CHECK(HashMap_Insert(m,&keys[i],&vals[i])==MAP_SUCCESS, "insert");
    CHECK(HashMap_Size(m)==40, "size 40");
    CHECK(HashMap_Insert(m,&keys[5],&vals[5])==MAP_KEY_DUPLICATE_ERROR, "duplicate key rejected");
    CHECK(HashMap_Size(m)==40, "size unchanged after duplicate");

    void* out=NULL;
    CHECK(HashMap_Find(m,&keys[17],&out)==MAP_SUCCESS && *(int*)out==1700, "find existing");
    int absent=999;
    CHECK(HashMap_Find(m,&absent,&out)==MAP_KEY_NOT_FOUND_ERROR, "find missing");
    CHECK(HashMap_Find(m,NULL,&out)==MAP_KEY_NULL_ERROR, "find NULL key");

    int cnt=0; CHECK(HashMap_ForEach(m,kv_count,&cnt)==40 && cnt==40, "forEach visits all");

    Map_Stats s = HashMap_GetStatistics(m);
    printf("   stats: buckets=%zu chains=%zu max=%zu avg=%zu\n", s.numberOfBuckets, s.numberOfChains, s.maxChainLength, s.averageChainLength);
    CHECK(s.numberOfBuckets>0 && s.numberOfChains>0 && s.maxChainLength>=s.averageChainLength, "statistics sane");

    CHECK(HashMap_Rehash(m, 200)==MAP_SUCCESS, "rehash");
    CHECK(HashMap_Size(m)==40, "size kept after rehash");
    int allfound=1; for(int i=0;i<40&&allfound;i++){ if(HashMap_Find(m,&keys[i],&out)!=MAP_SUCCESS || *(int*)out!=i*100) allfound=0; }
    CHECK(allfound, "all 40 pairs intact after rehash");
    Map_Stats s2 = HashMap_GetStatistics(m);
    CHECK(s2.numberOfBuckets>s.numberOfBuckets, "capacity grew after rehash");
    CHECK(HashMap_Rehash(NULL,10)==MAP_UNINITIALIZED_ERROR, "rehash NULL map");

    void *rk=NULL,*rv=NULL;
    CHECK(HashMap_Remove(m,&keys[0],&rk,&rv)==MAP_SUCCESS && *(int*)rk==0 && *(int*)rv==0, "remove returns key+value");
    CHECK(HashMap_Remove(m,&keys[0],&rk,&rv)==MAP_KEY_NOT_FOUND_ERROR, "second remove not found");
    CHECK(HashMap_Size(m)==39, "size 39 after remove");

    HashMap_Destroy(&m, kfree_cnt, vfree_cnt);
    CHECK(m==NULL, "destroy sets NULL");
    CHECK(key_frees==39 && val_frees==39, "destroy callbacks ran per remaining item");

    puts(fail?"HASH_TESTS_FAILED":"HASH_TESTS_ALL_PASS");
    return fail;
}
