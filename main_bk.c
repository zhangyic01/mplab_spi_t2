/*
 * File:   main.c
 * Author: YiChao
 *
 * Created on July 9, 2017, 4:30 PM
 */


//https://electrosome.com/spi-pic-microcontroller-mplab-xc8/
//https://www.youtube.com/watch?v=WAlvP9F5kY4
//to use logic analyzer. 1 build project. 2 build for debug 3 launch debugger

#include <stdio.h>
#include <stdlib.h>
#include "pic16F18345_config.h"
#include "spi.h"
#include "radio_config_Si4468_t2.h"

/*
 10.9. Two-Way Packet (Bidirectional Packet)
The purpose of the two-way packet example code is to demonstrate how to use the radio for a two-way link
communication. By default, the two devices are in receiver mode and waiting for either a request message or a
pushbutton event from the user. When the first pushbutton is pushed on one of the WMBs, it sends a request
immediately to the other radio. The LED activity shows if the radio packet is sent successfully. The receiver replies
with an acknowledgment followed by LED activity.
 */


void main_bk(void) {
    

    //*******************************
    // 1. Initialize MCU
    //*******************************
    //setup spi registers
    //SMP: SPI_MASTER = 1, spi master mode
    //CKP: SPI_POSEDGE = 0, 0 = Idle state for clock is a low level
    //SSMP: SPI Master mode, clock = FOSC/4
    //SSPEN: 1 - enable serial spi mode.
    //CKE: SPI_CLK_LOW_IN_IDLE = 0 - transmit occurs on transition from idle to active clock state. posedge to send
    
    //in short, in this setup, once bit 7 is ready, the first posedge will sample it.
    //when buffer is full, 8 bits are sent out. Refer to FIGURE 29-6. CKP,CKE = 0
    init_spi (SPI_MASTER,SPI_POSEDGE,SPI_M_FOSC_DIV4,SPI_CLK_LOW_IN_IDLE, 1);
    
    //setup MCU pins
    //1.setup master SPI_1 pins - Si4468
    TRISCbits.TRISC0 = 0; //RC0 = SCLK output
    TRISCbits.TRISC1 = 1; //RC1 = SDI input
    TRISCbits.TRISC2 = 0; //RC2 = SDO output
    TRISCbits.TRISC3 = 0; //R3  = SS output
    
    //2. setup slave SPI_2 pins - sensor. 
    TRISBbits.TRISB4 = 1; //RB4 = SCLK input
    TRISBbits.TRISB5 = 1; //RB5 = SDI input
    TRISBbits.TRISB6 = 0; //RB6 = SDO output
    TRISBbits.TRISB7 = 1; //RB7 = SS input
            
    /*    old example code
    TRISBbits.TRISB4 = 1; //1 = input. register controls RB pins.
    TRISAbits.TRISA0 = 0; //0 = output.register controls RA pins. Refer to REGISTER 12-2.  Each bit = TRISA0, TRISA1...
    PORTA = 0x0; //Latch PORTA to 0. REGISTER 12-1,  Each bit RA0,RA1...
    TRISCbits.TRISC7 = 0;
    LATCbits.LATC7 = 1; //select bit. default 1 is disabled spi slave
    LATCbits.LATC7 = 0;
    PORTCbits.RC7 = 1;
    PORTCbits.RC7 = 0;
    RC7 = 1;
    */
            
    TRISCbits.TRISC4 = 1; // GPIO_1 - correct
    TRISAbits.TRISA2 = 1; // nIRQ - correct
    //SDN
    
    //setup FW commands
    //No need to allocate as they are const type???    
    //setup FW configuration queue
    //UINT8** rf_config;
    //rf_config = malloc(31*sizeof(UINT8*));
    
    static const UINT8 rf_power_up                          [] = {RF_POWER_UP                            };    
    static const UINT8 rf_gpio_pin_cfg                      [] = {RF_GPIO_PIN_CFG                        };
    static const UINT8 rf_global_xo_tune_2                  [] = {RF_GLOBAL_XO_TUNE_2                    };
    static const UINT8 rf_global_config_1                   [] = {RF_GLOBAL_CONFIG_1                     };
    static const UINT8 rf_int_ctl_enable_1                  [] = {RF_INT_CTL_ENABLE_1                    };
    static const UINT8 rf_frr_ctl_a_mode_4                  [] = {RF_FRR_CTL_A_MODE_4                    };
    static const UINT8 rf_preamble_tx_length_9              [] = {RF_PREAMBLE_TX_LENGTH_9                };
    static const UINT8 rf_sync_config_10                    [] = {RF_SYNC_CONFIG_10                      };
    static const UINT8 rf_pkt_crc_config_12                 [] = {RF_PKT_CRC_CONFIG_12                   };
    static const UINT8 rf_pkt_rx_threshold_12               [] = {RF_PKT_RX_THRESHOLD_12                 };
    static const UINT8 rf_pkt_field_3_crc_config_12         [] = {RF_PKT_FIELD_3_CRC_CONFIG_12           };
    static const UINT8 rf_pkt_rx_field_1_crc_config_12      [] = {RF_PKT_RX_FIELD_1_CRC_CONFIG_12        };
    static const UINT8 rf_pkt_rx_field_4_crc_config_5       [] = {RF_PKT_RX_FIELD_4_CRC_CONFIG_5         };
    static const UINT8 rf_pkt_crc_seed_31_24_4              [] = {RF_PKT_CRC_SEED_31_24_4                };
    static const UINT8 rf_modem_mod_type_12                 [] = {RF_MODEM_MOD_TYPE_12                   };
    static const UINT8 rf_modem_freq_dev_0_1                [] = {RF_MODEM_FREQ_DEV_0_1                  };
    static const UINT8 rf_modem_tx_ramp_delay_12            [] = {RF_MODEM_TX_RAMP_DELAY_12              };
    static const UINT8 rf_modem_bcr_nco_offset_2_12         [] = {RF_MODEM_BCR_NCO_OFFSET_2_12           };
    static const UINT8 rf_modem_afc_limiter_1_3             [] = {RF_MODEM_AFC_LIMITER_1_3               };
    static const UINT8 rf_modem_agc_control_1               [] = {RF_MODEM_AGC_CONTROL_1                 };
    static const UINT8 rf_modem_agc_window_size_12          [] = {RF_MODEM_AGC_WINDOW_SIZE_12            };
    static const UINT8 rf_modem_raw_control_10              [] = {RF_MODEM_RAW_CONTROL_10                };
    static const UINT8 rf_modem_raw_search2_2               [] = {RF_MODEM_RAW_SEARCH2_2                 };
    static const UINT8 rf_modem_spike_det_2                 [] = {RF_MODEM_SPIKE_DET_2                   };
    static const UINT8 rf_modem_rssi_mute_1                 [] = {RF_MODEM_RSSI_MUTE_1                   };
    static const UINT8 rf_modem_dsa_ctrl1_5                 [] = {RF_MODEM_DSA_CTRL1_5                   };
    static const UINT8 rf_modem_chflt_rx1_chflt_coe13_7_0_12[] = {RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12  };
    static const UINT8 rf_modem_chflt_rx1_chflt_coe1_7_0_12 [] = {RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12   };
    static const UINT8 rf_modem_chflt_rx2_chflt_coe7_7_0_12 [] = {RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12   };
    static const UINT8 rf_pa_mode_4                         [] = {RF_PA_MODE_4                           };
    static const UINT8 rf_synth_pfdcp_cpff_7                [] = {RF_SYNTH_PFDCP_CPFF_7                  };
    static const UINT8 rf_match_value_1_12                  [] = {RF_MATCH_VALUE_1_12                    };
    static const UINT8 rf_freq_control_inte_8               [] = {RF_FREQ_CONTROL_INTE_8                 };    
    
    static const UINT8 rf_confg[32]= {
        rf_gpio_pin_cfg                      ,    
        rf_global_xo_tune_2                  ,
        rf_global_config_1                   ,
        rf_int_ctl_enable_1                  ,
        rf_frr_ctl_a_mode_4                  ,
        rf_preamble_tx_length_9              ,
        rf_sync_config_10                    ,
        rf_pkt_crc_config_12                 ,
        rf_pkt_rx_threshold_12               ,
        rf_pkt_field_3_crc_config_12         ,
        rf_pkt_rx_field_1_crc_config_12      ,
        rf_pkt_rx_field_4_crc_config_5       ,
        rf_pkt_crc_seed_31_24_4              ,
        rf_modem_mod_type_12                 ,
        rf_modem_freq_dev_0_1                ,
        rf_modem_tx_ramp_delay_12            ,
        rf_modem_bcr_nco_offset_2_12         ,
        rf_modem_afc_limiter_1_3             ,
        rf_modem_agc_control_1               ,
        rf_modem_agc_window_size_12          ,
        rf_modem_raw_control_10              ,
        rf_modem_raw_search2_2               ,
        rf_modem_spike_det_2                 ,
        rf_modem_rssi_mute_1                 ,
        rf_modem_dsa_ctrl1_5                 ,
        rf_modem_chflt_rx1_chflt_coe13_7_0_12,
        rf_modem_chflt_rx1_chflt_coe1_7_0_12 ,
        rf_modem_chflt_rx2_chflt_coe7_7_0_12 ,
        rf_pa_mode_4                         ,
        rf_synth_pfdcp_cpff_7                ,
        rf_match_value_1_12                  ,
        rf_freq_control_inte_8                 
    };
    
    //*******************************
    // 2. RF chip start
    //*******************************
    //a. wait for GPIO_1 pin is assert
    int timeout_cnter;
    while (PORTCbits.RC4 == 0) {
        //updated to some time statement, example is available in other computer
        timeout_cnter ++;
        if (timeout_cnter == 10000) {
            break;
        }
    } //Do nothing just wait
    
    //b. RF chip is ready and able to read FW cmd. Send POWER_UP
    //SPI buffer is 8 bits
    write_spi_fw_cmd(rf_power_up);  //takes 15 ms to complete
    
    //Waiting for response - 3 options. Read SPI CTS/ nIRQ de-assert / GPIO1 assert
    BOOL timeout = 0;
    BOOL nIRQ_unreliable = 0;
    BOOL GPIO_1_unreliable = 0;
    BOOL SPI_CTS_unreliable = 0; //CTS (clear to send) = 0xFF from SDO
    
    BOOL glbl_reset = 0;
    
    
    if (PORTAbits.RA2 == 1) {
        //printf "nIRQ is not high at initial phase!"
        nIRQ_unreliable = 1;
    }
    
    //looks like the device only need either CTS/GPIO_1/IRQ, one of them to handshake the cmd
    //Depends on how we configuration and connect the device. Here I will list 3 option code
    while(1) {
        if (nIRQ_unreliable == 1 && GPIO_1_unreliable == 1 && SPI_CTS_unreliable == 1) {
            glbl_reset = 1;
            break;
            //print: device is not setup correctly. Reset everything!
        }
        
        if (timeout == 1) {
            glbl_reset = 1;
            break;
            //print: waiting FW command timed out!
        }
        //option 1: nIRQ de-assert
        //The radio asserts the CHIP_READY interrupt flag if a command is completed. The interrupt flag can be monitored
        //by either the GET_CHIP_STATUS or the GET_INT_STATUS command. Apart from monitoring the interrupt flags,
        //the radio may pull down the NIRQ pin if this feature is enabled
        write_spi_fw_cmd(RF_GET_CHIP_STATUS);
        if(nIRQ_unreliable == 0 && PORTAbits.RA2 == 1) {
            break;
            //Response received, it is good, continue
        }
        //option 2: GPIO_1 = 0
        //Any GPIO can be configured for monitoring the CTS. GPIOs can be configured to go either high or low when the
        //chip has completed the command. The function of the GPIOs can be changed by the GPIO_PIN_CFG command.
        //By default, GPIO1 is set as "High when command completed, low otherwise"
        //I am not sure how it is getting cleared
        if (GPIO_1_unreliable == 0 && PORTCbits.RC4 == 1) {
            break;
            //Response received, it is good, continue
        }      
    }
    
    //option 3: CTS ready - it is time consuming task, does not go to while loop 
    //the CTS read procedure has to be repeated from the beginning as long as the CTS byte is not 0xFF
    while (SSPBUF != dummy) {
        read_spi_buffer();
    }
    
    //3. clear all interrupts. Nothing to do?????
    
    //4. radio boot
    //When we do not reset, we continue our program
    if (glbl_reset == 0) {

          //configure the device
        for (int i=0;i<sizeof(rf_confg);i++) {
            write_spi_fw_cmd(rf_confg[i]);
            
            
            read_fw_response(1); //handle later
        }
        
        //device config is done, give some drain time
        //TODO: add drain time
       
        
        //5. programming MCU to read/write RF data, and handle the data
        
        
        
    }
    //else, it goes back to top of main(), we are restart everything over
    
    
    
    
    
    
    
    /*
    while(1)
    {
        RC7 = 0; //Slave Select enable slave
        __delay_ms(1);
        
        //master SPI
        //Refer to Figure 30-1. 
        write_spi_data(0x1); //slave write to our input  
        PORTA = read_spi_data(); //slave read from our output

        write_spi_data(0x0);
        __delay_ms(1);
        write_spi_data(0x1);

        RC7 = 1;
        __delay_ms(100);
    }
    */
    
    return;
    
}
