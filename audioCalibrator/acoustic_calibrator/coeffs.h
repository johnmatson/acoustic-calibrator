// IIR coefficients used by 4 separate bandpass filters
//
// File created by John Matson
// November 26 2020

// filter coefficents for GEQ 1
#define IIR16_1_COEFF {\
	-1543,6884,195,390,195,\
	-2731,7723,668,1337,668,\
	-5657,9793,12750,25499,12750}
#define IIR16_1_ISF 2851
#define IIR16_1_NBIQ 3
#define IIR16_1_QFMAT 13

// filter coefficents for GEQ 2
#define IIR16_2_COEFF {\
	-3393,4799,799,1598,799,\
    -5340,645,-3493,0,3493,\
    -6002,9687,14261,-28522,14261}
#define IIR16_2_ISF 3587
#define IIR16_2_NBIQ 3
#define IIR16_2_QFMAT 13

// filter coefficents for GEQ 3
#define IIR16_3_COEFF {\
    -3393,-4799,799,-1598,799,\
    -5340,-645,-3493,0,3493,\
    -6002,-9687,14261,28522,14261}
#define IIR16_3_ISF 3587
#define IIR16_3_NBIQ 3
#define IIR16_3_QFMAT 13

// filter coefficents for GEQ 4
#define IIR16_4_COEFF {\
    -1543,-6884,195,-390,195,\
    -2731,-7723,668,-1337,668,\
    -5657,-9793,12750,-25499,12750}
#define IIR16_4_ISF 2851
#define IIR16_4_NBIQ 3
#define IIR16_4_QFMAT 13
