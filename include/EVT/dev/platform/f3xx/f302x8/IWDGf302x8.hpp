#ifndef _EVT_IWDGf302x8_
#define _EVT_IWDGf302x8_

#include <HALf3/stm32f3xx.h>

#include <EVT/dev/IWDG.hpp>

namespace EVT::core::DEV {


    class IWDGf302x8: public IWDG {
    public:

        /**
         * Constructor for the IWDG.
         * @param windowSize Window size that's compared to the counter value.
         * Must be a value between 0 and 4095
         */
        IWDGf302x8(uint32_t windowSize);

        /**
         * Refreshes the IWDG to avoid a system reset
         */
        void refresh();

    private:
        /** Instance of the HAL IWDG */
        IWDG_HandleTypeDef halIWDG;
    };

} // namespace EVT::core::DEV

#endif