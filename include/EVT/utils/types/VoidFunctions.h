/**
 * File containing Type definition for void function pointers.  Mainly
 * used as IRQHandlers
 */

#ifndef EVT_VOIDFUNCTIONS_H
#define EVT_VOIDFUNCTIONS_H

namespace EVT::core::types {

    using void_function_ptr_t = void (*)();

} // namespace EVT::core::types

#endif //EVT_VOIDFUNCTIONS_H
