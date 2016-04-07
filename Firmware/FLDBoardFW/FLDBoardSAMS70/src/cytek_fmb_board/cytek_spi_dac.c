/*
 * cytek_AD5687.c
 *ADC5687 user interface
 * Created: 2015/11/18/周三 17:53:36
 *  Author: lenovo
 */ 

 #include <ioport.h>
 #include <spi.h>
 #include <cytek_spi_dac.h>
 #include <cytek_time.h>
 #include <compiler.h>



 /**
	 *initialize the spi port.
	 *p_spi: point to 
	 * ul_pcs: npcs
	 * set_bits: read bits , just like SPI_CSR_BITS_11_BIT,and so on
 **/

 static void AD5687_spi_master_initialize(Spi *p_spi,uint8_t ul_pcs_ch,uint32_t set_bits)
 {
	 //puts("-I- Initialize SPI as master\r");
 
	 /* Configure an SPI peripheral. */
	 spi_enable_clock(p_spi);
	 spi_disable(p_spi);
	 spi_reset(p_spi);
	 spi_set_lastxfer(p_spi);
	 spi_set_master_mode(p_spi);
	 spi_disable_mode_fault_detect(p_spi);
	 spi_set_peripheral_chip_select_value(p_spi, spi_get_pcs(ul_pcs_ch));
	 spi_set_clock_polarity(p_spi, ul_pcs_ch, 0); 
	 spi_set_clock_phase(p_spi, ul_pcs_ch, 0);
	 spi_set_bits_per_transfer(p_spi, ul_pcs_ch,set_bits);
	 spi_set_baudrate_div(p_spi, ul_pcs_ch,(sysclk_get_peripheral_hz()/500000));
	 spi_set_transfer_delay(p_spi, ul_pcs_ch, 0x80,0xff);
	 //spi_set_transfer_delay(p_spi, ul_pcs_ch, 0x80,0x180);
	 spi_enable(p_spi);
 }


static void WriteRegister(uint32_t cmd_data,uint8_t ul_pcs)
 {
 	uint16_t cmd = (cmd_data>>16) & 0x00ff;
	uint16_t data_high8 = (cmd_data>>8) & 0x00ff;
	uint16_t data_low8 = cmd_data & 0x00ff;

	if(ul_pcs)
		ioport_set_pin_level(AD5687_CS1, PIN_LOW); //cs/sync
	else
		ioport_set_pin_level(AD5687_CS0, PIN_LOW);
	mdelay(1);
		
	spi_write(AD5687_SPI,cmd,ul_pcs,0);
	spi_write(AD5687_SPI,data_high8,ul_pcs,0);
	spi_write(AD5687_SPI,data_low8,ul_pcs,0);
	mdelay(1);
	 if(ul_pcs)
		 ioport_set_pin_level(AD5687_CS1, PIN_HIGH); //cs/syns
	else
		ioport_set_pin_level(AD5687_CS0, PIN_HIGH);
	
	mdelay(1);

 }



 //Function for Initializing/Configuring LDAC pin for Hardware use or software use
 /**
	 @brief void LdacInitialization(unsigned int LdacMode)
	 - Writes the values to LDAC Mask Register
	 @param LdacMode :{LDAC_RESPOND_DACA_DACB, LDAC_IGNORE_DACA_RESPOND_DACB, LDAC_RESPOND_DACA_IGNORE_DACB, LDAC_IGNORE_DACA_DACB}
	 -LDAC_RESPOND_DACA_DACB		  -This makes LDAC respond to both DAC A and DAC B		   
	 -LDAC_IGNORE_DACA_RESPOND_DACB   -This makes LDAC ignore DAC A and respond to DAC B
	 -LDAC_RESPOND_DACA_IGNORE_DACB   -This makes LDAC respond to DAC A and ignore DAC B 
	 -LDAC_IGNORE_DACA_DACB 		  -This makes LDAC ignore both DAC A and DAC B 
	 @ no return value
 */
