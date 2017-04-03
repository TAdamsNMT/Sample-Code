/**
 * @file bincalc.c
 * @author Thomas Adams
 * @date 17 November 2016
 *
 * Assignment: lab 9
 *
 * @brief When run, simulates a binary calculator with doubly-linked lists 
 * as registers.
 *
 * @details operations implemented include addition (+), subtraction (-), 
 * AND (&), OR (|), and XOR (^).  Dependent on dllist.o, which is reusable code
 * for doubly linked lists.  Everything considered pertinent to only this
 * problem is contained within this program, however.
 *
 *@bugs seems to be backward parsing
 *
 *@todo None
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "dllist.h"

#define ERR_MALLOC 1
#define ERR_SIZE   2
#define ERR_OP     3
#define ERR_BIGR1  4
#define ERR_BADR1  5
#define ERR_BIGR2  6
#define ERR_BADR2  7

struct cpu_t { 
        int word_size;
        int unsign;   /* 0 for signed, 1 for unsigned */
        /* flags */
        int overflow;
        int carry;
        int sign;
        int parity;
        int zero;
        /* registers */
        struct bit_t *r1_head;
        struct bit_t *r1_tail;
        struct bit_t *r2_head;
        struct bit_t *r2_tail;
        struct bit_t *r3_head;
        struct bit_t *r3_tail;
};

void err(int err_no);
int parse_op1(struct cpu_t *cpu, char *operand);
int parse_op2(struct cpu_t *cpu, char *operand);
void operate(struct cpu_t *cpu, char op);
void negative(struct bit_t *reg);
long to_dec(struct bit_t *reg);
unsigned long to_unsign(struct bit_t *reg);

int main(void)
{
        struct cpu_t *cpu;
        char sign_input = 'n';
        char bin_exp[1024]; /* holds the binary expression as a string */
        char *op;           /* holdes the operator */
        char *operand;      /* holds each operand as a string before parsing */
        int i;
        char go_agn;        /* holds whether the user wants to restart */
        int err_no;         /* holds error values */

        /* initial setup */
        cpu = malloc(sizeof(struct cpu_t));
        if(!cpu) {
                err(ERR_MALLOC);
                exit(ENOMEM);
        }
        cpu->r1_head = NULL;
        cpu->r1_tail = NULL;
        cpu->r2_head = NULL;
        cpu->r2_tail = NULL;
        cpu->r3_head = NULL;
        cpu->r3_tail = NULL;
        cpu->unsign = 0; 

        /* after initial setup, loop until user says we're done */
        START:

        /* in case of repeat via continue, perform cleanup */
        delete_list(cpu->r1_head);
        delete_list(cpu->r2_head);
        delete_list(cpu->r3_head);
        cpu->r1_head = NULL;
        cpu->r2_head = NULL;
        cpu->r3_head = NULL;
        cpu->r1_tail = NULL;
        cpu->r2_tail = NULL;
        cpu->r3_tail = NULL;
        cpu->unsign = 0;
                
        /* get word size */
        printf("enter word size: ");
        cpu->word_size = grab_int();
        if(cpu->word_size < 1 || cpu->word_size > 64) {
                err(ERR_SIZE);
                goto START;
        }

        /* get sign value */
        printf("unsigned values[y/N]: ");
        sign_input = 'n';
        sign_input = grab_char();
        if(sign_input =='y' || sign_input == 'Y')
                cpu->unsign = 1; 

        /* get the expression to parse */
        printf("enter binary expression: ");
        fgets(bin_exp, 1024, stdin); 

        /* parse the expression; start with finding the operator */
        if(strchr(bin_exp, '+'))
                op = "+";
        else if(strchr(bin_exp, '-'))
                op = "-";
        else if(strchr(bin_exp, '&'))
                op = "&";
        else if(strchr(bin_exp, '|'))
                op = "|";
        else if(strchr(bin_exp, '^')) 
                op = "^";
        else {
                err(ERR_OP);
                goto START;
             }

       /* now tokenize it to get 0's and 1's, but remove whitespace */
        psqueeze(bin_exp, ' ');
        psqueeze(bin_exp, '\n');

        /* parse the first operand, check errors */
        operand = strtok(bin_exp, op); 

        err_no = parse_op1(cpu, operand);
        if(err_no){
                err(err_no);
                goto START;
        }

        /* parse the second operand, check errors */
        operand = strtok(NULL, op); 

        err_no = parse_op2(cpu, operand);
        if(err_no){
                err(err_no);
                goto START;
        }

        /* perform the operation */
        operate(cpu, *op);

        /* change the - operator for printing, we'll have flipped r2 */
        if(*op == '-')
                op = "+";

        /* print the results */
        print_list(cpu->r1_head);
        printf("%c\n", *op);
        print_list(cpu->r2_head);
        for(i = 0; i < cpu->word_size; i++)
                printf("-");
        printf("\n");
        print_list(cpu->r3_head);

        printf("flags\n");
        printf("carry: %d\n", cpu->carry);
        printf("overflow: %d\n", cpu->overflow);
        printf("sign: %d\n", cpu->sign);
        printf("parity: %d\n", cpu->parity);
        printf("zero: %d\n", cpu->zero);

        printf("decimal: ");
        if(cpu->unsign)
                printf("%lu\n", to_unsign(cpu->r3_head));
        else
                printf("%ld\n", to_dec(cpu->r3_head));

        /* see if user wants to continue */
        printf("\ndo you want to continue [Y/n]?: ");
        go_agn = grab_char();
        if(go_agn == 'y' || go_agn == 'Y')
                goto START;

        /* if we aren't repeating just clean up and go home */
        delete_list(cpu->r1_head);
        delete_list(cpu->r2_head);
        delete_list(cpu->r3_head);
        free(cpu);

        printf("Goodbye\n");

        return 0;
}

