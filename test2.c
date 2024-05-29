#include <stdio.h>

void Move_Zombie_Phase() { //이동 페이즈 계산 좀비
    if (turn % 2 == 1 && !m_pull) {
        if (mz - 1 != mc && mz - 1 != mm) {
            if (c_aggro > m_aggro) {
                --mz;
            }
            else {
                ++mz;
            }
        }
        else {
            if (c_aggro > m_aggro) {
                --mz;
            }
            else {
                ++mz;
            }
        }
    }
}