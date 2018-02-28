/*!
\copyright  Copyright (c) 2008 - 2017 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    Part of 6.2
\file       av_headset_tasklist.h
\brief      Interface to simple list of VM tasks.
*/

#ifndef AV_HEADSET_TASKLIST_H
#define AV_HEADSET_TASKLIST_H

/*! @brief List of VM Tasks.
 */
typedef struct
{
    /*! List of tasks. */
    Task* tasks;

    /*! Number of tasks in #tasks. */
    uint16 size_list;
} TaskList;

/*! @brief Create a TaskList.

    @return TaskList* Pointer to new TaskList.
 */
TaskList* appTaskListInit(void);

/*! @brief Destroy a TaskList.

    @param list [IN] Pointer to a Tasklist.

    @return bool TRUE TaskList destroyed successfully.
                 FALSE TaskList destroy failure.

 */
void appTaskListDestroy(TaskList* list);

/*! @brief Add a task to a list.
 
    @param list [IN] Pointer to a Tasklist.
    @param add_task [IN] Task to add to the list.
    @return FALSE if the task is already on the list, otherwise TRUE.
 */
bool appTaskListAddTask(TaskList* list, Task add_task);

/*! @brief Remove a task from a list.

    @param list [IN] Pointer to a Tasklist.
    @param del_task [IN] Task to remove from the list.
    @return FALSE if the task was not on the list, otherwise TRUE.
 */
bool appTaskListRemoveTask(TaskList* list, Task del_task);

/*! @brief Return number of tasks in list.
 
    @param list [IN] Pointer to a Tasklist.

    @return uint16 Number of Tasks in the list. 
 */
uint16 appTaskListSize(TaskList* list);

/*! @brief Iterate through all tasks in a list.

    Pass NULL to next_task to start iterating at first task in the list.
    On each subsequent call next_task should be the task previously returned
    and appTaskListIterate will return the next task in the list.

    @param list [IN] Pointer to a Tasklist.
    @param next_task [IN/OUT] Pointer to task from which to iterate.

    @return bool TRUE next_task is returning a Task.
                 FALSE end of list, next_task is not returning a Task.
 */
bool appTaskListIterate(TaskList* list, Task* next_task);

/*! @brief Determine if a task is on a list.
 *
    @param list [IN] Pointer to a Tasklist.
    @param search_task [IN] Task to search for on list.

    @return bool TRUE search_task is on list, FALSE search_task is not on the list.
 */
bool appTaskListIsTaskOnList(TaskList* list, Task search_task);

/*! @brief Create a duplicate task list.
 *
    @param list [IN] Pointer to a Tasklist.

    @return TaskList * Pointer to duplicate task list.
 */
TaskList *appTaskListDuplicate(TaskList* list);

/*! @brief Send a message (with data) to all tasks in the task list.

    @param list [IN] Pointer to a TaskList.
    @param id The message ID to send to the TaskList.
    @param data Pointer to the message content.
    @param size_data The sizeof the message content.
*/
void appTaskListMessageSendWithSize(TaskList *list, MessageId id, void *data, uint16 size_data);

/*! @brief Send a message (with data) to all tasks in the task list.
    @param list [IN] Pointer to a TaskList.
    @param id The message ID to send to the TaskList.
    @param message Pointer to the message content.
    @note Assumes id is of a form such that appending a _T to the id creates the
    message structure type string.
*/
#define appTaskListMessageSend(list, id, message) \
    appTaskListMessageSendWithSize(list, id, message, sizeof(id##_T))

/*! @brief Send a message without content to all tasks in the task list.
    @param list [IN] Pointer to a TaskList.
    @param id The message ID to send to the TaskList.
*/
#define appTaskListMessageSendId(list, id) \
    appTaskListMessageSendWithSize(list, id, NULL, 0)

#endif /* AV_HEADSET_TASKLIST_H */


