#ifndef BTRGB_HPP
#define BTRGB_HPP

#include <string>

/* ==================[ DEFINES ]================== */
/* Tips:
 *
 *  -- Start with "BTRGB_" or some other unique
 *     prefix to avoid collisions with defines from
 *     other libraries.
 *
 *  -- Try to use a C++ const when possible. This
 *     allows the compiler to work its optimization
 *     magic.
 *
 *  -- If a define is only used once, it might make
 *     more sense to define it at the beginning of
 *     the function which it is used, and then
 *     undef it at the end of the function.
 */

#define BTRGB_M_OPT "M_opt"
#define BTRGB_OFFSET_OPT "Offset_opt"
#define BTRGB_M_REFL_OPT "M_refl_opt"

/* ============[ CONSTANTS and TYPES ]============ */
namespace btrgb {

const int TAG_MAKE = 0x010f;
const int TAG_MODEL = 0x0110;
const std::string UNSPECIFIED = "unspecified";
struct exif {
    std::string make = UNSPECIFIED;
    std::string model = UNSPECIFIED;
};

} // namespace btrgb

#endif