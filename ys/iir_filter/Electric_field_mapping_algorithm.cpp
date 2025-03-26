#include "Electric_field_mapping_algorithm.h"
#include <math.h>
#include <string.h>

Electric_field_mapping_algorithm::Electric_field_mapping_algorithm()
{
	num_with_breathe[0] = 4.37268879782e-06;
	num_with_breathe[1] = 1.749075519128e-05;
	num_with_breathe[2] = 2.623613278692e-05;
	num_with_breathe[3] = 1.749075519128e-05;
	num_with_breathe[4] = 4.37268879782e-06;
	den_with_breathe[0] = 1;
	den_with_breathe[1] = -3.753762756671;
	den_with_breathe[2] = 5.291152584163;
	den_with_breathe[3] = -3.318938604751;
	den_with_breathe[4] = 0.781618740279;

	num_without_breathe[0] = 1.533245520602e-09; 
	num_without_breathe[1] = 6.132982082408e-09; 
	num_without_breathe[2] = 9.199473123612e-09; 
	num_without_breathe[3] = 6.132982082408e-09;
	num_without_breathe[4] = 1.533245520602e-09;
	den_without_breathe[0] = 1;
	den_without_breathe[1] = -3.967162595949;
	den_without_breathe[2] = 5.902025861491;
	den_without_breathe[3] = -3.902558784823;
	den_without_breathe[4] = 0.9676955438131;


	for (int i = 0; i < 49; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			IIR_Filter_list_with_breathe[i][j] = new IIR_Filter(num_with_breathe, den_with_breathe);
			IIR_Filter_list_without_breathe[i][j] = new IIR_Filter(num_without_breathe, den_without_breathe);
		}
	}

}

void Electric_field_mapping_algorithm::Electric_field_mapping_algorithm_all
(
	int		operation_state,						//手术状态		0:默认没有任何操作;	1:建模;	2:标测;
	int		catheter_start,							//建模导管开始序号
	int		catheter_count,							//建模导管使用个数
	int		reference_start,						//参考导管开始序号
	int		reference_count,						//参考导管使用个数
	//int		debug_state1_model,						//debug使用，建模的两种状态	0:无任何处理;	1:呼吸门控;				(默认置1)
	int		debug_state2_mapping,					//debug使用，标测的两种状态	0:呼吸门控;	1:滤波;							(默认置0)
	float*	electric_field_positon_48_in,			//48路电场位置信号输入
	int*	breath_gate_sync,						//呼吸门控状态控制	0：表示无效	1：表示数据有效
	float*	electric_field_positon_48_out,			//48路电场位置信号处理完之后输出
	float*  blood_pool_impedance,					//血池阻抗
	float*  test									//测试使用
	)
{
	int zeropoint = reference_start + 5;

	//导管位置处理
	for (int i = catheter_start; i < catheter_start + catheter_count; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			electric_field_positon_48_in_tmp[i][j] = electric_field_positon_48_in[i * 3 + j] - electric_field_positon_48_in[zeropoint * 3 + j];
			electric_field_positon_48_out_tmp_with_breathe[i][j] = IIR_Filter_list_with_breathe[i][j]->filter(electric_field_positon_48_in_tmp[i][j]);
			electric_field_positon_48_out_tmp_without_breathe[i][j] = IIR_Filter_list_without_breathe[i][j]->filter(electric_field_positon_48_in_tmp[i][j]);
			electric_field_positon_48_out_tmp[i][j] = (float)(electric_field_positon_48_out_tmp_with_breathe)[i][j];
		}
	}
	//十级位置处理
	for (int i = reference_start; i < reference_start + reference_count; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			electric_field_positon_48_in_tmp[i][j] = electric_field_positon_48_in[i * 3 + j] - electric_field_positon_48_in[zeropoint * 3 + j];
			electric_field_positon_48_out_tmp_without_breathe[i][j] = IIR_Filter_list_without_breathe[i][j]->filter(electric_field_positon_48_in_tmp[i][j]);
			electric_field_positon_48_out_tmp[i][j] = (float)(electric_field_positon_48_out_tmp_without_breathe[i][j]);
		}
	}
	//零点位置处理
	for (int j = 0; j < 3; j++)
	{
		electric_field_positon_48_in_tmp[48][j] = electric_field_positon_48_in[zeropoint * 3 + j];
		electric_field_positon_48_out_tmp_with_breathe[48][j] = IIR_Filter_list_with_breathe[48][j]->filter(electric_field_positon_48_in_tmp[48][j]);
		electric_field_positon_48_out_tmp_without_breathe[48][j] = IIR_Filter_list_without_breathe[48][j]->filter(electric_field_positon_48_in_tmp[48][j]);
	}
	double zeropoint_with_breathe_square = pow(electric_field_positon_48_out_tmp_with_breathe[48][0], 2) + pow(electric_field_positon_48_out_tmp_with_breathe[48][1], 2) + pow(electric_field_positon_48_out_tmp_with_breathe[48][2], 2);
	double zeropoint_with_breathe = sqrt(zeropoint_with_breathe_square);
	double zeropoint_without_breathe_square = pow(electric_field_positon_48_out_tmp_without_breathe[48][0], 2) + pow(electric_field_positon_48_out_tmp_without_breathe[48][1], 2) + pow(electric_field_positon_48_out_tmp_without_breathe[48][2], 2);
	double zeropoint_without_breathe = sqrt(zeropoint_without_breathe_square);
	//门控信号
	*breath_gate_sync = zeropoint_without_breathe > zeropoint_with_breathe;
	*blood_pool_impedance = 1;
	test[0] = zeropoint_with_breathe;
	test[1] = zeropoint_without_breathe;
	//建模导管操作
	if (2 == operation_state)	//2:标测
	{
		if (1 == debug_state2_mapping)	//1:滤波
		{
			//复制没有有呼吸的导管位置
			for (int i = catheter_start; i < catheter_start + catheter_count; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					electric_field_positon_48_out_tmp[i][j] = (float)(electric_field_positon_48_out_tmp_without_breathe)[i][j];
				}
			}
		}
	}

	//复制处理后的位置信息到用户空间
	memcpy(electric_field_positon_48_out, electric_field_positon_48_out_tmp, sizeof(float)*48 * 3);
}