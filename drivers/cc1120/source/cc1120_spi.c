#include "cc1120_spi.h"
#include "cc1120_regs.h"
#include "cc1120_mcu.h"
#include "obc_logging.h"

// TODO: remove uint8_t i declarations before for loops (previously used for C89 compatibility)

/**
 * @brief - Reads from consecutive registers from the CC1120.
 * 
 * @param addr - The address of the first register to read.
 * @param data - The array to store the read data, or a pointer to a single uint8_t if len=1.
 * @param len - The number of registers to read.
 * @return OBC_ERR_CODE_SUCCESS - If the read was successful.
 * @return An error code - If the register is not valid, or the errCode byte is invalid.
 */
obc_error_code_t cc1120ReadSpi(uint8_t addr, uint8_t data[], uint8_t len) {
    obc_error_code_t errCode = OBC_ERR_CODE_SUCCESS;
    
    if (addr >= CC1120_REGS_EXT_ADDR) {
        errCode = OBC_ERR_CODE_INVALID_ARG;
        LOG_ERROR_CODE(errCode);
        return errCode;
    }

    if (len < 1) {
        errCode = OBC_ERR_CODE_INVALID_ARG;
        LOG_ERROR_CODE(errCode);
        return errCode;
    }

    uint8_t header = (len > 1) ? (R_BIT | BURST_BIT | addr) : (R_BIT | addr);

    RETURN_IF_ERROR_CODE(mcuCC1120CSAssert());
    errCode = cc1120SendByteReceiveStatus(header);
    
    RETURN_IF_ERROR_CODE(errCode);

    for(uint8_t i = 0; i < len; i++) {
        errCode = mcuCC1120SpiTransfer(0x00, data+i);
        RETURN_IF_ERROR_CODE(errCode);
    }

    errCode = mcuCC1120CSDeassert();
    return errCode;
}

/**
 * @brief - Reads from consecutive extended address space registers on the CC1120
 * 
 * @param addr - The address of the first register to read.
 * @param data - The array to store the read data, or a pointer to a single uint8_t if len=1.
 * @param len - The number of registers to read.
 * @return OBC_ERR_CODE_SUCCESS - If the read was successful.
 * @return An error code - If the register is not valid, or the errCode byte is invalid.
 */
obc_error_code_t cc1120ReadExtAddrSpi(uint8_t addr, uint8_t data[], uint8_t len) {
    obc_error_code_t errCode = OBC_ERR_CODE_SUCCESS;

    if ((addr > CC1120_REGS_EXT_PA_CFG3 && addr < CC1120_REGS_EXT_WOR_TIME1) ||
        (addr > CC1120_REGS_EXT_XOSC_TEST0 && addr < CC1120_REGS_EXT_RXFIRST) ||
        (addr > CC1120_REGS_EXT_FIFO_NUM_RXBYTES)) {
        errCode = OBC_ERR_CODE_INVALID_ARG; // invalid params
        LOG_ERROR_CODE(errCode);
        return errCode;
    }

    if (len < 1) {
        errCode = OBC_ERR_CODE_INVALID_ARG;
        LOG_ERROR_CODE(errCode);
        return errCode;
    }
    

    uint8_t header = (len > 1) ? (R_BIT | BURST_BIT | CC1120_REGS_EXT_ADDR) :
                                    (R_BIT | CC1120_REGS_EXT_ADDR);

    RETURN_IF_ERROR_CODE(mcuCC1120CSAssert());
    errCode = cc1120SendByteReceiveStatus(header);
    if (errCode != OBC_ERR_CODE_SUCCESS) {
        return errCode;
    }

    uint8_t recvData;
    RETURN_IF_ERROR_CODE(mcuCC1120SpiTransfer(addr, &recvData));
    if (recvData != 0x00) { // When sending the extended address, SO will return all zeros. See section 3.2.
        errCode = OBC_ERR_CODE_CC1120_READ_EXT_ADDR_SPI_FAILED;
        LOG_ERROR_CODE(errCode);
        return errCode;
    }

    for(uint8_t i = 0; i < len; i++) {
        errCode = mcuCC1120SpiTransfer(0x00, data+i);
        RETURN_IF_ERROR_CODE(errCode);
    }

    errCode = mcuCC1120CSDeassert();
    return errCode;
}

