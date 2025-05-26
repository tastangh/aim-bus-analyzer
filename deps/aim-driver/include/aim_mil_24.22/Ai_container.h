/* SPDX-FileCopyrightText: 2016-2021 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT OR GPL-2.0-or-later */


/*! \file Ai_container.h

    Header for container handling helper functions
*/
#ifndef AI_CONTAINER_H
#define AI_CONTAINER_H

#include "Ai_cdef.h"


/*! \def AI_ARRAY_COUNT
   Returns the number of elements of an array. \n
   Works only for static arrays with size known at compile time.
*/
#define AI_ARRAY_COUNT(x)  (sizeof(x)/sizeof(x[0]))


/*! \def AI_OFFSET_OF
    Returns the offset of a struct member in bytes
    @param type the type of the container struct the member belongs to
    @param member name of the member of the containing struct
*/
#define AI_OFFSET_OF(type, member)  ((AiSize) &(((type*) 0)->member))


/*! \def AI_CONTAINER_OF
 * Cast a member of a structure out to the containing structure
 *
 *               structure of 'type'
 * result-->----------------------------
 *          -                          -
 *          ----------------------------
 *    ptr----------->member            -
 *          ----------------------------
 *          -                          -
 *          -                          -
 *          ----------------------------
 * @param ptr the pointer to the member.
 * @param type the type of the container struct this is embedded in.
 * @member the name of the member within the struct.
 */
#define AI_CONTAINER_OF(ptr, type, member) (type *)((AiPtrDiff)(ptr) - AI_OFFSET_OF(type, member))


/*! \def AI_LIST_INIT
 * Initializes a linked list. Must be used before list can be initially used. \n
 * Empties the list if applied to a non-empty list.
 * @param list_head the list head structure to initialize
 */
#define AI_LIST_INIT(list_head) (list_head.next = list_head.prev = &list_head)


/*! \struct ai_list_head
 * This struct represents a doubly linked list.
 * It may be embedded in other structures to form
 * a list of the structure's instances
 */
struct ai_list_head
{
    struct ai_list_head* next;  /*!< Pointer to next element in list */
    struct ai_list_head* prev;  /*!< Pointer to previous element in list */
};


/*! \brief Checks if list is empty
 * @param head head of list to check for being empty
 * @return true if list is empty, false otherwise
 */
static AI_INLINE int ai_list_empty(const struct ai_list_head *head)
{
    return head->next == head;
}


/*! \brief Insert a new entry between two known consecutive entries.
 *
 * This is only for internal use by other, top-level list functions
 */
static AI_INLINE void __ai_list_add(struct ai_list_head* new_item, struct ai_list_head* prev,
                                    struct ai_list_head* next)
{
    next->prev = new_item;
    new_item->next = next;
    new_item->prev = prev;
    prev->next = new_item;
}


/*! \brief Add a new entry at beginning of list
 *
 * @param new new entry to be added
 * @param head list head of list to insert entry
 */
static AI_INLINE void ai_list_add(struct ai_list_head* new_item, struct ai_list_head* head)
{
    __ai_list_add(new_item, head, head->next);
}


/*! \brief Add entry at end of list
 *
 * @param new new entry to be added
 * @param head head of list where entry shall be added
 */
static AI_INLINE void ai_list_add_tail(struct ai_list_head* new_item, struct ai_list_head* head)
{
    __ai_list_add(new_item, head->prev, head);
}


/*! \brief Internal function for deleting a entry
 *
 * Makes the prev/next entries of the entry to be deleted point to each other.
 * This is only for internal use by other, top-level list functions
 */
static AI_INLINE void __ai_list_del(struct ai_list_head* prev, struct ai_list_head* next)
{
    next->prev = prev;
    prev->next = next;
}


/*! \brief Deletes entry from list.
 *
 * @param entry the element to delete from the list.
 */
static AI_INLINE void ai_list_del(struct ai_list_head* entry)
{
    __ai_list_del(entry->prev, entry->next);
    entry->next = 0;
    entry->prev = 0;
}


/*! \brief Checks if list element is last one in a list
 *
 * @param list the list element to check
 * @param head the head of the list
 */
static AI_INLINE AiBoolean ai_list_is_last(const struct ai_list_head *list, const struct ai_list_head *head)
{
    return list->next == head;
}


/*! \brief Checks if list element is first one in a list
 *
 * @param list the list element to check
 * @param head the head of the list
 */
static AI_INLINE AiBoolean ai_list_is_first(const struct ai_list_head *list, const struct ai_list_head *head)
{
    return list->prev == head;
}


/*! \brief Get the containing struct of a list entry
 *
 *                   list element of 'type'
 * result ----> ---------------------------------
 *              -                               -
 *              -                               -
 *              -                               -
 *              ---------------------------------
 *          ptr---->struct ai_list_head member  -
 *              ---------------------------------
 *
 * @param ptr pointer to struct ai_list_entry to get containing struct of.
 * @param type the type of the struct the ai_list_head is embedded in.
 * @param member the name of the ai_list_head member within the struct.
 */
