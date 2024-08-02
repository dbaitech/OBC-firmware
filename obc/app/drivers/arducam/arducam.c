#include "arducam.h"
#include "ov5642_config.h"

#include "obc_i2c_io.h"
#include "obc_spi_io.h"
#include "obc_reliance_fs.h"
#include "obc_board_config.h"

#include <gio.h>
#include <sci.h>
#include <spi.h>
#include <i2c.h>
#include <redposix.h>
#include <string.h>

// Constants

#define ARDUCAM_DELAY_2MS pdMS_TO_TICKS(2)

// Arduchip masks
#define ARDUCHIP_WRITE_MASK 0x80
#define ARDUCAM_RESET_CPLD_MASK 0x80

// Arduchip Opcodes
#define OP_READ_TEST_REG 0x00
#define OP_READ_CAPTURE_CONTROL_REG 0x01
#define OP_READ_SENSOR_TIMING_CONTROL_REG 0x03
#define OP_READ_FIFO_CONTROL_REG 0x04
#define OP_READ_SENSOR_POWER_CONTROL_REG 0x06
#define OP_RESET_CPLD (0x07 | ARDUCHIP_WRITE_MASK)
#define OP_FIFO_BURST_READ 0x3C
#define OP_FIFO_READ 0x3D
#define OP_READ_FW_VERSION 0x40
#define OP_READ_CAPTURE_STATUS_REG 0x41
#define OP_READ_FIFO_SIZE_LOWER 0x42
#define OP_READ_FIFO_SIZE_MIDDLE 0x43
#define OP_READ_FIFO_SIZE_UPPER 0x44
#define OP_WRITE_TEST_REG (OP_READ_TEST_REG | ARDUCHIP_WRITE_MASK)
#define OP_WRITE_CAPTURE_CONTROL_REG (OP_READ_CAPTURE_CONTROL_REG | ARDUCHIP_WRITE_MASK)
#define OP_WRITE_SENSOR_TIMING_CONTROL_REG (OP_READ_SENSOR_TIMING_CONTROL_REG | ARDUCHIP_WRITE_MASK)
#define OP_WRITE_FIFO_CONTROL_REG (OP_READ_FIFO_CONTROL_REG | ARDUCHIP_WRITE_MASK)
#define OP_WRITE_SENSOR_POWER_CONTROL_REG (OP_READ_SENSOR_POWER_CONTROL_REG | ARDUCHIP_WRITE_MASK)

// Camera Img Sensor (I2C) defines
#define CAM_I2C_ADDR 0x3C
#define I2C_MUTEX_TIMEOUT portMAX_DELAY
#define I2C_TRANSFER_TIMEOUT pdMS_TO_TICKS(100)

// SPI values
static spiDAT1_t arducamSPIDataFmt = {.CS_HOLD = 0, .CSNR = SPI_CS_NONE, .DFSEL = CAM_SPI_DATA_FORMAT, .WDEL = 0};

static const uint8_t cameraCS[CAMERA_COUNT] = {
    [PRIMARY] = CAM_CS_1,
    [SECONDARY] = CAM_CS_2,
};

static camera_id_t selectedCamera = PRIMARY;

typedef enum opcode {
  ARDUCAM_READ_TEST_REG,
  ARDUCAM_WRITE_TEST_REG,
  ARDUCAM_READ_CAPTURE_CONTROL_REG,
  ARDUCAM_WRITE_CAPTURE_CONTROL_REG,
  ARDUCAM_READ_SENSOR_TIMING_CONTROL_REG,
  ARDUCAM_WRITE_SENSOR_TIMING_CONTROL_REG,
  ARDUCAM_READ_FIFO_CONTROL_REG,
  ARDUCAM_WRITE_FIFO_CONTROL_REG,
  ARDUCAM_READ_SENSOR_POWER_CONTROL_REG,
  ARDUCAM_WRITE_SENSOR_POWER_CONTROL_REG,
  ARDUCAM_FIFO_BURST_READ,
  ARDUCAM_FIFO_READ,
  ARDUCAM_READ_FW_VERSION,
  ARDUCAM_READ_CAPTURE_STATUS_REG,
  ARDUCAM_READ_FIFO_SIZE_LOWER,
  ARDUCAM_READ_FIFO_SIZE_MIDDLE,
  ARDUCAM_READ_FIFO_SIZE_UPPER,
  ARDUCAM_RESET_CPLD,
} opcode_t;

