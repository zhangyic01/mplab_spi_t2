/*
 * Each .c file has to include whatever it needs to compile for this .c file.
 * For example, in this file, I access spi.h, and also the registers in pic.h file.
 * Thus I need to include both files.
 * 
 * In main.c file, it access enum in spi.h and init_spi in spi_setup.c file. Thus
 * main.c has to include spi.h, and spi.h always has to include define for function 
 * as well
 */



#include "pic16F18345_config.h"
#include "spi.h"
#include "radio_config_Si4468_t2.h"

//The 0x44 command ID has to be sent and eight clock pulses have to be
//generated on the SCLK pin. During the additional eight clock cycles, the radio clocks out the CTS as a byte on the
//SDO pin



void init_spi(spi_smp_t spi_mode, spi_cke_t spi_clk_edge, spi_sspm_t clk_cfg, spi_ckp_t clk_idle_cfg, int sspen) {
    
    //we only need to config SSPCON1, SSPSTAT. The rest bits are Read only bits
    
    SSPSTATbits.SMP = spi_mode;
    SSPSTATbits.CKE = spi_clk_edge;
    
    SSPCON1bits.SSPM = clk_cfg;
    SSPCON1bits.CKP = clk_idle_cfg;
    SSPCON1bits.SSPEN = sspen;

}

//Buffer is only 8 bits wide
UINT8 write_spi_buffer(UINT8 data){
    SSP1CON1bits.WCOL = 0;  
    SSPBUF = data;
    while (SSP1STATbits.BF == 1) {}; //receive complete, and buffer is full 
    return (SSPBUF);
}

//In Master mode, the data is transmitted/received as
//soon as the SSPxBUF register is written to.
//(I think the SS will toggle down as soon as SSPBUF is filled full)
void write_spi_fw_cmd(UINT8 *dat){

    for (int i=0; i<sizeof(dat); i++) {
        write_spi_buffer(dat[i]);
    }
}


UINT8 read_spi_buffer(){

    //write dummy data 0xFF so that SS is de-asserted and 
    //SDI is able to read data.
    //The read operation is always ongoing when SS is low. As a mster
    //we need to know what data we want to save.
    
    //Technically it writes 8 bits,
    //and it should also finish read 8 bits. Then, the BF should toggle.
    //!!!! ISSUE: if BF is not full, it will never full, pic will stall !!!!
    write_spi_fw_cmd(dummy);
    while(!SSPSTATbits.BF);
    
    return(SSPBUF);
}



BOOL wait_for_cts() {
    
    //constrain data format to be 8 bits
    UINT8 read_cmd_buff = RF_READ_CMD_BUFF;
    UINT8 cts = 0;
    
    do {
        write_spi_buffer(read_cmd_buff);
        cts = read_spi_buffer();
    } while(cts != 0xFF);
    //OR it times out return 0. Then handle when it times out. 
    
    
    return 1;
}

UINT8* read_fw_response(int num_bytes) {
    
    UINT8 resp[16];  //define with static size which is the max possible size of response
                     //because xc does not support dynamic allocation.
    BOOL cts_ready;
    
    cts_ready = wait_for_cts();
    
    if (cts_ready) {
        //write exact num of bytes as response size.
        for(int i=0;i<num_bytes;i++) {
            write_spi_buffer(dummy);
            resp[i] = read_spi_buffer();
        }
    }
    
    return *resp;  //return can only be pointer. 
}
