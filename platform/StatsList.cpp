/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "platform/StatsList.h"
#include <string.h>

namespace mbed {

StatsList *StatsList::_head = NULL;
SingletonPtr<PlatformMutex> StatsList::_mutex;

StatsList::StatsList() 
{
}

StatsList::StatsList(mbed_stats_type_t type)
{
    // Add to head of list
    _mutex->lock();
    _next = _head;
    _head = this;
    _next = NULL;
    _mutex->unlock();
}

StatsList::~StatsList()
{
    _mutex->lock();
    // Remove this object from the list
    if (_head == this) {
        _head = _next;
    } else {
        StatsList *p = _head;
        while (p->_next != this) {
            p = p->_next;
        }
        p->_next = _next;
    }
    _mutex->unlock();
}

int StatsList::get_each(void *stats, int count)
{    
    MBED_ASSERT(stats != NULL);
    interface_info_t *info = (interface_info_t *)stats;
    memset(info, 0, count * sizeof(interface_info_t));

    _mutex->lock();
    StatsList *list = _head;
    
    int i;
    for (i = 0; (i < count) && (list != NULL); i++) {
        list->read_stats((void *)&info[i]);
        list = list->_next;
    }
    _mutex->unlock();
    return i;
}

} // namespace mbed
