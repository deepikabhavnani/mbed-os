/*------------------------------------------------------------------------------
 * MDK - Component ::Event Recorder
 * Copyright (c) 2016-2017 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    EventRecorder.c
 * Purpose: Event Recorder for Debug Messages
 * Rev.:    V1.2.2
 *----------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include CMSIS_device_header
 
#include <string.h>
#include "EventRecorder.h"
#include "EventRecorderConf.h"
 
#if !defined(__USED)
  #define __USED __attribute__((used))
#endif
#if !defined(__WEAK)
  #define __WEAK __attribute__((weak))
#endif
#if !defined(__ALIGNED)
  #define __ALIGNED(x) __attribute__((aligned(x)))
#endif
#if !defined(__NO_INIT)
 #if   defined (__CC_ARM)                                           /* ARM Compiler 4/5 */
  #define __NO_INIT __attribute__ ((section (".bss.noinit"), zero_init))
 #elif defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)    /* ARM Compiler 6 */
  #define __NO_INIT __attribute__ ((section (".bss.noinit")))
 #elif defined (__GNUC__)                                           /* GNU Compiler */
  #define __NO_INIT __attribute__ ((section (".bss.noinit")))
 #else
  #warning "No compiler specific solution for __NO_INIT. __NO_INIT is ignored."
  #define __NO_INIT
 #endif
#endif
 
#define INT_LOG2(n) \
 ((n) == (1U<<31) ? 31 : (n) == (1U<<30) ? 30 : (n) == (1U<<29) ? 29 : (n) == (1U<<28) ? 28 : \
  (n) == (1U<<27) ? 27 : (n) == (1U<<26) ? 26 : (n) == (1U<<25) ? 25 : (n) == (1U<<24) ? 24 : \
  (n) == (1U<<23) ? 23 : (n) == (1U<<22) ? 22 : (n) == (1U<<21) ? 21 : (n) == (1U<<20) ? 20 : \
  (n) == (1U<<19) ? 19 : (n) == (1U<<18) ? 18 : (n) == (1U<<17) ? 17 : (n) == (1U<<16) ? 16 : \
  (n) == (1U<<15) ? 15 : (n) == (1U<<14) ? 14 : (n) == (1U<<13) ? 13 : (n) == (1U<<12) ? 12 : \
  (n) == (1U<<11) ? 11 : (n) == (1U<<10) ? 10 : (n) == (1U<< 9) ?  9 : (n) == (1U<< 8) ?  8 : \
  (n) == (1U<< 7) ?  7 : (n) == (1U<< 6) ?  6 : (n) == (1U<< 5) ?  5 : (n) == (1U<< 4) ?  4 : \
  (n) == (1U<< 3) ?  3 : (n) == (1U<< 2) ?  2 : (n) == (1U<< 1) ?  1 : (n) == (1U<< 0) ?  0 : -1)
 
/* Event Recorder Component */
#define CID_EVENT               0xFF
 
/* Event Recorder Messages */
#define MID_EVENT_INIT          0x00    // Initialization
#define MID_EVENT_START         0x01    // Start Recorder
#define MID_EVENT_STOP          0x02    // Stop Recorder
 
#define ID_EVENT_INIT   ((CID_EVENT << 8) | MID_EVENT_INIT   | EVENT_RECORD_FIRST | EVENT_RECORD_LAST)
#define ID_EVENT_START  ((CID_EVENT << 8) | MID_EVENT_START  | EVENT_RECORD_FIRST | EVENT_RECORD_LAST)
#define ID_EVENT_STOP   ((CID_EVENT << 8) | MID_EVENT_STOP   | EVENT_RECORD_FIRST | EVENT_RECORD_LAST)
 
/* Event Recorder Signature */
#define SIGNATURE               0xE1A5276BU
 
/* Number of Records in Event Buffer */
#define EVENT_RECORD_COUNT_BITS INT_LOG2(EVENT_RECORD_COUNT)
#if ((EVENT_RECORD_COUNT_BITS > 20) || (EVENT_RECORD_COUNT_BITS < 3))
#error "Invalid number of Event Buffer Records!"
#endif
 
