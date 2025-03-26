#ifndef _Electric_field_define_H_
#define _Electric_field_define_H_


//位置数据工作状态
enum OPERATION_STATE {
    IDLE,        // 无操作    0
    MODELING,    // 建模      1
    MAPPING      // 标测      2
};

//标测使用的数据处理类型
enum DEBUG_STATE2_MAPPING {
    GATING,      // 门控      0
    FILTERING    // 滤波      1
};


#endif	///_Electric_field_define_H_