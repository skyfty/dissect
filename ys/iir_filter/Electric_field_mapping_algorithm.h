#ifndef _Electric_field_mapping_algorithm_H_
#define _Electric_field_mapping_algorithm_H_
#include "iir_filter.h"
#include <stdio.h>

class Electric_field_mapping_algorithm {
public:
	Electric_field_mapping_algorithm();
	void Electric_field_mapping_algorithm_all
	(
		int		operation_state,						//����״̬		0:Ĭ��û���κβ���;	1:��ģ;	2:���;
		int		catheter_start,							//��ģ���ܿ�ʼ���
		int		catheter_count,							//��ģ����ʹ�ø���
		int		reference_start,						//�ο����ܿ�ʼ���
		int		reference_count,						//�ο�����ʹ�ø���
		//int		debug_state1_model,						//debugʹ�ã���ģ������״̬	0:���κδ���;	1:�����ſ�;				(Ĭ����1)
		int		debug_state2_mapping,					//debugʹ�ã���������״̬	0:�����ſ�;	1:�˲�;							(Ĭ����0)
		float* electric_field_positon_48_in,			//48·�糡λ���ź�����
		int* breath_gate_sync,						//�����ſ�״̬����	0����ʾ��Ч	1����ʾ������Ч
		float* electric_field_positon_48_out,			//48·�糡λ���źŴ�����֮�����
		float* blood_pool_impedance,					//Ѫ���迹
		float* test									//����ʹ��
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