/* Event Data Maximum Length */
#if ((EVENT_RECORD_COUNT * 8U) > 1024U)
#define EVENT_DATA_MAX_LENGTH   1024U
#else
#define EVENT_DATA_MAX_LENGTH   (EVENT_RECORD_COUNT * 8U)
#endif
 
/* Event Record Information */
#define EVENT_RECORD_ID_MASK    0x0000FFFFU
#define EVENT_RECORD_DLEN_POS   16
#define EVENT_RECORD_DLEN_MASK  0x00070000U
#define EVENT_RECORD_CTX_POS    16
#define EVENT_RECORD_CTX_MASK   0x00070000U
#define EVENT_RECORD_IRQ        0x00080000U
#define EVENT_RECORD_SEQ_POS    20
#define EVENT_RECORD_SEQ_MASK   0x00F00000U
#define EVENT_RECORD_FIRST      0x01000000U
#define EVENT_RECORD_LAST       0x02000000U
#define EVENT_RECORD_LOCKED     0x04000000U
#define EVENT_RECORD_VALID      0x08000000U
#define EVENT_RECORD_MSB_TS     0x10000000U
#define EVENT_RECORD_MSB_VAL1   0x20000000U
#define EVENT_RECORD_MSB_VAL2   0x40000000U
#define EVENT_RECORD_TBIT       0x80000000U
 
/* Event Record */
typedef struct {
  uint32_t ts;                  // Timestamp (32-bit, Toggle bit instead of MSB)
  uint32_t val1;                // Value 1   (32-bit, Toggle bit instead of MSB)
  uint32_t val2;                // Value 2   (32-bit, Toggle bit instead of MSB)
  uint32_t info;                // Record Information
                                //  [ 7.. 0]: Message ID (8-bit)
                                //  [15.. 8]: Component ID (8-bit)
                                //  [18..16]: Data Length (1..8) / Event Context
                                //      [19]: IRQ Flag
                                //  [23..20]: Sequence Number
                                //      [24]: First Record
                                //      [25]: Last Record
                                //      [26]: Locked Record
                                //      [27]: Valid Record
                                //      [28]: Timestamp MSB
                                //      [29]: Value 1 MSB
                                //      [30]: Value 2 MSB
                                //      [31]: Toggle bit
} EventRecord_t;
 
/* Event Buffer */
static EventRecord_t EventBuffer[EVENT_RECORD_COUNT] __NO_INIT __ALIGNED(16);
 
/* Event Filter: 1024 enable bits for 8-bit Component ID with 2-bit Level */
static uint8_t EventFilter[128] __NO_INIT __ALIGNED(4);
 
/* Event Recorder Status */
typedef struct {
  uint8_t  state;               // Recorder State: 0 - Inactive, 1 - Running
  uint8_t  context;             // Current Event Context
  uint16_t info_crc;            // EventRecorderInfo CRC16-CCITT
  uint32_t record_index;        // Current Record Index
  uint32_t records_written;     // Number of records written
  uint32_t records_dumped;      // Number of records dumped
  uint32_t ts_overflow;         // Timestamp overflow counter
  uint32_t ts_freq;             // Timestamp frequency
  uint32_t ts_last;             // Timestamp last value
  uint32_t init_count;          // Initialization counter
  uint32_t signature;           // Initialization signature
} EventStatus_t;
 
static EventStatus_t EventStatus __NO_INIT __ALIGNED(64);
 
typedef struct {
  uint32_t last_record_index;   // Index of last record fetched
  uint32_t records_lost;        // Number of records lost
} EventReaderStatus_t;

static EventReaderStatus_t EventReaderStatus __NO_INIT __ALIGNED(8);

