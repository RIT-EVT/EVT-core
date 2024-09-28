
#include <EVT/rtos/Threadx.hpp>
#include <EVT/rtos/BytePool.hpp>
#include <EVT/utils/log.hpp>

namespace log = EVT::core::log;

namespace core::rtos {

namespace {
    Initializable* initializableList;
    std::size_t initListLength;
    BytePoolBase *mainThreadPool;
}

TXError init(Initializable* initList, std::size_t length, BytePoolBase &pool) {
    TXError errorCode = pool.init();
    if (errorCode != Success)
        return errorCode;

    for (std::size_t i = 0; i < length; i++) {
        errorCode = initList[i].init(pool);
        if (errorCode != Success) {
            log::LOGGER.log(log::Logger::LogLevel::DEBUG, "Errored on item %u in initializer list.", i);
            return errorCode;
        }
    }
}

extern "C" void tx_application_define(void* first_unused_memory) {
    init(initializableList, initListLength, *mainThreadPool);
}

TXError startKernel(Initializable* initList, std::size_t length, BytePoolBase &pool) {
    initializableList = initList;
    initListLength = length;
    mainThreadPool = &pool;
    tx_kernel_enter();
    return Success;
}

void relinquish() {
    tx_thread_relinquish();
}

TXError sleep(uint32_t sleepTime) {
    uint32_t errorCode = tx_thread_sleep(sleepTime);
    return static_cast<TXError>(errorCode);
}

}//namespace core::rtos
