/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>
#include <cytek_ioport.h>
#include <cytek_config.h>
#include "cytek_spi_dac.h"
#ifdef CONF_BOARD_CONFIG_MPU_AT_INIT
#include "mpu.h"
#endif
					
#ifdef CONF_BOARD_CONFIG_MPU_AT_INIT
/**
 *	Default memory map
 *	Address range        Memory region      Memory type   Shareability  Cache policy
 *	0x00000000- 0x1FFFFFFF Code             Normal        Non-shareable  WT
 *	0x20000000- 0x3FFFFFFF SRAM             Normal        Non-shareable  WBWA
 *	0x40000000- 0x5FFFFFFF Peripheral       Device        Non-shareable  -
 *	0x60000000- 0x7FFFFFFF RAM              Normal        Non-shareable  WBWA
 *	0x80000000- 0x9FFFFFFF RAM              Normal        Non-shareable  WT
 *	0xA0000000- 0xBFFFFFFF Device           Device        Shareable
 *	0xC0000000- 0xDFFFFFFF Device           Device        Non Shareable
 *	0xE0000000- 0xFFFFFFFF System           -                  -
 */

/**
 * \brief Set up a memory region.
 */
static void _setup_memory_region( void )
{

	uint32_t dw_region_base_addr;
	uint32_t dw_region_attr;

	__DMB();

/**
 *	ITCM memory region --- Normal
 *	START_Addr:-  0x00000000UL
 *	END_Addr:-    0x00400000UL
 */
	dw_region_base_addr =
		ITCM_START_ADDRESS |
		MPU_REGION_VALID |
		MPU_DEFAULT_ITCM_REGION;

	dw_region_attr =
		MPU_AP_PRIVILEGED_READ_WRITE |
		mpu_cal_mpu_region_size(ITCM_END_ADDRESS - ITCM_START_ADDRESS) |
		MPU_REGION_ENABLE;

	mpu_set_region( dw_region_base_addr, dw_region_attr);

/**
 *	Internal flash memory region --- Normal read-only
 *	(update to Strongly ordered in write accesses)
 *	START_Addr:-  0x00400000UL
 *	END_Addr:-    0x00600000UL
 */

	dw_region_base_addr =
		IFLASH_START_ADDRESS |
		MPU_REGION_VALID |
		MPU_DEFAULT_IFLASH_REGION;

	dw_region_attr =
		MPU_AP_READONLY |
		INNER_NORMAL_WB_NWA_TYPE( NON_SHAREABLE ) |
		mpu_cal_mpu_region_size(IFLASH_END_ADDRESS - IFLASH_START_ADDRESS) |
		MPU_REGION_ENABLE;

	mpu_set_region( dw_region_base_addr, dw_region_attr);

/**
 *	DTCM memory region --- Normal
 *	START_Addr:-  0x20000000L
 *	END_Addr:-    0x20400000UL
 */

	/* DTCM memory region */
	dw_region_base_addr =
		DTCM_START_ADDRESS |
		MPU_REGION_VALID |
		MPU_DEFAULT_DTCM_REGION;

	dw_region_attr =
		MPU_AP_PRIVILEGED_READ_WRITE |
		mpu_cal_mpu_region_size(DTCM_END_ADDRESS - DTCM_START_ADDRESS) |
		MPU_REGION_ENABLE;

	mpu_set_region( dw_region_base_addr, dw_region_attr);

/**
 *	SRAM Cacheable memory region --- Normal
 *	START_Addr:-  0x20400000UL
 *	END_Addr:-    0x2043FFFFUL
 */
	/* SRAM memory  region */
	dw_region_base_addr =
		SRAM_FIRST_START_ADDRESS |
		MPU_REGION_VALID |
		MPU_DEFAULT_SRAM_REGION_1;

	dw_region_attr =
		MPU_AP_FULL_ACCESS    |
		INNER_NORMAL_WB_NWA_TYPE( NON_SHAREABLE ) |
		mpu_cal_mpu_region_size(SRAM_FIRST_END_ADDRESS - SRAM_FIRST_START_ADDRESS)
		| MPU_REGION_ENABLE;

	mpu_set_region( dw_region_base_addr, dw_region_attr);


/**
 *	Internal SRAM second partition memory region --- Normal
 *	START_Addr:-  0x20440000UL
 *	END_Addr:-    0x2045FFFFUL
 */
	/* SRAM memory region */
	dw_region_base_addr =
		SRAM_SECOND_START_ADDRESS |
		MPU_REGION_VALID |
		MPU_DEFAULT_SRAM_REGION_2;

	dw_region_attr =
		MPU_AP_FULL_ACCESS    |
		INNER_NORMAL_WB_NWA_TYPE( NON_SHAREABLE ) |
		mpu_cal_mpu_region_size(SRAM_SECOND_END_ADDRESS - SRAM_SECOND_START_ADDRESS) |
		MPU_REGION_ENABLE;

	mpu_set_region( dw_region_base_addr, dw_region_attr);

#ifdef MPU_HAS_NOCACHE_REGION
	dw_region_base_addr =
        SRAM_NOCACHE_START_ADDRESS |
        MPU_REGION_VALID |
        MPU_NOCACHE_SRAM_REGION;

    dw_region_attr =
        MPU_AP_FULL_ACCESS    |
        INNER_OUTER_NORMAL_NOCACHE_TYPE( SHAREABLE ) |
        mpu_cal_mpu_region_size(NOCACHE_SRAM_REGION_SIZE) |
        MPU_REGION_ENABLE;

    mpu_set_region( dw_region_base_addr, dw_region_attr);
#endif

/**
 *	Peripheral memory region --- DEVICE Shareable
 *	START_Addr:-  0x40000000UL
 *	END_Addr:-    0x5FFFFFFFUL
 */
	dw_region_base_addr =
		PERIPHERALS_START_ADDRESS |
		MPU_REGION_VALID |
		MPU_PERIPHERALS_REGION;

	dw_region_attr = MPU_AP_FULL_ACCESS |
		MPU_REGION_EXECUTE_NEVER |
		SHAREABLE_DEVICE_TYPE |
		mpu_cal_mpu_region_size(PERIPHERALS_END_ADDRESS - PERIPHERALS_START_ADDRESS)
		|MPU_REGION_ENABLE;

	mpu_set_region( dw_region_base_addr, dw_region_attr);


/**
 *	External EBI memory  memory region --- Strongly Ordered
 *	START_Addr:-  0x60000000UL
 *	END_Addr:-    0x6FFFFFFFUL
 */
	dw_region_base_addr =
		EXT_EBI_START_ADDRESS |
		MPU_REGION_VALID |
		MPU_EXT_EBI_REGION;

	dw_region_attr =
		MPU_AP_FULL_ACCESS |
		/* External memory Must be defined with 'Device' or 'Strongly Ordered' attribute for write accesses (AXI) */
		STRONGLY_ORDERED_SHAREABLE_TYPE |
		mpu_cal_mpu_region_size(EXT_EBI_END_ADDRESS - EXT_EBI_START_ADDRESS) |
		MPU_REGION_ENABLE;
	
		mpu_set_region( dw_region_base_addr, dw_region_attr);
	
	/**
	 *	SDRAM cacheable memory region --- Normal
	 *	START_Addr:-  0x70000000UL
	 *	END_Addr:-	  0x7FFFFFFFUL
	 */
		dw_region_base_addr =
			SDRAM_START_ADDRESS |
			MPU_REGION_VALID |
			MPU_DEFAULT_SDRAM_REGION;
	
		dw_region_attr =
			MPU_AP_FULL_ACCESS	  |
			INNER_NORMAL_WB_RWA_TYPE( SHAREABLE ) |
			mpu_cal_mpu_region_size(SDRAM_END_ADDRESS - SDRAM_START_ADDRESS) |
			MPU_REGION_ENABLE;
	
		mpu_set_region( dw_region_base_addr, dw_region_attr);
	
	/**
	 *	QSPI memory region --- Strongly ordered
	 *	START_Addr:-  0x80000000UL
	 *	END_Addr:-	  0x9FFFFFFFUL
	 */
		dw_region_base_addr =
			QSPI_START_ADDRESS |
			MPU_REGION_VALID |
			MPU_QSPIMEM_REGION;
	
		dw_region_attr =
			MPU_AP_FULL_ACCESS |
			STRONGLY_ORDERED_SHAREABLE_TYPE |
			mpu_cal_mpu_region_size(QSPI_END_ADDRESS - QSPI_START_ADDRESS) |
			MPU_REGION_ENABLE;
	
		mpu_set_region( dw_region_base_addr, dw_region_attr);
	
	
	/**
	 *	USB RAM Memory region --- Device
	 *	START_Addr:-  0xA0100000UL
	 *	END_Addr:-	  0xA01FFFFFUL
	 */
		dw_region_base_addr =
			USBHSRAM_START_ADDRESS |
			MPU_REGION_VALID |
			MPU_USBHSRAM_REGION;
	
		dw_region_attr =
			MPU_AP_FULL_ACCESS |
			MPU_REGION_EXECUTE_NEVER |
			SHAREABLE_DEVICE_TYPE |
			mpu_cal_mpu_region_size(USBHSRAM_END_ADDRESS - USBHSRAM_START_ADDRESS) |
			MPU_REGION_ENABLE;
	
		mpu_set_region( dw_region_base_addr, dw_region_attr);
	
	
		/* Enable the memory management fault , Bus Fault, Usage Fault exception */
		SCB->SHCSR |= (SCB_SHCSR_MEMFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk
						| SCB_SHCSR_USGFAULTENA_Msk);
	
		/* Enable the MPU region */
		mpu_enable( MPU_ENABLE | MPU_PRIVDEFENA);
	
		__DSB();
		__ISB();
	}
