/*
 * Copyright (c) 2017-2017 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "cmsis_os2.h"
#include "bm_types.h"
#include <stdlib.h>
#include <string.h>

#define OS_ASSERT(...)

//  ==== Kernel Management Functions ====
 
/// Initialize the RTOS Kernel.
/// \return status code that indicates the execution status of the function.
osStatus_t osKernelInitialize (void)
{
    //TODO
    return osError;
}
 
///  Get RTOS Kernel Information.
/// \param[out]    version       pointer to buffer for retrieving version information.
/// \param[out]    id_buf        pointer to buffer for retrieving kernel identification string.
/// \param[in]     id_size       size of buffer for kernel identification string.
/// \return status code that indicates the execution status of the function.
osStatus_t osKernelGetInfo (osVersion_t *version, char *id_buf, uint32_t id_size)
{
    //TODO
    return osError;
}
 
/// Get the current RTOS Kernel state.
/// \return current RTOS Kernel state.
osKernelState_t osKernelGetState (void)
{
    return osKernelRunning;
}
 
/// Start the RTOS Kernel scheduler.
/// \return status code that indicates the execution status of the function.
osStatus_t osKernelStart (void)
{
    //TODO
    return osError;
}
 
/// Lock the RTOS Kernel scheduler.
/// \return previous lock state (1 - locked, 0 - not locked, error code if negative).
int32_t osKernelLock (void)
{
    // Can't implement with pthreads
    return 0;
}
 
/// Unlock the RTOS Kernel scheduler.
/// \return previous lock state (1 - locked, 0 - not locked, error code if negative).
int32_t osKernelUnlock (void)
{
    //TODO
    return 0;
}
 
/// Restore the RTOS Kernel scheduler lock state.
/// \param[in]     lock          lock state obtained by \ref osKernelLock or \ref osKernelUnlock.
/// \return new lock state (1 - locked, 0 - not locked, error code if negative).
int32_t osKernelRestoreLock (int32_t lock)
{
    //TODO
    return 0;
}
 
/// Suspend the RTOS Kernel scheduler.
/// \return time in ticks, for how long the system can sleep or power-down.
uint32_t osKernelSuspend (void)
{
    //TODO
    return 0;
}
 
/// Resume the RTOS Kernel scheduler.
/// \param[in]     sleep_ticks   time in ticks for how long the system was in sleep or power-down mode.
void osKernelResume (uint32_t sleep_ticks)
{
    //TODO
    (void)sleep_ticks;
}
 
/// Get the RTOS kernel tick count.
/// \return RTOS kernel current tick count.
uint64_t osKernelGetTickCount (void)
{
    //TODO
    return 0;
}
 
/// Get the RTOS kernel tick frequency.
/// \return frequency of the kernel tick.
uint32_t osKernelGetTickFreq (void)
{
    //TODO
    return 1000;
}
 
/// Get the RTOS kernel system timer count.
/// \return RTOS kernel current system timer count as 32-bit value.
uint32_t osKernelGetSysTimerCount (void)
{
    //TODO
    return osKernelGetTickCount() & 0xFFFFFFFF;
}
 
/// Get the RTOS kernel system timer frequency.
/// \return frequency of the system timer.
uint32_t osKernelGetSysTimerFreq (void)
{
    return osKernelGetTickFreq();
}
 
 
//  ==== Thread Management Functions ====
 
/// Create a thread and add it to Active Threads.
/// \param[in]     func          thread function.
/// \param[in]     argument      pointer that is passed to the thread function as start argument.
/// \param[in]     attr          thread attributes; NULL: default values.
/// \return thread ID for reference by other functions or NULL in case of error.
osThreadId_t osThreadNew (osThreadFunc_t func, void *argument, const osThreadAttr_t *attr)
{
    //TODO
    return (osThreadId_t)NULL;
}

osThreadId_t osThreadContextNew (osThreadFunc_t func, void *argument, const osThreadAttr_t *attr, void *context)
{
    (void)context;
    return osThreadNew(func, argument, attr);
}
 
/// Get name of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return name as NULL terminated string.
const char *osThreadGetName (osThreadId_t thread_id)
{
    //TODO
    return NULL;
}

/// Return the thread ID of the current running thread.
/// \return thread ID for reference by other functions or NULL in case of error.
osThreadId_t osThreadGetId (void)
{
    //TODO
    return (osThreadId_t)0;
}

/// Get current thread state of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return current thread state of the specified thread.
osThreadState_t osThreadGetState (osThreadId_t thread_id)
{
    //TODO
    return osThreadError;
}
 
/// Get stack size of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return stack size in bytes.
uint32_t osThreadGetStackSize (osThreadId_t thread_id)
{
    //TODO
    return 0;
}
 
/// Get available stack space of a thread based on stack watermark recording during execution.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return remaining stack space in bytes.
uint32_t osThreadGetStackSpace (osThreadId_t thread_id)
{
    //TODO
    return 0;
}
 
/// Change priority of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \param[in]     priority      new priority value for the thread function.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadSetPriority (osThreadId_t thread_id, osPriority_t priority)
{
    //TODO
    return osError;
}
 
/// Get current priority of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return current priority value of the specified thread.
osPriority_t osThreadGetPriority (osThreadId_t thread_id)
{
    //TODO
    return osPriorityError;
}
 
/// Pass control to next thread that is in state \b READY.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadYield (void)
{
    return osOK;
}
 
/// Suspend execution of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadSuspend (osThreadId_t thread_id)
{
    //TODO
    return osError;
}
 
/// Resume execution of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadResume (osThreadId_t thread_id)
{
    //TODO
    return osError;
}
 
/// Detach a thread (thread storage can be reclaimed when thread terminates).
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadDetach (osThreadId_t thread_id)
{
    //TODO
    return osError;
}
 
/// Wait for specified thread to terminate.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadJoin (osThreadId_t thread_id)
{
    //TODO
    return osError;
}
 
/// Terminate execution of current running thread.
__NO_RETURN void osThreadExit (void)
{
    //TODO
    while (1) {
        // Should never reach here
    }
}
 
/// Terminate execution of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadTerminate (osThreadId_t thread_id)
{
    //TODO
    return osError;
}
 
/// Get number of active threads.
/// \return number of active threads.
uint32_t osThreadGetCount (void)
{
    // TODO
    return 0;
}
 
/// Enumerate active threads.
/// \param[out]    thread_array  pointer to array for retrieving thread IDs.
/// \param[in]     array_items   maximum number of items in array for retrieving thread IDs.
/// \return number of enumerated threads.
uint32_t osThreadEnumerate (osThreadId_t *thread_array, uint32_t array_items)
{
    // TODO
    return 0;
}
 
 
//  ==== Thread Flags Functions ====
 
/// Set the specified Thread Flags of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \param[in]     flags         specifies the flags of the thread that shall be set.
/// \return thread flags after setting or error code if highest bit set.
uint32_t osThreadFlagsSet (osThreadId_t thread_id, uint32_t flags)
{
    //TODO
    return 0;
}
 
/// Clear the specified Thread Flags of current running thread.
/// \param[in]     flags         specifies the flags of the thread that shall be cleared.
/// \return thread flags before clearing or error code if highest bit set.
uint32_t osThreadFlagsClear (uint32_t flags)
{
    //TODO
    return 0;
}
 
/// Get the current Thread Flags of current running thread.
/// \return current thread flags.
uint32_t osThreadFlagsGet (void)
{
    //TODO
    return 0;
}
 
/// Wait for one or more Thread Flags of the current running thread to become signaled.
/// \param[in]     flags         specifies the flags to wait for.
/// \param[in]     options       specifies flags options (osFlagsXxxx).
/// \param[in]     timeout       \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
/// \return thread flags before clearing or error code if highest bit set.
uint32_t osThreadFlagsWait (uint32_t flags, uint32_t options, uint32_t timeout)
{
    //TODO
    return 0;
}
 
 
//  ==== Generic Wait Functions ====
 
/// Wait for Timeout (Time Delay).
/// \param[in]     ticks         \ref CMSIS_RTOS_TimeOutValue "time ticks" value
/// \return status code that indicates the execution status of the function.
osStatus_t osDelay (uint32_t ticks)
{
    //TODO
    return osOK;
}
 
/// Wait until specified time.
/// \param[in]     ticks         absolute time in ticks
/// \return status code that indicates the execution status of the function.
osStatus_t osDelayUntil (uint64_t ticks)
{
    //TODO
    return osError;
}
 
 
//  ==== Timer Management Functions ====
 
/// Create and Initialize a timer.
/// \param[in]     func          start address of a timer call back function.
/// \param[in]     type          osTimerOnce for one-shot or osTimerPeriodic for periodic behavior.
/// \param[in]     argument      argument to the timer call back function.
/// \param[in]     attr          timer attributes; NULL: default values.
/// \return timer ID for reference by other functions or NULL in case of error.
osTimerId_t osTimerNew (osTimerFunc_t func, osTimerType_t type, void *argument, const osTimerAttr_t *attr)
{
    //TODO
    return (osTimerId_t)0;
}
 
/// Get name of a timer.
/// \param[in]     timer_id      timer ID obtained by \ref osTimerNew.
/// \return name as NULL terminated string.
const char *osTimerGetName (osTimerId_t timer_id)
{
    //TODO
    return 0;
}
 
/// Start or restart a timer.
/// \param[in]     timer_id      timer ID obtained by \ref osTimerNew.
/// \param[in]     ticks         \ref CMSIS_RTOS_TimeOutValue "time ticks" value of the timer.
/// \return status code that indicates the execution status of the function.
osStatus_t osTimerStart (osTimerId_t timer_id, uint32_t ticks)
{
    //TODO
    return osError;
}
 
/// Stop a timer.
/// \param[in]     timer_id      timer ID obtained by \ref osTimerNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osTimerStop (osTimerId_t timer_id)
{
    //TODO
    return osError;
}
 
/// Check if a timer is running.
/// \param[in]     timer_id      timer ID obtained by \ref osTimerNew.
/// \return 0 not running, 1 running.
uint32_t osTimerIsRunning (osTimerId_t timer_id)
{
    return 0;
}
 
/// Delete a timer.
/// \param[in]     timer_id      timer ID obtained by \ref osTimerNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osTimerDelete (osTimerId_t timer_id)
{
    //TODO
    return osError;
}
 
 
//  ==== Event Flags Management Functions ====
 
/// Create and Initialize an Event Flags object.
/// \param[in]     attr          event flags attributes; NULL: default values.
/// \return event flags ID for reference by other functions or NULL in case of error.
osEventFlagsId_t osEventFlagsNew (const osEventFlagsAttr_t *attr)
{
    os_event_flags_t *ef_id;
    if (core_util_is_isr_active) {
        return (osEventFlagsId_t)0;
    }

    bool user_mem = attr->cb_mem && (attr->cb_size >= sizeof(os_event_flags_t));
    if (user_mem) {
        ef_id = (os_event_flags_t *)attr->cb_mem;
    } else {
        ef_id = (os_event_flags_t *)malloc(sizeof(os_event_flags_t));
    }
    memset(ef_id, 0, sizeof(os_semaphore_t));
    ef_id->name         = attr->name;
    ef_id->event_flags  = 0;
    ef_id->state        = 1;
        
    return (osEventFlagsId_t)ef_id;
}
 
/// Get name of an Event Flags object.
/// \param[in]     ef_id         event flags ID obtained by \ref osEventFlagsNew.
/// \return name as NULL terminated string.
const char *osEventFlagsGetName (osEventFlagsId_t ef_id)
{
    os_event_flags_t *event_flag = (os_event_flags_t *)ef_id;
    return event_flag->name;
}
 
/// Set the specified Event Flags.
/// \param[in]     ef_id         event flags ID obtained by \ref osEventFlagsNew.
/// \param[in]     flags         specifies the flags that shall be set.
/// \return event flags after setting or error code if highest bit set.
uint32_t osEventFlagsSet (osEventFlagsId_t ef_id, uint32_t flags)
{
    os_event_flags_t *ef = (os_event_flags_t *)ef_id;

    core_util_critical_section_enter();
    ef->event_flags |= flags;
    core_util_critical_section_exit();

    return ef->event_flags;
}
 
/// Clear the specified Event Flags.
/// \param[in]     ef_id         event flags ID obtained by \ref osEventFlagsNew.
/// \param[in]     flags         specifies the flags that shall be cleared.
/// \return event flags before clearing or error code if highest bit set.
uint32_t osEventFlagsClear (osEventFlagsId_t ef_id, uint32_t flags)
{
    os_event_flags_t *ef = (os_event_flags_t *)ef_id;

    core_util_critical_section_enter();
    ef->event_flags &= ~flags;
    core_util_critical_section_exit();

    return ef->event_flags;
}
 
/// Get the current Event Flags.
/// \param[in]     ef_id         event flags ID obtained by \ref osEventFlagsNew.
/// \return current event flags.
uint32_t osEventFlagsGet (osEventFlagsId_t ef_id)
{
    os_event_flags_t *ef = (os_event_flags_t *)ef_id;
    return ef->event_flags;
}
 
/// Wait for one or more Event Flags to become signaled.
/// \param[in]     ef_id         event flags ID obtained by \ref osEventFlagsNew.
/// \param[in]     flags         specifies the flags to wait for.
/// \param[in]     options       specifies flags options (osFlagsXxxx).
/// \param[in]     timeout       \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
/// \return event flags before clearing or error code if highest bit set.
uint32_t osEventFlagsWait (osEventFlagsId_t ef_id, uint32_t flags, uint32_t options, uint32_t timeout)
{
    os_event_flags_t *ef = (os_event_flags_t *)ef_id;
    mbed::LowPowerTimer ef_timer;
    int pending = timeout;
    ef_timer.start();
    do {
        core_util_critical_section_enter();
        uint32_t event_flags;
        event_flags = ef->event_flags;
        if ((((options & osFlagsWaitAll) != 0U) && ((event_flags & flags) != flags)) ||
            (((options & osFlagsWaitAll) == 0U) && ((event_flags & flags) == 0U))) {
            event_flags = 0U;
        } else {
            ef->event_flags &= ~flags;
        }

        core_util_critical_section_exit();

        if ((timeout !=0) && (timeout > ef_timer.read_us())) {
            ef_timer.stop();
            break;
        }
        pending -= ef_timer.read_us();
        int wait_time = pending < 1000 ? pending : 1000;
        wait_us(wait_time);
        // sleep
    } while(1);
}
 
/// Delete an Event Flags object.
/// \param[in]     ef_id         event flags ID obtained by \ref osEventFlagsNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osEventFlagsDelete (osEventFlagsId_t ef_id)
{
    os_event_flags_t *ef = (os_event_flags_t *)ef_id;
    if (!ef_id->user_mem) {
        free(ef_id);
    }
    return osOK;
}
 
//  ==== Mutex Management Functions ====
 
/// Create and Initialize a Mutex object.
/// \param[in]     attr          mutex attributes; NULL: default values.
/// \return mutex ID for reference by other functions or NULL in case of error.
osMutexId_t osMutexNew (const osMutexAttr_t *attr)
{
    os_mutex_t *mut;
    bool user_mem = attr->cb_mem && (attr->cb_size >= sizeof(os_mutex_t));
    if (user_mem) {
        mut = (os_mutex_t *)attr->cb_mem;
    } else {
        mut = malloc(sizeof(os_mutex_t));
    }
    memset(mut, 0, sizeof(os_mutex_t));
    mut->user_mem = user_mem;
    mut->name = attr->name;

    return (osMutexId_t)mut;
}
 
/// Get name of a Mutex object.
/// \param[in]     mutex_id      mutex ID obtained by \ref osMutexNew.
/// \return name as NULL terminated string.
const char *osMutexGetName (osMutexId_t mutex_id)
{
    os_mutex_t *mut = (os_mutex_t *)mutex_id;
    return mut->name;
}
 
/// Acquire a Mutex or timeout if it is locked.
/// \param[in]     mutex_id      mutex ID obtained by \ref osMutexNew.
/// \param[in]     timeout       \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
/// \return status code that indicates the execution status of the function.
osStatus_t osMutexAcquire (osMutexId_t mutex_id, uint32_t timeout)
{
    //TODO - ISR check
    os_mutex_t *mut = (os_mutex_t *)mutex_id;
    mut->count++;
    return osOK;
}
 
/// Release a Mutex that was acquired by \ref osMutexAcquire.
/// \param[in]     mutex_id      mutex ID obtained by \ref osMutexNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osMutexRelease (osMutexId_t mutex_id)
{
    os_mutex_t *mut = (os_mutex_t *)mutex_id;
    OS_ASSERT(mut->count > 0);
    mut->count--;
    return osOK;
}
 
/// Get Thread which owns a Mutex object.
/// \param[in]     mutex_id      mutex ID obtained by \ref osMutexNew.
/// \return thread ID of owner thread or NULL when mutex was not acquired.
osThreadId_t osMutexGetOwner (osMutexId_t mutex_id)
{
    //TODO
    return (osThreadId_t)0;
}
 
/// Delete a Mutex object.
/// \param[in]     mutex_id      mutex ID obtained by \ref osMutexNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osMutexDelete (osMutexId_t mutex_id)
{
    os_mutex_t *mut = (os_mutex_t *)mutex_id;
    if (!mut->user_mem) {
        free(mut);
    }

    return osOK;
}
 
 
//  ==== Semaphore Management Functions ====
 
/// Create and Initialize a Semaphore object.
/// \param[in]     max_count     maximum number of available tokens.
/// \param[in]     initial_count initial number of available tokens.
/// \param[in]     attr          semaphore attributes; NULL: default values.
/// \return semaphore ID for reference by other functions or NULL in case of error.
osSemaphoreId_t osSemaphoreNew (uint32_t max_count, uint32_t initial_count, const osSemaphoreAttr_t *attr)
{
    os_semaphore_t *semaphore;
    
    // Check parameters
    if ((max_count == 0U) || (max_count > 255) || (initial_count > max_count))
    {
        return NULL;
    }

    bool user_mem = attr->cb_mem && (attr->cb_size >= sizeof(os_semaphore_t));
    if (user_mem) {
        semaphore = (os_semaphore_t *)attr->cb_mem;
    } else {
        semaphore = malloc(sizeof(os_semaphore_t));
    }
    memset(semaphore, 0, sizeof(os_semaphore_t));

    semaphore->name = attr->name;
    semaphore->tokens      = (uint16_t)initial_count;
    semaphore->max_tokens  = (uint16_t)max_count;
    semaphore->state       = 1;
        
    return (osSemaphoreId_t)semaphore;
}
 
/// Get name of a Semaphore object.
/// \param[in]     semaphore_id  semaphore ID obtained by \ref osSemaphoreNew.
/// \return name as NULL terminated string.
const char *osSemaphoreGetName (osSemaphoreId_t semaphore_id)
{
    os_semaphore_t *semaphore = (os_semaphore_t *)semaphore_id;
    return semaphore->name;
}



void try_sem_acquire() {
    // try to acquire
    if (atomic_dec16_nz(&semaphore->tokens) != 0U) {
        // done acquire
        return osOK;
    }
}

/// Acquire a Semaphore token or timeout if no tokens are available.
/// \param[in]     semaphore_id  semaphore ID obtained by \ref osSemaphoreNew.
/// \param[in]     timeout       \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
/// \return status code that indicates the execution status of the function.
osStatus_t osSemaphoreAcquire (osSemaphoreId_t semaphore_id, uint32_t timeout)
{
    os_semaphore_t *semaphore = (os_semaphore_t *)semaphore_id;
    LowPowerTimer sem_timer;
    int pending = timeout;
    sem_timer.start();

    do {
        // try to acquire
        if (atomic_dec16_nz(&semaphore->tokens) != 0U) {
            // done acquire
            sem_timer.stop();
            return osOK;
        }
        if (timeout !=0) && (timeout > sem_timer.read_us()) {
            sem_timer.stop();
            break;
        }
        pending -= sem_timer.read_us();
        int wait_time = pending < 1000 ? pending : 1000;
        wait_us(wait_time);
        // sleep
    }while(1)

    return osError;
}
 
/// Release a Semaphore token that was acquired by \ref osSemaphoreAcquire.
/// \param[in]     semaphore_id  semaphore ID obtained by \ref osSemaphoreNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osSemaphoreRelease (osSemaphoreId_t semaphore_id)
{
    os_semaphore_t *semaphore = (os_semaphore_t *)semaphore_id;
    if (atomic_inc16_lt(&semaphore->tokens, semaphore->max_tokens) < semaphore->max_tokens) {
       return osOK;
    }
    return osError;
}
 
/// Get current Semaphore token count.
/// \param[in]     semaphore_id  semaphore ID obtained by \ref osSemaphoreNew.
/// \return number of tokens available.
uint32_t osSemaphoreGetCount (osSemaphoreId_t semaphore_id)
{
    os_semaphore_t *semaphore = (os_semaphore_t *)semaphore_id;
    return semaphore->tokens;
}
 
/// Delete a Semaphore object.
/// \param[in]     semaphore_id  semaphore ID obtained by \ref osSemaphoreNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osSemaphoreDelete (osSemaphoreId_t semaphore_id)
{
    os_semaphore_t *semaphore = (os_semaphore_t *)semaphore_id;
    if (!semaphore->user_mem) {
        free(semaphore);
    }
    return osOK;
}
 
 
//  ==== Memory Pool Management Functions ====
 
/// Create and Initialize a Memory Pool object.
/// \param[in]     block_count   maximum number of memory blocks in memory pool.
/// \param[in]     block_size    memory block size in bytes.
/// \param[in]     attr          memory pool attributes; NULL: default values.
/// \return memory pool ID for reference by other functions or NULL in case of error.
osMemoryPoolId_t osMemoryPoolNew (uint32_t block_count, uint32_t block_size, const osMemoryPoolAttr_t *attr)
{
    //TODO
    return (osMemoryPoolId_t)0;
}
 
/// Get name of a Memory Pool object.
/// \param[in]     mp_id         memory pool ID obtained by \ref osMemoryPoolNew.
/// \return name as NULL terminated string.
const char *osMemoryPoolGetName (osMemoryPoolId_t mp_id)
{
    //TODO
    return 0;
}
 
/// Allocate a memory block from a Memory Pool.
/// \param[in]     mp_id         memory pool ID obtained by \ref osMemoryPoolNew.
/// \param[in]     timeout       \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
/// \return address of the allocated memory block or NULL in case of no memory is available.
void *osMemoryPoolAlloc (osMemoryPoolId_t mp_id, uint32_t timeout)
{
    //TODO
    return 0;
}
 
/// Return an allocated memory block back to a Memory Pool.
/// \param[in]     mp_id         memory pool ID obtained by \ref osMemoryPoolNew.
/// \param[in]     block         address of the allocated memory block to be returned to the memory pool.
/// \return status code that indicates the execution status of the function.
osStatus_t osMemoryPoolFree (osMemoryPoolId_t mp_id, void *block)
{
    //TODO
    return osError;
}
 
/// Get maximum number of memory blocks in a Memory Pool.
/// \param[in]     mp_id         memory pool ID obtained by \ref osMemoryPoolNew.
/// \return maximum number of memory blocks.
uint32_t osMemoryPoolGetCapacity (osMemoryPoolId_t mp_id)
{
    //TODO
    return 0;
}
 
/// Get memory block size in a Memory Pool.
/// \param[in]     mp_id         memory pool ID obtained by \ref osMemoryPoolNew.
/// \return memory block size in bytes.
uint32_t osMemoryPoolGetBlockSize (osMemoryPoolId_t mp_id)
{
    //TODO
    return 0;
}
 
/// Get number of memory blocks used in a Memory Pool.
/// \param[in]     mp_id         memory pool ID obtained by \ref osMemoryPoolNew.
/// \return number of memory blocks used.
uint32_t osMemoryPoolGetCount (osMemoryPoolId_t mp_id)
{
    //TODO
    return 0;
}
 
/// Get number of memory blocks available in a Memory Pool.
/// \param[in]     mp_id         memory pool ID obtained by \ref osMemoryPoolNew.
/// \return number of memory blocks available.
uint32_t osMemoryPoolGetSpace (osMemoryPoolId_t mp_id)
{
    //TODO
    return 0;
}
 
/// Delete a Memory Pool object.
/// \param[in]     mp_id         memory pool ID obtained by \ref osMemoryPoolNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osMemoryPoolDelete (osMemoryPoolId_t mp_id)
{
    //TODO
    return osError;
}
 
 
//  ==== Message Queue Management Functions ====
 
/// Create and Initialize a Message Queue object.
/// \param[in]     msg_count     maximum number of messages in queue.
/// \param[in]     msg_size      maximum message size in bytes.
/// \param[in]     attr          message queue attributes; NULL: default values.
/// \return message queue ID for reference by other functions or NULL in case of error.
osMessageQueueId_t osMessageQueueNew (uint32_t msg_count, uint32_t msg_size, const osMessageQueueAttr_t *attr)
{
    //TODO
    return (osMessageQueueId_t)0;
}
 
/// Get name of a Message Queue object.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \return name as NULL terminated string.
const char *osMessageQueueGetName (osMessageQueueId_t mq_id)
{
    //TODO
    return 0;
}
 
/// Put a Message into a Queue or timeout if Queue is full.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \param[in]     msg_ptr       pointer to buffer with message to put into a queue.
/// \param[in]     msg_prio      message priority.
/// \param[in]     timeout       \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
/// \return status code that indicates the execution status of the function.
osStatus_t osMessageQueuePut (osMessageQueueId_t mq_id, const void *msg_ptr, uint8_t msg_prio, uint32_t timeout)
{
    //TODO
    return osError;
}
 
/// Get a Message from a Queue or timeout if Queue is empty.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \param[out]    msg_ptr       pointer to buffer for message to get from a queue.
/// \param[out]    msg_prio      pointer to buffer for message priority or NULL.
/// \param[in]     timeout       \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
/// \return status code that indicates the execution status of the function.
osStatus_t osMessageQueueGet (osMessageQueueId_t mq_id, void *msg_ptr, uint8_t *msg_prio, uint32_t timeout)
{
    //TODO
    return osError;
}
 
/// Get maximum number of messages in a Message Queue.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \return maximum number of messages.
uint32_t osMessageQueueGetCapacity (osMessageQueueId_t mq_id)
{
    //TODO
    return 0;
}
 
/// Get maximum message size in a Memory Pool.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \return maximum message size in bytes.
uint32_t osMessageQueueGetMsgSize (osMessageQueueId_t mq_id)
{
    //TODO
    return 0;
}
 
/// Get number of queued messages in a Message Queue.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \return number of queued messages.
uint32_t osMessageQueueGetCount (osMessageQueueId_t mq_id)
{
    //TODO
    return 0;
}
 
/// Get number of available slots for messages in a Message Queue.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \return number of available slots for messages.
uint32_t osMessageQueueGetSpace (osMessageQueueId_t mq_id)
{
    //TODO
    return 0;
}
 
/// Reset a Message Queue to initial empty state.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osMessageQueueReset (osMessageQueueId_t mq_id)
{
    //TODO
    return osError;
}
 
/// Delete a Message Queue object.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osMessageQueueDelete (osMessageQueueId_t mq_id)
{
    //TODO
    return osError;
}
