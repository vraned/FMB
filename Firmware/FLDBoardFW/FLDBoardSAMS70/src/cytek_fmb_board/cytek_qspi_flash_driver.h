/*
 * cytek_qspi_flash_driver.h
 *
 * Created: 2016/1/14/周四 10:13:39
 *  Author: lenovo
 */ 


#ifndef CYTEK_QSPI_FLASH_DRIVER_H_
#define CYTEK_QSPI_FLASH_DRIVER_H_

//#include <status_codes.h>
#include <qspi.h>
#include <compiler.h>
#include "cytek_fmb_global_err_table.h"

#define FLASH_ID  0x20BA18

/********instructions code ********/

	/* RESET Operations */
#define	SPI_FLASH_INS_REN		  		0x66		/* reset enable */
#define	SPI_FLASH_INS_RMEM		  		0x99		/* reset memory */

	/* IDENTIFICATION Operations */
#define	SPI_FLASH_INS_RDID        			0x9F		/* read Identification */
#define	SPI_FLASH_INS_RDID_ALT    			0x9E		/* read Identification */
#define	SPI_FLASH_INS_MULT_IO_RDID   		0xAF		/* read multiple i/o read id */
#define	SPI_FLASH_INS_DISCOVER_PARAMETER	0x5A /* read serial flash discovery parameter */

	/* READ operations */
#define	SPI_FLASH_INS_READ        			0x03		/* read data bytes */
#define	SPI_FLASH_INS_FAST_READ   			0x0B	/* read data bytes at higher speed */
#define	SPI_FLASH_INS_DOFR       			0x3B	/* dual output Fast Read */
#define	SPI_FLASH_INS_DIOFR				0x0B 	/* dual input output Fast Read */
#define	SPI_FLASH_INS_DIOFR_ALT1			0x3B 	/* dual input output Fast Read */
#define	SPI_FLASH_INS_DIOFR_ALT2   		0xBB	/* dual input output Fast Read */
#define	SPI_FLASH_INS_QOFR        			0x6B	/* quad output Fast Read */
#define	SPI_FLASH_INS_QIOFR				0x0B 	/* quad output Fast Read */
#define	SPI_FLASH_INS_QIOFR_ALT1			0x6B 	/* quad input/output Fast Read */
#define	SPI_FLASH_INS_QIOFR_ALT2   		0xEB	/* quad input output Fast Read */

	/* WRITE operations */
#define	SPI_FLASH_INS_WREN        			0x06		/* write enable */
#define	SPI_FLASH_INS_WRDI        			0x04		/* write disable */

	/* REGISTER operations */
#define	SPI_FLASH_INS_RDSR      			0x05		/* read status register */
#define	SPI_FLASH_INS_WRSR      			0x01		/* write status register */
#define	SPI_FLASH_INS_RDLR                  		0xE8 	/* read lock register */
#define	SPI_FLASH_INS_CMD_WRLR           	0xE5		/* write lock register */
#define	SPI_FLASH_INS_RFSR     			 	0x70		/* read flag status register */
#define	SPI_FLASH_INS_CLFSR     			0x50		/* clear flag status register */
#define	SPI_FLASH_INS_RDNVCR    			0xB5	/* read non volatile configuration register */
#define	SPI_FLASH_INS_WRNVCR    			0xB1	/* write non volatile configuration register */
#define	SPI_FLASH_INS_RDVCR     			0x85		/* read volatile configuration register */
#define	SPI_FLASH_INS_WRVCR     			0x81		/* write volatile configuration register */
#define	SPI_FLASH_INS_RDVECR    			0x65		/* read volatile enhanced configuration register */
#define	SPI_FLASH_INS_WRVECR    			0x61		/* write volatile enhanced configuration register */

	/* PROGRAM operations */
#define	SPI_FLASH_INS_PP          			0x02		/* PAGE PROGRAM */
#define	SPI_FLASH_INS_DIPP        			0xA2	/* DUAL INPUT FAST PROGRAM */
#define	SPI_FLASH_INS_DIEPP				0x02		/* EXTENDED DUAL INPUT FAST PROGRAM */
#define	SPI_FLASH_INS_DIEPP_ALT1			0xA2 	/* EXTENDED DUAL INPUT FAST PROGRAM (alt 1) */
#define	SPI_FLASH_INS_DIEPP_ALT2   		0xD2	/* EXTENDED DUAL INPUT FAST PROGRAM (alt 2) */
#define	SPI_FLASH_INS_QIPP        			0x32		/* QUAD INPUT FAST PROGRAM */

#define	SPI_FLASH_INS_QIEPP				0x02 	/* EXTENDED QUAD INPUT FAST PROGRAM */
#define	SPI_FLASH_INS_QIEPP_ALT1			0x32 	/* EXTENDED QUAD INPUT FAST PROGRAM (alt 1) */
#define	SPI_FLASH_INS_QIEPP_ALT2			0x38 	/* EXTENDED QUAD INPUT FAST PROGRAM */

	/* ERASE operations */
#define	SPI_FLASH_INS_SSE         			0x20		/* subsector erase */
#define	SPI_FLASH_INS_SE          				0xD8	/* sector erase */
#define	SPI_FLASH_INS_DE		  			0xC4	/* die erase */
#define	SPI_FLASH_INS_PER         			0x7A	/* program Erase Resume */
#define	SPI_FLASH_INS_PES         			0x75		/* program Erase Suspend */

	/* OTP operations */
#define	SPI_FLASH_INS_RDOTP				0x4B 	/* read OTP array */
#define	SPI_FLASH_INS_PROTP				0x42 	/* program OTP array */

	/* 4-BYTE ADDRESS operation  */
