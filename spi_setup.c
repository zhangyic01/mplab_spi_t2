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

void init_spi(spi_smp_t spi_mode, spi_cke_t spi_clk_edge, spi_sspm_t clk_cfg, spi_ckp_t clk_idle_cfg, int sspen) {
    
    SSPSTATbits.SMP = spi_mode;
    SSPSTATbits.CKE = spi_clk_edge;
    
    SSPCONbits.SSPM = clk_cfg;
    SSPCONbits.CKP = clk_idle_cfg;
    SSPCONbits.SSPEN = sspen;

}

//dat should be some data struct for commands or pure data?
void write_spi_data(int dat){
    SSPBUF = dat;
}
unsigned read_spi_data_rdy(){
    if (SSPSTATbits.BF) {
        return 1;
    } else {
        return 0;
    }
}

int read_spi_data(){
    //wait until data ready return 1
    //I think it checks every cycle
    while(!read_spi_data);

    return(SSPBUF);
}
