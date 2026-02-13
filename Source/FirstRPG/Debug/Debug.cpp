#include "Debug.h"
#include "GenericPlatform/GenericPlatformStackWalk.h"

FString GetCleanStackTrace(int32 SkipLines = 2)
{
	// 定义一个缓冲区来存放堆栈字符
	// 32768 是一个比较安全的长度，因为堆栈可能很深
	ANSICHAR StackBuffer[32768];
	StackBuffer[0] = 0;

	// 参数 1: 缓冲区
	// 参数 2: 缓冲区大小
	// 参数 3: 忽略多少层栈顶（用来跳过当前封装函数）
	FPlatformStackWalk::StackWalkAndDump(StackBuffer, 32768, SkipLines);

	return FString(ANSI_TO_TCHAR(StackBuffer));
}

inline FColor GetColor(DebugVerbosity Verbosity)
{
	switch (Verbosity)
	{
	case DebugVerbosity::Verbose: return FColor::Green;
	case ELogVerbosity::Warning: return FColor::Yellow;
	case ELogVerbosity::Error: return FColor::Red;
	default: return FColor::Blue;
	}
}

void UDebug::DebugLog(UObject* WorldContextObject, FString Message,DebugVerbosity Verbosity)
{
#if !UE_BUILD_SHIPPING
	GEngine->AddOnScreenDebugMessage(-1,20.f,GetColor(DebugVerbosity::Verbose),
		FString::Printf(TEXT("Class: %s, %s"),
		*WorldContextObject->GetName(),*Message));
	
	
	switch (Verbosity)
	{
		case DebugVerbosity::Verbose:
		{
			UE_LOG(LogTemp,Verbose,TEXT("Class: %s, %s"),
				*WorldContextObject->GetName(),*Message);
			break;
		}
		case DebugVerbosity::Warning:
		{
			UE_LOG(LogTemp,Warning,TEXT("Class: %s, %s"),
				*WorldContextObject->GetName(),*Message);
			break;
		}
		case DebugVerbosity::Error:
		{
			UE_LOG(LogTemp,Error,TEXT("Class: %s, %s\n Stack: %s"),
				*WorldContextObject->GetName(),*Message,*GetCleanStackTrace());
			break;
		}
	}
#endif
}

void UDebug::InternalLog(const FString& ClassName, const FString& CppFuncName, DebugVerbosity Verbosity,
	const FString& Message)
{
#if !UE_BUILD_SHIPPING
	GEngine->AddOnScreenDebugMessage(-1,20.f,GetColor(Verbosity),
		FString::Printf(TEXT("Class: %s, Function: %s, %s"),
			*ClassName,*CppFuncName,*Message));
	
	
	switch (Verbosity)
	{
	case DebugVerbosity::Verbose:
		{
			UE_LOG(LogTemp,Verbose,TEXT("Class: %s,Function: %s, %s"),
				*ClassName,*CppFuncName,*Message);
			break;
		}
	case DebugVerbosity::Warning:
		{
			UE_LOG(LogTemp,Warning,TEXT("Class: %s,Function: %s, %s"),
				*ClassName,*CppFuncName,*Message);
			break;
		}
	case DebugVerbosity::Error:
		{
			UE_LOG(LogTemp,Error,TEXT("Class: %s, Function: %s, %s,\n Stack: %s"),
			*ClassName,*CppFuncName,*Message,*GetCleanStackTrace());
			break;
		}
	}
#endif
	
}


























