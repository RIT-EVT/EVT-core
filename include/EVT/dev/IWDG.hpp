#ifndef _EVT_IWDG_
#define _EVT_IWDG_

namespace EVT::core::DEV {

    class IWDG {
    public:
        virtual void refresh() = 0;
    };

} // namespace EVT::core::DEV

#endif