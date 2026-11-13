#include <stdio.h>
#include <stdlib.h>
#include "gen_BST/gen.bst.h"

static int less_int(void* l, void* r){ return *(int*)l < *(int*)r; }
static int collect(void* e, void* c){ int** p=(int**)c; *(*p)++ = *(int*)e; return 0; } /* 0 = continue */
static int fail=0;
#define CHECK(cond, msg) do{ if(!(cond)){ printf("FAIL: %s\n", msg); fail=1; } else printf("ok: %s\n", msg); }while(0)

static size_t count_nodes(BSTree* t){
    int buf[64]; int* w=buf;
    BSTreeForEach(t, BSTREE_TRAVERSAL_INORDER, collect, &w);
    return (size_t)(w-buf);
}
static void inorder_seq(BSTree* t, int* buf, int* n){
    int* w=buf; BSTreeForEach(t, BSTREE_TRAVERSAL_INORDER, collect, &w); *n=(int)(w-buf);
}

int main(void){
    /* ---- empty tree boundaries ---- */
    BSTree* t = BSTreeCreate(less_int);
    CHECK(t!=NULL, "create");
    CHECK(BSTreeItrBegin(t)==BSTreeItrEnd(t), "empty tree: begin==end (no infinite loop)");
    CHECK(BSTreeItrGet(BSTreeItrEnd(t))==NULL, "get(end)==NULL");
    int buf[64]; int* w=buf;
    CHECK(BSTreeForEach(t, BSTREE_TRAVERSAL_INORDER, collect, &w)==BSTreeItrEnd(t) && w==buf, "forEach on empty: 0 actions, returns end");
    CHECK(BSTreeItrRemove(BSTreeItrEnd(t))==BSTreeItrEnd(t), "remove(end) is a safe no-op");
    CHECK(BSTreeItrNext(BSTreeItrEnd(t))==NULL && BSTreeItrPrev(BSTreeItrEnd(t))==NULL, "next/prev(end) safe");
    CHECK(BSTreeForEach(NULL, BSTREE_TRAVERSAL_INORDER, collect, &w)==NULL, "forEach(NULL tree) safe");
    CHECK(BSTreeInsert(NULL, buf)==NULL && BSTreeInsert(t, NULL)==NULL, "insert NULL args safe");

    /* ---- build known tree: {50,30,70,20,40,60,80} ---- */
    int v[7]={50,30,70,20,40,60,80};
    for(int i=0;i<7;i++) BSTreeInsert(t,&v[i]);
    CHECK(count_nodes(t)==7, "7 nodes inserted");
    int seq[8],n; inorder_seq(t,seq,&n);
    int expect[7]={20,30,40,50,60,70,80}; int match=(n==7);
    for(int i=0;i<7&&match;i++) if(seq[i]!=expect[i]) match=0;
    CHECK(match, "in-order traversal sorted");

    /* pre/post order sanity on this exact shape */
    w=buf; BSTreeForEach(t, BSTREE_TRAVERSAL_PREORDER, collect, &w);
    CHECK(buf[0]==50&&buf[1]==30&&buf[2]==20, "pre-order starts 50,30,20");
    w=buf; BSTreeForEach(t, BSTREE_TRAVERSAL_POSTORDER, collect, &w);
    { int post[7]={20,40,30,60,80,70,50}; int pm=1; for(int i=0;i<7&&pm;i++) if(buf[i]!=post[i]) pm=0;
      CHECK(pm, "post-order exact: 20,40,30,60,80,70,50"); }

    /* ---- duplicate insert ---- */
    int dup=30;
    CHECK(BSTreeInsert(t,&dup)==BSTreeItrEnd(t), "duplicate insert returns end (rejected)");
    CHECK(count_nodes(t)==7, "duplicate insert changed nothing");

    /* ---- remove leaf (20) ---- */
    BSTreeItr it=BSTreeItrBegin(t); /* begin == 20 */
    CHECK(*(int*)BSTreeItrGet(it)==20, "begin==20");
    BSTreeItrRemove(it);
    inorder_seq(t,seq,&n); match=(n==6);
    int e2[6]={30,40,50,60,70,80}; for(int i=0;i<6&&match;i++) if(seq[i]!=e2[i]) match=0;
    CHECK(match, "leaf removal keeps order");

    /* ---- remove one-child (60 after removing 70's child setup): remove 20 done; now remove 70 (has only 80 after 60 removed) - first remove 60 (leaf) then 70 one-child ---- */
    it=BSTreeItrBegin(t); while(it!=BSTreeItrEnd(t)&&*(int*)BSTreeItrGet(it)!=60) it=BSTreeItrNext(it);
    BSTreeItrRemove(it); /* 60 leaf */
    it=BSTreeItrBegin(t); while(it!=BSTreeItrEnd(t)&&*(int*)BSTreeItrGet(it)!=70) it=BSTreeItrNext(it);
    BSTreeItrRemove(it); /* 70 now has only right child 80 */
    inorder_seq(t,seq,&n); match=(n==4);
    int e3[4]={30,40,50,80}; for(int i=0;i<4&&match;i++) if(seq[i]!=e3[i]) match=0;
    CHECK(match, "one-child removal keeps order");

    /* ---- remove two-children ROOT (50): successor is 80 ---- */
    it=BSTreeItrBegin(t); while(it!=BSTreeItrEnd(t)&&*(int*)BSTreeItrGet(it)!=50) it=BSTreeItrNext(it);
    BSTreeItr ret=BSTreeItrRemove(it);
    CHECK(ret!=NULL && *(int*)BSTreeItrGet(ret)==80, "two-children root removal returns node holding successor data");
    inorder_seq(t,seq,&n); match=(n==3);
    int e4[3]={30,40,80}; for(int i=0;i<3&&match;i++) if(seq[i]!=e4[i]) match=0;
    CHECK(match, "root removal keeps order (UAF regression)");

    /* ---- remove down to empty, then reuse ---- */
    while(BSTreeItrBegin(t)!=BSTreeItrEnd(t)) BSTreeItrRemove(BSTreeItrBegin(t));
    CHECK(BSTreeItrBegin(t)==BSTreeItrEnd(t), "tree drained to empty");
    int nv=42; BSTreeInsert(t,&nv);
    CHECK(*(int*)BSTreeItrGet(BSTreeItrBegin(t))==42, "tree reusable after draining");

    BSTreeDestroy(&t,NULL);
    CHECK(t==NULL, "destroy sets NULL");
    puts(fail?"BST_TESTS_FAILED":"BST_TESTS_ALL_PASS");
    return fail;
}