/* Global Event Recorder Information */
typedef struct {
  uint8_t    protocol_type;     // Protocol Type: 1 - DAP
  uint8_t    reserved;          // Reserved (must be zero)
  uint16_t   protocol_version;  // Protocol Version: [15..8]=major, [7..0]=minor
  // Version 1.0 specific information
  uint32_t       record_count;  // Number of Records in Event Buffer
  EventRecord_t *event_buffer;  // Pointer to Event Buffer
  uint8_t       *event_filter;  // Pointer to Event Filter
  EventStatus_t *event_status;  // Pointer to Event Status
  uint8_t        ts_source;     // Timestamp source
  uint8_t        reserved3[3];  // Reserved (must be zero)
} EventRecorderInfo_t;
 
extern const EventRecorderInfo_t EventRecorderInfo;
__USED const EventRecorderInfo_t EventRecorderInfo =
{
  1U, 0U,
  (1U << 8) | 0U,               // Protocol Version 1.0
  EVENT_RECORD_COUNT,
  &EventBuffer[0],
  &EventFilter[0],
  &EventStatus,
  EVENT_TIMESTAMP_SOURCE,
  { 0U, 0U, 0U }
};
 
 
/* Atomic operations helper functions */
 
static uint8_t atomic_inc8 (uint8_t *mem) {
#if (__CORTEX_M >= 3U)
  register uint8_t ret;
 
  do {
    ret = __LDREXB(mem);
  } while (__STREXB(ret + 1U, mem));
 
  return ret;
#else
  uint32_t irqmask = __get_PRIMASK();
  uint8_t  ret;
 
  __disable_irq();
  ret = *mem;
  *mem = ret + 1U;
  if (irqmask == 0U) {
    __enable_irq();
  }
 
  return ret;
#endif
}
 
static uint32_t atomic_inc32 (uint32_t *mem) {
#if (__CORTEX_M >= 3U)
  register uint32_t ret;
 
  do {
    ret = __LDREXW(mem);
  } while (__STREXW(ret + 1U, mem));
 
  return ret;
#else
  uint32_t irqmask = __get_PRIMASK();
  uint32_t ret;
 
  __disable_irq();
  ret = *mem;
  *mem = ret + 1U;
  if (irqmask == 0U) {
    __enable_irq();
  }
 
  return ret;
#endif
}
 
static uint32_t atomic_xch32 (uint32_t *mem, uint32_t val) {
#if (__CORTEX_M >= 3U)
  register uint32_t ret;
 
  do {
    ret = __LDREXW(mem);
  } while (__STREXW(val, mem));
 
  return ret;
#else
  uint32_t irqmask = __get_PRIMASK();
  uint32_t ret;
 
  __disable_irq();
  ret = *mem;
  *mem = val;
  if (irqmask == 0U) {
    __enable_irq();
  }
 
  return ret;
#endif
}
 
 
__STATIC_INLINE uint32_t GetContext (void) {
  return ((uint32_t)atomic_inc8(&EventStatus.context));
}
 
__STATIC_INLINE uint32_t GetRecordIndex (void) {
  return (atomic_inc32(&EventStatus.record_index));
}

__STATIC_INLINE uint32_t GetLastFetchIndex (void) {
  return EventReaderStatus.last_record_index;
}

__STATIC_INLINE uint32_t UpdateLastFetchIndex (uint32_t index) {
  return (atomic_xch32(&EventReaderStatus.last_record_index, index));
}

__STATIC_INLINE uint32_t GetRecordsLost (void) {
  return EventReaderStatus.records_lost;
}

__STATIC_INLINE uint32_t UpdateLostRecords (uint32_t count) {
  return (atomic_xch32(&EventReaderStatus.records_lost, count));
}

__STATIC_INLINE uint32_t UpdateTS (uint32_t ts) {
  return (atomic_xch32(&EventStatus.ts_last, ts));
}
 
__STATIC_INLINE uint32_t IncrementOverflowTS (void) {
  return (atomic_inc32(&EventStatus.ts_overflow));
}
 
__STATIC_INLINE void IncrementRecordsWritten (void) {
  atomic_inc32(&EventStatus.records_written);
}
 
__STATIC_INLINE void IncrementRecordsDumped (void) {
  atomic_inc32(&EventStatus.records_dumped);
}
 
