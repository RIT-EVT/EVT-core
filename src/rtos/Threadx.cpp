
#include <EVT/rtos/Threadx.hpp>
#include <EVT/utils/log.hpp>

namespace core::rtos {

TXError init(Initializable* initList, std::size_t length, BytePoolBase &pool) {

    TXError errorCode = pool.init();
    if (errorCode != Success)
        return errorCode;

    for (int i = 0; i < length; i++) {
        errorCode = initList[i].init(pool);
        if (errorCode != Success) {
            //Todo: figure out how to log which one in the list had an error.
            return errorCode;
        }
    }
}

}//namespace core::rtos
