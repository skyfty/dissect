#ifndef _Electric_field_mapping_algorithm_H_
#define _Electric_field_mapping_algorithm_H_
#include "iir_filter.h"
#include <stdio.h>

class Electric_field_mapping_algorithm {
public:
	Electric_field_mapping_algorithm();
	void Electric_field_mapping_algorithm_all
	(
		int		operation_state,						//手术状态		0:默认没有任何操作;	1:建模;	2:标测;
		int		catheter_start,							//建模导管开始序号
		int		catheter_count,							//建模导管使用个数
		int		reference_start,						//参考导管开始序号
		int		reference_count,						//参考导管使用个数
		//int		debug_state1_model,						//debug使用，建模的两种状态	0:无任何处理;	1:呼吸门控;				(默认置1)
		int		debug_state2_mapping,					//debug使用，标测的两种状态	0:呼吸门控;	1:滤波;							(默认置0)
		float* electric_field_positon_48_in,			//48路电场位置信号输入
		int* breath_gate_sync,						//呼吸门控状态控制	0：表示无效	1：表示数据有效
		float* electric_field_positon_48_out,			//48路电场位置信号处理完之后输出
		float* blood_pool_impedance,					//血池阻抗
		float* test									//测试使用
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