__STATIC_INLINE uint32_t LockRecord (uint32_t *mem) {
#if (__CORTEX_M >= 3U)
  register uint32_t rval, wval;
 
  do {
    rval = __LDREXW(mem);
    if ((rval & EVENT_RECORD_LOCKED)) {
      wval = rval & ~EVENT_RECORD_VALID;
    } else {
      wval = rval | (EVENT_RECORD_VALID | EVENT_RECORD_LOCKED);
    }
  } while (__STREXW(wval, mem));
 
  return rval;
#else
  uint32_t irqmask = __get_PRIMASK();
  uint32_t rval, wval;
 
  __disable_irq();
  rval = *mem;
  if ((rval & EVENT_RECORD_LOCKED)) {
    wval = rval & ~EVENT_RECORD_VALID;
  } else {
    wval = rval | (EVENT_RECORD_VALID | EVENT_RECORD_LOCKED);
  }
  *mem = wval;
  if (irqmask == 0U) {
    __enable_irq();
  }
 
  return rval;
#endif
}
 
__STATIC_INLINE uint32_t UnlockRecord (uint32_t *mem, uint32_t info) {
#if (__CORTEX_M >= 3U)
  register uint32_t val, ret;
 
  do {
    val = __LDREXW(mem);
    if ((val & EVENT_RECORD_VALID)) {
      val = info |  EVENT_RECORD_VALID;
      ret = 1U;
    } else {
      val = (val & ~EVENT_RECORD_LOCKED) ^ EVENT_RECORD_TBIT;
      ret = 0U;
    }
  } while (__STREXW(val, mem));
 
  return ret;
#else
  uint32_t irqmask = __get_PRIMASK();
  uint32_t val, ret;
 
  __disable_irq();
  val = *mem;
  if ((val & EVENT_RECORD_VALID)) {
    val = info |  EVENT_RECORD_VALID;
    ret = 1U;
  } else {
    val = (val & ~EVENT_RECORD_LOCKED) ^ EVENT_RECORD_TBIT;
    ret = 0U;
  }
  *mem = val;
  if (irqmask == 0U) {
    __enable_irq();
  }
 
  return ret;
#endif
}
 
 
/**
  Calculate CRC16-CCITT (16-bit, polynom=0x1021, init_value=0xFFFF)
  \param[in]    data  pointer to data
  \param[in]    len   data length (in bytes)
  \return             CRC16-CCITT value
*/
static uint16_t crc16_ccitt (const uint8_t *data, uint32_t len) {
  uint16_t crc;
  uint32_t n;
 
  crc = 0xFFFFU;
  while (len) {
    crc ^= (*data++ << 8);
    for (n = 8U; n != 0U; n--) {
      if (crc & 0x8000U) {
        crc <<= 1;
        crc  ^= 0x1021U;
      } else {
        crc <<= 1;
      }
    }
    len--;
  }
 
  return crc;
}
 
 
/**
  Record a single item
  \param[in]    id     event identifier (component, message with context & first/last flags)
  \param[in]    ts     timestamp
  \param[in]    val1   first data value
  \param[in]    val2   second data value
  \return       status (1=Success, 0=Failure)
*/
static uint32_t EventRecordItem (uint32_t id, uint32_t ts, uint32_t val1, uint32_t val2) {
  EventRecord_t *record;
  uint32_t cnt, i;
  uint32_t info;
  uint32_t tbit;
  uint32_t seq;
 
  for (cnt = EVENT_RECORD_COUNT; cnt; cnt--) {
    i = GetRecordIndex();
    record = &EventBuffer[i & (EVENT_RECORD_COUNT - 1U)];
    info = LockRecord(&record->info);
    if ((info & EVENT_RECORD_LOCKED) == 0U) {
      seq  = (info + (1U << EVENT_RECORD_SEQ_POS)) & EVENT_RECORD_SEQ_MASK;
      tbit = (info ^ EVENT_RECORD_TBIT) & EVENT_RECORD_TBIT;
      record->ts   = (ts   & ~EVENT_RECORD_TBIT) | tbit;
      record->val1 = (val1 & ~EVENT_RECORD_TBIT) | tbit;
      record->val2 = (val2 & ~EVENT_RECORD_TBIT) | tbit;
      info =  id                                   |
              seq                                  |
             ((ts   >> 3) & EVENT_RECORD_MSB_TS)   |
             ((val1 >> 2) & EVENT_RECORD_MSB_VAL1) |
             ((val2 >> 1) & EVENT_RECORD_MSB_VAL2) |
              tbit;
      if (UnlockRecord(&record->info, info) != 0U) {
        IncrementRecordsWritten();
        return 1U;
      }
    }
  }
 
  IncrementRecordsDumped();
  return 0U;
}

