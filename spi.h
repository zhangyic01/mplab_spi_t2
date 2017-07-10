/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

//SSP STATUS register
typedef enum
{
   SPI_MASTER = 1,
   SPI_SLAVE = 0
}spi_smp_t;


typedef enum 
{
    SPI_POSEDGE = 0,
    SPI_NEGEDGE = 1
}spi_cke_t;

//SSP CON register
typedef enum
{
    SPI_M_T2_DIV2    = 0011, // SPI Master mode, clock = T2_match/2
    SPI_M_FOSC_DIV64 = 0010, // SPI Master mode, clock = FOSC/64
    SPI_M_FOSC_DIV16 = 0001, // SPI Master mode, clock = FOSC/16
    SPI_M_FOSC_DIV4  = 0000, // SPI Master mode, clock = FOSC/4
    SPI_S_IN_DIS     = 0101, // SPI Slave mode, clock = SCK pin, SS pin control disabled, SS can be used as I/O pin
    SPI_S_IN_EN      = 0100, // SPI Slave mode, clock = SCK pin, SS pin control enabled
    SPI_M_FOSC_CUST  = 1010 // SPI Master mode, clock = FOSC/(4 * (SSPADD+1))    
} spi_sspm_t;

typedef enum 
{
    SPI_CLK_HIGH_IN_IDLE = 1,
    SPI_CLK_LOW_IN_IDLE = 0
}spi_ckp_t;


extern void init_spi(spi_smp_t spi_mode, spi_cke_t spi_clk_edge, spi_sspm_t clk_cfg, spi_ckp_t clk_idle_cfg, int sspen);
extern void write_spi_data(int dat);
extern unsigned read_spi_data_rdy();
extern int read_spi_data();