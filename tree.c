#include "tree.h"

//functions on a node
void attachNode( struct memsys *memsys, int *node_ptr,void *src, unsigned int width ){
	struct Node new;
	int nodeAddr = memmalloc(memsys, sizeof(struct Node));
	new.data = memmalloc(memsys, width);
	if((nodeAddr==MEMNULL)||(new.data==MEMNULL)){
		fprintf(stderr, "ERROR: Memmalloc failed\n");
		exit(0);
	}
	setval(memsys, src, width, new.data);
	new.gte = MEMNULL;
	new.lt = MEMNULL;
	setval(memsys, &new, sizeof(struct Node),nodeAddr );
	*node_ptr = nodeAddr;
}

void attachChild( struct memsys *memsys, int *node_ptr,void *src, unsigned int width, int direction ){
	
	struct Node node;
	getval(memsys, &node, sizeof(struct Node), *node_ptr);
	if(direction<0){
		attachNode(memsys, &(node.lt), src, width);
	}
	else{
		attachNode(memsys, &(node.gte), src, width);
	}
	setval(memsys, &node, sizeof(struct Node), *node_ptr);

}

int comparNode( struct memsys *memsys, int *node_ptr,int (*compar)(const void *, const void *), void *target,unsigned int width ){
	struct Node node;
	void* data = malloc(width);
	int returnVal;
	getval(memsys, &node, sizeof(struct Node), *node_ptr);
	getval(memsys, data, width, node.data);
	returnVal =  compar(target, data);
	free(data);
	return returnVal;
}

int next( struct memsys *memsys, int *node_ptr, int direction ){
	struct Node node;
	getval(memsys, &node, sizeof(struct Node), *node_ptr);
	if(direction<0){
		return node.lt;
	}
	else{
		return node.gte;
	}
}

void readNode( struct memsys *memsys, int *node_ptr, void *dest, unsigned int width ){
	struct Node node;
	if(*node_ptr==MEMNULL){
		fprintf(stderr, "ERROR: Node pointer points to nothing, the node does not exist\n");
		exit(0);
	}
	getval(memsys, &node, sizeof(struct Node), *node_ptr);
	getval(memsys, dest, width, node.data);
}

void detachNode( struct memsys *memsys, int *node_ptr ){
	struct Node node;
	if(*node_ptr==MEMNULL){
		fprintf(stderr, "ERROR: Node pointer points to nothing, the node does not exist\n");
		exit(0);
	}
	getval(memsys, &node, sizeof(struct Node), *node_ptr);
	memfree(memsys, node.data);
	memfree(memsys, *node_ptr);
	*node_ptr = MEMNULL;
}

void freeNodes( struct memsys *memsys, int *node_ptr ){
	int lt, ge;

	if(*node_ptr!=MEMNULL){
		lt = next(memsys, node_ptr, -1);
		ge = next(memsys, node_ptr, 1);
		freeNodes(memsys, &lt);
		freeNodes(memsys, &ge);
		detachNode(memsys, node_ptr);
	}
}

//functions on tree
struct Tree *newTree( struct memsys *memsys, unsigned int width ){
	struct Tree *new = malloc(sizeof(struct Tree));
	if(new==NULL){
		fprintf(stderr, "ERROR: Malloc failed, no memory allocated\n");
		exit(0);
	}
	new->width = width;
	new->root = MEMNULL;
	return new;
}

void freeTree( struct memsys *memsys, struct Tree *tree ){
	int addr = tree->root;
	if(addr == MEMNULL){
		fprintf(stderr, "ERROR: tree pointer points to NULL, no tree to free");
		exit(0);
	}
	freeNodes(memsys, &addr);
	free(tree);
}

void addItem( struct memsys *memsys, struct Tree *tree, int (*compar)(const void *, const void *),void *src ){
	struct Node currNode;
	int prv, curr; 
	int direction;
	if(tree->root ==MEMNULL){

		attachNode(memsys, &(tree->root), src, tree->width);
	}
	else{
		curr = tree->root;
		//getval(memsys, &currNode, sizeof(struct Node), tree->root);
		while(curr!=MEMNULL){
			getval(memsys, &currNode, sizeof(struct Node), curr);
			direction = comparNode(memsys,&curr,compar,src, tree->width);
			prv = curr;
			curr = next(memsys, &curr, direction);
		}
		attachChild(memsys, &prv, src, tree->width, direction);
	}
}


int searchItem( struct memsys *memsys, struct Tree *tree, int (*compar)(const void *, const void *),void *target ){

	//struct Node currNode;
	int curr; 
	int direction;
	if(tree->root==MEMNULL){
		fprintf(stderr, "The tree pointer points to nothing, there is no tree\n");
		exit(0);
	}
	curr = tree->root;
	while(curr!=MEMNULL){
		//getval(memsys, &currNode, sizeof(struct Node), curr);
		//data = currNode.data;
		direction = comparNode(memsys,&curr,compar,target, tree->width);

		if(direction==0){
			readNode(memsys, &curr, target, tree->width);
			return 1;
		}
		curr = next(memsys, &curr, direction);
	}
	return 0;
}