/**
  Fetch record Items
  \param[in]    buf Pointer to buffer data
  \return       The number of mbed_stats_stack_t structures that have been filled,
                this is equal to the number of stacks on the system.
*/
uint32_t EventFetchItems (uint32_t *buf) {

  uint32_t i = GetRecordIndex() - 1;    // Last filled record index
  uint32_t x = GetLastFetchIndex();     // Last index fetched
  EventRecord_t *records = (EventRecord_t *)buf;

  if ((i > x) && ((i - x) > EVENT_RECORD_COUNT)) {
      uint32_t count = GetRecordsLost();
      count += (i - x - EVENT_RECORD_COUNT);
      UpdateLostRecords(count);
      x = i - EVENT_RECORD_COUNT;
  }

  uint32_t cnt = 0;
  for (; x < i; cnt++) {
    EventRecord_t *current = &EventBuffer[x & (EVENT_RECORD_COUNT - 1U)];
    memcpy(&records[cnt], current, sizeof(EventRecord_t));
    x++;
  }
  UpdateLastFetchIndex(i);
  return cnt;
}
 
/**
  Check event filter based on specified level and component
  \param[in]    id     event identifier (level, component number, message number)
  \return              1=Enabled, 0=Disabled
*/
__STATIC_INLINE uint32_t EventCheckFilter (uint32_t id) {
  if (EventStatus.state == 0U) {
    return 0U;
  }
  return ((EventFilter[(id >> (8 + 3)) & 0x7FU] >> ((id >> 8) & 0x7U)) & 1U);
}
 
/**
  Get timestamp and handle overflow
 
  \return       timestamp (32-bit)
*/
static uint32_t EventGetTS (void) {
  uint32_t ts;
  uint32_t ts_last;
 
  ts = EventRecorderTimerGet();
  ts_last = UpdateTS(ts);
 
  if (ts < ts_last) {
    IncrementOverflowTS();
  }
 
  return (ts);
}
 
 
#if   (EVENT_TIMESTAMP_SOURCE == 0)
#if ((__CORTEX_M < 3U) || (__CORTEX_M == 23U))
static uint32_t TimeStamp __NO_INIT;
#endif

#elif (EVENT_TIMESTAMP_SOURCE == 1)
 
/* SysTick Period in cycles */
#define SYSTICK_PERIOD  ((uint32_t)(((uint64_t)(SYSTICK_PERIOD_US)*(SYSTICK_CLOCK))/1000000U))
 
/* SysTick Variables */
static volatile uint32_t SysTickOverflowCounter = 0U;
static volatile uint8_t  SysTickOverflowUpdated = 0U;
 
/* SysTick IRQ Handler */
void SysTick_Handler (void);
void SysTick_Handler (void) {
  SysTickOverflowCounter++;
  SysTickOverflowUpdated = 1U;
}
 
/* Initialize SysTick */
__STATIC_INLINE uint32_t SysTickInit (void) {
  SysTick->LOAD = SYSTICK_PERIOD - 1U;
  SysTick->VAL  = 0U;
  SysTick->CTRL = SysTick_CTRL_ENABLE_Msk     |
                  SysTick_CTRL_TICKINT_Msk    |
                  SysTick_CTRL_CLKSOURCE_Msk;
  return (SYSTICK_CLOCK);
}
 
