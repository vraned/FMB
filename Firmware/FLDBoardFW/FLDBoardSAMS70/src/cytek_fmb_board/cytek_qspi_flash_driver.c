/*
 * cytek_qspi_flash_driver.c
 *
 * Created: 2016/1/14/周四 10:13:15
 *  Author: lenovo
 */ 

#include  <qspi.h>

#include "cytek_config.h"
#include <delay.h>
#include <sysclk.h>
#include "cytek_qspi_flash_driver.h"


#define DEBUG_FLASH

#ifdef CONFIG_DEBUG_QSPI_FLASH
#define dbg_printf printf
#else
#define dbg_printf(...)
#endif

#define COUNT_FOR_FLASH_TIMEOUT  0x10000000

/*dev_frame: operate flash register in qspi mode*/
 static struct qspi_inst_frame_t *dev_frame;

/*mem_frame: operate flash memerory in qspi mode*/
static struct qspi_inst_frame_t *mem_frame;

static struct flash_object *flash_obj;

struct qspid_t *qspi_driver;

static struct qspi_config_t mode_config = {QSPI_MR_SMM_MEMORY, false, false, QSPI_LASTXFER, QSPI_MR_NBBITS_8_BIT, 0, 0, 0, 0, 50000000, 0, false, false, 0};

static void n25q128_memory_access(struct qspid_t *qspid, uint8_t instr, uint32_t addr, uint32_t *tx_data, uint32_t *rx_data, enum qspi_access read_write, uint32_t size, uint8_t secure)
{
	qspid->qspi_command.instruction = instr;
	qspid->qspi_buffer.data_tx = tx_data;
	qspid->qspi_buffer.data_rx = rx_data;
	mem_frame->addr = addr;
	mem_frame->inst_frame.bm.b_inst_en = 1;
	mem_frame->inst_frame.bm.b_data_en = 1;
	mem_frame->inst_frame.bm.b_addr_en = 1;
	qspid->qspi_frame = mem_frame;

	if (read_write == QSPI_WRITE_ACCESS) {
		mem_frame->inst_frame.bm.b_tfr_type = (QSPI_IFR_TFRTYP_TRSFR_WRITE_MEMORY >> QSPI_IFR_TFRTYP_Pos);
		qspid->qspi_buffer.tx_data_size = size;
	} else {
#if 0      //zrw remove
		mem_frame->addr += 1;
#endif	
		mem_frame->inst_frame.bm.b_tfr_type = (QSPI_IFR_TFRTYP_TRSFR_READ_MEMORY >> QSPI_IFR_TFRTYP_Pos);
		qspid->qspi_buffer.rx_data_size = size;		
	}
	qspi_flash_access_memory(qspid, read_write, secure);
}

static void n25q128_exec_command(struct qspid_t *qspid, uint8_t instr, uint32_t *tx_data, uint32_t *rx_data, enum qspi_access read_write, uint32_t size)
{
	
	qspid->qspi_command.instruction = instr;
	dev_frame->inst_frame.bm.b_inst_en = 1;
	qspid->qspi_frame = dev_frame;
	qspid->qspi_buffer.data_tx = tx_data;
	qspid->qspi_buffer.data_rx = rx_data;
#if 0
	/** To prevent unaligned access , size:0, 4, 8, 12, 16,...*/
	if((size % sizeof(uint32_t)) && size > 1) {
		size += (sizeof(uint32_t) - (size % sizeof(uint32_t)));
	}
#endif
	if(read_write == QSPI_CMD_ACCESS) {
		dev_frame->inst_frame.bm.b_tfr_type = (QSPI_IFR_TFRTYP_TRSFR_READ >> QSPI_IFR_TFRTYP_Pos);
		dev_frame->inst_frame.bm.b_data_en = 0;
	} else if (read_write == QSPI_WRITE_ACCESS) {
		dev_frame->inst_frame.bm.b_data_en = 1;
		dev_frame->inst_frame.bm.b_tfr_type = (QSPI_IFR_TFRTYP_TRSFR_WRITE >> QSPI_IFR_TFRTYP_Pos);
		qspid->qspi_buffer.tx_data_size = size;
	} else {
		dev_frame->inst_frame.bm.b_tfr_type = (QSPI_IFR_TFRTYP_TRSFR_READ >> QSPI_IFR_TFRTYP_Pos);
		dev_frame->inst_frame.bm.b_data_en = 1;
		qspid->qspi_buffer.rx_data_size = size;
	}

	qspi_flash_execute_command(qspid, read_write);	
}

