// Copyright (c) 2017 Danny Angelo Carminati Grein
// MIT License, see LICENSE file for details

#pragma once

// cr_mode defines how much we validate global state transfer between
// instances. The default is CR_UNSAFE, you can choose another mode by
// defining CR_HOST, ie.: #define CR_HOST CR_SAFEST
enum cr_mode { CR_SAFEST = 0, CR_SAFE = 1, CR_UNSAFE = 2, CR_DISABLE = 3 };

// cr_op is passed into the guest process to indicate the current operation
// happening so the process can manage its internal data if it needs.
enum cr_op {
    CR_LOAD = 0,
    CR_STEP = 1,
    CR_UNLOAD = 2,
    CR_CLOSE = 3,
};

enum cr_failure {
    CR_NONE,     // No error
    CR_SEGFAULT, // SIGSEGV / EXCEPTION_ACCESS_VIOLATION
    CR_ILLEGAL,  // illegal instruction (SIGILL) / EXCEPTION_ILLEGAL_INSTRUCTION
    CR_ABORT,    // abort (SIGBRT)
    CR_MISALIGN, // bus error (SIGBUS) / EXCEPTION_DATATYPE_MISALIGNMENT
    CR_BOUNDS,   // EXCEPTION_ARRAY_BOUNDS_EXCEEDED
    CR_STACKOVERFLOW,     // EXCEPTION_STACK_OVERFLOW
    CR_STATE_INVALIDATED, // one or more global data section changed and does
                          // not safely match basically a failure of
                          // cr_plugin_validate_sections
    CR_BAD_IMAGE, // The binary is not valid - compiler is still writing it
    CR_INITIAL_FAILURE, // Plugin version 1 crashed, cannot rollback
    CR_OTHER,           // Unknown or other signal,
    CR_USER = 0x100,
};

struct cr_plugin;
typedef int (*cr_plugin_main_func)(struct cr_plugin *ctx, enum cr_op operation);

// public interface for the plugin context, this has some user facing
// variables that may be used to manage reload feedback.
// - userdata may be used by the user to pass information between reloads
// - version is the reload counter (after loading the first instance it will
//   be 1, not 0)
// - failure is the (platform specific) last error code for any crash that may
//   happen to cause a rollback reload used by the crash protection system

struct cr_plugin {
    void *p;
    void *userdata;
    unsigned int version;
    enum cr_failure failure;
    unsigned int next_version;
    unsigned int last_working_version;
};

//
// Global OS specific defines/customizations
//
#if defined(_WIN32)
#define CR_WINDOWS
#define CR_PLUGIN(name) "" name ".dll"
#elif defined(__linux__)
#define CR_LINUX
#define CR_PLUGIN(name) "lib" name ".so"
#elif defined(__APPLE__)
#define CR_OSX
#define CR_PLUGIN(name) "lib" name ".dylib"
#else
#error "Unknown/unsupported platform, please open an issue if you think this \
platform should be supported."
#endif // CR_WINDOWS || CR_LINUX || CR_OSX

//
// Global compiler specific defines/customizations
//
#if defined(_MSC_VER)
#if defined(__cplusplus)
#define CR_EXPORT extern "C" __declspec(dllexport)
#define CR_IMPORT extern "C" __declspec(dllimport)
#else
#define CR_EXPORT __declspec(dllexport)
#define CR_IMPORT __declspec(dllimport)
#endif
#endif // defined(_MSC_VER)

#if defined(__GNUC__) // clang & gcc
#if defined(__cplusplus)
#define CR_EXPORT extern "C" __attribute__((visibility("default")))
#else
#define CR_EXPORT __attribute__((visibility("default")))
#endif
#define CR_IMPORT
#endif // defined(__GNUC__)

#if defined(__MINGW32__)
#undef CR_EXPORT
#if defined(__cplusplus)
#define CR_EXPORT extern "C" __declspec(dllexport)
#else
#define CR_EXPORT __declspec(dllexport)
#endif
#endif

// Guest specific compiler defines/customizations
#if defined(_MSC_VER)
#pragma section(".state", read, write)
#define CR_STATE __declspec(allocate(".state"))
#endif // defined(_MSC_VER)

#if defined(CR_OSX)
#define CR_STATE __attribute__((used, section("__DATA,__state")))
#else
#if defined(__GNUC__) // clang & gcc
#define CR_STATE __attribute__((section(".state")))
#endif // defined(__GNUC__)
#endif
