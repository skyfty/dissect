#ifndef _Electric_field_define_H_
#define _Electric_field_define_H_


//位置数据工作状态
enum OPERATION_STATE {
    IDLE,        // 无操作    0
    MODELING,    // 建模      1
    MAPPING      // 标测      2
};

//标测使用的数据处理类型
enum RESPIRATORY_MODE {
    NOTHING,     // 无算法    0
    GATING,      // 门控      1
    ADAPTIVE     // 滤波      2
};


#endif	///_Electric_field_define_H_