static void n25q128_read_register(uint8_t command,uint8_t *buffer,uint32_t size)
{
	n25q128_exec_command(qspi_driver,command,0,(uint32_t *)buffer,QSPI_READ_ACCESS,size);
}

static void n25q128_write_register(uint8_t command,uint8_t *buffer,uint32_t size)
{
	n25q128_exec_command(qspi_driver,command,(uint32_t *)buffer,0,QSPI_WRITE_ACCESS,size);
}

static void n25q128_send_command(uint8_t command)
{
	n25q128_exec_command(qspi_driver,command,NULL,NULL,QSPI_CMD_ACCESS,0);
}


static uint32_t n25q128_read_id(void)
{
	uint32_t read_id;
	uint32_t device_id = 0;
	uint8_t buffer;
	uint8_t *ptr = (uint8_t *)&read_id;

	n25q128_exec_command(qspi_driver,SPI_FLASH_INS_RDID,0,&read_id,QSPI_READ_ACCESS,3);
	buffer = *(ptr+2);
	*(ptr+2) = *(ptr);
	*(ptr) = buffer;
	device_id = read_id & 0xffffff;
	return device_id;
}

static uint8_t n25q128_read_volatile_config_reg(void)
{
	uint8_t volatile_config_reg = 0;
	n25q128_read_register(SPI_FLASH_INS_RDVCR,&volatile_config_reg,1);
	return volatile_config_reg;
}

static uint8_t n25q128_read_enhanced_volatile_config_reg(void)
{
	uint8_t volatile_config_reg = 0;
	n25q128_read_register(SPI_FLASH_INS_RDVECR,&volatile_config_reg,1);
	return volatile_config_reg;
}

static uint16_t n25q128_read_nonvolatile_config_reg(void)
{
//!!! be careful of bit 0, and it is good to set it to 1
	uint8_t buffer[2]={0,0,};
	uint16_t nonvolatile_config_reg = 0;
	n25q128_read_register(SPI_FLASH_INS_RDNVCR,buffer,2);
	nonvolatile_config_reg = (buffer[0] << 8) +buffer[1];
	return nonvolatile_config_reg;
}

static uint8_t n25q128_read_flag_status_reg(void)
{
	uint8_t flag_status_reg = 0;
	n25q128_read_register(SPI_FLASH_INS_RFSR,&flag_status_reg,1);
	return flag_status_reg;
}

static uint8_t n25q128_read_status_reg(void)
{
	uint8_t status_reg = 0;
	n25q128_read_register(SPI_FLASH_INS_RDSR,&status_reg,1);
	return status_reg;
}

static uint8_t n25q128_read_lock_reg(void)
{
	uint8_t lock_reg;
	n25q128_read_register(SPI_FLASH_INS_RDLR,&lock_reg,1);
	return lock_reg;
}


static void n25q128_reset_enable(void)
{
	n25q128_send_command(SPI_FLASH_INS_REN);
}

static void n25q128_reset_memory(void)
{
	n25q128_send_command(SPI_FLASH_INS_RMEM);
}


static void n25q128_soft_reset(void)
{
	n25q128_reset_enable();
	delay_us(10);
	n25q128_reset_memory();
	delay_us(10);	
}

static void n25q128_clear_flag_status_reg(void)
{
	n25q128_send_command(SPI_FLASH_INS_CLFSR);
}

