//
// Created by ssm on 20. 10. 30..
//

#ifndef SE4_INPUTDATA_H
#define SE4_INPUTDATA_H

namespace se4 {
    // Key, MousePosition 등이 이것을 상속하거나 덕타이핑
    struct InputData{};
    struct InputData2f : InputData {
        float x;
        float y;
    };
}

#endif //SE4_INPUTDATA_H
