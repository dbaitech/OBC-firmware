#include "spi.h"

/* Board macros for registers, ports, CS pins, data formats, etc. */

#ifdef RM46_LAUNCHPAD
    // Serial config
	#define UART_PRINT_REG  scilinREG
	#define UART_READ_REG   scilinREG 

    // Fram SPI config
    #define FRAM_spiREG     spiREG3
    #define FRAM_spiPORT    spiPORT3
    #define FRAM_CS         1
    static spiDAT1_t framSPIDataFmt = {.CS_HOLD=0,
                                        .CSNR = SPI_CS_NONE, 
                                        .DFSEL = SPI_FMT_1, 
                                        .WDEL = 0};

    // SD Card SPI config
    #define SDC_SPI_PORT         spiPORT3
    #define SDC_SPI_REG          spiREG3
    #define SDC_SPI_CS           1UL
    #define SDC_SPI_DATA_FORMAT  SPI_FMT_0

#elif defined(OBC_REVISION_1)
    // Serial config
	#define UART_PRINT_REG  sciREG 
	#define UART_READ_REG   sciREG

    // Fram SPI config
    #define FRAM_spiREG     spiREG1
    #define FRAM_spiPORT    spiPORT1
    #define FRAM_CS         1
    static spiDAT1_t framSPIDataFmt = {.CS_HOLD=0,
                                        .CSNR = SPI_CS_NONE, 
                                        .DFSEL = SPI_FMT_1, 
                                        .WDEL = 0};
    
    // SD Card SPI config
    #define SDC_SPI_PORT         spiPORT1
    #define SDC_SPI_REG          spiREG1
    #define SDC_SPI_CS           0UL
    #define SDC_SPI_DATA_FORMAT  SPI_FMT_0

#elif defined(OBC_REVISION_2)
	#error Serial port or Fram SPI module not chosen for OBC_REVISION_2

#else
	#error Board configuration not defined
#endif