#pragma once

#include <QList>
#include <stdint.h>
#include <vector>

/************************************************************
	Timing constants (assumption Sampling Frequency = 200 Hz)
 ************************************************************/
#define PT150MS				((int32_t)	(30))
#define PT200MS				((int32_t)	(40))
#define PT360MS				((int32_t)	(72))
#define PT900MS				((int32_t)	(180))
#define PT1000MS			((int32_t)	(200))
#define PT2000MS			((int32_t)	(400))
#define PT4000MS			((int32_t)	(800))
#define GENERAL_DELAY		((int32_t)	(38))

 /*************************************************************
	 RR Limits constants for startup (92,116, 166 % OF 200)
 *************************************************************/
#define RR92PERCENT			((int32_t)	(184))
#define RR116PERCENT		((int32_t)	(232))
#define RR166PERCENT		((int32_t)	(332))
 /************************************************************
	 General constants
  ************************************************************/
#define LP_BUFFER_SIZE               ((int32_t)  (12))
#define HP_BUFFER_SIZE               ((int32_t)  (32))
#define DR_BUFFER_SIZE               ((int32_t)  (4))
#define MVA_BUFFER_SIZE              ((int32_t)  (30))
#define RR_BUFFER_SIZE               ((int32_t)  (8))

  /************************************************************
	  PT constants
   ************************************************************/
#define FILTER_FORM		2
#define SQR_LIM_VAL			((int32_t)  (65535))		// We have to limit the Squaring function to avoid overflow once squaring numbers.

#define SQR_LIM_OUT			((uint32_t) (7680000))	// Hardlimiting output of Sqauring filter
#define MVA_LIM_VAL			((int32_t)	(7680000))	// Limiting factor for MVA signal

   // values for State-Machine: PT_data.PT_state
#define START_UP		0
#define LEARN_PH_1		1
#define LEARN_PH_2		2
#define DETECTING		3

// Regular and Irregular Heart-rate
#define IRREGULAR_HR		1
#define REGULAR_HR			0


class PanTompkins
{
public:
    struct PT_output {
        int32_t lp;
        int32_t hp;
        int32_t dr;
        int32_t sqr;
        int32_t mv;
        int32_t thi1;
        int32_t skpi;
        int32_t npki;
        int32_t thf1;
    };
    typedef QList< PT_output> PT_output_List;

private:

	/************************************************************
		Data types
	 ************************************************************/
	struct PT_struct
	{
		int32_t LP_pointer;
		int32_t HP_pointer;
		int32_t MVA_pointer;
		int32_t	PT_state;							//	State of the process
		int32_t Recent_RR_M;						//	Mean of most recent RR

		int32_t LPF_val;
		int32_t HPF_val;
		int32_t DRF_val;
		uint32_t SQF_val;
		uint32_t MVA_val;


		uint32_t ThI1;								// Threshold I1 (Integrated signal)
		uint32_t SPKI;								// Signal peak estimate Integrated
		uint32_t NPKI;								// Noise peak estimate Integrated
		uint32_t ThI2;								// Threshold I2 (Integrated signal)

        int32_t ThF1;								// Threshold F1 (Band-passed signal)
        int32_t SPKF;								// Signal peak estimate BP
		int32_t NPKF;								// Noise peak estimate BP
		int32_t ThF2;								// Threshold F2 (Band-passed signal)

		int32_t RR_M;								//  General mean of RR within acceptable range
		int32_t RR_Low_L;							//	RR Low limit 
		int32_t RR_High_L;							//	RR High limit 
		int32_t RR_Missed_L;						//	RR missed limit 
		int32_t HR_State;							//  HR-State can be regular or irregular

		int32_t LP_buf[LP_BUFFER_SIZE];				//  LP filter buffer
		int32_t HP_buf[HP_BUFFER_SIZE];				//  HP filter buffer
		int32_t DR_buf[DR_BUFFER_SIZE];				//  DR filter buffer
		uint32_t MVA_buf[MVA_BUFFER_SIZE];			//  MVA filter buffer
		int32_t RR_AVRG1_buf[RR_BUFFER_SIZE];		//  RR average 1 buffer
		int32_t RR_AVRG2_buf[RR_BUFFER_SIZE];		//  RR average 2 buffer
	};

public:
	explicit PanTompkins();
	int32_t Rcount = 0;

