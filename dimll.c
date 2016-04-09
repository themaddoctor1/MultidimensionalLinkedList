/**
 * Multidimensional Linked List v 1.0
 *
 * Christopher Hittner (c) 9 April 2016
 */

#include <stdlib.h>
#include <stdio.h>
#include "dimll.h"

struct dim_ll {
    LLnode front; //The first node
    int layer;    //The layer this is on.
};

struct ll_node {
    void* val;    //The value of the node
    LLnode next;  //The next node
};

DimLL makeDimLL() {
    //Claim memory.
    DimLL list = (DimLL) malloc(sizeof(struct dim_ll));

    //Empty list is layer zero w/ no front node.
    list->front = NULL;
    list->layer = 0;
    
    return list;
}

DimLL buildDimLL(int layers, LLnode front) {
    //Claim memory.
    DimLL list = (DimLL) malloc(sizeof(struct dim_ll));

    //Puts the given values in their places.
    list->front = front;
    list->layer = layers;

    return list;
}

LLnode makeLLnode() {
    LLnode node = (LLnode) malloc(sizeof(struct ll_node));

    node->val = NULL;
    node->next = NULL;

    return node;
    
}

int sizeOf(DimLL d) {
    LLnode l = d->front;
    if(d->front == NULL)
	return 0;

    //Will hold the value one less than this layer's size
    //until the layer number is taken into account. The last item
    //is excluded because that item needs to be counted
    //separately, since it is the only list than can be
    //not full.
    int size = 1;
    while(l->next != NULL) {
	l = l->next;
	size++;
    }

    int i;
    if(d->layer == 0)
	return 1 + size;
    else
	return (size << 2 * d->layer) + sizeOf((DimLL) l->val);
	
}

/**
 * Returns the length of the Linked List on this layer.
 */
int length(DimLL d) {
    LLnode curr = d->front;
    int len = 0;

    while(curr != NULL) {
	curr = curr->next;
	len++;
    }

    return len;
}

int isEmpty(DimLL d) {
    //If the front is NULL, there is nothing.
    return d->front == NULL;
}


/**
 * Tries to push the value onto the list, and returns the
 * overflow from the addition.
 */
void* pushVal(DimLL d, void* value) {
    
    //Holds the layer of the given DimLL
    int layer = d->layer;
    
    //The node currently in question.
    LLnode curr = d->front;
    if(curr == NULL) {
	d->front = makeLLnode();
	if(layer == 0)
	    d->front->val = value;
	else {
	    DimLL sublist = buildDimLL(layer-1, NULL);
	    d->front->val = sublist;
	    pushVal(sublist, value);
	}
	
	return NULL;
	
    }
    
    //Holds the index where the item is being inserted.
    int idx = 0;
    while(curr->next != NULL) {
	curr = curr->next;
	idx++;
    }

    //If at the bottom layer, insert the item or report overflow.
    if(layer == 0) {
	if(idx == 3)
	    return value;
	else {
	    curr ->next = makeLLnode();
	    curr = curr->next;
	    curr->val = value;
	    return NULL;
	}
    }
    
    //A new list should be inserted.
    void* overflow = pushVal((DimLL) curr->val, value);
    if(overflow != NULL) { //There was overflow
	if(idx == 3) {//No more list can be added; return the overflow
	    return overflow;
	}
	
	//Builds the next value in the list
	curr->next = makeLLnode();
	DimLL sublist = buildDimLL(layer-1, NULL);
	curr->next->val = sublist;
	overflow = pushVal(sublist, value);
	
        
    }

    return NULL;
    
}

int addVal(DimLL d, void* value) {
    
    void* overflow = pushVal(d, value);
    if(overflow != NULL) {
	DimLL sublist = buildDimLL(d->layer, d->front);
	d->layer += 1;
	d->front = makeLLnode();
	d->front->val = sublist;
	pushVal(d, value);
	return 1;
    }
    return 0;
}

void* getVal(DimLL d, int idx) {
    
    if(d->layer == 0) {
	//Iterates to get the right value
	int i;
	LLnode curr = d->front;
	for(i = 0; i < idx; i++)
	    curr = curr->next;
	
	return curr->val;
    }

    //Find which list to add to.
    int index = (idx & (3 << (2 * d->layer))) >> 2 * d->layer;

    int i;
    LLnode curr = d->front;
    for(i = 0; i < index; i++)
	curr = curr->next;

    int subIdx = idx - (index << (2 * d->layer));
    
    return getVal((DimLL) curr->val, subIdx);
    
}

void* remVal(DimLL d, int idx) {
    if(d->layer == 0) {
	//Will hold the removed item.
	void* rem;

	if(idx == 0) {
	    LLnode node = d->front;
	    rem = node->val;
	    d->front = node->next;

	    //Free the node
	    node->val = NULL;
	    node->next = NULL;
	    free(node);

	    return rem;
	    
	}
	int i;
	LLnode curr = d->front; //Will hold the value before the target
	for(i = 0; i < idx-1; i++)
	    curr = curr->next;

	LLnode node = curr->next;
	rem = node->val;
	curr->next = node->next;

	//Free the node
	node->val = NULL;
	node->next = NULL;
	free(node);

	return rem;
    } else {

	int targ = (idx & (3 << (2 * d->layer))) >> (2 * d->layer);

	//First, remove the back item. If that list is empty, remove it.
	int len = length(d);
	LLnode curr;
	void* push = NULL; //The thing to be added next.
	
	int i;
        for(i = len; i > targ+1; i--) {
	    curr = d->front;
	    int j;
	    for(j = 0; j < i-1; j++)
		curr = curr->next;
	    if(i == len-1 && isEmpty((DimLL) curr->next->val)) {
		//The last list in the list needs to be destroyed.
	        DimLL del = (DimLL) curr->next->val;
		del->layer = 0;
		free(del);
		curr->next->val = NULL;
		free(curr->next);
		len--;
	    }

	    //Pulls an item from the latest list to be moved forward.
	    void* pull = remVal((DimLL) curr->val, 0);

	    //Tests the basis case.
	    if(push != NULL)
		addVal((DimLL) curr->val, push);
	    
	    push = pull;
	}

	curr = d->front;
	for(i = 0; i < targ; i++)
	    curr = curr->next;

	void* result = remVal((DimLL) curr->val, idx - (targ << (2 * d->layer)));
	addVal((DimLL) curr->val, push);

	return result;
	
    }

    
}

/**
 * A test that ensures that the program works.
 */
void workingTest() {
    //The sample list
    DimLL list = makeDimLL();

    //val is a pointer that will be added to list.
    char* val = "abc";

    //The length of the list in testing
    int len = 260;

    //Creates a test list and assigns values.
    char** test[len];
    int i;
    for(i = 0; i < len; i++)
	test[i] = (void*) (val + i%16);

    //Adds the values
    for(i = 0; i < len; i++)
	addVal(list, (void*) (val + i%16));

    //Outputs them to be sure that they were put in correctly.
    for(i = 0; i < len; i++) {
	printf("Index %i:\n", i);
	printf("%p\n", getVal(list, i));
    }
    
    
}

int main() {
    workingTest();
}