/*n25q128_check_busy: check if flash is  writing/programing/erasing
	return true if yes, while return fase if no
*/
static bool n25q128_check_busy(void)
{	
	uint8_t status_reg;
	uint8_t check_bit = SPI_FLASH_WIP;
	status_reg = n25q128_read_status_reg();
	if( check_bit & status_reg )
		return true;
	else
		return false;
}

static enum cytek_fmb_state n25q128_timeout(uint32_t seconds)
{

	static uint32_t counter = 0;
	if (seconds == 0)   /* Set timeout to 0 */
		counter = 0;

	if (counter >= (seconds * COUNT_FOR_FLASH_TIMEOUT)){
		counter = 0;
		return FLASH_TIMEOUT;
	}else{
		counter++;
		return FLASH_OPERATION_ONGONING;
	} 
} 
static enum cytek_fmb_state  n25q128_wait_cmd_excution_complete( uint32_t seconds)
{
	n25q128_timeout(0);
	while( n25q128_check_busy()){
		if (n25q128_timeout(seconds) == FLASH_TIMEOUT)
			return FLASH_TIMEOUT;
	}
	return FLASH_OK;
}

static void n25q128_write_enable(void)
{
	uint8_t status_reg;
	n25q128_send_command(SPI_FLASH_INS_WREN);
	do{
		status_reg = n25q128_read_status_reg();
	}while(~status_reg & SPI_SR1_WEL);
}

static void n25q128_write_disable(void)
{
	uint8_t status_reg;
	n25q128_send_command(SPI_FLASH_INS_WRDI);
	do{
		status_reg = n25q128_read_status_reg();
	}while(status_reg & SPI_SR1_WEL);
}

static enum cytek_fmb_state n25q128_write_volatile_config_reg(uint8_t value)
{
	enum cytek_fmb_state state;
	n25q128_write_enable();
	delay_us(1);
	n25q128_write_register(SPI_FLASH_INS_WRVCR,&value,1);
	state = n25q128_wait_cmd_excution_complete(1);
	n25q128_write_disable();
	return state;
}

static enum cytek_fmb_state n25q128_write_enhanced_volatile_config_reg(uint8_t value)
{
	enum cytek_fmb_state state;
	n25q128_write_enable();
	delay_us(1);
	n25q128_write_register(SPI_FLASH_INS_WRVECR,&value,1);
	state = n25q128_wait_cmd_excution_complete(1);
	n25q128_write_disable();
	return state;
}

static enum cytek_fmb_state n25q128_write_nonvolatile_config_reg(uint16_t value)
{
	//!!! be careful of bit 0, and it is good to set it to 1
	uint8_t buffer[3];
	enum cytek_fmb_state state;
	buffer[0] = (value>>8)&0xff;    //high byte first be sent
	buffer[1] = value&0xff;            //low byte later
	n25q128_write_enable();
	delay_us(1);
	dbg_printf("send: 0x%x, 0x%x\n",buffer[0],buffer[1]);
	n25q128_write_register(SPI_FLASH_INS_WRNVCR,buffer,2);
	state = n25q128_wait_cmd_excution_complete(1);
	n25q128_write_disable();
	return state;
}

static void n25q128_write_status_reg(uint8_t value)
{
	n25q128_write_enable();
	delay_us(1);
	n25q128_write_register(SPI_FLASH_INS_WRSR,&value,1);
	n25q128_write_disable();
}

/*n25q128_lock_sector: not verified yet*/
static enum cytek_fmb_state n25q128_lock_sector(uint32_t address,uint32_t len)
{
	uint8_t TB, BP, SR;
	uint32_t i, protected_area, start_sector;
	uint32_t sector_size, num_of_sectors;

	sector_size = flash_obj ->Desc.flash_sector_size;
	num_of_sectors = flash_obj ->Desc.flash_sector_count;

	n25q128_write_enable();

	start_sector = address / sector_size;
	protected_area = len / sector_size;

