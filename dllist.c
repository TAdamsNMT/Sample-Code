/**
 * @file dllist.c
 * @author Thomas Adams
 * @date Nov 14 2016
 * 
 * Assignment : lab 9
 * 
 * @brief contains functions and structures to implement a doubly linked list
 * of characters
 * 
 * @details the implemented doubly linked list contains chars as node values.
 * functions implemented include: creation of a node, printing a node (both 
 * with and without previous and next addresses), printing an entire list, 
 * inserting at the head and tail of lists, inserting before and after
 * particular nodes, counting the elements of a list, finding elements, 
 * finding the tail of a list, deleting elements, and deleting a whole list.
 * 
 * @bugs None
 *
 * @todo None
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include "dllist.h"



/**
 * Creates a node of a doubly linked list containing the given char value
 * @param n the character to store in the newly created node
 * @return a pointer to the newly created node
 */
struct bit_t *create_node(char c)
{
        struct bit_t *node = malloc( sizeof(struct bit_t) );

        /* check for malloc failure */
        if(!node)
                exit(ENOMEM);

        /* setup the values in the node */
        node->n = c;
        node->next = NULL;
        node->prev = NULL;
        return node;
}

/**
 * Prints the given node's stored value
 * @param node a pointer to the node to print
 */
void print_node(struct bit_t *node)
{
        /* Check for a NULL pointer just in case */
        if(node == NULL){
                printf("NULL");
                return;
        }

        printf("address = %p, ", node);
        printf("value = %c, ", node->n);
        printf("next = %p", node->next);
        printf("prev = %p", node->prev);
}

/**
 * Prints the addresses, stored values, and stored pointers in all elements of
 * the given doubly linked list
 * @param head the head of the list to print
 */
void print_verbose(struct bit_t *head)
{
        struct bit_t *cur = head;/* placeholder for walking down the list */

        /* Check for a NULL pointer just in case */
        if(head == NULL){ 
                printf("NULL\n");
                return;
        }

        /* walk down the list and print each node */
        while(cur != NULL){
                print_node(cur);
                printf("\n");
                cur = cur->next;
        }
}

/**
 * Prints the stored values of the given doubly linked list, starting with 
 * the head.
 * @param list a node of the doubly linked list to print
 */
void print_list(struct bit_t *list)
{
        /* Check for a NULL pointer just in case */
        if(list == NULL){ 
                printf("NULL\n");
                return;
        }

        /* walk back to the head of the list to print from the start */
        while(list->prev != NULL)
                list = list->prev;

        /* walk down the list and print each value */
        while(list->next != NULL){
                printf("%c", list->n + '0');
                list = list->next;
        }
        /* print the last value with \n instead of , */
        printf("%c\n", list->n + '0');
}

/**
 * inserts a given node at the head of a given list.  Then, returns the head
 * of the new list.
 * @param list a member of the list that is being inserted into.  Works best
 * if it is the head.
 * @param node the node to insert into the list
 * @return the new head of the list after insertion
 */
struct bit_t *insert_head(struct bit_t *list, struct bit_t *node)
{
        struct bit_t *head = list;

        if(head == NULL)        /* catch the NULL case */
                return node;

        while(head->prev != NULL) /* make sure head is actually the head */
                head = head->prev;

        node->next = head;
        head->prev = node;
        return node;        
}

/**
 * inserts a given node at the tail of a given list. Then, returns the tail
 * of the new list.
 * @param list a node of the list that is being inserted into.  Works best
 * if it is the tail
 * @param node the node to insert into the list
 * @return the tail of the new list
 */
struct bit_t *insert_tail(struct bit_t *list, struct bit_t *node)
{
        struct bit_t *tail = list;
        if(tail == NULL)           /* catch the NULL case */
                return node;

        while(tail->next != NULL)  /* make sure tail is actually the tail */
                tail = tail->next;
        
        tail->next = node;
        node->prev = tail;
        return node;
}

/**
 * inserts a given node before the other given node in a list
 * @param list the node of the list being inserted into
 * @param node the node to insert
 * @return the head of the list after insertion
 */
struct bit_t *insert_before(struct bit_t *list, struct bit_t *node)
{
        if(list == NULL)        /* 'insert' into empty list */
                return node;

        /* insert before list */
        node->next = list;
        if(list->prev != NULL)
                list->prev->next = node;
        node->prev = list->prev;
        list->prev = node;

        /* now find and return the head */
        while(node->prev != NULL)
                node = node->prev;
        return node;
}

/**
 * inserts a given node after the other given node in a list
 * @param list the node of the list being inserted into
 * @param node the node to insert
 * @return the tail of the list after insertion
 */
struct bit_t *insert_after(struct bit_t *list, struct bit_t *node)
{
        if(list == NULL)        /* 'insert' into empty list */
                return node;

        /* insert before list */
        node->prev = list;
        if(list->next != NULL)
                list->next->prev = node;
        node->next = list->next;
        list->next = node;

