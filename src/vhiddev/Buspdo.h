#pragma once

EXTERN_C_START

#define MAX_ID_LEN 80

typedef
BOOLEAN
(*PPDO_GET_CRISPINESS_LEVEL)(
    _In_  PVOID   Context,
    _Out_ PUCHAR  Level
    );

typedef
BOOLEAN
(*PPDO_SET_CRISPINESS_LEVEL)(
    _In_  PVOID  Context,
    _In_  UCHAR  Level
    );

typedef
BOOLEAN
(*PPDO_IS_CHILD_PROTECTED)(
    _In_ PVOID  Context
    );

//
// Interface for getting and setting power level etc.,
//
typedef struct _PDO_INTERFACE_STANDARD {
    INTERFACE                    InterfaceHeader;
    PPDO_GET_CRISPINESS_LEVEL    GetCrispinessLevel;
    PPDO_SET_CRISPINESS_LEVEL    SetCrispinessLevel;
    PPDO_IS_CHILD_PROTECTED      IsSafetyLockEnabled;
} PDO_INTERFACE_STANDARD, * PPDO_INTERFACE_STANDARD;

EXTERN_C_END