	if (protected_area == 0 || protected_area > num_of_sectors)
		return FLASH_ERROR_INVALID_ADDR;

	if ((start_sector != 0 && (start_sector + protected_area) != num_of_sectors) || (protected_area & (protected_area - 1)) != 0)
		return FLASH_ERROR_INVALID_ADDR;

	if (address/sector_size < num_of_sectors/2){
		TB = 1;
	}else{
		TB = 0;
	}

	BP = 1;
	for (i = 1; i <= num_of_sectors; i = i*2){
		if (protected_area == i){
			break;
		}
		BP++;
	}

	SR = (((BP & 8) >> 3) << 6) | (TB << 5) | ((BP & 7) << 2);

	n25q128_write_status_reg(SR);
	return FLASH_OK;

}


/*flash_unlock_allsector: not verified yet*/
static void n25q128_unlock_allsector(void)
{
	n25q128_write_status_reg(0x00);
}

/*******************************************************************************
n25q128_block_offset:
Description:  This function is used to obtain the sector's start address
		Return the sector start address
*******************************************************************************/
uint32_t n25q128_block_offset(uint16_t sector_number)
{
	return (sector_number << flash_obj ->Desc.flash_sector_size_bit);
}


static enum cytek_fmb_state  n25q128_gen_program(uint32_t addr, uint8_t *buffer , uint32_t size, uint8_t mode)
{
	uint8_t flag_status_reg;
	enum cytek_fmb_state ret;

	// Step 1: Validate address input
	if(!(addr < flash_obj ->Desc.flash_size))
		return FLASH_ERROR_INVALID_ADDR;

	// Step 2: Check whether any previous Write, Program or Erase cycle is on-going
	if(n25q128_check_busy()) 
		return FLASH_OPERATION_ONGONING;

	// Step 3: Disable Write protection
	flash_obj ->GenOp.write_enable();
	
	//step 4: Send data in buffer
	n25q128_memory_access(qspi_driver,mode,addr,(uint32_t *) buffer,0,QSPI_WRITE_ACCESS,size,0);
	
	// Step 5: Wait until the operation completes or a timeout occurs.
	ret = n25q128_wait_cmd_excution_complete(1);

	flag_status_reg  = n25q128_read_flag_status_reg();
	n25q128_clear_flag_status_reg();

	if((flag_status_reg & SPI_FSR_PROT) && (flag_status_reg & SPI_FSR_PROGRAM))
		return FLASH_SECTOR_PROTECTED;

	return ret;
}


/*FlashDataProgram:  page program*/
static enum cytek_fmb_state n25q128_data_program(uint32_t addr, uint8_t *buffer , uint16_t size, uint8_t mode)
{
	enum cytek_fmb_state ret = FLASH_OK;
	
	// record how many bytes can be to programed in one page
	uint16_t data_offset;

	if( addr+size > flash_obj->Desc.flash_size)
		return FLASH_ERROR_INVALID_ADDR;
	
	// Enabling the Write
	flash_obj->GenOp.write_enable();
	
	// Computing the starting alignment, i.e. the distance from the 64 bytes boundary
	data_offset = (flash_obj->Desc.flash_page_size - (addr & flash_obj->Desc.flash_page_addr_mask) ) & flash_obj->Desc.flash_page_addr_mask;
	if (data_offset > size)
		data_offset = size;
	if (data_offset > 0){
		//programe data_offset bytes in the addr start page 
		ret = n25q128_gen_program(addr, buffer, data_offset, mode);
		if (FLASH_OK != ret)
			return ret;
	}

	for ( ; (data_offset+flash_obj->Desc.flash_page_size) < size; data_offset += flash_obj->Desc.flash_page_size){
		ret = n25q128_gen_program(addr+data_offset, buffer+data_offset, flash_obj->Desc.flash_page_size, mode);
		if (FLASH_OK != ret)
			return ret;
	}

	if (size > data_offset)
		ret = n25q128_gen_program(addr+data_offset, buffer+data_offset, (size -data_offset), mode);