/**
 * @brief - Writes to consecutive registers on the CC1120.
 * 
 * @param addr - The address of the first register to write to.
 * @param data - The array of data to write to the registers, or a pointer to a single uint8_t if len=1.
 * @param len - The number of registers to write.
 * @return OBC_ERR_CODE_SUCCESS - If the write was successful.
 * @return An error code - If the register is not valid, or the errCode byte is invalid.
 */
obc_error_code_t cc1120WriteSpi(uint8_t addr, uint8_t data[], uint8_t len) {
    obc_error_code_t errCode = OBC_ERR_CODE_SUCCESS;

    if(addr >= CC1120_REGS_EXT_ADDR) {
        errCode = OBC_ERR_CODE_INVALID_ARG;
        LOG_ERROR_CODE(errCode);
        return errCode;
    }

    if (len < 1) {
        errCode = OBC_ERR_CODE_INVALID_ARG;
        LOG_ERROR_CODE(errCode);
        return errCode;
    }

    uint8_t header = (len > 1) ? (BURST_BIT | addr) : addr;

    RETURN_IF_ERROR_CODE(mcuCC1120CSAssert());
    errCode = cc1120SendByteReceiveStatus(header);
    RETURN_IF_ERROR_CODE(errCode);

    for(uint8_t i = 0; i < len; i++) {
        errCode = cc1120SendByteReceiveStatus(data[i]);
        RETURN_IF_ERROR_CODE(errCode);
    }

    errCode = mcuCC1120CSDeassert();
    return errCode;
}

/**
 * @brief - Writes to consecutive extended address space registers on the CC1120.
 * 
 * @param addr - The address of the first register to write to.
 * @param data - The array of data to write to the registers, or a pointer to a single uint8_t if len=1.
 * @param len - The number of registers to write.
 * @return OBC_ERR_CODE_SUCCESS - If the write was successful.
 * @return An error code - If the register is not valid, or the errCode byte is invalid.
 */
obc_error_code_t cc1120WriteExtAddrSpi(uint8_t addr, uint8_t data[], uint8_t len) {
    obc_error_code_t errCode = OBC_ERR_CODE_SUCCESS;

    if ((addr > CC1120_REGS_EXT_PA_CFG3 && addr < CC1120_REGS_EXT_WOR_TIME1) ||
        (addr > CC1120_REGS_EXT_XOSC_TEST0 && addr < CC1120_REGS_EXT_RXFIRST) ||
        (addr > CC1120_REGS_EXT_FIFO_NUM_RXBYTES)) {
        errCode = OBC_ERR_CODE_INVALID_ARG;
        LOG_ERROR_CODE(errCode);
        return errCode;
    }

    if (len < 1) {
        errCode = OBC_ERR_CODE_INVALID_ARG;
        LOG_ERROR_CODE(errCode);
        return errCode;
    }
    
    uint8_t header = (len > 1) ? (BURST_BIT | CC1120_REGS_EXT_ADDR) : CC1120_REGS_EXT_ADDR;

    RETURN_IF_ERROR_CODE(mcuCC1120CSAssert());
    errCode = cc1120SendByteReceiveStatus(header);
    if (errCode != OBC_ERR_CODE_SUCCESS) {
        return errCode;
    }

    uint8_t recvData;
    RETURN_IF_ERROR_CODE(mcuCC1120SpiTransfer(addr, &recvData));
    if (recvData != 0x00) { // When sending the extended address, SO will return all zeros. See section 3.2.
        errCode = OBC_ERR_CODE_CC1120_WRITE_EXT_ADDR_SPI_FAILED;
        LOG_ERROR_CODE(errCode);
        return errCode;
    }
    
    for(uint8_t i = 0; i < len; i++) {
        errCode = cc1120SendByteReceiveStatus(data[i]);
        RETURN_IF_ERROR_CODE(errCode);
    }
    
    errCode = mcuCC1120CSDeassert();
    return errCode;
}

/**
 * @brief Calls a strobe command on the CC1120.
 * 
 * @param addr - The address of the strobe command.
 * @return OBC_ERR_CODE_SUCCESS - If the strobe command was successful.
 * @return An error code - If the register is not valid, or the errCode byte is invalid.
 */
