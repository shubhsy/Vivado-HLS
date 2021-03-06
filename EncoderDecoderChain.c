/*
 * Empty C++ Application
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xpolar.h"
#include <unistd.h>
#include "xparameters.h"

u32 USER_DEFINED_BA_TABLE[128]={
		0,	0,	0,	0,	0,	0,	0,	2,	0,	0,	0,	2,	0,	2,	2,	2,   //5
			0,	0,	0,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2, //13
				0,	2,	2,	1,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2, //14 //1
					2,	1,	2,	2,	2,	2,	2,	2,	0,	0,	0,	0,	0,	0,	0,	0, //7 //1
						0,	1,	2,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1, //1 //14
							1,	1,	1,	1,	1,	1,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0, // //8
								0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
									0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0
};

int main()
{
	init_platform();
	xpolar_polar_parameters polar_params;
	polar_params.N=128;
	polar_params.K=40;
	polar_params.AUGMENT = AUGMENT_CRC;
	polar_params.CRC_SEL = CRC_SEL_24c;
	polar_params.ITLV = 1;
	u32 compressed_length = polar_params.N / 16;
	u32 compressed_BA_TABLE[compressed_length];
	xpolar_compress_bit_allocation_table(USER_DEFINED_BA_TABLE, compressed_BA_TABLE,
	compressed_length);
	for (int i = 0; i < compressed_length; i++) {
		polar_params.BA_TABLE[i] = compressed_BA_TABLE[i];
	}
	xpolar_Config* polar_cfg1;
	xpolar_Config* polar_cfg2;
	xpolar polar1, polar2;
	int	status;
	polar_cfg1 = xpolar_LookupConfig(XPAR_XPOLAR_0_DEVICE_ID);
	polar_cfg2 = xpolar_LookupConfig(XPAR_POLAR_1_DEVICE_ID);
	if (!polar_cfg1) {
		//std::cout << "ERROR: Could not lookup Polar" << std::endl;
		return 1;
	}
	/*else
		std::cout << "LOOKUP CONFIG DONE!!" << std::endl;*/
	status = xpolar_CfgInitialize(&polar1, polar_cfg1);
	if(status!=XST_SUCCESS){
		//std::cout<<"ERROR: Could not initialize Polar"<<std::endl;
		return 1;
	}
	status = xpolar_CfgInitialize(&polar2, polar_cfg2);
	if(status!=XST_SUCCESS){
			//std::cout<<"ERROR: Could not initialize Polar"<<std::endl;
			return 1;
		}
	/*else
		std::cout << "INITIALIZED POLAR!!" << std::endl;*/

	polar1.code_id=0x04;
	xpolar_set_CORE_AXI_WR_PROTECT(&polar1, 0);
	xpolar_set_CORE_CODE_WR_PROTECT(&polar1, 0);
	xpolar_set_CORE_AXIS_ENABLE(&polar1, 0);
	xpolar_set_CORE_AXIS_WIDTH(&polar1, 0);
	/*if(xpolar_get_CORE_AXIS_ENABLE(&polar))
		std::cout<<"AXIS ENABLED\n";
	else if(!xpolar_get_CORE_AXIS_ENABLE(&polar))
		std::cout<<"AXIS NOT ENABLED\n";*/
	//xpolar_set_CORE_ISR(&polar, 127);
	xpolar_set_CORE_IER(&polar1, 64);
	//xpolar_set_CORE_IDR(&polar, 0);
	//xpolar_set_CORE_ECC_ISR(&polar, 4095);
	//xpolar_set_CORE_ECC_IER(&polar, 0x00000FFF);
	//xpolar_set_CORE_ECC_IDR(&polar, 0);
	xpolar_add_polar_params(&polar1, &polar_params);
	xpolar_set_CORE_CODE_WR_PROTECT(&polar1, 1);
	xpolar_set_CORE_AXIS_ENABLE(&polar1, 63);
	xpolar_set_CORE_AXI_WR_PROTECT(&polar1, 1);

	polar2.code_id=0x04;
	xpolar_set_CORE_AXI_WR_PROTECT(&polar2, 0);
	xpolar_set_CORE_CODE_WR_PROTECT(&polar2, 0);
	xpolar_set_CORE_AXIS_ENABLE(&polar2, 0);
	xpolar_set_CORE_AXIS_WIDTH(&polar2, 0);
	/*if(xpolar_get_CORE_AXIS_ENABLE(&polar))
		std::cout<<"AXIS ENABLED\n";
	else if(!xpolar_get_CORE_AXIS_ENABLE(&polar))
		std::cout<<"AXIS NOT ENABLED\n";*/
	//xpolar_set_CORE_ISR(&polar, 127);
	xpolar_set_CORE_IER(&polar2, 64);
	//xpolar_set_CORE_IDR(&polar, 0);
	//xpolar_set_CORE_ECC_ISR(&polar, 4095);
	//xpolar_set_CORE_ECC_IER(&polar, 0x00000FFF);
	//xpolar_set_CORE_ECC_IDR(&polar, 0);
	xpolar_add_polar_params(&polar2, &polar_params);
	xpolar_set_CORE_CODE_WR_PROTECT(&polar2, 1);
	xpolar_set_CORE_AXIS_ENABLE(&polar2, 63);
	xpolar_set_CORE_AXI_WR_PROTECT(&polar2, 1);

	cleanup_platform();

	return 0;
}