	return ret;
}


static enum cytek_fmb_state n25q128_program(uint32_t addr,uint8_t * buffer , uint16_t size)
{
	 return n25q128_data_program(addr,buffer,size,SPI_FLASH_INS_PP);
}


/*n25q128_read: read data from flash, and must be 4 bytes aligned*/
static enum cytek_fmb_state n25q128_read(uint32_t addr,uint8_t *buffer,uint32_t size,uint8_t mode)
{
	if(addr >= flash_obj->Desc.flash_size)
		return FLASH_ERROR_INVALID_ADDR;

	n25q128_memory_access(qspi_driver,mode,addr,0,(uint32_t *) buffer,QSPI_READ_ACCESS,size,0);

	return FLASH_OK;
}

static enum cytek_fmb_state n25q128_sector_erase( uint8_t sector_number )
{
	uint8_t send_buffer[3];
	uint8_t flag_status_reg;
	uint32_t sector_addr;

	// Step 1: Validate the sector number input
	if( sector_number >= flash_obj ->Desc.flash_sector_count)
		return FLASH_ERROR_INVALID_SECTORNR;
	
	sector_addr = flash_obj ->GenOp.block_offset(sector_number);
	
	// Step 2: Check whether any previous Write, Program or Erase cycle is on going

	if(n25q128_check_busy()) 
		return FLASH_OPERATION_ONGONING;

	// Step 3: Disable Write protection
	flash_obj ->GenOp.write_enable();

	// Step 4: Initialize the data (address) packet to be sent serially
	send_buffer[0] = ( sector_addr & 0xff0000 ) >>16;
	send_buffer[1] = ( sector_addr & 0xff00 ) >>8;
	send_buffer[2] = sector_addr & 0xff;
	
	// Step 5: Send the packet (Instruction & address) serially
	n25q128_write_register(SPI_FLASH_INS_SE,send_buffer,3);
	
	// Step 6: Wait until the operation completes or a timeout occurs.
	n25q128_wait_cmd_excution_complete(3);
	
	flag_status_reg = n25q128_read_flag_status_reg();
	n25q128_clear_flag_status_reg();	
	
	if((flag_status_reg & SPI_FSR_PROT) && (flag_status_reg & SPI_FSR_ERASE))
		return FLASH_SECTOR_PROTECTED;
	
	return FLASH_OK;
}

/*subsector_number: 0~4095*/
static enum cytek_fmb_state n25q128_subsector_erase( uint16_t subsector_number )
{
	uint8_t  send_buffer[3];
	uint8_t flag_status_reg;
	uint32_t subsector_addr = 0;

	// Step 1: Validate the subsector number input
	if( subsector_number >= flash_obj ->Desc.flash_subsector_count )
		return FLASH_ERROR_INVALID_SUBSECTORNR;

	subsector_addr = subsector_number << flash_obj ->Desc.flash_subsector_size_bit;
		
	// Step 2: Check whether any previous Write, Program or Erase cycle is on going
	if(n25q128_check_busy()) 
		return FLASH_OPERATION_ONGONING;

	// Step 3: Disable Write protection
	flash_obj ->GenOp.write_enable();
	
	// Step 4: Initialize the data (address) packet to be sent serially
	send_buffer[0] = ( subsector_addr & 0xff0000 ) >> 16;
	send_buffer[1] = ( subsector_addr & 0xff00 ) >> 8;
	send_buffer[2] = subsector_addr & 0xff;

	// Step 5: Send the packet (Instruction & address) serially
	n25q128_write_register(SPI_FLASH_INS_SSE,send_buffer,3);
		
	// Step 6: Wait until the operation completes or a timeout occurs.
	n25q128_wait_cmd_excution_complete(3);

	flag_status_reg = n25q128_read_flag_status_reg(); 
	n25q128_clear_flag_status_reg();
			
