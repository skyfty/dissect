#ifndef _Electric_field_define_H_
#define _Electric_field_define_H_


//λ�����ݹ���״̬
enum OPERATION_STATE {
    IDLE,        // �޲���    0
    MODELING,    // ��ģ      1
    MAPPING      // ���      2
};

//���ʹ�õ����ݴ�������
enum RESPIRATORY_MODE {
    NOTHING,     // ���㷨    0
    GATING,      // �ſ�      1
    ADAPTIVE     // �˲�      2
};


#endif	///_Electric_field_define_H_