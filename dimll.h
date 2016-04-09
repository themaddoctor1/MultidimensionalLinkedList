/**
 * Multidimensional Linked List v 1.0
 *
 * Christopher Hittner (c) 9 April 2016
 */

//DimLL is the Dimensional Linked List.
struct dim_ll;
typedef struct dim_ll* DimLL;

//An LLNode is a Linked List node.
struct ll_node;
typedef struct ll_node* LLnode;

/**
 * Creates an empty Dimensional Linked List.
 */
DimLL makeDimLL();

/**
 * Creates a predefined Dimensional Linked List based
 * on already allocated values.
 *
 * NOTE: Lists made here should not be used outside of the
 *       list's dedicated functions.
 */
DimLL buildDimLL(int layers, LLnode front);

/**
 * Calculates the size of a dimensiomal Linked List.
 */
int sizeOf(DimLL d);

/**
 * Determines whether or not a list is empty. This should
 * be faster than determining whether or not sizeOf(d) == 0.
 */
int isEmpty(DimLL d);

/**
 * Puts the provided value at the end of the list.
 *
 * Returns: Whether or not the list grew in dimensions.
 */
int addVal(DimLL d, void* value);

/**
 * Gets the value stored at the given index.
 */
void* getVal(DimLL d, int idx);

/**
 * Removes the value stored at the given index.
 */
void* remVal(DimLL d, int idx); 