/* Get SysTick Time */
__STATIC_INLINE uint32_t SysTickGet (void) {
  uint32_t load;
  uint32_t val;
  uint32_t ovf;

  load = SysTick->LOAD;
  do {
    SysTickOverflowUpdated = 0U;
    val = SysTick->VAL;
    ovf = SysTickOverflowCounter;
  } while (SysTickOverflowUpdated != 0U);

  return (((load + 1U) * ovf) + (load - val));
}
 
#elif (EVENT_TIMESTAMP_SOURCE == 2)
#ifdef RTE_CMSIS_RTOS2
#include "cmsis_os2.h"
#else
#include "cmsis_os.h"
#endif
#endif
 
 
/**
  Initialize timer hardware
  \return       timer frequency in Hz
*/
#if (EVENT_TIMESTAMP_SOURCE < 3)
__WEAK uint32_t EventRecorderTimerInit (void) {
#if   (EVENT_TIMESTAMP_SOURCE == 0)
  #if ((__CORTEX_M >= 3U) && (__CORTEX_M != 23U))
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    return SystemCoreClock;
  #else
    TimeStamp = 0U;
    return 0U;
  #endif
#elif (EVENT_TIMESTAMP_SOURCE == 1)
  return (SysTickInit());
#elif (EVENT_TIMESTAMP_SOURCE == 2)
  return SystemCoreClock;
#endif
}
#endif
 
/**
  Get Time from timer hardware
  \return       timer value (32-bit)
*/
#if (EVENT_TIMESTAMP_SOURCE < 3)
__WEAK uint32_t EventRecorderTimerGet (void) {
#if   (EVENT_TIMESTAMP_SOURCE == 0)
  #if ((__CORTEX_M >= 3U) && (__CORTEX_M != 23U))
    return (DWT->CYCCNT);
  #else
    return (TimeStamp++);
  #endif
#elif (EVENT_TIMESTAMP_SOURCE == 1)
  return (SysTickGet());
#elif (EVENT_TIMESTAMP_SOURCE == 2)
  #if (defined(osCMSIS) && (osCMSIS < 0x20000U))
    return (osKernelSysTick());
  #else
    return (osKernelGetSysTimerCount());
  #endif
#endif
}
#endif
 
 
/**
  Initialize Event Recorder
  \param[in]    recording   initial level mask for event record filter
  \param[in]    start       initial recording setup (1=start, 0=stop)
  \return       status (1=Success, 0=Failure)
*/
uint32_t EventRecorderInitialize (uint32_t recording, uint32_t start) {
  EventRecord_t *record;
  uint16_t crc;
  uint32_t ts;
  uint32_t n;
 
  crc = crc16_ccitt((const uint8_t *)&EventRecorderInfo, sizeof(EventRecorderInfo));
 
  if (EventStatus.signature != SIGNATURE) {
    EventStatus.signature = SIGNATURE;
    EventStatus.info_crc = crc;
    EventStatus.init_count = 1U;
  } else {
    if (EventStatus.info_crc != crc) {
      EventStatus.info_crc = crc;
      EventStatus.init_count = 1U;
    } else {
      EventStatus.init_count++;
    }
  }
 
  if (EventStatus.init_count == 1U) {
    memset(&EventBuffer[0], 0, sizeof(EventBuffer));
    memset(&EventFilter[0], 0, sizeof(EventFilter));
    EventRecorderEnable(recording, 0x00U, 0xFEU);
    EventStatus.state = (start != 0U) ? 1U : 0U;
    EventStatus.context         = 0U;
    EventStatus.record_index    = 0U;
    EventStatus.records_written = 0U;
    EventStatus.records_dumped  = 0U;
    EventStatus.ts_freq = EventRecorderTimerInit();
    EventStatus.ts_last = 0U;
    EventStatus.ts_overflow = 0U;
    EventReaderStatus.last_record_index = 0U;
    EventReaderStatus.records_lost = 0U;
  } else {
    for (n = 0U; n < EVENT_RECORD_COUNT; n++) {
      record = &EventBuffer[n];
      if (record->info & EVENT_RECORD_LOCKED) {
        record->info &= ~(EVENT_RECORD_LOCKED | EVENT_RECORD_VALID);
      }
    }
#if   ((EVENT_TIMESTAMP_SOURCE == 0) && ((__CORTEX_M >= 3U) && (__CORTEX_M != 23U)))
    EventStatus.ts_freq = EventRecorderTimerInit();
#elif ((EVENT_TIMESTAMP_SOURCE >= 1) && (EVENT_TIMESTAMP_SOURCE <= 3))
    EventStatus.ts_freq = EventRecorderTimerInit();
    EventStatus.ts_last = 0U;
    EventStatus.ts_overflow = 0U;
#endif
  }
 
  ts = EventGetTS();
 
  EventRecordItem(ID_EVENT_INIT, ts, EventStatus.init_count, 0U);
 
  return 1U;
}
 