void selectCameraSPIBus(camera_id_t cameraID) { selectedCamera = cameraID; }

camera_id_t getSelectedCamera(void) { return selectedCamera; }

// CS assumed to be asserted
static obc_error_code_t arducamTransmitOpcode(opcode_t opcode) {
  obc_error_code_t errCode;
  switch (opcode) {
    case ARDUCAM_READ_TEST_REG:
      LOG_IF_ERROR_CODE(spiTransmitByte(CAM_SPI_REG, &arducamSPIDataFmt, OP_READ_TEST_REG));
      break;
    case ARDUCAM_WRITE_TEST_REG:
      LOG_IF_ERROR_CODE(spiTransmitByte(CAM_SPI_REG, &arducamSPIDataFmt, OP_WRITE_TEST_REG));
      break;
    case ARDUCAM_READ_CAPTURE_CONTROL_REG:
      LOG_IF_ERROR_CODE(spiTransmitByte(CAM_SPI_REG, &arducamSPIDataFmt, OP_READ_CAPTURE_CONTROL_REG));
      break;
    case ARDUCAM_WRITE_CAPTURE_CONTROL_REG:
      LOG_IF_ERROR_CODE(spiTransmitByte(CAM_SPI_REG, &arducamSPIDataFmt, OP_WRITE_CAPTURE_CONTROL_REG));
      break;
    case ARDUCAM_READ_SENSOR_TIMING_CONTROL_REG:
      LOG_IF_ERROR_CODE(spiTransmitByte(CAM_SPI_REG, &arducamSPIDataFmt, OP_READ_SENSOR_TIMING_CONTROL_REG));
      break;
    case ARDUCAM_WRITE_SENSOR_TIMING_CONTROL_REG:
      LOG_IF_ERROR_CODE(spiTransmitByte(CAM_SPI_REG, &arducamSPIDataFmt, OP_WRITE_SENSOR_TIMING_CONTROL_REG));
      break;
    case ARDUCAM_READ_FIFO_CONTROL_REG:
      LOG_IF_ERROR_CODE(spiTransmitByte(CAM_SPI_REG, &arducamSPIDataFmt, OP_READ_FIFO_CONTROL_REG));
      break;
    case ARDUCAM_WRITE_FIFO_CONTROL_REG:
      LOG_IF_ERROR_CODE(spiTransmitByte(CAM_SPI_REG, &arducamSPIDataFmt, OP_WRITE_FIFO_CONTROL_REG));
      break;
    case ARDUCAM_READ_SENSOR_POWER_CONTROL_REG:
      LOG_IF_ERROR_CODE(spiTransmitByte(CAM_SPI_REG, &arducamSPIDataFmt, OP_READ_SENSOR_POWER_CONTROL_REG));
      break;
    case ARDUCAM_WRITE_SENSOR_POWER_CONTROL_REG:
      LOG_IF_ERROR_CODE(spiTransmitByte(CAM_SPI_REG, &arducamSPIDataFmt, OP_WRITE_SENSOR_POWER_CONTROL_REG));
      break;
    case ARDUCAM_FIFO_BURST_READ:
      LOG_IF_ERROR_CODE(spiTransmitByte(CAM_SPI_REG, &arducamSPIDataFmt, OP_FIFO_BURST_READ));
      break;
    case ARDUCAM_FIFO_READ:
      LOG_IF_ERROR_CODE(spiTransmitByte(CAM_SPI_REG, &arducamSPIDataFmt, OP_FIFO_READ));
      break;
    case ARDUCAM_READ_FW_VERSION:
      LOG_IF_ERROR_CODE(spiTransmitByte(CAM_SPI_REG, &arducamSPIDataFmt, OP_READ_FW_VERSION));
      break;
    case ARDUCAM_READ_CAPTURE_STATUS_REG:
      LOG_IF_ERROR_CODE(spiTransmitByte(CAM_SPI_REG, &arducamSPIDataFmt, OP_READ_CAPTURE_STATUS_REG));
      break;
    case ARDUCAM_READ_FIFO_SIZE_LOWER:
      LOG_IF_ERROR_CODE(spiTransmitByte(CAM_SPI_REG, &arducamSPIDataFmt, OP_READ_FIFO_SIZE_LOWER));
      break;
    case ARDUCAM_READ_FIFO_SIZE_MIDDLE:
      LOG_IF_ERROR_CODE(spiTransmitByte(CAM_SPI_REG, &arducamSPIDataFmt, OP_READ_FIFO_SIZE_MIDDLE));
      break;
    case ARDUCAM_READ_FIFO_SIZE_UPPER:
      LOG_IF_ERROR_CODE(spiTransmitByte(CAM_SPI_REG, &arducamSPIDataFmt, OP_READ_FIFO_SIZE_UPPER));
      break;
    case ARDUCAM_RESET_CPLD:
      LOG_IF_ERROR_CODE(spiTransmitByte(CAM_SPI_REG, &arducamSPIDataFmt, OP_RESET_CPLD));
      break;
    default:
      LOG_ERROR_CODE(OBC_ERR_CODE_INVALID_ARG);
      errCode = OBC_ERR_CODE_INVALID_ARG;
  }

  return errCode;
}

