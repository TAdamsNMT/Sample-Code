/**
 * @file dllist.h
 * @author Thomas Adams
 * @date Nov 9 2016
 * 
 * Assignment : lab 9
 * 
 * @brief a header file for dllist.c
 * 
 * @details see dllist.c
 * 
 * @bugs None
 *
 * @todo None
 */
#ifndef DLLIST_H_
#define DLLIST_H_


struct bit_t {
        unsigned char n;

        struct bit_t *next;
        struct bit_t *prev;
};

struct bit_t *create_node(char c);
void print_node(struct bit_t *node);
void print_verbose(struct bit_t *list);
void print_list(struct bit_t *list);
struct bit_t *insert_head(struct bit_t *list, struct bit_t *node);
struct bit_t *insert_tail(struct bit_t *list, struct bit_t *node);
struct bit_t *insert_before(struct bit_t *list, struct bit_t *node);
struct bit_t *insert_after(struct bit_t *list, struct bit_t *node);
int count_nodes(struct bit_t *list);
struct bit_t *find_node(struct bit_t *list, char c);
struct bit_t *delete_node(struct bit_t *list, char c);
void delete_list(struct bit_t *list);
struct bit_t *find_head(struct bit_t *list);
struct bit_t *find_tail(struct bit_t *list);
int grab_int();
double grab_double();
char grab_char();
void psqueeze(char *s, char c);


#endif
