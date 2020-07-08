#ifndef FREERTOS_TASKS_H_ADDITIONS_H
#define FREERTOS_TASKS_H_ADDITIONS_H


/**
 * task. h
 * <PRE>int pcTaskSetName( TaskHandle_t xTaskToConfigure, const char *pcName);</PRE>
 *
 *
 * \defgroup pcTaskSetName pcTaskSetName
 * \ingroup TaskUtils
 */
void pcTaskSetName(TaskHandle_t xTaskToConfigure, const char *pcName);

#endif