    template <class _InIt>
    int32_t detect(_InIt _First, _InIt _Last, PT_output_List *list = nullptr) {
        // ------ Pass the signal sample by sample mimicing a real-time scenario ----------- //
        for (auto c = _First; c != _Last; ++c) {
			// This is the main function of the algorithm
			int32_t delay = StateMachine((int32_t)*c);
            // ------- A positive delay to current sample is returned in case of beat detection ----------- //
            if (delay != 0) {
                ++Rcount;
            }
			if (list != nullptr) {
                PT_output ptoutput;
                ptoutput.lp = PT_get_LPFilter_output();
                ptoutput.hp = PT_get_HPFilter_output();
                ptoutput.dr = PT_get_DRFilter_output();
                ptoutput.sqr = PT_get_SQRFilter_output();
                ptoutput.mv = PT_get_MVFilter_output();
                ptoutput.thi1 = PT_get_ThI1_output();
                ptoutput.skpi = PT_get_SKPI_output();
                ptoutput.npki = PT_get_NPKI_output();
                ptoutput.thf1 = PT_get_ThF1_output();
                list->push_back(ptoutput);
			}
        }
        return Rcount;
    }

    void detect(int32_t c) {
        if (StateMachine(c) != 0) {
            ++Rcount;
        }
    }

protected:
	/**********************************************************************
		Function Prototypes
	 **********************************************************************/
	void Init();
	int32_t StateMachine(int32_t datum);
	void LearningPhase1(uint32_t* pkI, int32_t* pkBP);
	void LPFilter(int32_t* val);
	void HPFilter(void);
	void DerivFilter(void);
	void SQRFilter(void);
	void MVAFilter(void);
	uint32_t PeakDtcI(void);
	void PeakDtcDR(int32_t DR_sample);
	void PeakDtcBP(int32_t DR_sample);
	void UpdateRR(int32_t qrs);
	void UpdateThI(uint32_t* PEAKI, int8_t NOISE_F);
	void UpdateThF(int32_t* PEAKF, int8_t NOISE_F);

	/**********************************************************************
		Debuggin Functions
	***********************************************************************/
	int32_t PT_get_LPFilter_output(void);
	int32_t PT_get_HPFilter_output(void);
	int32_t PT_get_DRFilter_output(void);
	uint32_t PT_get_MVFilter_output(void);
	uint32_t PT_get_SQRFilter_output(void);
	int32_t PT_get_ShortTimeHR_output(int32_t Fs);
	int32_t PT_get_LongTimeHR_output(int32_t Fs);
	uint32_t PT_get_ThI1_output(void);
	int32_t PT_get_ThF1_output(void);
	uint32_t PT_get_SKPI_output(void);
	uint32_t PT_get_NPKI_output(void);
	int32_t PT_get_SPKF_output(void);
	int32_t PT_get_NPKF_output(void);
	int32_t PT_get_HRState_output(void);
	int32_t PT_get_State_output(void);

private:

	int32_t LP_y_new;
	int32_t LP_y_old;
	int32_t Prev_valBP;
	int32_t Prev_Prev_valBP;
	int32_t Best_PeakBP;
	int32_t Prev_valDR;
	int32_t Prev_Prev_valDR;
	int32_t Best_PeakDR;
	int32_t Old_PeakDR;
	int32_t Count_SinceRR;
	int32_t RR1_p;
	int32_t RR2_p;
	int32_t RR1_sum;
	int32_t RR2_sum;
	int32_t BlankTimeCnt;
	int32_t SBcntI;
	int32_t SB_peakBP;
	int32_t SB_peakDR;
	int32_t y_h;
	int32_t st_mean_pkBP;

	uint32_t MV_sum;
	uint32_t PEAKI_temp;
	uint32_t st_mx_pk;
	uint32_t st_mean_pk;
	uint32_t Prev_val;
	uint32_t Prev_Prev_val;
	uint32_t SB_peakI;

private:
	/********************************************************************************
		PT Algorithm data and buffers, see PT_init for detailed use of each parameter.
	 ********************************************************************************/
	struct PT_struct PT_data;
	struct PT_struct* const PT_dptr = &PT_data;
};

