#ifndef ALTERSCRIPT_H
#define ALTERSCRIPT_H

struct List {
  char *name;
  char *command;
  struct List *next, *prev;
};

/* List Services - List.c */
extern void ListMoveNext(struct List **hist, int i);
extern void ListMovePrev(struct List **hist, int i);
extern void ListMoveToHead(struct List **hist);
extern void ListMoveToTail(struct List **hist);
extern int ListFindIndexOf(struct List **hist, char *pszValue, int val);
extern BOOL ListMoveToEntry(struct List **hist, char *pszValue, int val);
extern struct List *ListAdd(struct List **hist, char *pszValue1, char *pszValue2, int *count);
extern void ListFree(struct List **hist);
extern void ListRemoveEntry(struct List **hist, int *count);
extern struct List *ListRemoveAll(struct List **hist, int *count);

#endif