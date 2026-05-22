/**
 * @file list.cpp
 * @brief Sorted linked list implementation for vCard line entries
 * @author Stefan Malesevic
 */
#include <cstring>
#include <cstdio>

#include <list.h>

void list_insert(VcardNode **head, int order, const char *content) {
    VcardNode *node  = new VcardNode;
    node->order      = order;
    node->content    = new char[strlen(content) + 1];
    strcpy(node->content, content);
    node->next       = nullptr;

    /* Advance past all nodes with order <= new order (stable insert) */
    VcardNode **pos = head;
    while (*pos != nullptr && (*pos)->order <= order) {
        pos = &(*pos)->next;
    }
    node->next = *pos;
    *pos       = node;
}

void list_free(VcardNode **head) {
    VcardNode *cur = *head;
    while (cur != nullptr) {
        VcardNode *next = cur->next;
        delete[] cur->content;
        delete cur;
        cur = next;
    }
    *head = nullptr;
}

void list_print(const VcardNode *head, FILE *out) {
    const VcardNode *cur = head;
    while (cur != nullptr) {
        fprintf(out, "%s\n", cur->content);
        cur = cur->next;
    }
}
