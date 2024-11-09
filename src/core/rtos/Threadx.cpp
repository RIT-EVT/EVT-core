#include <core/rtos/BytePool.hpp>
#include <core/rtos/Threadx.hpp>
#include <core/utils/log.hpp>

namespace log = core::log;

namespace core::rtos {

namespace {
Initializable** initializableList;
size_t initListLength;
BytePoolBase* mainThreadPool;

TXError init(Initializable* initList[], std::size_t length, BytePoolBase& pool) {
    TXError errorCode = pool.init();
    if (errorCode != SUCCESS)
        return errorCode;

    return bulkInitialize(initList, length, pool);
}

extern "C" void tx_application_define(void* first_unused_memory) {
    init(initializableList, initListLength, *mainThreadPool);
}

} // namespace

TXError startKernel(Initializable* initList[], std::size_t length, BytePoolBase& pool) {
    initializableList = initList;
    initListLength    = length;
    mainThreadPool    = &pool;
    // tx_kernel_enter calls tx_application_define, which initializes everything in initList.
    tx_kernel_enter();
    return SUCCESS;
}

TXError bulkInitialize(Initializable** initList, std::size_t length, BytePoolBase& pool) {
    TXError errorCode = SUCCESS;
    for (std::size_t i = 0; i < length; i++) {
        errorCode = initList[i]->init(pool);
        if (errorCode != SUCCESS) {
            log::LOGGER.log(log::Logger::LogLevel::DEBUG,
                            "Errored on item %u in initializer list.\n\rError code: %u",
                            i,
                            errorCode);
            return errorCode;
        }
    }
    return errorCode;
}

void relinquish() {
    tx_thread_relinquish();
}

TXError sleep(uint32_t sleepTime) {
    uint32_t errorCode = tx_thread_sleep(sleepTime);
    return static_cast<TXError>(errorCode);
}

} // namespace core::rtos