	if((flag_status_reg & SPI_FSR_PROT) && (flag_status_reg & SPI_FSR_ERASE))
		return FLASH_SECTOR_PROTECT;
			
	return FLASH_OK;
}


enum cytek_fmb_state flash_write(uint32_t addr, uint8_t *buffer, uint32_t size)
{
	uint16_t subsector_start = 0;
	uint16_t subsector_end = 0;
	enum cytek_fmb_state ret;

	if( (addr+size) > flash_obj->Desc.flash_size)
		return FLASH_ERROR_INVALID_ADDR;
	
	subsector_start = addr >> flash_obj->Desc.flash_subsector_size_bit ;
	subsector_end= (addr+size) >> flash_obj->Desc.flash_subsector_size_bit;

	dbg_printf("erase_subsector_start: 0x%x    erase_subsector_end: 0x%x\n",subsector_start ,subsector_end);
	
	for(;subsector_start <= subsector_end; subsector_start++ ){
		dbg_printf("erase_subsector:  0x%x\n",subsector_start);
		ret = n25q128_subsector_erase(subsector_start);		
		if( ret != FLASH_OK)
			return ret;
		delay_us(1);
	}

	return n25q128_data_program(addr,buffer,size,SPI_FLASH_INS_PP);
}

enum cytek_fmb_state flash_read(uint32_t addr, uint8_t *buffer, uint32_t size)
{
	return flash_obj ->GenOp.data_read(addr,buffer,size,SPI_FLASH_INS_READ);
}


static void flash_frame_init(void)
{
	dev_frame = (struct qspi_inst_frame_t *)malloc (sizeof(struct qspi_inst_frame_t));
	memset(dev_frame, 0, sizeof(struct qspi_inst_frame_t));
	dev_frame->inst_frame.bm.b_width = QSPI_IFR_WIDTH_SINGLE_BIT_SPI;  //default mode

	mem_frame = (struct qspi_inst_frame_t *)malloc (sizeof(struct qspi_inst_frame_t));
	memset(mem_frame, 0, sizeof(struct qspi_inst_frame_t));
	mem_frame->inst_frame.bm.b_width = QSPI_IFR_WIDTH_SINGLE_BIT_SPI;
}

static void flash_driver_init(void)
{ 
	qspi_driver = (struct qspid_t *)malloc (sizeof(struct qspid_t));
	memset(qspi_driver, 0, sizeof(struct qspid_t));
	qspi_driver->qspi_hw = QSPI;

}

enum cytek_fmb_state flash_init(void)
{
	enum status_code status = STATUS_OK;
	uint32_t device_id = 0;
	pmc_enable_periph_clk(ID_QSPI);
	flash_frame_init();
	flash_driver_init();
	
	//initialise the qspi port
	status = qspi_initialize(qspi_driver->qspi_hw,&mode_config);
	 if(status != STATUS_OK ){
		printf("ERROR: %s failed\n",__func__);
		return FLASH_ERROR_INIT;
	 }
	 
	 device_id =  n25q128_read_id();
	 if(device_id != FLASH_ID){
	 	printf("ERROR: flash read id error!\n");
	 	return FLASH_ERROR_INIT;
	 }
	 
	flash_obj = (struct flash_object *)malloc (sizeof(struct flash_object));
	memset(flash_obj, 0, sizeof(struct flash_object));

	/* flash description */
	flash_obj ->Desc.flash_id = 0x20ba18;
	flash_obj ->Desc.flash_size = 0x1000000;
	flash_obj ->Desc.flash_sector_count = 0x100;
	flash_obj ->Desc.flash_sector_size = 0x10000;
	flash_obj ->Desc.flash_sector_size_bit = 16;
	flash_obj ->Desc.flash_subsector_count = 0x1000;
	flash_obj ->Desc.flash_subsector_size= 0x1000;
	flash_obj ->Desc.flash_subsector_size_bit = 12;
	flash_obj ->Desc.flash_page_count = 0x10000;
	flash_obj ->Desc.flash_page_size = 0x100;
	flash_obj ->Desc.flash_OTP_size = 0x40;
	flash_obj ->Desc.flash_page_addr_mask = 0x00FF;
	flash_obj ->Desc.flash_subsector_addr_mask = 0x0FFF;
	flash_obj ->Desc.flash_sector_addr_mask = 0xFFFF;	
	flash_obj ->Desc.num_addr_byte = 3;	// this device support only 3 byte address mode 