#define ai_list_entry(ptr, type, member) AI_CONTAINER_OF(ptr, type, member)


/*! \brief Get the first element from a list
 *
 * The list must not be empty.
 * @param ptr the list head to take the element from.
 * @param type the type of the list element.
 * @param member the name of the ai_list_head member within the list element.
 */
#define ai_list_first_entry(ptr, type, member) ai_list_entry((ptr)->next, type, member)


/*! \brief Get the last element from a list
 *
 * The list must not be empty.
 * @param ptr the list head to take the element from.
 * @param type the type of the list element
 * @param member the name of the ai_list_head member within the list element.
 */
#define ai_list_last_entry(ptr, type, member) ai_list_entry((ptr)->prev, type, member)


/*! \brief Get the first element from a list
 *
 * Will check if list is empty and return NULL in this case
 * @param ptr the list head to take the element from.
 * @param type the type of the list element
 * @param member the name of the ai_list_head member within the list element.
 */
#define ai_list_first_entry_or_null(ptr, type, member) (!ai_list_empty(ptr) ? ai_list_first_entry(ptr, type, member) : NULL)


/*! \brief Get the next element from a list with given type
 *
 * @param pos the element to get next element of
 * @param type the type of the list element
 * @param member the name of the ai_list_head member within the list element.
 */
#define ai_list_next_entry(pos, type, member) \
            ai_list_entry((pos)->member.next, type, member)


/*! \brief Get the previous element from a list with given type
 *
 * @param pos the element to get previous element of
 * @param type the type of the list element
 * @param member the name of the ai_list_head member within the list element.
 */
#define ai_list_prev_entry(pos, type, member) \
            ai_list_entry((pos)->member.prev, type, member)


/*! \brief Iterate over a list
 *
 * @param current pointer to struct ai_list_head to use as a loop cursor.
 * @param head the head of the list
 */
#define ai_list_for_each(current, head) \
        for (current = (head)->next; current != (head); current = current->next)


/*! \brief Iterate over a list backwards
 *
 * @param current pointer to struct ai_list_head to use as a loop cursor.
 * @param head the head of the list.
 */
#define ai_list_for_each_prev(current, head) \
        for (current = (head)->prev; current != (head); current = current->prev)


/*! \brief Iterate over list with given type
 *
 * @param pos the type * to use as a loop cursor.
 * @param head the head of the list to iterate.
 * @param type the type of the struct that contains the list anchor
 * @param member the name of the list_head that serves as list anchor within the struct.
 */
#define ai_list_for_each_entry(pos, head, type, member)           \
        for (pos = ai_list_first_entry(head, type, member);        \
            &pos->member != (head);                                 \
            pos = ai_list_next_entry(pos, type, member))


/*! \brief Iterate backwards over list with given type
 *
 * @param pos the type * to use as a loop cursor.
 * @param head the head of the list to iterate.
 * @param type the type of the struct that contains the list anchor
 * @param member the name of the ai_list_head that serves as list anchor within the struct.
 */
#define ai_list_for_each_entry_reverse(pos, head, type, member)    \
            for (pos = ai_list_last_entry(head, type, member);     \
                &pos->member != (head);                            \
                pos = ai_list_prev_entry(pos, type, member))


/*! \brief Iterate over list of given type while removal of current entry is safe
 *
 * @param pos the type * to use as a loop cursor.
 * @param n another type * to use as temporary storage for the next element
 * @param head the head of the list to iterate over
 * @param type the type of the struct that contains the list anchor
 * @param member the name of the ai_list_head that serves as list anchor within the struct.
 */
#define ai_list_for_each_entry_safe(pos, n, head, type, member)                  \
            for (pos = ai_list_first_entry(head, type, member),        \
                 n = ai_list_next_entry(pos, type, member);                       \
                 &pos->member != (head);                                    \
                 pos = n, n = ai_list_next_entry(n, type, member))


/*! \brief Iterate backwards over list of give type while removal of current entry is safe
 *
 * @param pos the type * to use as a loop cursor.
 * @param n another type * to use as temporary storage for the next element
 * @param head the head of the list to iterate over
 * @param type the type of the struct that contains the list anchor
 * @param member the name of the ai_list_head that serves as list anchor within the struct.
 */
#define ai_list_for_each_entry_safe_reverse(pos, n, head, type, member)          \
        for (pos = ai_list_last_entry(head, type, member),         \
                n = ai_list_prev_entry(pos, type, member);                       \
                &pos->member != (head);                                    \
                pos = n, n = ai_list_prev_entry(n,type, member))


#endif /* AI_CONTAINER_H */
/* end: Ai_container.h */
