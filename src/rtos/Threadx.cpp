
#include <EVT/rtos/Threadx.hpp>
#include <EVT/utils/log.hpp>

namespace log = EVT::core::log;

namespace core::rtos {

TXError init(Initializable* initList, std::size_t length, BytePoolBase &pool) {

    TXError errorCode = pool.init();
    if (errorCode != Success)
        return errorCode;

    for (int i = 0; i < length; i++) {
        errorCode = initList[i].init(pool);
        if (errorCode != Success) {
            log::LOGGER.log(log::Logger::LogLevel::DEBUG, "Errored on item %u in initializer list.", i);
            return errorCode;
        }
    }
}

void relinquish() {
    //this method doesn't take any arguments in reality but this appeases the compiler
    tx_thread_relinquish(0);
}

}//namespace core::rtos