/**
 * Prints out the appropriate responses for various errors that might come
 * up in main
 * @param err_no the number of the error, based on predefined values
 */
void err(int err_no)
{
        switch(err_no) {
        case ERR_MALLOC:
                printf("Sorry, the calculator could not be malloced! No memory\n");
                printf("Goodbye.\n");
                return;
        case ERR_SIZE:
                printf("error in word size, must be between 1 and 64\n");
                return;
        case ERR_OP:
                printf("error in operator. retry\n");
                return;
        case ERR_BIGR1:
                printf("error in input -- length is greater than word size\n");
                printf("error in first operand. retry\n");
                return;
        case ERR_BADR1:
                printf("error in input -- something other than a 1 or 0 entered\n");
                printf("error in first operand. retry\n");
                return;
        case ERR_BIGR2:
                printf("error in input -- length is greater than word size\n");
                printf("error in second operand. retry\n");
                return;
        case ERR_BADR2:
                printf("error in input -- something other than a 1 or 0 entered\n");
                printf("error in second operand. retry\n");
                return;

        }
}


/**
 * Parses the first operand from a given string.  Returns an error code, if
 * an error occurred
 * @param cpu the cpu to parse into
 * @param str the string to parse
 * @return an error code, or 0 for no error
 */
int parse_op1(struct cpu_t *cpu, char *operand)
{
        struct bit_t *node;
        int i;
        char values[64] = {0};

        /* check for too long error */
        if( strlen(operand) > cpu->word_size ) 
               return ERR_BIGR1;

        /* pad the operand with zeros using strncat */
        while( strlen(values) + strlen(operand) < cpu->word_size){
                strncat(values, "0", 2);
        }
        
        /* then copy the operand into the padded string */
        strncat(values, operand, 64);

        /* parse the given operand */
        for(i = strlen(values) - 1; i >= 0; i--)
        {
                if(values[i] == '0')
                        node = create_node(0);
                else if(values[i] == '1')
                        node = create_node(1);
                else 
                        return ERR_BADR1;
                cpu->r1_head = insert_head(cpu->r1_head, node);
        }

        /* assign the head of the list */
         cpu->r1_tail = find_tail(cpu->r1_head); 

        return 0;
}