void AD5687_ladc_init(uint32_t ladc_mode,uint8_t ul_pcs)
 {
		#ifdef HARDWARE_LDAC		 
			 ioport_set_pin_level(AD5687_LDAC, PIN_HIGH); 
			 mdelay(1);
			 ioport_set_pin_level(AD5687_LDAC, PIN_LOW); 
			 mdelay(1);		 
		#else 		 
			 AD5687_ladc_maskregister(ladc_mode,ul_pcs);		 
		#endif
 }

	 
 //Function for writing and updating DAC channel (uses Command 1 or 3 to write based on the requirement)
 /**
	 @brief void WriteData(unsigned int commandmode, unsigned int DACselect, unsigned int Data)
	 - Writes the data to the selected DAC with the selected mode of writing
	 @param commandmode: {REG_WRITE_TO_INPUTREG,REG_WRITE_UPDATE_DAC_CHANNEL }
		 -REG_WRITE_TO_INPUTREG 		//Command 1: Writes the data to the input register(LDAC dependent)
		 -REG_WRITE_UPDATE_DAC_CHANNEL	//Command 2: Writes and updates the DAC channel(independent of LDAC)
	 @param DACselect :{DAC_A,DAC_B,DAC_ALL}
		 - DAC_A selects DAC A
		 - DAC_B selects DAC B
		 - DAC_ALL selects DAC A and DAC B	 
	 @param Data : {0x000000 - 0xFFFFFF}
	 @ no return value
 **/
 void AD5687_write_data(uint32_t commandmode, uint32_t DACselect, uint32_t Data,uint8_t ul_pcs)
 {
	   uint32_t RegisterData = 0x00;
	   RegisterData = commandmode + DACselect + Data;  //Merging the data recieved into a single register
 	   WriteRegister(RegisterData,ul_pcs);
 }

	 
 //Function for Updating DAC register from Input register.
 /**
	 @brief void UpdateDacReg(unsigned int DACselect)
	 - Updates DAC register with Input Register.
	 @param DACselect :{DAC_A,DAC_B,DAC_ALL}
		 - DAC_A selects DAC A
		 - DAC_B selects DAC B
		 - DAC_ALL selects DAC A and DAC B
	 @ no return value
 **/
void AD5687_update_dac_reg(uint32_t DACselect,uint8_t ul_pcs)
{
	uint32_t RegisterData = 0x00;		
	RegisterData = REG_UPDATE_DACREG_WITH_INPUTREG + DACselect ;  //Merging the data recieved into a single register
 	WriteRegister(RegisterData,ul_pcs);
}

 
	 
 //Function for Powerdown/ Power up DAC
 /**
	 @brief void PowerDownReg(unsigned int DACAmode, unsigned int DACBmode)
	 - Writes the values to Power down/Power up register
	 @param DACBmode : {PD_DACB_NORMALMODE, PD_DACB_1K_TO_GND, PD_DACB_100K_TO_GND, PD_DACB_THREE_STATE}
	 - PD_DACB_NORMALMODE	  //puts DAC B in normal mode
	 - PD_DACB_1K_TO_GND	  //puts DAC B in powerdown : 1k to GND
	 - PD_DACB_100K_TO_GND	  //puts DAC B in powerdown : 100k to GND
	 - PD_DACB_THREE_STATE	  //puts DAC B in powerdown : Three state
   @param DACAmode : {PD_DACA_NORMALMODE, PD_DACA_1K_TO_GND, PD_DACA_100K_TO_GND, PD_DACA_THREE_STATE}
	 - PD_DACA_NORMALMODE	  //puts DAC A in normal mode
	 - PD_DACA_1K_TO_GND	  //puts DAC A in powerdown : 1k to GND
	 - PD_DACA_100K_TO_GND	  //puts DAC A in powerdown : 100k to GND
	 - PD_DACA_THREE_STATE	  //puts DAC A in powerdown : Three state
	 @ no return value
 **/
 void AD5687_powerdown_reg(uint32_t DACAmode, uint32_t DACBmode,uint8_t ul_pcs)
 {
		uint32_t RegisterData = 0x000000;
	 
	   RegisterData = REG_POWERDOWN_UP + DACBmode + DACAmode;  //Merging the data recieved into a single register
	   WriteRegister(RegisterData,ul_pcs);
	 
 }
 
	 
	 
 //Function for LDAC Mask Register
 /**
	 @brief void LdacMaskRegister(unsigned int LdacMode)
	 - Writes the values to the LDAC Mask Register
	 @param LdacMode :{LDAC_RESPOND_DACA_DACB, LDAC_IGNORE_DACA_RESPOND_DACB, LDAC_RESPOND_DACA_IGNORE_DACB, LDAC_IGNORE_DACA_DACB}
	 -LDAC_RESPOND_DACA_DACB		  //This makes LDAC respond to both DAC A and DAC B 		
	 -LDAC_IGNORE_DACA_RESPOND_DACB   //This makes LDAC ignore DAC A and respond to DAC B
	 -LDAC_RESPOND_DACA_IGNORE_DACB   //This makes LDAC respond to DAC A and ignore DAC B 
	 -LDAC_IGNORE_DACA_DACB 		  //This makes LDAC ignore both DAC A and DAC B 
	 @ no return value
 **/
 void AD5687_ladc_maskregister(uint32_t LdacMode,uint8_t ul_pcs)
 {
	 uint32_t RegisterData = 0x000000;
	 
	 RegisterData = REG_HARDWARE_LDAC_MASK + LdacMode; //Merging the data recieved into a single register
	 WriteRegister(RegisterData,ul_pcs);	 
 }
 
 
 //Function for Software Reset (Power-on reset)
 /**
	 @brief void SoftwareReset(void)
	 - Writes the values to Software Reset register.
	 @param no parameters
	 @ no return value
 **/