/**
  Enable recording of events with specified level and component range
  \param[in]    recording   level mask for event record filter
  \param[in]    comp_start  first component number of range
  \param[in]    comp_end    last Component number of range
  \return       status (1=Success, 0=Failure)
*/
uint32_t EventRecorderEnable (uint32_t recording, uint32_t comp_start, uint32_t comp_end) {
  uint32_t ofs;
  uint32_t i, j;
 
  if ((comp_start >= 0xFFU) || (comp_end >= 0xFFU)) {
    return 0U;
  }
 
  for (ofs = 0U, i = 0U; i < 4U; i++, ofs += 32U) {
    if ((recording & (1U << i)) != 0U) {
      for (j = comp_start; j <= comp_end; j++) {
        EventFilter[ofs + (j >> 3)] |= (1U << (j & 0x7U));
      }
    }
  }
 
  return 1U;
}
 
/**
  Disable recording of events with specified level and component range
  \param[in]    recording   level mask for event record filter
  \param[in]    comp_start  first component number of range
  \param[in]    comp_end    last Component number of range
  \return       status (1=Success, 0=Failure)
*/
uint32_t EventRecorderDisable (uint32_t recording, uint32_t comp_start, uint32_t comp_end) {
  uint32_t ofs;
  uint32_t i, j;
 
  if ((comp_start >= 0xFFU) || (comp_end >= 0xFFU)) {
    return 0U;
  }
 
  for (ofs = 0U, i = 0U; i < 4U; i++, ofs += 32U) {
    if ((recording & (1U << i)) != 0U) {
      for (j = comp_start; j <= comp_end; j++) {
        EventFilter[ofs + (j >> 3)] &= ~(1U << (j & 0x7U));
      }
    }
  }
 
  return 1U;
}
 
/**
  Start event recording
  \return       status (1=Success, 0=Failure)
*/
uint32_t EventRecorderStart (void) {
  uint32_t ts;
 
  if (EventStatus.state != 0U) {
    return 1U;
  }
 
  ts = EventGetTS();
 
  EventRecordItem(ID_EVENT_START, ts, 0U, 0U);
 
  EventStatus.state = 1U;
 
  return 1U;
}
 
/**
  Stop event recording
  \return       status (1=Success, 0=Failure)
*/
uint32_t EventRecorderStop (void) {
  uint32_t ts;
 
  if (EventStatus.state == 0U) {
    return 1U;
  }
 
  ts = EventGetTS();
 
  EventStatus.state = 0U;
 
  EventRecordItem(ID_EVENT_STOP, ts, 0U, 0U);
 
  return 1U;
}
 
