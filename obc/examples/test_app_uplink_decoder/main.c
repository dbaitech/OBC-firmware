#include "obc_print.h"
#include "obc_scheduler_config.h"
#include "obc_spi_io.h"
#include "comms_manager.h"
#include "uplink_decoder.h"

#include <FreeRTOS.h>
#include <os_task.h>

#include <sys_common.h>
#include <sys_core.h>
#include <gio.h>
#include <sci.h>
#include <string.h>

#define STOP_ON_ERROR(text, _ret)                   \
  errCode = _ret;                                   \
  if (errCode != OBC_ERR_CODE_SUCCESS) {            \
    sciPrintf("%s returned %d\r\n", text, errCode); \
    sciPrintf("Stopping example\r\n");              \
    while (1)                                       \
      ;                                             \
  }                                                 \
  sciPrintf("%s was successful\r\n", text)

static StaticTask_t taskBuffer;
static StackType_t taskStack[1024];

static comms_state_t commsManagerState = COMMS_STATE_UPLINKING;

void vTaskFunction(void *pvParameters) {
  // Use packet encoded for ping immediate command
  packed_ax25_i_frame_t ax25Data = {
      .data =
          {
              0x7e, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
              0xf0, 0x0,  0x1,  0x2,  0x3,  0x4,  0x5,  0x6,  0x7,  0x8,  0x9,  0xa,  0xb,  0xc,  0xd,  0xe,  0xf,
              0xf,  0x8a, 0x5,  0xda, 0xa0, 0xb7, 0x78, 0x22, 0xf8, 0x70, 0xe5, 0x16, 0x31, 0x94, 0xf9, 0x4b, 0x40,
              0x4c, 0x7c, 0xc9, 0x46, 0x61, 0x87, 0x29, 0x69, 0xad, 0xaf, 0xa8, 0x7a, 0x5d, 0x20, 0x7d, 0x70, 0x68,
              0xb6, 0x52, 0xcc, 0x44, 0x72, 0x97, 0xd1, 0x7b, 0x85, 0x90, 0x9b, 0x98, 0x53, 0xd8, 0x8e, 0x9a, 0x17,
              0x57, 0xa5, 0x32, 0xbe, 0x9f, 0x3,  0xb1, 0x11, 0x92, 0x34, 0x23, 0xda, 0xe9, 0x87, 0x46, 0x99, 0xf4,
              0x4a, 0x11, 0x8e, 0xe2, 0x3b, 0xea, 0x7d, 0x8a, 0x52, 0x50, 0x50, 0x20, 0x37, 0x90, 0xbb, 0x80, 0x77,
              0x2a, 0x1c, 0x92, 0xa3, 0x52, 0x46, 0x2e, 0xda, 0x40, 0x71, 0xf5, 0xb6, 0x37, 0xbc, 0x1e, 0xd,  0x6d,
              0x81, 0x60, 0x66, 0x63, 0x98, 0x13, 0x8c, 0x75, 0xf3, 0x37, 0xc4, 0x22, 0x2c, 0x9a, 0x87, 0xd4, 0x89,
              0x11, 0x1a, 0x59, 0xf7, 0x1c, 0x36, 0x39, 0xaa, 0x99, 0xf,  0xba, 0xea, 0x40, 0x88, 0x0,  0x87, 0x5a,
              0xc7, 0xce, 0x96, 0x26, 0x16, 0xcb, 0x77, 0xa1, 0xac, 0xd9, 0x19, 0xe2, 0xb0, 0x8b, 0x14, 0x6e, 0xc3,
              0xe9, 0xdb, 0xd4, 0x47, 0xa3, 0x6e, 0x1b, 0xca, 0x89, 0x87, 0xc,  0x59, 0x6b, 0x6c, 0x13, 0xa7, 0xe,
              0xfb, 0x19, 0x56, 0xf0, 0x7b, 0x90, 0xd5, 0x3,  0x82, 0x2f, 0x86, 0xb2, 0x16, 0x7b, 0x3,  0x24, 0x6,
              0xc0, 0xba, 0xda, 0xf5, 0xc,  0x61, 0x93, 0xec, 0xc4, 0x7a, 0xee, 0xeb, 0xb0, 0xa3, 0x89, 0xf2, 0xbe,
              0x4d, 0x16, 0x46, 0x6c, 0x90, 0xee, 0x80, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
              0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
              0x0,  0x0,  0x0,  0x0,  0x7c, 0x4f, 0x40, 0x7e,
          },
      .length = 280};
  sciPrintf("Start sending data to the decode data queue\r\n");
  obc_error_code_t errCode;
  for (uint16_t i = 0; i < ax25Data.length; i++) {
    sciPrintf("Sending data at %d\r\n", i);
    STOP_ON_ERROR("Sending data", sendToDecodeDataQueue(&ax25Data.data[i]));
  }
  sciPrintf("Data finished sending. Sending UPLINK FINISHED EVENT to comms manager\r\n");

  comms_event_t uplinkFinishedEvent = {.eventID = COMMS_EVENT_UPLINK_FINISHED};
  STOP_ON_ERROR("sendToCommsManagerQueue", sendToCommsManagerQueue(&uplinkFinishedEvent));
  sciPrintf("Finished demo\r\n");
  while (1)
    ;
}

int main(void) {
  sciInit();
  spiInit();

  initSciPrint();
  initSpiMutex();

  obcSchedulerInitTask(OBC_SCHEDULER_CONFIG_ID_LOGGER);
  obcSchedulerCreateTask(OBC_SCHEDULER_CONFIG_ID_LOGGER);

  obcSchedulerInitTask(OBC_SCHEDULER_CONFIG_ID_ALARM_MGR);
  obcSchedulerInitTask(OBC_SCHEDULER_CONFIG_ID_TELEMETRY_MGR);
  obcSchedulerInitTask(OBC_SCHEDULER_CONFIG_ID_COMMAND_MGR);
  obcSchedulerInitTask(OBC_SCHEDULER_CONFIG_ID_COMMS_MGR);
  obcSchedulerInitTask(OBC_SCHEDULER_CONFIG_ID_COMMS_UPLINK_DECODER);
  obcSchedulerInitTask(OBC_SCHEDULER_CONFIG_ID_COMMS_DOWNLINK_ENCODER);

  obcSchedulerCreateTask(OBC_SCHEDULER_CONFIG_ID_ALARM_MGR);
  obcSchedulerCreateTask(OBC_SCHEDULER_CONFIG_ID_TELEMETRY_MGR);
  obcSchedulerCreateTask(OBC_SCHEDULER_CONFIG_ID_COMMAND_MGR);
  obcSchedulerCreateTaskWithArgs(OBC_SCHEDULER_CONFIG_ID_COMMS_MGR, &commsManagerState);
  obcSchedulerCreateTask(OBC_SCHEDULER_CONFIG_ID_COMMS_UPLINK_DECODER);
  obcSchedulerCreateTask(OBC_SCHEDULER_CONFIG_ID_COMMS_DOWNLINK_ENCODER);

  xTaskCreateStatic(vTaskFunction, "Demo", 1024, NULL, 1, taskStack, &taskBuffer);

  vTaskStartScheduler();

  while (1)
    ;
}
