add_definitions(-DUNICODE -D_UNICODE)

add_library(breakpad STATIC
    "breakpad/src/common/windows/string_utils-inl.h"
    "breakpad/src/common/windows/guid_string.h"
    "breakpad/src/google_breakpad/common/minidump_format.h"
    "breakpad/src/google_breakpad/common/breakpad_types.h"
    "breakpad/src/client/windows/crash_generation/crash_generation_client.h"
    "breakpad/src/common/scoped_ptr.h"
    "breakpad/src/client/windows/handler/exception_handler.cc"
    "breakpad/src/common/windows/string_utils.cc"
    "breakpad/src/common/windows/guid_string.cc"
    "breakpad/src/client/windows/crash_generation/crash_generation_client.cc"
    )
target_include_directories(breakpad PUBLIC
    "breakpad/src"
)
