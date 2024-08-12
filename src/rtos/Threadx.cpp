
#include <EVT/rtos/Threadx.hpp>

namespace core::rtos {

void init(Initializable* initList, std::size_t length, BytePoolBase&poolptr) {
    for (int i = 0; i < length; i++) {
        initList[i].init(poolptr);
    }
}

}//namespace core::rtos
