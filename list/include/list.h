/**
 * @file list.h
 * @brief Sorted linked list for vCard entries
 * @author Stefan Malesevic
 */
#ifndef _LIST_H_
#define _LIST_H_

#include <cstdio>

/**
 * @brief Order values for sorting vCard fields
 */
typedef enum {
    ORDER_HEADER = 0,
    ORDER_NAME   = 1,
    ORDER_ORG    = 2,
    ORDER_TEL    = 3,
    ORDER_EMAIL  = 4,
    ORDER_FOOTER = 5
} VcardOrder;

/**
 * @brief Node of the linked list
 */
struct VcardNode {
    int        order;
    char      *content;
    VcardNode *next;
};

/**
 * @brief Insert a line into the list sorted by order
 * @param head pointer to the head pointer
 * @param order sort value
 * @param content the vCard line to insert
 */
void list_insert(VcardNode **head, int order, const char *content);

/**
 * @brief Free all nodes in the list
 * @param head pointer to the head pointer
 */
void list_free(VcardNode **head);

/**
 * @brief Print all entries to a stream
 * @param head head of the list
 * @param out output stream
 */
void list_print(const VcardNode *head, FILE *out);

#endif /* _LIST_H_ */