obc_error_code_t arducamReadTestReg(uint8_t *buffer) {
  obc_error_code_t errCode;
  obc_error_code_t prevCode;

  if (buffer == NULL) {
    return OBC_ERR_CODE_INVALID_ARG;
  }

  RETURN_IF_ERROR_CODE(assertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  LOG_IF_ERROR_CODE(arducamTransmitOpcode(ARDUCAM_READ_TEST_REG));
  if (errCode == OBC_ERR_CODE_SUCCESS) {
    LOG_IF_ERROR_CODE(spiReceiveByte(CAM_SPI_REG, &arducamSPIDataFmt, buffer));
  }
  prevCode = errCode;
  RETURN_IF_ERROR_CODE(deassertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  // Reset overwritten error code
  errCode = prevCode;

  return errCode;
}

obc_error_code_t arducamWriteTestReg(uint8_t value) {
  obc_error_code_t errCode;
  obc_error_code_t prevCode;

  RETURN_IF_ERROR_CODE(assertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  LOG_IF_ERROR_CODE(arducamTransmitOpcode(ARDUCAM_WRITE_TEST_REG));
  if (errCode == OBC_ERR_CODE_SUCCESS) {
    LOG_IF_ERROR_CODE(spiTransmitByte(CAM_SPI_REG, &arducamSPIDataFmt, value));
  }
  prevCode = errCode;
  RETURN_IF_ERROR_CODE(deassertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  // Reset overwritten error code
  errCode = prevCode;

  return errCode;
}

obc_error_code_t arducamReadCaptureControlReg(uint8_t *buffer) {
  obc_error_code_t errCode;
  obc_error_code_t prevCode;
  if (buffer == NULL) {
    return OBC_ERR_CODE_INVALID_ARG;
  }

  RETURN_IF_ERROR_CODE(assertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  LOG_IF_ERROR_CODE(arducamTransmitOpcode(ARDUCAM_READ_CAPTURE_CONTROL_REG));
  if (errCode == OBC_ERR_CODE_SUCCESS) {
    LOG_IF_ERROR_CODE(spiReceiveByte(CAM_SPI_REG, &arducamSPIDataFmt, buffer));
  }
  prevCode = errCode;
  RETURN_IF_ERROR_CODE(deassertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  // Reset overwritten error code
  errCode = prevCode;

  return errCode;
}

obc_error_code_t arducamWriteCaptureControlReg(uint8_t value) {
  obc_error_code_t errCode;
  obc_error_code_t prevCode;
  if (value > 7) {
    return OBC_ERR_CODE_INVALID_ARG;
  }

  RETURN_IF_ERROR_CODE(assertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  LOG_IF_ERROR_CODE(arducamTransmitOpcode(ARDUCAM_WRITE_CAPTURE_CONTROL_REG));
  if (errCode == OBC_ERR_CODE_SUCCESS) {
    LOG_IF_ERROR_CODE(spiTransmitByte(CAM_SPI_REG, &arducamSPIDataFmt, value));
  }
  prevCode = errCode;
  RETURN_IF_ERROR_CODE(deassertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  // Reset overwritten error code
  errCode = prevCode;

  return errCode;
}

obc_error_code_t arducamReadSensorTimingControlReg(uint8_t *buffer) {
  obc_error_code_t errCode;
  obc_error_code_t prevCode;
  if (buffer == NULL) {
    return OBC_ERR_CODE_INVALID_ARG;
  }

  RETURN_IF_ERROR_CODE(assertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  LOG_IF_ERROR_CODE(arducamTransmitOpcode(ARDUCAM_READ_SENSOR_TIMING_CONTROL_REG));
  if (errCode == OBC_ERR_CODE_SUCCESS) {
    LOG_IF_ERROR_CODE(spiReceiveByte(CAM_SPI_REG, &arducamSPIDataFmt, buffer));
  }
  prevCode = errCode;
  RETURN_IF_ERROR_CODE(deassertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  // Reset overwritten error code
  errCode = prevCode;

  return errCode;
}

obc_error_code_t arducamWriteSensorTimingControlReg(uint8_t value) {
  obc_error_code_t errCode;
  obc_error_code_t prevCode;

  RETURN_IF_ERROR_CODE(assertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  LOG_IF_ERROR_CODE(arducamTransmitOpcode(ARDUCAM_WRITE_SENSOR_TIMING_CONTROL_REG));
  if (errCode == OBC_ERR_CODE_SUCCESS) {
    LOG_IF_ERROR_CODE(spiTransmitByte(CAM_SPI_REG, &arducamSPIDataFmt, value));
  }
  prevCode = errCode;
  RETURN_IF_ERROR_CODE(deassertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  // Reset overwritten error code
  errCode = prevCode;

  return errCode;
}

obc_error_code_t arducamReadFIFOControlReg(uint8_t *buffer) {
  obc_error_code_t errCode;
  obc_error_code_t prevCode;
  if (buffer == NULL) {
    return OBC_ERR_CODE_INVALID_ARG;
  }

  RETURN_IF_ERROR_CODE(assertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  LOG_IF_ERROR_CODE(arducamTransmitOpcode(ARDUCAM_READ_FIFO_CONTROL_REG));
  if (errCode == OBC_ERR_CODE_SUCCESS) {
    LOG_IF_ERROR_CODE(spiReceiveByte(CAM_SPI_REG, &arducamSPIDataFmt, buffer));
  }
  prevCode = errCode;
  RETURN_IF_ERROR_CODE(deassertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  // Reset overwritten error code
  errCode = prevCode;

  return errCode;
}

obc_error_code_t arducamWriteFIFOControlReg(uint8_t value) {
  obc_error_code_t errCode;
  obc_error_code_t prevCode;
  RETURN_IF_ERROR_CODE(assertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  LOG_IF_ERROR_CODE(arducamTransmitOpcode(ARDUCAM_WRITE_FIFO_CONTROL_REG));
  if (errCode == OBC_ERR_CODE_SUCCESS) {
    LOG_IF_ERROR_CODE(spiTransmitByte(CAM_SPI_REG, &arducamSPIDataFmt, value));
  }
  prevCode = errCode;
  RETURN_IF_ERROR_CODE(deassertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  // Reset overwritten error code
  errCode = prevCode;

  return errCode;
}

obc_error_code_t arducamReadSensorPowerControlReg(uint8_t *buffer) {
  obc_error_code_t errCode;
  obc_error_code_t prevCode;
  if (buffer == NULL) {
    return OBC_ERR_CODE_INVALID_ARG;
  }

  RETURN_IF_ERROR_CODE(assertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  LOG_IF_ERROR_CODE(arducamTransmitOpcode(ARDUCAM_READ_SENSOR_POWER_CONTROL_REG));
  if (errCode == OBC_ERR_CODE_SUCCESS) {
    LOG_IF_ERROR_CODE(spiReceiveByte(CAM_SPI_REG, &arducamSPIDataFmt, buffer));
  }
  prevCode = errCode;
  RETURN_IF_ERROR_CODE(deassertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  // Reset overwritten error code
  errCode = prevCode;

  return errCode;
}

obc_error_code_t arducamWriteSensorPowerControlReg(uint8_t value) {
  obc_error_code_t errCode;
  obc_error_code_t prevCode;
  RETURN_IF_ERROR_CODE(assertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  LOG_IF_ERROR_CODE(arducamTransmitOpcode(ARDUCAM_WRITE_SENSOR_POWER_CONTROL_REG));
  if (errCode == OBC_ERR_CODE_SUCCESS) {
    LOG_IF_ERROR_CODE(spiTransmitByte(CAM_SPI_REG, &arducamSPIDataFmt, value));
  }
  prevCode = errCode;
  RETURN_IF_ERROR_CODE(deassertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  // Reset overwritten error code
  errCode = prevCode;

  return errCode;
}

obc_error_code_t arducamReadFIFO(uint8_t *buffer) {
  obc_error_code_t errCode;
  obc_error_code_t prevCode;
  if (buffer == NULL) {
    return OBC_ERR_CODE_INVALID_ARG;
  }

  RETURN_IF_ERROR_CODE(assertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  LOG_IF_ERROR_CODE(arducamTransmitOpcode(ARDUCAM_FIFO_READ));
  if (errCode == OBC_ERR_CODE_SUCCESS) {
    LOG_IF_ERROR_CODE(spiReceiveByte(CAM_SPI_REG, &arducamSPIDataFmt, buffer));
  }
  prevCode = errCode;
  RETURN_IF_ERROR_CODE(deassertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  // Reset overwritten error code
  errCode = prevCode;

  return errCode;
}

obc_error_code_t arducamBurstReadFIFO(uint8_t *buffer, size_t bufferSize) {
  obc_error_code_t errCode;
  obc_error_code_t prevCode;

  if (buffer == NULL) {
    return OBC_ERR_CODE_INVALID_ARG;
  }

  RETURN_IF_ERROR_CODE(assertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  LOG_IF_ERROR_CODE(arducamTransmitOpcode(ARDUCAM_FIFO_BURST_READ));
  for (size_t index = 0; (index < bufferSize) && (errCode == OBC_ERR_CODE_SUCCESS); index++) {
    LOG_IF_ERROR_CODE(spiReceiveByte(CAM_SPI_REG, &arducamSPIDataFmt, &buffer[index]));
  }
  prevCode = errCode;
  RETURN_IF_ERROR_CODE(deassertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  // Reset overwritten error code
  errCode = prevCode;

  return errCode;
}

obc_error_code_t arducamReadFWVersion(uint8_t *version) {
  obc_error_code_t errCode;
  obc_error_code_t prevCode;
  if (version == NULL) {
    return OBC_ERR_CODE_INVALID_ARG;
  }

  RETURN_IF_ERROR_CODE(assertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  LOG_IF_ERROR_CODE(arducamTransmitOpcode(ARDUCAM_READ_FW_VERSION));
  if (errCode == OBC_ERR_CODE_SUCCESS) {
    LOG_IF_ERROR_CODE(spiReceiveByte(CAM_SPI_REG, &arducamSPIDataFmt, version));
  }
  prevCode = errCode;
  RETURN_IF_ERROR_CODE(deassertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  // Reset overwritten error code
  errCode = prevCode;
  return errCode;
}

obc_error_code_t arducamReadCaptureStatusReg(uint8_t *status) {
  obc_error_code_t errCode;
  obc_error_code_t prevCode;
  if (status == NULL) {
    return OBC_ERR_CODE_INVALID_ARG;
  }

  RETURN_IF_ERROR_CODE(assertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  LOG_IF_ERROR_CODE(arducamTransmitOpcode(ARDUCAM_READ_CAPTURE_STATUS_REG));
  if (errCode == OBC_ERR_CODE_SUCCESS) {
    LOG_IF_ERROR_CODE(spiReceiveByte(CAM_SPI_REG, &arducamSPIDataFmt, status));
  }
  prevCode = errCode;
  RETURN_IF_ERROR_CODE(deassertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  // Reset overwritten error code
  errCode = prevCode;
  return errCode;
}

obc_error_code_t arducamReadFIFOSize(uint32_t *fifoSize) {
  obc_error_code_t errCode;
  obc_error_code_t prevCode;
  uint8_t upper = 0;
  uint8_t middle = 0;
  uint8_t lower = 0;
  if (fifoSize == NULL) {
    return OBC_ERR_CODE_INVALID_ARG;
  }

  *fifoSize = 0;

  // Get upper bits
  RETURN_IF_ERROR_CODE(assertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  LOG_IF_ERROR_CODE(arducamTransmitOpcode(ARDUCAM_READ_FIFO_SIZE_UPPER));
  if (errCode == OBC_ERR_CODE_SUCCESS) {
    LOG_IF_ERROR_CODE(spiReceiveByte(CAM_SPI_REG, &arducamSPIDataFmt, &upper));
  }
  prevCode = errCode;
  RETURN_IF_ERROR_CODE(deassertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  // Reset overwritten error code
  errCode = prevCode;

  // Get middle bits
  if (errCode == OBC_ERR_CODE_SUCCESS) {
    RETURN_IF_ERROR_CODE(assertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
    LOG_IF_ERROR_CODE(arducamTransmitOpcode(ARDUCAM_READ_FIFO_SIZE_MIDDLE));
    if (errCode == OBC_ERR_CODE_SUCCESS) {
      LOG_IF_ERROR_CODE(spiReceiveByte(CAM_SPI_REG, &arducamSPIDataFmt, &middle));
    }
    prevCode = errCode;
    RETURN_IF_ERROR_CODE(deassertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
    // Reset overwritten error code
    errCode = prevCode;
  }

  // Get lower bits
  if (errCode == OBC_ERR_CODE_SUCCESS) {
    RETURN_IF_ERROR_CODE(assertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
    LOG_IF_ERROR_CODE(arducamTransmitOpcode(ARDUCAM_READ_FIFO_SIZE_LOWER));
    if (errCode == OBC_ERR_CODE_SUCCESS) {
      LOG_IF_ERROR_CODE(spiReceiveByte(CAM_SPI_REG, &arducamSPIDataFmt, &lower));
    }
    prevCode = errCode;
    RETURN_IF_ERROR_CODE(deassertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
    // Reset overwritten error code
    errCode = prevCode;
  }

  *fifoSize = upper;
  *fifoSize = (*fifoSize << 8) | middle;
  *fifoSize = (*fifoSize << 8) | lower;

  return errCode;
}

obc_error_code_t resetCPLD(void) {
  obc_error_code_t errCode;
  obc_error_code_t prevCode;

  RETURN_IF_ERROR_CODE(assertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  LOG_IF_ERROR_CODE(arducamTransmitOpcode(ARDUCAM_RESET_CPLD));
  if (errCode == OBC_ERR_CODE_SUCCESS) {
    LOG_IF_ERROR_CODE(spiTransmitByte(CAM_SPI_REG, &arducamSPIDataFmt, ARDUCAM_RESET_CPLD_MASK));
  }
  prevCode = errCode;
  RETURN_IF_ERROR_CODE(deassertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  // Reset overwritten error code
  errCode = prevCode;
  if (errCode == OBC_ERR_CODE_SUCCESS) {
    vTaskDelay(pdMS_TO_TICKS(2));
    RETURN_IF_ERROR_CODE(assertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
    LOG_IF_ERROR_CODE(arducamTransmitOpcode(ARDUCAM_RESET_CPLD));
    if (errCode == OBC_ERR_CODE_SUCCESS) {
      LOG_IF_ERROR_CODE(
          spiTransmitByte(CAM_SPI_REG, &arducamSPIDataFmt, (ARDUCAM_RESET_CPLD_MASK & (~ARDUCAM_RESET_CPLD_MASK))));
    }
    // Reset overwritten error code
    prevCode = errCode;
    RETURN_IF_ERROR_CODE(deassertChipSelect(CAM_SPI_PORT, cameraCS[selectedCamera]));
  }

  return errCode;
}

obc_error_code_t camWriteSensorReg16_8(uint32_t regID, uint8_t regDat) {
  uint8_t reg_tx_data[3] = {(regID >> 8), (regID & 0x00FF), regDat};
  return i2cSendTo(CAM_I2C_ADDR, 3, reg_tx_data, I2C_MUTEX_TIMEOUT, I2C_TRANSFER_TIMEOUT);
}

obc_error_code_t camReadSensorReg16_8(uint32_t regID, uint8_t *regDat) {
  obc_error_code_t errCode;
  uint8_t reg_id_tx_data[2] = {(regID >> 8), (regID & 0x00FF)};
  RETURN_IF_ERROR_CODE(i2cSendTo(CAM_I2C_ADDR, 2, reg_id_tx_data, I2C_MUTEX_TIMEOUT, I2C_TRANSFER_TIMEOUT));
  RETURN_IF_ERROR_CODE(i2cReceiveFrom(CAM_I2C_ADDR, 1, regDat, I2C_MUTEX_TIMEOUT, I2C_TRANSFER_TIMEOUT));
  return errCode;
}

obc_error_code_t camWriteSensorRegs16_8(const sensor_config_t reglist[], uint16_t reglistLen) {
  obc_error_code_t errCode;

  for (int i = 0; i < reglistLen; i++) {
    RETURN_IF_ERROR_CODE(camWriteSensorReg16_8(reglist[i].reg, reglist[i].val));
  }

  return OBC_ERR_CODE_SUCCESS;
}
