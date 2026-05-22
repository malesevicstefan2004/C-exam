/**
 * @file list.h
 * @brief Sorted linked list for vCard line entries
 * @author Stefan Malesevic
 */
#ifndef _LIST_H_
#define _LIST_H_

#include <cstdio>

/**
 * @brief Sort priority for each vCard section
 *
 * The order follows the vCard 3.0 output format:
 * header fields first, footer fields last.
 */
typedef enum {
    ORDER_HEADER = 0, /**< BEGIN:VCARD and VERSION:3.0 */
    ORDER_NAME   = 1, /**< N: and FN: name fields */
    ORDER_ORG    = 2, /**< ORG: organization field */
    ORDER_TEL    = 3, /**< TEL; telephone fields */
    ORDER_EMAIL  = 4, /**< EMAIL; address fields */
    ORDER_FOOTER = 5  /**< REV: and END:VCARD */
} VcardOrder;

/**
 * @brief Single node in the sorted linked list
 */
struct VcardNode {
    int        order;   /**< Sort priority (see VcardOrder) */
    char      *content; /**< Heap-allocated vCard line string */
    VcardNode *next;    /**< Pointer to the next node, nullptr at end */
};

/**
 * @brief Insert a vCard line into the list in sorted order
 *
 * Nodes with equal order values are appended after existing
 * nodes of the same order, preserving insertion order within a group.
 *
 * @param head    Address of the head pointer of the list
 * @param order   Sort priority of the new entry (see VcardOrder)
 * @param content The vCard line string to insert (copied to heap)
 */
void list_insert(VcardNode **head, int order, const char *content);

/**
 * @brief Free all nodes and their heap-allocated content
 * @param head Address of the head pointer; set to nullptr after freeing
 */
void list_free(VcardNode **head);

/**
 * @brief Write all list entries line by line to an output stream
 * @param head Head of the list (read-only traversal)
 * @param out  Target FILE stream
 */
void list_print(const VcardNode *head, FILE *out);

#endif /* _LIST_H_ */