/**
 * Parses the second operand from a given string.  Returns an error code if 
 * an error occurred
 * @param cpu the cpu to parse into
 * @param str the string to parse
 * @return an error code, or 0 for no error
 */
int parse_op2(struct cpu_t *cpu, char *operand)
{
        struct bit_t *node;
        int i;
        char values[64] = {0};

        /* check for too long error */
        if( strlen(operand) > cpu->word_size ) 
               return ERR_BIGR2;


        /* pad the operand with zeros using strncat */
        while( strlen(values) + strlen(operand) < cpu->word_size){
                strncat(values, "0", 2);
        }
        
        /* then copy the operand into the padded string */
        strncat(values, operand, 64);

        /* parse the given operand */
        for(i = strlen(values) - 1; i >= 0; i--)
        {
                if(values[i] == '0')
                        node = create_node(0);
                else if(values[i] == '1')
                        node = create_node(1);
                else 
                        return ERR_BADR2;
                cpu->r2_head = insert_head(cpu->r2_head, node);
        }

        /* assign the head of the list */
         cpu->r2_tail = find_tail(cpu->r2_head); 

        return 0;
}

/**
 * Given a cpu structure and an operation to perform, performs the operation
 * on the first and second registers and stores the result in the third
 * register.  Then updates cpu flags.
 * @param cpu a pointer to the cpu structure to manipulate
 * @param op a character representing which operation to perform. Implemented
 * operations include +, -, &, |, and ^.
 */
void operate(struct cpu_t *cpu, char op)
{
        struct bit_t *cur_first = cpu->r1_tail;
        struct bit_t *cur_second = cpu->r2_tail;
        struct bit_t *insert;
        char carry = 0;
        char result;

        switch (op){
        
        case '+':
                /* iterate backwards through registries */
                while(cur_first != NULL)
                {
                        /* insert the sum bit */
                        result = cur_first->n ^ cur_second->n ^ carry;
                        insert = create_node(result);
                        cpu->r3_head = insert_head(cpu->r3_head, insert);

                        /* figure out if there's a carry */
                        carry = (cur_first->n & cur_second->n) | 
                                (cur_first->n & carry) | (cur_second->n & carry);
                        /* iterate backwards */
                        cur_first = cur_first->prev;
                        cur_second = cur_second->prev;
                }

                /* set the carry bit */
                cpu->carry = carry;

                /* set the overflow bit based on whether it's unsigned */  
                if(cpu->unsign)
                        cpu->overflow = carry;
                else {
                /* if it's signed, overflow if addends are the same sign and
                 * the sign of the addend doesn't match the sign of the result
                 */
                        cpu->overflow = (cpu->r1_head->n == cpu->r2_head->n) & 
                                (cpu->r1_head->n != cpu->r3_head->n);
                }

                break;


        case '-':       
                negative(cur_second); /* negate the second addend then add */

                /* iterate backwards through registries */
                while(cur_first != NULL)
                {
                        /* insert the sum bit */
                        result = cur_first->n ^ cur_second->n ^ carry;
                        insert = create_node(result);
                        cpu->r3_head = insert_head(cpu->r3_head, insert);

                        /* figure out if there's a carry */
                        carry = (cur_first->n & cur_second->n) | 
                                (cur_first->n & carry) | (cur_second->n & carry);
                        /* iterate backwards */
                        cur_first = cur_first->prev;
                        cur_second = cur_second->prev;
                }

                /* set the carry bit */
                cpu->carry = carry;

                /* set the overflow bit based on whether it's unsigned */  
                if(cpu->unsign)
                        cpu->overflow = carry;
                else {
                /* if it's signed, overflow if addends are the same sign and
                 * the sign of the addend doesn't match the sign of the result
                 */
                        cpu->overflow = (cpu->r1_head->n == cpu->r2_head->n) & 
                                (cpu->r1_head->n != cpu->r3_head->n);
                }

                break;
        case '&':
                /* iterate backwards through registries */
                while(cur_first != NULL)
                {
                        /* insert the AND bit */
                        result = cur_first->n & cur_second->n;
                        insert = create_node(result);
                        cpu->r3_head = insert_head(cpu->r3_head, insert);

                        /* iterate backwards */
                        cur_first = cur_first->prev;
                        cur_second = cur_second->prev;
                }
                cpu->carry = 0;
                cpu->overflow = 0;
                break;
        case '|':
                /* iterate backwards through registries */
                while(cur_first != NULL)
                {
                        /* insert the OR bit */
                        result = cur_first->n | cur_second->n;
                        insert = create_node(result);
                        cpu->r3_head = insert_head(cpu->r3_head, insert);

                        /* iterate backwards */
                        cur_first = cur_first->prev;
                        cur_second = cur_second->prev;
                }
                cpu->carry = 0;
                cpu->overflow = 0;
                break;
        case '^':
                /* iterate backwards through registries */
                while(cur_first != NULL)
                {
                        /* insert the XOR bit */
                        result = cur_first->n ^ cur_second->n;
                        insert = create_node(result);
                        cpu->r3_head = insert_head(cpu->r3_head, insert);

                        /* iterate backwards */
                        cur_first = cur_first->prev;
                        cur_second = cur_second->prev;
                }
                cpu->carry = 0;
                cpu->overflow = 0;
                break;
        default:
                printf("The calculator is somehow operating on an invalid ");
                printf("operator.\nThis should never ever happen, something broke");
                return;
        }

        /* set the tail */
        cpu->r3_tail = find_tail(cpu->r3_head);

        /* set non-op-dependent flags */

        if(cpu->r3_head->n == 0)
                cpu->sign = 0;
        else
                cpu->sign = 1;

        /* iterate through the result updating flags for each member */
        /* start assuming all zeros; update flags for every 1 found */
        cpu->parity = 1;
        cpu->zero = 1;
        for(cur_first = cpu->r3_head; cur_first != NULL; cur_first = cur_first->next)
        {
                /* if 1 is found, flip parity and unset zero */
                if(cur_first->n)
                {
                        cpu->parity = !cpu->parity;
                        cpu->zero = 0;
                }
        } 
}

