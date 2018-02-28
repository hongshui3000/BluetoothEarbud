/*!
\copyright  Copyright (c) 2008 - 2017 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    Part of 6.2
\file       av_headset_tasklist.c
\brief      Implementation of a simple list of VM tasks.
*/

#include "av_headset.h"
#include "av_headset_tasklist.h"

#include <panic.h>

/******************************************************************************
 * Internal functions
 ******************************************************************************/
/*! @brief Find the index in the task list array for a given task.
 */
static bool appTaskListFindTaskIndex(TaskList* list, Task search_task, uint16* index)
{
    for (uint16 iter = 0; iter < list->size_list; iter++)
    {
        if (list->tasks[iter] == search_task)
        {
            *index = iter;
            return TRUE;
        }
    }
    return FALSE;
}

/******************************************************************************
 * External API functions
 ******************************************************************************/
/*! @brief Create a TaskList.
 */
TaskList* appTaskListInit(void)
{
    TaskList* new_list = (TaskList*)PanicUnlessMalloc(sizeof(TaskList));
    if (new_list)
    {
        new_list->size_list = 0;
        new_list->tasks = NULL;
    }
    return new_list;
}

/*! @brief Destroy a TaskList.
 */
void appTaskListDestroy(TaskList* list)
{
    PanicNull(list);

    free(list->tasks);
    free(list);
}

/*! @brief Determine if a task is on a list.
 */
bool appTaskListIsTaskOnList(TaskList* list, Task search_task)
{
    uint16 tmp;
    PanicNull(list);
    return appTaskListFindTaskIndex(list, search_task, &tmp);
}

/*! @brief Add a task to a list.
 */
bool appTaskListAddTask(TaskList* list, Task add_task)
{
    PanicNull(list);
    PanicNull(add_task);

    /* if already in the list, just return */
    if (appTaskListIsTaskOnList(list, add_task))
        return FALSE;

    /* Resize list */
    list->tasks = realloc(list->tasks, sizeof(Task) * (list->size_list + 1));
    PanicNull(list->tasks);

    /* Add task to list */
    list->tasks[list->size_list] = add_task;
    list->size_list += 1;

    return TRUE;
}

/*! @brief Remove a task from a list.
 */
bool appTaskListRemoveTask(TaskList* list, Task del_task)
{
    uint16 index = 0;

    PanicNull(list);
    PanicNull(del_task);

    if (appTaskListFindTaskIndex(list, del_task, &index))
    {
        uint16 tasks_to_end = list->size_list - index - 1;
        memmove(&list->tasks[index], &list->tasks[index] + 1, sizeof(Task) * tasks_to_end);
        list->size_list -= 1;

        if (!list->size_list)
        {
            free(list->tasks);
            list->tasks = NULL;
        }
        else
        {
            /* resize list, if not an empty list now, then check realloc successfully
             * returned the memory */
            list->tasks = realloc(list->tasks, sizeof(Task) * list->size_list);
            PanicNull(list->tasks);
        }

        return TRUE;
    }
    else
        return FALSE;
}

/*! @brief Return number of tasks in list.
 */
uint16 appTaskListSize(TaskList* list)
{
    PanicNull(list);
    return list->size_list;
}

/*! @brief Iterate through all tasks in a list.
 */
bool appTaskListIterate(TaskList* list, Task* next_task)
{
    uint16 index = 0;

    PanicNull(list);
    
    /* empty list, we're done */
    if (!list->size_list)
        return FALSE;

    /* next_task == NULL to start at index 0 */
    if (*next_task == 0)
    {
        *next_task = list->tasks[index];
        return TRUE;
    }
    else
    {
        /* move to next task */
        if (appTaskListFindTaskIndex(list, *next_task, &index))
        {
            if (index + 1 < list->size_list)
            {
                *next_task = list->tasks[index+1];
                return TRUE;
            }
        }
    }
    /* end of the list */
    *next_task = 0;
    return FALSE;
}


/*! @brief Create a duplicate task list.
 */
TaskList *appTaskListDuplicate(TaskList* list)
{
    TaskList *new_list = appTaskListInit();
    if (new_list)
    {
        new_list->size_list = list->size_list;
        new_list->tasks = PanicUnlessMalloc(sizeof(Task) * new_list->size_list);
        memcpy(new_list->tasks, list->tasks, sizeof(Task) * new_list->size_list);
    }

    return new_list;
}


void appTaskListMessageSendWithSize(TaskList *list, MessageId id, void *data, uint16 size_data)
{
    if (list->size_list)
    {
        int index;
        for (index = 1; index < list->size_list; index++)
        {
            if (size_data)
            {
                void *copy = PanicUnlessMalloc(size_data);
                memcpy(copy, data, size_data);
                MessageSend(list->tasks[index], id, copy);
            }
            else
                MessageSend(list->tasks[index], id, NULL);
        }

        /* Send last message */
        MessageSend(list->tasks[0], id, size_data ? data : NULL);
    }
    else
        MessageFree(id, size_data ? data : NULL);
}