obc_error_code_t cc1120StrobeSpi(uint8_t addr) {
    obc_error_code_t errCode = OBC_ERR_CODE_SUCCESS;

    if (addr < CC1120_STROBE_SRES || addr > CC1120_STROBE_SNOP) {
        errCode = OBC_ERR_CODE_INVALID_ARG;
        LOG_ERROR_CODE(errCode);
        return errCode;
    }

    RETURN_IF_ERROR_CODE(mcuCC1120CSAssert());    
    errCode = cc1120SendByteReceiveStatus(addr);
    RETURN_IF_ERROR_CODE(errCode);

    errCode = mcuCC1120CSDeassert();
    return errCode;
}

/**
 * @brief - Reads consecutive registers from the FIFO memory.
 * 
 * @param data - The array to store the read data, or a pointer to a single uint8_t if len=1.
 * @param len - The number of registers to read.
 * @return OBC_ERR_CODE_SUCCESS - If the read was successful.
 * @return An error code - If the errCode byte is invalid.
 */
obc_error_code_t cc1120ReadFifo(uint8_t data[], uint8_t len) {
    obc_error_code_t errCode = OBC_ERR_CODE_SUCCESS;

    if (len < 1) {
        errCode = OBC_ERR_CODE_INVALID_ARG;
        LOG_ERROR_CODE(errCode);
        return errCode;
    }

    uint8_t header = (len > 1) ? (R_BIT | BURST_BIT | CC1120_REGS_FIFO_ACCESS_STD) :
                                (R_BIT | CC1120_REGS_FIFO_ACCESS_STD);

    RETURN_IF_ERROR_CODE(mcuCC1120CSAssert());
    if (cc1120SendByteReceiveStatus(header) != OBC_ERR_CODE_SUCCESS) {
        errCode = OBC_ERR_CODE_INVALID_ARG;
        return errCode;
    }

    for(uint8_t i = 0; i < len; i++) {
        errCode = mcuCC1120SpiTransfer(0x00, data+i);
        RETURN_IF_ERROR_CODE(errCode);
    }

    errCode = mcuCC1120CSDeassert();
    return errCode;
}

/**
 * @brief - Writes consecutive registers to the FIFO memory.
 * 
 * @param data - The array of data to write to the registers, or a pointer to a single uint8_t if len=1.
 * @param len - The number of registers to write.
 * @return OBC_ERR_CODE_SUCCESS - If the write was successful.
 * @return An error code - If the errCode byte is invalid.
 */
obc_error_code_t cc1120WriteFifo(uint8_t data[], uint8_t len) {
    obc_error_code_t errCode = OBC_ERR_CODE_SUCCESS;

    if (len < 1) {
        errCode = OBC_ERR_CODE_INVALID_ARG;
        LOG_ERROR_CODE(errCode);
        return errCode;
    }

    uint8_t header = (len > 1) ? (BURST_BIT | CC1120_REGS_FIFO_ACCESS_STD) :
                                CC1120_REGS_FIFO_ACCESS_STD;

    RETURN_IF_ERROR_CODE(mcuCC1120CSAssert());
    errCode = cc1120SendByteReceiveStatus(header);
    if (errCode != OBC_ERR_CODE_SUCCESS) {
        return errCode;
    }

    for(uint8_t i = 0; i < len; i++) {
        errCode = mcuCC1120SpiTransfer(0x00, data+i);
        RETURN_IF_ERROR_CODE(errCode);
    }

    errCode = mcuCC1120CSDeassert();
    return errCode;
}

/**
 * @brief - Reads consecutive registers directly from the FIFO on the CC1120.
 * 
 * @param addr - The address of the first register to read. Range 0x00 - 0xFF.
 * @param data - The array to store the read data, or a pointer to a single uint8_t if len=1.
 * @param len - The number of registers to read.
 * @return OBC_ERR_CODE_SUCCESS - If the read was successful.
 * @return An error code - If the register is not valid, or the errCode byte is invalid.
 */