        /* now find and return the tail */
        while(node->next != NULL)
                node = node->next;
        return node;
}

/**
 * counts the number of nodes in a given list
 * @param node a node of the list to count
 * @return the number of nodes in the given list
 */
int count_nodes(struct bit_t *node)
{
        int count = 0;            /* counter for walking through the list */
        struct bit_t *cur = node; /* placeholder for walking through the list*/

        /* iterate forward through the list */
        while(cur != NULL) {
                cur = cur->next;
                count++;
        }

        /* iterate backward through the list */
        cur = node;
        while(cur != NULL) {
                cur = cur->prev;
                count++;
        }

        /* if node was not null, we have now counted node twice */
        if(node != NULL)
                count--;

        return count;
}

/**
 * Searches a given list for a node with a certain value, and returns a pointer 
 * to that node.
 * @param head a pointer to the head of the list to search
 * @param n the value to search for
 * @return the node containing the given value, or NULL if the value was not
 * found.
 */
struct bit_t *find_node(struct bit_t *list, char c)
{
        struct bit_t *cur = list; /* placeholder for walking down the list */

        /* walk forward through the list */
        while(cur != NULL && cur->n != c)
                cur = cur->next;

        /* if we found it return it */
        if(cur != NULL && cur->n == c) 
                return cur;

        cur = list;

        /* walk backward through the list */
        while(cur != NULL && cur->n != c)
                cur = cur->prev;

        return cur;
}

/**
 * Deletes a node with a given value from the given list.
 * @param list a pointer to any node of the list to delete from
 * @param n the value to delete from the list
 * @return a pointer to the head of the list after n was deleted
 */
struct bit_t *delete_node(struct bit_t *list, char c)
{
        struct bit_t *tmp = NULL; /* will hold the node as it is deleted */

        /* check the NULL case */
        if(list == NULL)
                return list;

        /* try and find the node we want to delete */
        tmp = find_node(list, c);

        /* make sure we aren't deleting our list pointer unless it's 1 item*/
        if(list == tmp) {
                if(list->prev != NULL) {
                        list = list->prev;
                }
                else if(list->next != NULL) {
                        list = list->next;
                }
                else {
                        free(list);
                        return NULL;
                }
        }

        if(tmp == NULL) {
                /* if node not found, just find and return the head */
                while(list->prev != NULL)
                        list = list->prev;
                return list;
        } 

        /* now delete the node we found */
        if(tmp->prev != NULL)
                tmp->prev->next = tmp->next;
        if(tmp->next != NULL)
                tmp->next->prev = tmp->prev;
        free(tmp);

        /* now return the head */
        while(list->prev != NULL)
                        list = list->prev;
        return list;
}

/**
 * Deletes an entire list and all values in it, freeing all memory used
 * @param list a pointer to any node of the list to delete
 */
void delete_list(struct bit_t *list)
{
        /* delete the heads until the whole list is just one NULL pointer */
        while(list != NULL)
                list = delete_node(list, list->n);

        /* then free that NULL pointer */
        free(list);
}

/**
 * Finds and returns the head of the given list
 * @param list a pointer to any node of the list to search 
 * @return the head of the given list
 */
struct bit_t *find_head(struct bit_t *list)
{
        if(list == NULL)
                return list;

        while(list->prev != NULL)
                list = list->prev;

        return list;
}

/**
 * Finds and returns the tail of the given list
 * @param list a pointer to any node of the list to search 
 * @return the tail of the given list
 */
struct bit_t *find_tail(struct bit_t *list)
{
        if(list == NULL)
                return list;

        while(list->next != NULL)
                list = list->next;

        return list;
}

int isequal(double a, double b)
{
        return islessequal(a, b) && isgreaterequal(a, b);
}

/**
 * Grabs an int from stdin in a sanitary manner
 * @return an int grabbed from stdin
 */
int grab_int()
{
        char buf[1024];
        fgets(buf, 1024, stdin);
        return atoi(buf);
}

/**
 * Grabs a double from stdin in a sanitary manner
 * @return a double grabbed from stdin
 */
double grab_double()
{
        char buf[1024];
        fgets(buf, 1024, stdin);
        return atof(buf);
}

/**
 * Grabs a char from stdin in a sanitary manner
 * @return a double grabbed from stdin
 */
char grab_char()
{
        int tmp; /* temporary holder for user input */
        int ans; /* answer to be returned */

        while((tmp = getchar()) != '\n')
                ans = tmp;

        return ans;
}

/**
 * given a string and an integer (ascii) value, removes every character with
 * that value from the string.
 * @param s a pointer to the string to squeeze
 * @param c the value to "squeeze out" of s
 */
void psqueeze(char *s, char c)
{
	int i, j;
	
	for (i = j = 0; *(s + i) != '\0'; i++)
		if (*(s + i) != c)
			*(s + j++) = *(s + i);

	s[j] = '\0';
}
