#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */

void q_delete(struct list_head *head)
{
    if (!head || list_empty(head))
        return;
    element_t *e = list_entry(head, element_t, list);
    list_del(head);
    q_release_element(e);
}

int cmp(struct list_head *left, struct list_head *right)
{
    char *L = "", *R = "";
    if (list_entry(left, element_t, list)->value)
        L = list_entry(left, element_t, list)->value;
    if (list_entry(right, element_t, list)->value)
        R = list_entry(right, element_t, list)->value;
    return strcmp(L, R);
};

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));

    if (!head)
        return NULL;

    INIT_LIST_HEAD(head);

    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    element_t *entry, *safe;

    if (!head)
        return;

    list_for_each_entry_safe (entry, safe, head, list) {
        if (entry->value)
            free(entry->value);
        list_del(&entry->list);
        free(entry);
    }

    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    element_t *e = malloc(sizeof(element_t));

    if (!e || !s)
        return false;

    if (!(e->value = strdup(s))) {
        free(e);
        return false;
    }
    list_add(&e->list, head);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    element_t *e = malloc(sizeof(element_t));

    if (!e || !s)
        return false;

    if (!(e->value = strdup(s))) {
        free(e);
        return false;
    }
    list_add_tail(&e->list, head);

    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *e = list_first_entry(head, element_t, list);
    if (sp && snprintf(sp, bufsize, "%s", e->value) == bufsize)
        sp[bufsize - 1] = '\0';

    list_del(head->next);

    return e;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *e = list_last_entry(head, element_t, list);
    if (sp && snprintf(sp, bufsize, "%s", e->value) == bufsize)
        sp[bufsize - 1] = '\0';

    list_del(head->prev);

    return e;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    int size = 0;
    struct list_head *node;

    if (!head)
        return 0;

    list_for_each (node, head) {
        size++;
    }

    return size;
}

/* Delete the middle node in queue */
bool __q_delete_mid(struct list_head *head)
{
    if (!head || list_empty(head))
        return false;
    struct list_head *node, *tmp = head->next;
    int i = 0;
    list_for_each (node, head) {
        if (i++ & 0x1)
            tmp = tmp->next;
    }
    q_delete(tmp);
    return true;
}

bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    return __q_delete_mid(head);
}

/* Delete all nodes that have duplicate string */
bool __q_delete_dup(struct list_head *head)
{
    if (!head || list_empty(head))
        return false;
    struct list_head *node, *prev, *tmp;
    list_for_each (node, head) {
        if (node->next == head)
            break;
        if (cmp(node, node->next))
            continue;
        else {
            prev = node;
            q_delete(node->next);
            node = prev->next;
            while (node != head && cmp(prev, node) == 0) {
                tmp = node->next;
                q_delete(node);
                node = tmp;
            }
            node = prev->prev;
            q_delete(prev);
        }
    }
    return true;
}

bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return __q_delete_dup(head);
}

/* Swap every two adjacent nodes */
void __q_swap(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;
    struct list_head *node;
    list_for_each (node, head) {
        if (node->next == head)
            break;
        struct list_head *next = node->next;
        list_del(node);
        list_add(node, next);
    }
}

void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    __q_swap(head);
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