/**
  Record an event with variable data size
  \param[in]    id     event identifier (level, component number, message number)
  \param[in]    data   event data buffer
  \param[in]    len    event data length
  \return       status (1=Success, 0=Failure)
*/
uint32_t EventRecordData (uint32_t id, const void *data, uint32_t len) {
  const uint8_t *dptr;
  uint32_t ts;
  uint32_t ctx;
  uint32_t val[2];
  uint32_t ret;
 
  if ((data == NULL) || (len > EVENT_DATA_MAX_LENGTH)) {
    return 0U;
  }
 
  if (EventCheckFilter(id) == 0U) {
    return 1U;
  }
 
  ts = EventGetTS();
 
  id &= EVENT_RECORD_ID_MASK;
  id |= (__get_IPSR() != 0U) ? EVENT_RECORD_IRQ : 0U;
  dptr = (const uint8_t *)data;
 
  if (len == 0U) {
    ret = EventRecordItem(id, ts, 0U, 0U);
    return (ret);
  }
 
  if (len <= 8U) {
    val[0] = 0U;
    val[1] = 0U;
    memcpy(val, dptr, len);
    id |= (len << EVENT_RECORD_DLEN_POS) & EVENT_RECORD_DLEN_MASK;
    ret = EventRecordItem(id | EVENT_RECORD_FIRST | EVENT_RECORD_LAST, ts, val[0], val[1]);
    return (ret);
  }
 
  ctx = (GetContext() << EVENT_RECORD_CTX_POS) & EVENT_RECORD_CTX_MASK;
 
  memcpy(val, dptr, 8U);
  dptr += 8U;
  len  -= 8U;
  id |= ctx;
  ret = EventRecordItem(id | EVENT_RECORD_FIRST, ts, val[0], val[1]);
  if (ret == 0U) {
    return 0U;
  }
 
  id = 0xFF01U | ctx;
 
  while (len > 8U) {
    memcpy(val, dptr, 8U);
    dptr += 8U;
    len  -= 8U;
    ret = EventRecordItem(id++, ts, val[0], val[1]);
    if (ret == 0U) {
      return 0U;
    }
  }
 
  val[0] = 0U;
  val[1] = 0U;
  memcpy(val, dptr, len);
  id &= ~0xFF00U;
  id |= len << 8;
  ret = EventRecordItem(id | EVENT_RECORD_LAST, ts, val[0], val[1]);
 
  return (ret);
}
 
/**
  Record an event with two 32-bit data values
  \param[in]    id     event identifier (level, component number, message number)
  \param[in]    val1   first data value
  \param[in]    val2   second data value
  \return       status (1=Success, 0=Failure)
*/
uint32_t EventRecord2 (uint32_t id, uint32_t val1, uint32_t val2) {
  uint32_t ts;
  uint32_t ret;
 
  if (EventCheckFilter(id) == 0U) {
    return 1U;
  }
 
  ts = EventGetTS();
 
  id &= EVENT_RECORD_ID_MASK;
  id |= (__get_IPSR() != 0U) ? EVENT_RECORD_IRQ : 0U;
 
  ret = EventRecordItem(id | EVENT_RECORD_FIRST | EVENT_RECORD_LAST, ts, val1, val2);
 
  return (ret);
}
 
/**
  Record an event with four 32-bit data values
  \param[in]    id     event identifier (level, component number, message number)
  \param[in]    val1   first data value
  \param[in]    val2   second data value
  \param[in]    val3   third data value
  \param[in]    val4   fourth data value
  \return       status (1=Success, 0=Failure)
*/
uint32_t EventRecord4 (uint32_t id,
                       uint32_t val1, uint32_t val2, uint32_t val3, uint32_t val4) {
  uint32_t ts;
  uint32_t ctx;
  uint32_t ret;
 
  if (EventCheckFilter(id) == 0U) {
    return 1U;
  }
 
  ts = EventGetTS();
 
  id &= EVENT_RECORD_ID_MASK;
  id |= (__get_IPSR() != 0U) ? EVENT_RECORD_IRQ : 0U;
  ctx = (GetContext() << EVENT_RECORD_CTX_POS) & EVENT_RECORD_CTX_MASK;
 
  ret = EventRecordItem(id | ctx | EVENT_RECORD_FIRST, ts, val1, val2);
  if (ret == 0U) {
    return 0U;
  }
  ret = EventRecordItem(1U | ctx | EVENT_RECORD_LAST,  ts, val3, val4);
 
  return (ret);
}
