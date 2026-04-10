#pragma once
#include <cstdint>
#include <windows.h>
#include "reference/OriginalInternal.h"

#pragma once
#include <cstdint>
#include <windows.h>

// Helper macros to strip one level of parentheses
#define UNWRAP(...) __VA_ARGS__

class OriginalInternal {
public:
  static uintptr_t BaseAddress;
  static uintptr_t FromOffset(uintptr_t offset) {
    return BaseAddress + (offset - 0x10000000); // Ghidra offset
  }

  // Generic field access by offset
  template <typename T>
  static T* Field(void* instance, uintptr_t offset) {
    return reinterpret_cast<T*>(
        reinterpret_cast<uintptr_t>(instance) + offset);
  }

  // Reference version
  template <typename T>
  static T& Ref(void* instance, uintptr_t offset) {
    return *Field<T>(instance, offset);
  }
};

// For non‑static member functions with Parameters
#define DEFINE_MEMBER_FUNCTION(retType, name, offset, params, args)            \
  retType name params {                                                        \
    static auto fn =                                                           \
        reinterpret_cast<retType(__thiscall *)(void *, UNWRAP params)>(        \
            OriginalInternal::FromOffset(offset));                             \
    return fn(this, UNWRAP args);                                              \
  }

// For non‑static member functions without Parameters (0 Params)
#define DEFINE_MEMBER_FUNCTION_0(retType, name, offset)                        \
  retType name() {                                                             \
    static auto fn = reinterpret_cast<retType(__thiscall *)(void *)>(          \
        OriginalInternal::FromOffset(offset));                                 \
    return fn(this);                                                           \
  }

// For static functions with Parameters
#define DEFINE_STATIC_FUNCTION(retType, name, offset, params, args)            \
  static retType name params {                                                 \
    static auto fn = reinterpret_cast<retType(__thiscall *)(UNWRAP params)>(   \
        OriginalInternal::FromOffset(offset));                                 \
    return fn(UNWRAP args);                                                    \
  }

// For static functions without Parameters (0 Params)
#define DEFINE_STATIC_FUNCTION_0(retType, name, offset)                        \
  static retType name() {                                                      \
    static auto fn = reinterpret_cast<retType(__thiscall *)()>(                \
        OriginalInternal::FromOffset(offset));                                 \
    return fn();                                                               \
  }