	/* device operation */
	flash_obj ->GenOp.read_device_id = n25q128_read_id;
	flash_obj ->GenOp.soft_reset = n25q128_soft_reset;
	flash_obj ->GenOp.read_status_register = n25q128_read_status_reg;
	flash_obj ->GenOp.data_program = n25q128_program;   
	flash_obj ->GenOp.data_read = n25q128_read; 
	flash_obj ->GenOp.sector_erase = n25q128_sector_erase;  
	flash_obj ->GenOp.subsector_erase = n25q128_subsector_erase; 
	flash_obj ->GenOp.die_erase = NULL;
	flash_obj ->GenOp.bulk_erase = NULL;
	flash_obj ->GenOp.block_offset = n25q128_block_offset; 
	flash_obj ->GenOp.write_enable = n25q128_write_enable;  
	flash_obj ->GenOp.write_disable = n25q128_write_disable;
	flash_obj ->GenOp.program_erase_suspend = NULL;
	flash_obj ->GenOp.program_erase_resume = NULL;
	flash_obj ->GenOp.clear_flag_status_register = n25q128_clear_flag_status_reg;
	flash_obj ->GenOp.read_nonvolatile_configuration_register = n25q128_read_nonvolatile_config_reg;
	flash_obj ->GenOp.read_volatile_configuration_register = n25q128_read_volatile_config_reg;
	flash_obj ->GenOp.read_volatile_enhanced_configuration_register = n25q128_read_enhanced_volatile_config_reg;
	flash_obj ->GenOp.read_flag_status_register  = n25q128_read_flag_status_reg;   
	flash_obj ->GenOp.write_nonvolatile_configuration_register = n25q128_write_nonvolatile_config_reg;
	flash_obj ->GenOp.write_volatile_configuration_register= n25q128_write_volatile_config_reg;
	flash_obj ->GenOp.write_volatile_enhanced_configuration_register = n25q128_write_enhanced_volatile_config_reg;
	flash_obj ->GenOp.enter_4byte_address_mode = NULL;
	flash_obj ->GenOp.exit_4byte_address_mode = NULL;
	flash_obj ->GenOp.lock_sector = n25q128_lock_sector; 
	flash_obj ->GenOp.read_lock_register = n25q128_read_lock_reg;
	flash_obj ->GenOp.unlock_all_sector = n25q128_unlock_allsector;   
	flash_obj ->GenOp.OTP_program = NULL; 
	flash_obj ->GenOp.OTP_read = NULL;

	return FLASH_OK;
}

#if 0
void flash_debug(void)
{
	enum cytek_fmb_state state;

#define BUFFER_SIZE  5120   //BUFFER_SIZE is limited by stack size,so change the stack size before change BUFFER_SIZE
#define FLASH_ADDRESS   0xaeffff

	uint32_t i = 0;	
	uint8_t buffer[BUFFER_SIZE];
	flash_init();
	for(; i < BUFFER_SIZE; i++)
		buffer[i] = i%256;
	state = flash_write(FLASH_ADDRESS,buffer,BUFFER_SIZE);
	while( state != FLASH_OK ){
		dbg_printf(" Flash write error   %d\n",state);
	}
	
	for(; i < BUFFER_SIZE; i++)
		buffer[i] = 0;
	while(1){
		flash_read(FLASH_ADDRESS,buffer,BUFFER_SIZE);
		for(i=0;i<BUFFER_SIZE;i++)
			dbg_printf("%d ", buffer[i]);
	
	}
}

#endif