void AD5687_software_reset(uint8_t ul_pcs)
{
	uint32_t RegisterData = 0x000000;
	
	  RegisterData = REG_SOFTWARE_RESET ;  //This setting configures the part for Software Reset
	  WriteRegister(RegisterData,ul_pcs);
	
}
 
 //Function for Internal Reference setup
 /**
	 @brief void InternalReferenceSetup(unsigned int IntRefState)
	 - Writes the values to Internal Reference setup register.
	 @param IntRefState :{INTREF_ON, INTREF_OFF}
		 - INTREF_ON	//Turns ON internal reference
	 - INTREF_OFF	//Turns OFF internal reference
	 @ no return value
 **/
 void AD5687_internal_reference_setup(uint32_t IntRefState,uint8_t ul_pcs)
 {
	 uint32_t RegisterData = 0x000000;
	 
	   RegisterData = REG_INTERNAL_REFERENCE + IntRefState;    //Merging the data recieved into a single register
	   WriteRegister(RegisterData,ul_pcs);
 }

 
 //Function for Daisy Chain Enable
 /**
	 @brief void DaisyChain(unsigned int DCmode)
	 - Writes the values to Daisy Chain Enable register.
	 @param DCmode :{DAISY_STANDALONE, DAISY_DCEN_MODE}
		 - DAISY_STANDALONE   //Standalone mode(default)
	 - DAISY_DCEN_MODE	  //Enables Daisy Chain mode
	 @ no return value
 **/
 void AD5687_daisy_chain(uint32_t DCmode,uint8_t ul_pcs)
 {
	 uint32_t RegisterData = 0x000000;
	 
	   RegisterData = REG_INTERNAL_REFERENCE + DCmode;	//Merging the data recieved into a single register
	   WriteRegister(RegisterData,ul_pcs);
	 
 }

 
 //Function for Enabling Readback register
 /**
	 @brief void ReadBackEnable(unsigned int DACselect)
	 - Writes the values to enable Readback function
	 @param DACselect :{DAC_A,DAC_B,DAC_ALL}
		 - DAC_A selects DAC A
		 - DAC_B selects DAC B
		 - DAC_ALL selects DAC A and DAC B
	 @ no return value
 **/
 void AD5687_readback_enable(uint32_t DACselect,uint8_t ul_pcs)
 {
	uint32_t RegisterData = 0x000000;
	 
	   RegisterData = REG_READBACK ;  //This setting configures the part for read mode
	   WriteRegister(RegisterData,ul_pcs);
	 
	   RegisterData = 0x000000; 	  
	   WriteRegister(RegisterData,ul_pcs);   //Follow this write operation with a second write, a NOP condition, 0x000000
									  //During this write, the data from the register is clocked out on the SDO line.
	 
 }


  //Initializing AD5687 to default settings
 void AD5687_init(void)
 {
	 AD5687_spi_master_initialize(SPI0,1,SPI_CSR_BITS_8_BIT);
 	 AD5687_spi_master_initialize(SPI0,0,SPI_CSR_BITS_8_BIT);
	
     ioport_set_pin_level(AD5687_LDAC, PIN_LOW);
	 ioport_set_pin_level(AD5687_RESET, PIN_LOW); 
	 mdelay(1); 
	 ioport_set_pin_level(AD5687_RESET, PIN_HIGH); 
	
 }

 void set_diff_xducer_offset(uint32_t vref)
 {     
 /*AD5687 use only 16 bits,so we just use vref the low 16 bits, although the vref is a uint32_t*/
	AD5687_write_data(REG_WRITE_UPDATE_DAC_CHANNEL, DAC_A,vref,0);
 	printf("%s %u\n",__func__,vref);
 }

 void set_sample_xducer_offset(uint32_t vref)
 {
 /*AD5687 use only 16 bits,so we just use vref the low 16 bits, although the vref is a uint32_t*/
	AD5687_write_data(REG_WRITE_UPDATE_DAC_CHANNEL, DAC_B,vref,0);
 	printf("%s %u\n",__func__,vref);
 }

 void set_pressure_ref(uint32_t vref)
 {
 /*AD5687 use only 16 bits,so we just use vref the low 16 bits, although the vref is a uint32_t*/
	AD5687_write_data(REG_WRITE_UPDATE_DAC_CHANNEL, DAC_A,vref,1);
 	printf("%s %u\n",__func__,vref);
 }
  
 /**example:  transmit data to dacA and dacB to form a high,middlw,and low wave

		AD5687_init();
		mdelay(10);
		while(true){
				AD5687_write_data(REG_WRITE_UPDATE_DAC_CHANNEL, DAC_ALL,0xffff,1);
				AD5687_write_data(REG_WRITE_UPDATE_DAC_CHANNEL, DAC_ALL,0x8888,1);
				AD5687_write_data(REG_WRITE_UPDATE_DAC_CHANNEL, DAC_ALL,0x0000,1);

				AD5687_write_data(REG_WRITE_UPDATE_DAC_CHANNEL, DAC_ALL,0x0000,0);
				AD5687_write_data(REG_WRITE_UPDATE_DAC_CHANNEL, DAC_ALL,0x8888,0);
				AD5687_write_data(REG_WRITE_UPDATE_DAC_CHANNEL, DAC_ALL,0xffff,0);
		}

 **/
 
 
