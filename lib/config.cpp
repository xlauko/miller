module;

#include "miller/version.h"

export module miller;

export namespace mi {

    constexpr auto miller_version = MILLER_VERSION;
    constexpr auto miller_version_major = MILLER_VERSION_MAJOR;
    constexpr auto miller_version_minor = MILLER_VERSION_MINOR;
    constexpr auto miller_version_patch = MILLER_VERSION_PATCH;

} // namespace mi