/**
 * Negates the given binary register.  That is, shifts all bits and adds 1.
 * @param reg the register to negate
 */
void negative(struct bit_t *reg)
{
        int carry = 1;      /* for adding 1 */
        int tmp;            /* temporary variable for updating carry */
        /* get the tail of the register */
        reg = find_tail(reg);

        /* negate each node and add the carry */
        while(reg != NULL) {
                tmp = carry & !(reg->n);
                reg->n = carry ^ !(reg->n);
                carry = tmp;
                reg = reg->prev;
        }

}

/**
 * Given a bit_t register, returns the decimal value of that register as
 * a signed decimal.
 * @param list an element of the register to convert to a signed decimal
 * @return the value of list as a signed long decimal
 */
long to_dec(struct bit_t *list)
{
        long number = 0;

        /* find the head of the list */
        while(list->prev != NULL)
                list = list->prev;

        /* set whether it is positive or negative */
        if(list->n == 1)
                number = -1;

        /* now iterate through and add each bit to number */
        while(list->next != NULL) {
                list = list->next;

                number = number * 2 + list->n;
        }
        return number;
}

/**
 * Given a bit_t register, return the decimal value of that register as
 * an unsigned decimal.
 * @param list an element of the register to convert to an unsigned decimal
 * @return a long that contains the unsigned decimal value of the register
 */
unsigned long to_unsign(struct bit_t *list)
{
        long number = 0;

        /* find the head of the list */
        while(list->prev != NULL)
                list = list->prev;

        /* now iterate through and add each bit to number */
        while(list != NULL) {
                number = number * 2 + list->n;
                list = list->next;
        }
        return number;
}
