#include <stdlib.h>
#include <string.h>  
#include <stdio.h>
#include "memsys.h"

struct Node {
  int data;
  int lt;
  int gte;
};

struct Tree {
  unsigned int width;
	int root; 
};

//functions on a node
void attachNode( struct memsys *memsys, int *node_ptr,void *src, unsigned int width );
void attachChild( struct memsys *memsys, int *node_ptr,void *src, unsigned int width, int direction );
int comparNode( struct memsys *memsys, int *node_ptr,int (*compar)(const void *, const void *), void *target,unsigned int width );
int next( struct memsys *memsys, int *node_ptr, int direction );
void readNode( struct memsys *memsys, int *node_ptr, void *dest, unsigned int width );
void detachNode( struct memsys *memsys, int *node_ptr );
void freeNodes( struct memsys *memsys, int *node_ptr );

//functions on tree
struct Tree *newTree( struct memsys *memsys, unsigned int width );
void freeTree( struct memsys *memsys, struct Tree *tree );
void addItem( struct memsys *memsys, struct Tree *tree, int (*compar)(const void *, const void *),void *src );

//the last 20%
int searchItem( struct memsys *memsys, struct Tree *tree, int (*compar)(const void *, const void *),void *target );