#endif

#ifdef CONF_BOARD_ENABLE_TCM_AT_INIT
#if defined(__GNUC__)
extern char _itcm_lma, _sitcm, _eitcm;
#endif

/** \brief  TCM memory enable
* The function enables TCM memories
*/
static inline void tcm_enable(void)
{

	__DSB();
	__ISB();
	
	SCB->ITCMCR = (SCB_ITCMCR_EN_Msk  | SCB_ITCMCR_RMW_Msk | SCB_ITCMCR_RETEN_Msk);
	SCB->DTCMCR = ( SCB_DTCMCR_EN_Msk | SCB_DTCMCR_RMW_Msk | SCB_DTCMCR_RETEN_Msk);
	
	__DSB();
	__ISB();
}
#else
/** \brief  TCM memory Disable

	The function enables TCM memories
 */
static inline void tcm_disable(void) 
{

	__DSB();
	__ISB();
	SCB->ITCMCR &= ~(uint32_t)(1UL);
	SCB->DTCMCR &= ~(uint32_t)SCB_DTCMCR_EN_Msk;
	__DSB();
	__ISB();
}
#endif

void board_init(void)
{
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
#ifndef CONF_BOARD_KEEP_WATCHDOG_AT_INIT
			/* Disable the watchdog */
			WDT->WDT_MR = WDT_MR_WDDIS;
#endif

#ifdef CONF_BOARD_CONFIG_MPU_AT_INIT
		_setup_memory_region();
#endif

#if 0  //not used now
#ifdef CONF_BOARD_ENABLE_TCM_AT_INIT
	/* TCM Configuration */
	EFC->EEFC_FCR = (EEFC_FCR_FKEY_PASSWD | EEFC_FCR_FCMD_CGPB 
					| EEFC_FCR_FARG(8));
	EFC->EEFC_FCR = (EEFC_FCR_FKEY_PASSWD | EEFC_FCR_FCMD_SGPB
					| EEFC_FCR_FARG(7));
	tcm_enable();

#if defined(__GNUC__)
		volatile char *dst = &_sitcm;
		volatile char *src = &_itcm_lma;
		/* copy code_TCM from flash to ITCM */
		while(dst < &_eitcm){
			*dst++ = *src++;
		}
#endif
#else
		/* TCM Configuration */
		EFC->EEFC_FCR = (EEFC_FCR_FKEY_PASSWD | EEFC_FCR_FCMD_CGPB 
						| EEFC_FCR_FARG(8));
		EFC->EEFC_FCR = (EEFC_FCR_FKEY_PASSWD | EEFC_FCR_FCMD_CGPB 
						| EEFC_FCR_FARG(7));
		
		tcm_disable();
#endif

#endif
	ioport_init();


#ifdef  CYTEK_FMB_CONSOLE_USART1
	/* Configure UART1 pins */
	ioport_set_pin_peripheral_mode(UART_RXD_GPIO, UART_RXD_FLAGS);
	MATRIX->CCFG_SYSIO |= CCFG_SYSIO_SYSIO4;
	ioport_set_pin_peripheral_mode(UART_TXD_GPIO, UART_TXD_FLAGS);
#endif
#ifdef  CYTEK_FMB_CONSOLE_UART0
	/* Configure UART0 pins */
	ioport_set_pin_peripheral_mode(UART_RXD_GPIO, UART_RXD_FLAGS);
	ioport_set_pin_peripheral_mode(UART_TXD_GPIO, UART_TXD_FLAGS);
#endif


#ifdef CYTEK_FMB_SENSOR_INPUT
/*****sensor use IO input mode*****/
#if 0
	sensor_init(SENSOR1_GPIO,SENSOR1_FLAGS,SENSOR1_SENSE);
	sensor_init(SENSOR2_GPIO,SENSOR2_FLAGS,SENSOR2_SENSE);
	sensor_init(SENSOR3_GPIO,SENSOR3_FLAGS,SENSOR3_SENSE);
	sensor_init(SENSOR4_GPIO,SENSOR4_FLAGS,SENSOR4_SENSE);
	sensor_init(SENSOR5_GPIO,SENSOR5_FLAGS,SENSOR5_SENSE);
	sensor_init(SENSOR6_GPIO,SENSOR6_FLAGS,SENSOR6_SENSE);
	sensor_init(SENSOR7_GPIO,SENSOR7_FLAGS,SENSOR7_SENSE);
	sensor_init(SENSOR8_GPIO,SENSOR8_FLAGS,SENSOR8_SENSE);
	sensor_init(SENSOR9_GPIO,SENSOR9_FLAGS,SENSOR9_SENSE);
	sensor_init(SENSOR10_GPIO,SENSOR10_FLAGS,SENSOR10_SENSE);
	sensor_init(SENSOR11_GPIO,SENSOR11_FLAGS,SENSOR11_SENSE);
	sensor_init(SENSOR12_GPIO,SENSOR12_FLAGS,SENSOR12_SENSE);
	sensor_init(SENSOR13_GPIO,SENSOR13_FLAGS,SENSOR13_SENSE);
	sensor_init(SENSOR14_GPIO,SENSOR14_FLAGS,SENSOR14_SENSE);
	sensor_init(SENSOR15_GPIO,SENSOR15_FLAGS,SENSOR15_SENSE);
	sensor_init(SENSOR16_GPIO,SENSOR16_FLAGS,SENSOR16_SENSE);
	sensor_init(SENSOR17_GPIO,SENSOR17_FLAGS,SENSOR17_SENSE);
	sensor_init(SENSOR18_GPIO,SENSOR18_FLAGS,SENSOR18_SENSE);
	sensor_init(SENSOR19_GPIO,SENSOR19_FLAGS,SENSOR19_SENSE);
	sensor_init(SENSOR20_GPIO,SENSOR20_FLAGS,SENSOR20_SENSE);
#endif
	//sensor_init(17);
#endif    //CYTEK_FMB_SENSOR_INPUT

#ifdef CYTEK_FMB_SENSOR_IRQ 
/*****sensor use  input irq  mode*****/

#endif

#ifdef CYTEK_FMB_SPI0
	ioport_set_pin_peripheral_mode(SPI0_MISO_GPIO, SPI0_MISO_FLAGS);
	ioport_set_pin_peripheral_mode(SPI0_MOSI_GPIO, SPI0_MOSI_FLAGS);
	ioport_set_pin_peripheral_mode(SPI0_SPCK_GPIO, SPI0_SPCK_FLAGS);

	ioport_set_pin_dir(SPI0_NPCS0_GPIO,IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(SPI0_NPCS0_GPIO,IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_dir(SPI0_NPCS1_GPIO,IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(SPI0_NPCS1_GPIO,IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_dir(SPI0_DEV_RESET, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(SPI0_DEV_LADC, IOPORT_DIR_OUTPUT);
#endif	//CYTEK_FMB_SPI0


#ifdef CYTEK_FMB_SPI1 
	ioport_set_pin_peripheral_mode(SPI1_MISO_GPIO, SPI1_MISO_FLAGS);
	ioport_set_pin_peripheral_mode(SPI1_NPCS0_GPIO, SPI1_NPCS0_FLAGS);
	ioport_set_pin_peripheral_mode(SPI1_NPCS1_GPIO, SPI1_NPCS1_FLAGS);
	ioport_set_pin_peripheral_mode(SPI1_NPCS2_GPIO, SPI1_NPCS2_FLAGS);
	ioport_set_pin_peripheral_mode(SPI1_SPCK_GPIO, SPI1_SPCK_FLAGS);
#endif   //CYTEK_FMB_SPI1 

#ifdef CYTEK_FMB_QSPI
	ioport_set_pin_peripheral_mode(QSPI_QSCK_GPIO, QSPI_QSCK_FLAGS);
	ioport_set_pin_peripheral_mode(QSPI_QCS_GPIO, QSPI_QCS_FLAGS);
	ioport_set_pin_peripheral_mode(QSPI_QIO0_GPIO, QSPI_QIO0_FLAGS);
	ioport_set_pin_peripheral_mode(QSPI_QIO1_GPIO, QSPI_QIO1_FLAGS);
	ioport_set_pin_peripheral_mode(QSPI_QIO2_GPIO, QSPI_QIO2_FLAGS);
	ioport_set_pin_peripheral_mode(QSPI_QIO3_GPIO, QSPI_QIO3_FLAGS);
#endif  //CYTEK_FMB_QSPI


#ifdef CYTEK_FMB_TWIHS1
	ioport_set_pin_peripheral_mode(TWIHS1_DATA_GPIO, TWIHS1_DATA_FLAGS);
	ioport_set_pin_peripheral_mode(TWIHS1_CLK_GPIO, TWIHS1_CLK_FLAGS);
#endif  //CYTEK_FMB_TWIHS1

#ifdef CYTEK_FMB_TWIHS2
	ioport_set_pin_peripheral_mode(TWIHS2_DATA_GPIO, TWIHS2_DATA_FLAGS);
	ioport_set_pin_peripheral_mode(TWIHS2_CLK_GPIO, TWIHS2_CLK_FLAGS);
#endif  //CYTEK_FMB_TWIHS2



#ifdef CYTEK_FMB_IO_LED
	ioport_as_output_init(LED1_GPIO, IOPORT_DIR_OUTPUT,LED1_INACTIVE_LEVEL);
	ioport_as_output_init(LED2_GPIO, IOPORT_DIR_OUTPUT,LED2_INACTIVE_LEVEL);
	ioport_as_output_init(LED3_GPIO, IOPORT_DIR_OUTPUT,LED3_INACTIVE_LEVEL);
	ioport_as_output_init(LED4_GPIO, IOPORT_DIR_OUTPUT,LED4_INACTIVE_LEVEL);
	ioport_as_output_init(LED5_GPIO, IOPORT_DIR_OUTPUT,LED5_INACTIVE_LEVEL);
	ioport_as_output_init(LED6_GPIO, IOPORT_DIR_OUTPUT,LED6_INACTIVE_LEVEL);
//	ioport_as_output_init(LED7_GPIO, IOPORT_DIR_OUTPUT,LED7_INACTIVE_LEVEL);  //lead to system failed
	ioport_as_output_init(LED8_GPIO, IOPORT_DIR_OUTPUT,LED8_INACTIVE_LEVEL);
	ioport_as_output_init(LED9_GPIO, IOPORT_DIR_OUTPUT,LED9_INACTIVE_LEVEL);
	ioport_as_output_init(LED10_GPIO, IOPORT_DIR_OUTPUT,LED10_INACTIVE_LEVEL);
#endif


#ifdef CYTEK_FMB_PUMP
	ioport_as_output_init(PUMP1_GPIO, IOPORT_DIR_OUTPUT,PUMP1_INACTIVE_LEVEL);
	ioport_as_output_init(PUMP2_GPIO, IOPORT_DIR_OUTPUT,PUMP2_INACTIVE_LEVEL);
	ioport_as_output_init(PUMP3_GPIO, IOPORT_DIR_OUTPUT,PUMP3_INACTIVE_LEVEL);
#endif


#ifdef CYTEK_FMB_LASER
	ioport_as_output_init(LASER1_GPIO, IOPORT_DIR_OUTPUT,LASER1_INACTIVE_LEVEL);
	ioport_as_output_init(LASER2_GPIO, IOPORT_DIR_OUTPUT,LASER2_INACTIVE_LEVEL);
	ioport_as_output_init(LASER3_GPIO, IOPORT_DIR_OUTPUT,LASER3_INACTIVE_LEVEL);
#endif

#ifdef CYTEK_FMB_POWER_STATE
	ioport_as_output_init(POWERGOOD_GPIO, IOPORT_DIR_OUTPUT,POWERGOOD_INACTIVE_LEVEL);
	ioport_as_output_init(STATEGOOD_GPIO, IOPORT_DIR_OUTPUT,STATEGOOD_INACTIVE_LEVEL);
#endif
	
#ifdef  CYTEK_FMB_SOLENOID
	ioport_as_output_init(SOLENOID1_GPIO, IOPORT_DIR_OUTPUT,SOLENOID1_INACTIVE_LEVEL);
	ioport_as_output_init(SOLENOID2_GPIO, IOPORT_DIR_OUTPUT,SOLENOID2_INACTIVE_LEVEL);
	ioport_as_output_init(SOLENOID3_GPIO, IOPORT_DIR_OUTPUT,SOLENOID3_INACTIVE_LEVEL);
	ioport_as_output_init(SOLENOID4_GPIO, IOPORT_DIR_OUTPUT,SOLENOID4_INACTIVE_LEVEL);
	ioport_as_output_init(SOLENOID5_GPIO, IOPORT_DIR_OUTPUT,SOLENOID5_INACTIVE_LEVEL);
	ioport_as_output_init(SOLENOID6_GPIO, IOPORT_DIR_OUTPUT,SOLENOID6_INACTIVE_LEVEL);
#endif

    // init in motor_init def CYTEK_FMB_MOTOR
	ioport_as_output_init(MOTOR1_GPIO, IOPORT_DIR_OUTPUT, MOTOR1_INACTIVE_LEVEL);
	ioport_as_output_init(MOTOR1_DIR, IOPORT_DIR_OUTPUT, MOTOR_DIR_DOWN);
	ioport_as_output_init(MOTOR1_PULL, IOPORT_DIR_OUTPUT, MOTOR1_PULL_INACTIVE_LEVEL);
	ioport_as_output_init(MOTOR2_GPIO, IOPORT_DIR_OUTPUT, MOTOR2_INACTIVE_LEVEL);
	ioport_as_output_init(MOTOR2_DIR, IOPORT_DIR_OUTPUT, MOTOR_DIR_DOWN);
	ioport_as_output_init(MOTOR2_PULL, IOPORT_DIR_OUTPUT, MOTOR2_PULL_INACTIVE_LEVEL);

//#ifdef CYTEK_FMB_FLOW_METER
	ioport_set_pin_peripheral_mode(TWIHS0_DATA_GPIO, TWIHS0_DATA_FLAGS);
	ioport_set_pin_peripheral_mode(TWIHS0_CLK_GPIO, TWIHS0_CLK_FLAGS);
//#endif

    AD5687_init();
}