#define	SPI_FLASH_4B_MODE_ENTER   		0xB7	/* enter 4-byte address mode */
#define	SPI_FLASH_4B_MODE_EXIT	  		0xE9		/* exit 4-byte address mode */

	/* DEEP POWER-DOWN operation */
#define	SPI_FLASH_INS_ENTERDPD			0xB9	 /* enter deep power-down */
#define	SPI_FLASH_INS_RELEASEDPD			0xA8 	 /* release deep power-down */

struct flash_description
{
	uint32_t		flash_id;
	uint8_t		flash_type;
	uint32_t		starting_address;/* Start Address of the Flash Device */
	uint32_t		flash_page_addr_mask;
	uint32_t		flash_subsector_addr_mask;
	uint32_t		flash_sector_addr_mask;
	uint32_t		flash_sector_count;
	uint32_t		flash_subsector_count;
	uint32_t		flash_subsector_size_bit;
	uint32_t		flash_page_size;
	uint32_t		flash_page_count;
	uint32_t		flash_sector_size;
	uint32_t		flash_sector_size_bit;
	uint32_t		flash_subsector_size;
	uint32_t 		flash_size;
	uint32_t		flash_OTP_size;
	uint8_t		flash_die_count;
	uint32_t		flash_die_size;
	uint32_t		flash_die_size_bit;
	uint32_t		size;			/* The density of flash device in bytes */
	uint32_t		buffer_size;			/* In bytes */
	uint8_t		data_width;		/* In bytes */
	uint32_t		num_addr_byte;	/* Num of bytes used to address memory */
} ;

 struct flash_operation
{
	uint32_t (*block_offset)(uint16_t);
	uint32_t (*read_device_id)(void ); 
	uint8_t (*read_status_register)(void);
	void (*soft_reset)(void);
	enum cytek_fmb_state (*data_program)(uint32_t addr,uint8_t * buffer , uint16_t size);
	enum cytek_fmb_state (*data_read)(uint32_t addr,uint8_t *buffer,uint32_t size,uint8_t mode);
	enum cytek_fmb_state (*sector_erase)( uint8_t sector_number );
	enum cytek_fmb_state (*subsector_erase)( uint16_t subsector_number );
	void (*die_erase)(void);
	void (*bulk_erase)(void);
	void (*write_enable)(void);
	void (*write_disable)(void);
	enum cytek_fmb_state (*program_erase_suspend)(void);
	enum cytek_fmb_state (*program_erase_resume)(void);
	void (*clear_flag_status_register)(void);
	uint16_t (*read_nonvolatile_configuration_register)(void);
	uint8_t (*read_volatile_configuration_register)(void);
	uint8_t (*read_volatile_enhanced_configuration_register)(void);
	uint8_t (*read_flag_status_register)(void);
	enum cytek_fmb_state (*write_nonvolatile_configuration_register)(uint16_t value);
	enum cytek_fmb_state (*write_volatile_configuration_register)(uint8_t);
	enum cytek_fmb_state (*write_volatile_enhanced_configuration_register)(uint8_t);
	enum cytek_fmb_state (*enter_4byte_address_mode) (void);
	enum cytek_fmb_state (*exit_4byte_address_mode) (void);
	enum cytek_fmb_state (*lock_sector)(uint32_t addr,uint32_t len);
	uint8_t (*read_lock_register)(void);
	void (*unlock_all_sector)(void);
	enum cytek_fmb_state (*OTP_program)(uint8_t *, uint32_t);
	enum cytek_fmb_state (*OTP_read)(uint8_t *, uint32_t);

};

 struct flash_object
 {
	struct flash_description Desc;
	struct flash_operation	GenOp;
 };


/*******************************************************************************
Status Register Definitions (see Datasheet)
*******************************************************************************/
enum status_bit
{
	SPI_FLASH_SRWD		= 0x80,				/* status Register Write Protect */
	SPI_FLASH_BP3		= 0x40,				/* block Protect Bit3 */
	SPI_FLASH_TB			= 0x20,				/* top/Bottom bit */
	SPI_FLASH_BP2		= 0x10,				/* block Protect Bit2 */
	SPI_FLASH_BP1		= 0x08,				/* block Protect Bit1 */
	SPI_FLASH_BP0		= 0x04,				/* block Protect Bit0 */
	SPI_FLASH_WEL		= 0x02,				/* write Enable Latch */
	SPI_FLASH_WIP		= 0x01,				/* write/Program/Erase in progress bit */
};

/*******************************************************************************
flag Status Register Definitions (see Datasheet)
*******************************************************************************/
enum
{
	SPI_FSR_PROG_ERASE_CTL		= 0x80,
	SPI_FSR_ERASE_SUSP			= 0x40,
	SPI_FSR_ERASE				= 0x20,
	SPI_FSR_PROGRAM				= 0x10,
	SPI_FSR_VPP					= 0x08,
	SPI_FSR_PROG_SUSP			= 0x04,
	SPI_FSR_PROT					= 0x02,
	SPI_FSR_ADDR_MODE			= 0x01
};

/* Status register masks */
#define SPI_SR1_WIP				(1 << 0)
#define SPI_SR1_WEL				(1 << 1)
#define SPI_SR1_BP0				(1 << 2)
#define SPI_SR1_BP1				(1 << 3)
#define SPI_SR1_BP2				(1 << 4)
#define SPI_SR1_E_FAIL			(1 << 5)
#define SPI_SR1_P_FAIL			(1 << 6)
#define SPI_SR1_SRWD			(1 << 7)

enum cytek_fmb_state flash_read(uint32_t addr, uint8_t *buffer, uint32_t size);
enum cytek_fmb_state flash_write(uint32_t addr, uint8_t *buffer, uint32_t size);
enum cytek_fmb_state flash_init(void);

//void flash_debug(void);


#endif /* CYTEK_QSPI_FLASH_DRIVER_H_ */
