#ifndef FREERTOS_TASKS_C_ADDITIONS_H
#define FREERTOS_TASKS_C_ADDITIONS_H

#include <stdint.h>

int pcTaskSetName(TaskHandle_t xTaskToConfigure, const char *pcName) /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
{
    TCB_t *pxTCB;
    pxTCB = prvGetTCBFromHandle(xTaskToConfigure);
    const char *pcname = pxTCB->pcTaskName;

    /* Store the task name in the TCB. */
    if (pcName != NULL) {
        for (int x = (UBaseType_t)0; x < (UBaseType_t)configMAX_TASK_NAME_LEN; x++) {
            pxTCB->pcTaskName[x] = pcName[x];

            if (pcName[x] == (char)0x00) {
                break;
            } else {
                mtCOVERAGE_TEST_MARKER();
            }
        }

        /* Ensure the name string is terminated in the case that the string length
		was greater or equal to configMAX_TASK_NAME_LEN. */
        pxTCB->pcTaskName[configMAX_TASK_NAME_LEN - 1] = '\0';
    } 
}

#endif