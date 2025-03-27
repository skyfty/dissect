#ifndef _Electric_field_mapping_algorithm_H_
#define _Electric_field_mapping_algorithm_H_
#include "iir_filter.h"
#include <stdio.h>

class Electric_field_mapping_algorithm {
public:
	Electric_field_mapping_algorithm();
	void Electric_field_mapping_algorithm_all
	(
		int		operation_state,
		int		catheter_start,
		int		catheter_count,
		int		reference_start,
		int		reference_count,
		//int		debug_state1_model,
		int		respiratory_mode,
		float* electric_field_positon_48_in,
		int* breath_gate_sync,
		float* electric_field_positon_48_out,
		float* blood_pool_impedance,
		float* test
	);
private:
	double num_with_breathe[5];
	double den_with_breathe[5];
	IIR_Filter* IIR_Filter_list_with_breathe[49][3];
	double num_without_breathe[5];
	double den_without_breathe[5];
	IIR_Filter* IIR_Filter_list_without_breathe[49][3];
	double electric_field_positon_48_in_tmp[49][3] = { 0 };
	double electric_field_positon_48_out_tmp_with_breathe[49][3] = { 0 };
	double electric_field_positon_48_out_tmp_without_breathe[49][3] = { 0 };
	float electric_field_positon_48_out_tmp[49][3] = { 0 };
};

#endif	///_Electric_field_mapping_algorithm_H_