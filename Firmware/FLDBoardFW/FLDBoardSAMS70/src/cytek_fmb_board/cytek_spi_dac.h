/*
 * cytek_spi_dac.h
 *
 * Created: 2015/11/18/周三 17:50:10
 *  Author: lenovo
 */ 


#ifndef CYTEK_SPI_DAC_H_
#define CYTEK_SPI_DAC_H_

#include <cytek_fmb_board.h>

#define 	AD5687_SPI  SPI0
#define 	AD5687_CHIP1 0
#define 	AD5687_CHIP2 1
#define	AD5687_MISO 	SPI0_MISO_GPIO
#define 	AD5687_MOSI 	SPI0_MOSI_GPIO
#define 	AD5687_SCLK 		SPI0_SPCK_GPIO
#define 	AD5687_CS0 		SPI0_NPCS0_GPIO
#define 	AD5687_CS1		SPI0_NPCS1_GPIO
#define 	AD5687_RESET 	SPI0_DEV_RESET
#define 	AD5687_RESET_ACTIVE 	SPI0_DEV_RESET_ACTIVE_LEVEL
#define 	AD5687_RESET_INACTIVE 	SPI0_DEV_RESET_INACTIVE_LEVEL
#define 	AD5687_LDAC 	SPI0_DEV_LADC
#define 	AD5687_LDAC_ACTIVE 	SPI0_DEV_LADC_ACTIVE_LEVEL
#define 	AD5687_LDAC_INACTIVE 	SPI0_DEV_LADC_INACTIVE_LEVEL


//REGISTER MAP

#define	 NO_OF_BYTES 0x03 //Size of Register in Bytes

//Hardware/Software LDAC
#define 	HARDWARE_LDAC          //keep this uncommented for using Hardware LDAC. Comment this to use software LDAC.

#define 	PIN_HIGH 0x1
#define 	PIN_LOW  0x0

#define 	REG_WRITE_TO_INPUTREG               0x100000    //Command 1: Write to input register n (LDAC dependent)
#define 	REG_UPDATE_DACREG_WITH_INPUTREG     0x200000    //Command 2: Update DAC register with Input register
#define 	REG_WRITE_UPDATE_DAC_CHANNEL        0x300000    //Command 3: Write and update DAC channel n
#define	REG_POWERDOWN_UP                    0x400000    //Command 4: Powerdown/Powerup DAC
#define	REG_HARDWARE_LDAC_MASK              0x500000    //Command 5: Hardware LDAC mask register
#define	REG_SOFTWARE_RESET                  0x600000    //Command 6: Software reset(Power-on Reset)
#define	REG_INTERNAL_REFERENCE              0x700000    //Command 7: Internal Reference Setup register
#define	REG_DCEN                            0x800000    //Command 8: Setup DCEN register(Daisy Chain Enable)
#define	REG_READBACK                        0x900000    //Command 9: Setup readback register(Readback enable)

//DAC SELECTION ADDRESS BITS
#define	DAC_A   0x010000    // Selects DAC A
#define 	DAC_B   0x080000    // Selects DAC B
#define 	DAC_ALL 0x090000    // Selects DAC A and DAC B

//Daisy Chain Enable REGISTER
#define 	DAISY_STANDALONE    0X000000    //Standalone mode
#define 	DAISY_DCEN_MODE     0X000001    //Daisy Chain mode

//INTERNAL REFERENCE
#define 	INTREF_ON    0X000000     // Turns Internal Reference ON
#define 	INTREF_OFF   0X000001     // Turns Internal Reference OFF

//LDAC MASK REGISTER(for software purpose only)
#define 	LDAC_RESPOND_DACA_DACB           0X000000  //Responds to LDAC pin of both DAC A and DAC B
#define 	LDAC_IGNORE_DACA_RESPOND_DACB    0X000001  //Ignores LDAC pin of DAC A and responds to DAC B
#define 	LDAC_RESPOND_DACA_IGNORE_DACB    0X000008  //Responds to LDAC pin of DAC A and ignores to DAC B
#define 	LDAC_IGNORE_DACA_DACB            0X000009  //Ignores the transitions in LDAC pin of both DAC A and DAC B

//POWERDOWN REGISTER
#define 	PD_DACA_NORMALMODE     0X00000C   //Puts DAC A in normal mode
#define 	PD_DACA_1K_TO_GND      0X00000D   //Puts DAC A in powerdown mode: 1k to GND
#define 	PD_DACA_100K_TO_GND    0X00000E   //Puts DAC A in powerdown mode: 100k to GND
#define 	PD_DACA_THREE_STATE    0X00000F   //Puts DAC A in powerdown mode: Three state

#define 	PD_DACB_NORMALMODE     0X000030   //Puts DAC B in normal mode
#define 	PD_DACB_1K_TO_GND      0X000070   //Puts DAC B in powerdown mode: 1k to GND
#define 	PD_DACB_100K_TO_GND    0X0000B0   //Puts DAC B in powerdown mode: 100k to GND
#define 	PD_DACB_THREE_STATE    0X0000F0   //Puts DAC B in powerdown mode: Three state



//------------------------
// Internal Function declarations
//------------------------

void AD5687_init(void);//InitializeAD5687(void);
void AD5687_ladc_init(uint32_t ladc_mode,uint8_t ul_pcs);//void LdacInitialization(unsigned int LdacMode);
void AD5687_write_data(uint32_t commandmode, uint32_t DACselect, uint32_t Data,uint8_t ul_pcs);//void WriteData(unsigned int commandmode, unsigned int DACselect, unsigned int Data);
void AD5687_update_dac_reg(uint32_t DACselect,uint8_t ul_pcs);//void UpdateDacReg(unsigned int DACselect);
void AD5687_powerdown_reg(uint32_t DACAmode, uint32_t DACBmode,uint8_t ul_pcs);//void PowerDownReg(unsigned int DACAmode, unsigned int DACBmode);
void AD5687_ladc_maskregister(uint32_t LdacMode,uint8_t ul_pcs);//void LdacMaskRegister( unsigned int LdacMode);
void AD5687_software_reset(uint8_t ul_pcs);//void SoftwareReset(void);
void AD5687_internal_reference_setup(uint32_t IntRefState,uint8_t ul_pcs);//void InternalReferenceSetup(unsigned int IntRefState);
void AD5687_daisy_chain(uint32_t DCmode,uint8_t ul_pcs);//void DaisyChain(unsigned int DCmode);
void AD5687_readback_enable(uint32_t DACselect,uint8_t ul_pcs);//void ReadBackEnable(unsigned int DACselect);
void set_diff_xducer_offset(uint32_t vref);
void set_sample_xducer_offset(uint32_t vref);
void set_pressure_ref(uint32_t vref);


#endif /* CYTEK_SPI_DAC_H_ */
