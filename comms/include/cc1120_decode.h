#ifndef COMMS_INCLUDE_CC1120_DECODE_H
#define COMMS_INCLUDE_CC1120_DECODE_H

#include "obc_logging.h"

typedef unsigned char aes_block_t[128];

//placeholder until command manager pr gets merged in since it will have this type defined already
/*PLACEHOLDER*/
typedef struct {
	union {
        /*
		tle_data_uplink_t tleDataUplink;
		aro_data_uplink_t aroDataUplink;
        secondary_payload_activation_t secondayPayloadActivation;
        rtc_sync_t rtcSync;
        secondary_payload_activation_time_t secondaryPayloadActivationTime;
        call_eps_mcu_reset_t callEPSMCUReset;
        call_cdh_mcu_reset_t callCDHMCUReset;
		time_delay_img_capture_and_downlink_t timeDelayImgCaptureAndDownlink;
        direct_eps_power_rail_control_t directEPSPowerRailControl;
        comms_echo_back_t commsEchoBack;
        change_value_parm_mnger_t changeValueParmMnger;
        echo_parm_mnger_value_t echoParmMngerValue;
        manually_activate_emergency_mode_specified_time_t manuallyActivateEmergencyModeSpecifiedTime;
        downlink_full_sys_logs_next_pass_t downlinkFullSysLogsNextPass;
        */
	};
	uint32_t timestamp;
	uint8_t id;
	bool isTimeTagged;
} cmd_msg_t;
/*PLACEHOLDER*/

obc_error_code_t ax25Recv(uint8_t *in, uint8_t *out);

obc_error_code_t rsDecode(uint8_t *in, aes_block_t *out);

obc_error_code_t aes128Decrypt(aes_block_t in, cmd_msg_t out);

obc_error_code_t tabulateCommands(cmd_msg_t command);

void initDecodeDataTask(void);

#endif /* COMMS_INCLUDE_CC1120_DECODE_H */