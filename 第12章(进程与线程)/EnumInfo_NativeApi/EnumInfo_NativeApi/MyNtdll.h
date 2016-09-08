#pragma once

#include <windows.h>

typedef LONG    NTSTATUS;
typedef ULONG   ACCESS_MASK;
typedef ULONG	KPRIORITY ;
typedef DWORD	ACCESS_MASK ;

#define NT_SUCCESS(status)          ((NTSTATUS)(status)>=0)
#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004L)
#define STATUS_ACCESS_DENIED        ((NTSTATUS)0xC0000022L)

typedef struct _UNICODE_STRING
{
    USHORT Length;
    USHORT MaximumLength;
    PWSTR Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef enum _SYSTEM_INFORMATION_CLASS {
	SystemBasicInformation,				// 1 Y N
	SystemProcessorInformation,			// 1 Y N
	SystemPerformanceInformation,		// 2 Y N
	SystemTimeOfDayInformation,			// 3 Y N
	SystemNotImplemented1,				// 4 Y N
	SystemProcessesAndThreadsInformation, // 5 Y N
	SystemCallCounts,					// 6 Y N
	SystemConfigurationInformation,		// 7 Y N
	SystemProcessorTimes,				// 8 Y N
	SystemGlobalFlag,					// 9 Y Y
	SystemNotImplemented2,				// 10 Y N
	SystemModuleInformation,			// 11 Y N
	SystemLockInformation,				// 12 Y N
	SystemNotImplemented3,				// 13 Y N
	SystemNotImplemented4,				// 14 Y N
	SystemNotImplemented5,				// 15 Y N
	SystemHandleInformation,			// 16 Y N
	SystemObjectInformation,			// 17 Y N
	SystemPagefileInformation,			// 18 Y N
	SystemInstructionEmulationCounts,	// 19 Y N
	SystemInvalidInfoClass1,			// 20
	SystemCacheInformation,				// 21 Y Y
	SystemPoolTagInformation,			// 22 Y N
	SystemProcessorStatistics,			// 23 Y N
	SystemDpcInformation,				// 24 Y Y
	SystemNotImplemented6,				// 25 Y N
	SystemLoadImage,					// 26 N Y
	SystemUnloadImage,					// 27 N Y
	SystemTimeAdjustment,				// 28 Y Y
	SystemNotImplemented7,				// 29 Y N
	SystemNotImplemented8,				// 30 Y N
	SystemNotImplemented9,				// 31 Y N
	SystemCrashDumpInformation,			// 32 Y N
	SystemExceptionInformation,			// 33 Y N
	SystemCrashDumpStateInformation,	// 34 Y Y/N
	SystemKernelDebuggerInformation,	// 35 Y N
	SystemContextSwitchInformation,		// 36 Y N
	SystemRegistryQuotaInformation,		// 37 Y Y
	SystemLoadAndCallImage,				// 38 N Y
	SystemPrioritySeparation,			// 39 N Y
	SystemNotImplemented10,				// 40 Y N
	SystemNotImplemented11,				// 41 Y N
	SystemInvalidInfoClass2,			// 42
	SystemInvalidInfoClass3,			// 43
	SystemTimeZoneInformation,			// 44 Y N
	SystemLookasideInformation,			// 45 Y N
	SystemSetTimeSlipEvent,				// 46 N Y
	SystemCreateSession,				// 47 N Y
	SystemDeleteSession,				// 48 N Y
	SystemInvalidInfoClass4,			// 49
	SystemRangeStartInformation,		// 50 Y N
	SystemVerifierInformation,			// 51 Y Y
	SystemAddVerifier,					// 52 N Y
	SystemSessionProcessesInformation	// 53 Y N
} SYSTEM_INFORMATION_CLASS;

typedef struct _VM_COUNTERS {
	ULONG PeakVirtualSize;
	ULONG VirtualSize;
	ULONG PageFaultCount;
	ULONG PeakWorkingSetSize;
	ULONG WorkingSetSize;
	ULONG QuotaPeakPagedPoolUsage;
	ULONG QuotaPagedPoolUsage;
	ULONG QuotaPeakNonPagedPoolUsage;
	ULONG QuotaNonPagedPoolUsage;
	ULONG PagefileUsage;
	ULONG PeakPagefileUsage;
} VM_COUNTERS, *PVM_COUNTERS;

/*
*Information Class 5
*/
typedef struct _SYSTEM_PROCESSES {
	ULONG NextEntryDelta;
	ULONG ThreadCount;
	ULONG Reserved1[6];
	LARGE_INTEGER CreateTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER KernelTime;
	UNICODE_STRING ProcessName;
	KPRIORITY BasePriority;
	ULONG ProcessId;
	ULONG InheritedFromProcessId;
	ULONG HandleCount;
	ULONG Reserved2[2];
	VM_COUNTERS VmCounters;
	IO_COUNTERS IoCounters; // Windows 2000 only
	//SYSTEM_THREADS Threads[1];
} SYSTEM_PROCESSES, *PSYSTEM_PROCESSES;

// 定义NtQuerySystemInformation函数
typedef NTSTATUS (WINAPI *PFNNtQuerySystemInformation)(   
	SYSTEM_INFORMATION_CLASS   SystemInformationClass,	// 定义服务类型号
	PVOID   SystemInformation,							// 用户存储信息的缓冲区
	ULONG   SystemInformationLength,					// 缓冲区大小
	PULONG   ReturnLength   );							// 返回信息长度