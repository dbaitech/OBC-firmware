#include "tca6424.h"
#include "obc_i2c_io.h"
#include "obc_logging.h"
#include "obc_errors.h"

#define TCA6424A_I2C_TRANSFER_TIMEOUT pdMS_TO_TICKS(100)
#define TCA6424A_I2C_ADDRESS 0x20

#define TCA6424A_INPUT_PORT_ZERO_ADDR 0x00
#define TCA6424A_INPUT_PORT_ONE_ADDR 0x01
#define TCA6424A_INPUT_PORT_TWO_ADDR 0x02

#define TCA6424A_OUTPUT_PORT_ZERO_ADDR 0x03
#define TCA6424A_OUTPUT_PORT_ONE_ADDR 0x04
#define TCA6424A_OUTPUT_PORT_TWO_ADDR 0x06

#define TCA6424A_CONFIGURATION_PORT_ZERO_ADDR 0x0A
#define TCA6424A_CONFIGURATION_PORT_ONE_ADDR 0x0B
#define TCA6424A_CONFIGURATION_PORT_TWO_ADDR 0x0C

#define MAX_PIN_COUNT 7U
#define MAX_PORT_COUNT 2U

static obc_error_code_t writeTCA6424ARegister(uint8_t addr, uint8_t* data, uint8_t size);
static obc_error_code_t readTCA6424ARegister(uint8_t addr, uint8_t* data, uint8_t size);

obc_error_code_t configureTCA6424APin(uint8_t pinLocation, TCA6424A_gpio_config_t gpioPinConfig) {
  uint8_t pinPort = (pinLocation & 0xF0) >> 4;
  uint8_t pinIndex = pinLocation & 0x0F;
  if (pinIndex > MAX_PIN_COUNT || pinPort > MAX_PORT_COUNT) return OBC_ERR_CODE_INVALID_ARG;

  uint8_t pinState = (gpioPinConfig == TCA6424A_GPIO_CONFIG_INPUT) ? 0x01 : 0x00;
  uint8_t configurationPortAddress = TCA6424A_CONFIGURATION_PORT_ZERO_ADDR + pinPort;
  uint8_t configurationPort = 0;

  obc_error_code_t errCode;
  RETURN_IF_ERROR_CODE(readTCA6424ARegister(configurationPortAddress, &configurationPort, 1));

  configurationPort |= (pinState << pinIndex);
  RETURN_IF_ERROR_CODE(writeTCA6424ARegister(configurationPortAddress, &configurationPort, 1));
  return OBC_ERR_CODE_SUCCESS;
}

obc_error_code_t driveTCA6424APinOutput(TCA6424A_pin_state_t pinState) {
  uint8_t pinPort = (pinState.pinLocation & 0xF0) >> 4;
  uint8_t pinIndex = pinState.pinLocation & 0x0F;
  if (pinIndex > MAX_PIN_COUNT || pinPort > MAX_PORT_COUNT) return OBC_ERR_CODE_INVALID_ARG;

  uint8_t outputPortAddress = TCA6424A_OUTPUT_PORT_ZERO_ADDR + pinPort;
  uint8_t outputPort = 0;

  obc_error_code_t errCode;
  RETURN_IF_ERROR_CODE(configureTCA6424APin(pinState.pinLocation, TCA6424A_GPIO_CONFIG_OUTPUT));
  RETURN_IF_ERROR_CODE(readTCA6424ARegister(outputPortAddress, &outputPort, 1));

  outputPort |= ((pinState.IOPortValue & 0x01) << pinIndex);
  RETURN_IF_ERROR_CODE(writeTCA6424ARegister(outputPortAddress, &outputPort, 1));
  return OBC_ERR_CODE_SUCCESS;
}

obc_error_code_t readTCA6424APinInput(TCA6424A_pin_state_t* pinState) {
  uint8_t pinPort = (pinState->pinLocation & 0xF0) >> 4;
  uint8_t pinIndex = pinState->pinLocation & 0x0F;
  if (pinIndex > MAX_PIN_COUNT || pinPort > MAX_PORT_COUNT) return OBC_ERR_CODE_INVALID_ARG;
  if (pinState == NULL) return OBC_ERR_CODE_INVALID_ARG;

  obc_error_code_t errCode;
  uint8_t IOPortAddress = TCA6424A_INPUT_PORT_ZERO_ADDR + pinPort;
  uint8_t IOPortValue = 0;

  RETURN_IF_ERROR_CODE(readTCA6424ARegister(IOPortAddress, &IOPortValue, 1));
  IOPortValue = (IOPortValue & (0x01 << pinIndex)) ? TCA6424A_GPIO_HIGH : TCA6424A_GPIO_LOW;
  pinState->IOPortValue = IOPortValue;
  return OBC_ERR_CODE_SUCCESS;
}

static obc_error_code_t writeTCA6424ARegister(uint8_t addr, uint8_t* data, uint8_t size) {
  if (data == NULL) return OBC_ERR_CODE_INVALID_ARG;

  uint8_t slaveReg = (size > 1) ? (addr | (0x01 << 7)) : addr;
  obc_error_code_t errCode;
  RETURN_IF_ERROR_CODE(i2cWriteReg(TCA6424A_I2C_ADDRESS, slaveReg, data, size));
  return OBC_ERR_CODE_SUCCESS;
}

static obc_error_code_t readTCA6424ARegister(uint8_t addr, uint8_t* data, uint8_t size) {
  if (data == NULL) return OBC_ERR_CODE_INVALID_ARG;

  uint8_t slaveReg = (size > 1) ? (addr | (0x01 << 7)) : addr;
  obc_error_code_t errCode;
  RETURN_IF_ERROR_CODE(i2cReadReg(TCA6424A_I2C_ADDRESS, slaveReg, data, size, TCA6424A_I2C_TRANSFER_TIMEOUT));
  return OBC_ERR_CODE_SUCCESS;
}
