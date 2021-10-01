#ifndef _EVT_IWDGf302x8_
#define _EVT_IWDGf302x8_

#include <HALf3/stm32f3xx.h>

#include <EVT/dev/IWDG.hpp>

namespace EVT::core::DEV {

    class IWDGf302x8: public IWDG {
    public:
        IWDGf302x8(uint32_t windowSize);
        void refresh();
    private:
        IWDG_HandleTypeDef halIWDG;
    };

} // namespace EVT::core::DEV

#endif