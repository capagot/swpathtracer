#ifndef CORE_CORE_H
#define CORE_CORE_H

#define PI_f 3.141592653589793238462643383279502884f
#define DEG_TO_RAD PI_f / 180.0f
#define APP_NAME "SWPathtracer"
#define APP_MAJOR_VERSION "0"
#define APP_MINOR_VERSION "2"

#ifdef ENABLE_FEEXCEPTION
    #define ENABLE_FEEXCEPTION_HANDLING() do { \
        feenableexcept(FE_INVALID | FE_DIVBYZERO); \
    } while (0)

    #define DISABLE_FEEXCEPTION_HANDLING() do { \
        fedisableexcept(FE_INVALID | FE_DIVBYZERO); \
    } while (0)
#else
    #define ENABLE_FEEXCEPTION_HANDLING()
    #define DISABLE_FEEXCEPTION_HANDLING()
#endif // ENABLE_FEEXCEPTION

#ifdef DEBUG
    #define APP_BUILD_MODE "Debug"
#else
    #define APP_BUILD_MODE "Release"
#endif // DEBUG

#endif  // CORE_CORE_H