obc_error_code_t cc1120ReadFifoDirect(uint8_t addr, uint8_t data[], uint8_t len) {
    obc_error_code_t errCode = OBC_ERR_CODE_SUCCESS;

    /*
    // Always true, since addr is uint8_t:
    if (addr < CC1120_FIFO_TX_START || addr > CC1120_FIFO_RX_END) {
        errCode = OBC_ERR_CODE_INVALID_ARG;
        LOG_ERROR_CODE(errCode);
        return errCode;
    }
    */

    if (len < 1) {
        errCode = OBC_ERR_CODE_INVALID_ARG;
        LOG_ERROR_CODE(errCode);
        return errCode;
    }

    uint8_t header = (len > 1) ? (R_BIT | BURST_BIT | CC1120_REGS_FIFO_ACCESS_DIR) :
                                (R_BIT | CC1120_REGS_FIFO_ACCESS_DIR);

    RETURN_IF_ERROR_CODE(mcuCC1120CSAssert());
    errCode = cc1120SendByteReceiveStatus(header);
    if (errCode != OBC_ERR_CODE_SUCCESS) {
        return errCode;
    }

    uint8_t ignore;
    RETURN_IF_ERROR_CODE(mcuCC1120SpiTransfer(addr, &ignore));
    for(uint8_t i = 0; i < len; i++) {
        errCode = mcuCC1120SpiTransfer(0x00, data+i);
        RETURN_IF_ERROR_CODE(errCode);
    }

    errCode = mcuCC1120CSDeassert();
    return errCode;
}

/**
 * @brief - Writes consecutive registers directly to the FIFO on the CC1120.
 * 
 * @param addr - The address of the first register to write to. Range 0x00 - 0xFF.
 * @param data - The array of data to write to the registers, or a pointer to a single uint8_t if len=1.
 * @param len - The number of registers to write.
 * @return OBC_ERR_CODE_SUCCESS - If the write was successful.
 * @return An error code - If the register is not valid, or the errCode byte is invalid.
 */
obc_error_code_t cc1120WriteFifoDirect(uint8_t addr, uint8_t data[], uint8_t len) {
    obc_error_code_t errCode = OBC_ERR_CODE_SUCCESS;

    /*
    // Always true, since addr is uint8_t:
    if (addr < CC1120_FIFO_TX_START || addr > CC1120_FIFO_RX_END) {
        errCode = OBC_ERR_CODE_INVALID_ARG;
        LOG_ERROR_CODE(errCode);
        return errCode;
    }
    */

    if (len < 1) {
        errCode = OBC_ERR_CODE_INVALID_ARG;
        LOG_ERROR_CODE(errCode);
        return errCode;
    }
    
    uint8_t header = (len > 1) ? (BURST_BIT | CC1120_REGS_FIFO_ACCESS_DIR) :
                                CC1120_REGS_FIFO_ACCESS_DIR;


    RETURN_IF_ERROR_CODE(mcuCC1120CSAssert());
    errCode = cc1120SendByteReceiveStatus(header);
    if (errCode!= OBC_ERR_CODE_SUCCESS) {
        return errCode;
    }

    uint8_t ignore;
    RETURN_IF_ERROR_CODE(mcuCC1120SpiTransfer(addr, &ignore));
    for(uint8_t i = 0; i < len; i++) {
        errCode = mcuCC1120SpiTransfer(0x00, data+i);
        RETURN_IF_ERROR_CODE(errCode);
    }
    
    errCode = mcuCC1120CSDeassert();
    return errCode;
}

/**
 * @brief - Reads the status register on the CC1120 and consecutively sends a byte over SPI.
 * 
 * @param data - The data to send to the status register.
 * @return OBC_ERR_CODE_SUCCESS - If the status byte is valid.
 * @return OBC_ERR_CODE_CC1120_INVALID_STATUS_BYTE - If the status byte is invalid.
 */
obc_error_code_t cc1120SendByteReceiveStatus(uint8_t data) {
    obc_error_code_t errCode = OBC_ERR_CODE_CC1120_INVALID_STATUS_BYTE;
    union cc_st ccstatus;

    // TODO: This is a hacky way to do this. We should implement a mutex + timeout.
    for (uint8_t i = 1; i <= 5; i++) {
        RETURN_IF_ERROR_CODE(mcuCC1120SpiTransfer(data, &ccstatus.data));
        if (!(ccstatus.ccst.chip_ready == 1)) {
            errCode = OBC_ERR_CODE_SUCCESS;
            break;
        }
    }

    return errCode;
}
