/*
 * SPDX-FileCopyrightText: 2020 Efabless Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * SPDX-License-Identifier: Apache-2.0
 */

// This include is relative to $CARAVEL_PATH (see Makefile)
#include <defs.h>
#include <stub.c>


#define fir_core_setup (*(volatile uint32_t*)0x30000000)
#define fir_core_len (*(volatile uint32_t*)0x30000010)
#define fir_core_tap (*(volatile uint32_t*)0x30000040)
#define fir_core_Xn (*(volatile uint32_t*)0x30000080)
#define fir_core_Yn (*(volatile uint32_t*)0x30000084)

#define setup_ptr ((volatile int32_t*)0x30000000)
#define len_ptr ((volatile int32_t*)0x30000010)
#define tap_ptr ((volatile int32_t*)0x30000040)
#define Xn_ptr ((volatile int32_t*)0x30000080)
#define Yn_ptr ((volatile int32_t*)0x30000084)



extern int* fir();

// --------------------------------------------------------

/*
	MPRJ Logic Analyzer Test:
		- Observes counter value through LA probes [31:0] 
		- Sets counter initial value through LA probes [63:32]
		- Flags when counter value exceeds 500 through the management SoC gpio
		- Outputs message to the UART when the test concludes successfuly
*/

void main()
{
	int j;
	
	/* Set up the housekeeping SPI to be connected internally so	*/
	/* that external pin changes don't affect it.			*/

	// reg_spi_enable = 1;
	// reg_spimaster_cs = 0x00000;

	// reg_spimaster_control = 0x0801;

	// reg_spimaster_control = 0xa002;	// Enable, prescaler = 2,
                                        // connect to housekeeping SPI

	// Connect the housekeeping SPI to the SPI master
	// so that the CSB line is not left floating.  This allows
	// all of the GPIO pins to be used for user functions.

	// The upper GPIO pins are configured to be output
	// and accessble to the management SoC.
	// Used to flad the start/end of a test 
	// The lower GPIO pins are configured to be output
	// and accessible to the user project.  They show
	// the project count value, although this test is
	// designed to read the project count through the
	// logic analyzer probes.
	// I/O 6 is configured for the UART Tx line

        reg_mprj_io_31 = GPIO_MODE_MGMT_STD_OUTPUT;
        reg_mprj_io_30 = GPIO_MODE_MGMT_STD_OUTPUT;
        reg_mprj_io_29 = GPIO_MODE_MGMT_STD_OUTPUT;
        reg_mprj_io_28 = GPIO_MODE_MGMT_STD_OUTPUT;
        reg_mprj_io_27 = GPIO_MODE_MGMT_STD_OUTPUT;
        reg_mprj_io_26 = GPIO_MODE_MGMT_STD_OUTPUT;
        reg_mprj_io_25 = GPIO_MODE_MGMT_STD_OUTPUT;
        reg_mprj_io_24 = GPIO_MODE_MGMT_STD_OUTPUT;
        reg_mprj_io_23 = GPIO_MODE_MGMT_STD_OUTPUT;
        reg_mprj_io_22 = GPIO_MODE_MGMT_STD_OUTPUT;
        reg_mprj_io_21 = GPIO_MODE_MGMT_STD_OUTPUT;
        reg_mprj_io_20 = GPIO_MODE_MGMT_STD_OUTPUT;
        reg_mprj_io_19 = GPIO_MODE_MGMT_STD_OUTPUT;
        reg_mprj_io_18 = GPIO_MODE_MGMT_STD_OUTPUT;
        reg_mprj_io_17 = GPIO_MODE_MGMT_STD_OUTPUT;
        reg_mprj_io_16 = GPIO_MODE_MGMT_STD_OUTPUT;

        reg_mprj_io_15 = GPIO_MODE_USER_STD_OUTPUT;
        reg_mprj_io_14 = GPIO_MODE_USER_STD_OUTPUT;
        reg_mprj_io_13 = GPIO_MODE_USER_STD_OUTPUT;
        reg_mprj_io_12 = GPIO_MODE_USER_STD_OUTPUT;
        reg_mprj_io_11 = GPIO_MODE_USER_STD_OUTPUT;
        reg_mprj_io_10 = GPIO_MODE_USER_STD_OUTPUT;
        reg_mprj_io_9  = GPIO_MODE_USER_STD_OUTPUT;
        reg_mprj_io_8  = GPIO_MODE_USER_STD_OUTPUT;
        reg_mprj_io_7  = GPIO_MODE_USER_STD_OUTPUT;
        reg_mprj_io_5  = GPIO_MODE_USER_STD_OUTPUT;
        reg_mprj_io_4  = GPIO_MODE_USER_STD_OUTPUT;
        reg_mprj_io_3  = GPIO_MODE_USER_STD_OUTPUT;
        reg_mprj_io_2  = GPIO_MODE_USER_STD_OUTPUT;
        reg_mprj_io_1  = GPIO_MODE_USER_STD_OUTPUT;
        reg_mprj_io_0  = GPIO_MODE_USER_STD_OUTPUT;

        reg_mprj_io_6  = GPIO_MODE_MGMT_STD_OUTPUT;

	// Set UART clock to 64 kbaud (enable before I/O configuration)
	// reg_uart_clkdiv = 625;
	reg_uart_enable = 1;
	
	// Now, apply the configuration
	reg_mprj_xfer = 1;
	while (reg_mprj_xfer == 1);

        // Configure LA probes [31:0], [127:64] as inputs to the cpu 
	// Configure LA probes [63:32] as outputs from the cpu
	reg_la0_oenb = reg_la0_iena = 0x00000000;    // [31:0]
	reg_la1_oenb = reg_la1_iena = 0xFFFFFFFF;    // [63:32]
	reg_la2_oenb = reg_la2_iena = 0x00000000;    // [95:64]
	reg_la3_oenb = reg_la3_iena = 0x00000000;    // [127:96]

	// Flag start of the test 
	
	reg_mprj_datal = 0xAB400000;
	
	// Set Counter value to zero through LA probes [63:32]
	reg_la1_data = 0x00000000;

	// Configure LA probes from [63:32] as inputs to disable counter write
	reg_la1_oenb = reg_la1_iena = 0x00000000;    

/*
	while (1) {
		if (reg_la0_data_in > 0x1F4) {
			reg_mprj_datal = 0xAB410000;
			break;
		}
	}
*/	
	int* tmp = fir();
	reg_mprj_datal = *tmp << 16;
	reg_mprj_datal = *(tmp+1) << 16;
	reg_mprj_datal = *(tmp+2) << 16;
	reg_mprj_datal = *(tmp+3) << 16;
	reg_mprj_datal = *(tmp+4) << 16;
	reg_mprj_datal = *(tmp+5) << 16;
	reg_mprj_datal = *(tmp+6) << 16;
	reg_mprj_datal = *(tmp+7) << 16;
	reg_mprj_datal = *(tmp+8) << 16;
	reg_mprj_datal = *(tmp+9) << 16;
	reg_mprj_datal = *(tmp+10) << 16;
	
		

	//print("\n");
	//print("Monitor: Test 1 Passed\n\n");	// Makes simulation very long!
	reg_mprj_datal = 0xAB510000;
//	int length = 11;
	
	int temp;
	int data_length = 64;
	
	for(int t=0; t<2; t++) {
		////data length
		int32_t* ptr = len_ptr;
		*ptr = data_length;
		////coefficient write
		ptr = tap_ptr;
		*(ptr+0) = 0;
		*(ptr+1) = -10;
		*(ptr+2) = -9;
		*(ptr+3) = 23;
		*(ptr+4) = 56;
		*(ptr+5) = 63;
		*(ptr+6) = 56;
		*(ptr+7) = 23;
		*(ptr+8) = -9;
		*(ptr+9) = -10;
		*(ptr+10) = 0;
	
		////coefficient read check
		for(int i=0; i< 11; i++){
				reg_mprj_datal = *(ptr + i) << 16;
		}
	
		
		////fir setup
		ptr = setup_ptr;
		*ptr = 0x00000001; //ap_start
	
		////start mark('hA5),notify testbench to start timer
		reg_mprj_datal = 0x00A50000;
	
		////fir start
		for(int k=0; k<data_length; k++){
				////wait X[n]
				while(!((fir_core_setup &(0x00000010)) == 0x00000010)); 
				fir_core_Xn = k;
				////wait Y[n]
				while(!((fir_core_setup&(0x00000020)) == 0x00000020));
				if(k == data_length-1){
					temp = fir_core_Yn;
					reg_mprj_datal = temp << 16;
//					reg_mprj_datal = ((temp&0x000000FF) << 24) | 0x005A0000;
					reg_mprj_datal = ((temp << 24 ) & 0xFF000000) | 0x005A0000;
//					reg_mprj_datal = ((fir_core_Yn << 24 ) & 0xFF000000) | 0x005A0000;
				}
				else {
				    reg_mprj_datal = fir_core_Yn << 16;
//					temp = fir_core_Yn;
				}
//				if(k == 10){
//				////final Y[7:0],End Mark('h5A),notify testbench to end timer
//				reg_mprj_datal = ((fir_core_Yn&0x000000FF) << 24) | 0x005A0000;
//  				}
        }
	}
	
}

