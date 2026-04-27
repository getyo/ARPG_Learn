#include "Debug.h"

#include "Engine/Engine.h"
#include "GenericPlatform/GenericPlatformStackWalk.h"

FString GetCleanStackTrace(int32 SkipLines = 2)
{
	// 定义足够大的缓冲区
	ANSICHAR StackBuffer[16384];
	StackBuffer[0] = 0;

	// 使用官方提供的原生接口，这不会报错
	FPlatformStackWalk::StackWalkAndDump(StackBuffer, sizeof(StackBuffer), SkipLines);

	// 将原始输出转换为 FString
	FString RawStackTrace = ANSI_TO_TCHAR(StackBuffer);
    
	// 按照行进行拆分
	TArray<FString> Lines;
	RawStackTrace.ParseIntoArrayLines(Lines);

	FString CleanStackTrace;
	for (const FString& Line : Lines)
	{
		// 核心过滤：如果包含 UnknownFunction，直接跳过这一行
		if (!Line.Contains(TEXT("UnknownFunction"), ESearchCase::IgnoreCase))
		{
			CleanStackTrace += Line + TEXT("\n");
		}
	}

	return CleanStackTrace;
}

inline FColor GetColor(DebugVerbosity Verbosity)
{
	switch (Verbosity)
	{
	case Verbose: return FColor::Green;
	case Warning: return FColor::Yellow;
	case Error: return FColor::Red;
	default: return FColor::Blue;
	}
}

void UDebug::DebugLog(UObject* WorldContextObject, FString Message,DebugVerbosity Verbosity)
{
#if !UE_BUILD_SHIPPING
	GEngine->AddOnScreenDebugMessage(-1,20.f,GetColor(Verbosity),
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


























