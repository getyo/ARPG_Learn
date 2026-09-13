#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Debug.generated.h"

UENUM()
enum DebugVerbosity
{
	Verbose,
	Warning,
	Error,
};

UCLASS()
class FIRSTRPG_API UDebug : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Debug",meta = (WorldContext = "WorldContextObject"))
	static void DebugLog(UObject* WorldContextObject, FString Message,DebugVerbosity Verbosity);
	
	static void InternalLog(const FString &ClassName, const FString &CppFuncName, DebugVerbosity Verbosity, const FString& Message);
};
#if !UE_BUILD_SHIPPING
	#define CPP_LOG(Verbosity,Msg) \
	UDebug::InternalLog(GetName(), FString(__FUNCTION__), Verbosity, Msg)
#define ASSERT(Expr) checkf(Expr, TEXT("ASSERT FAILED:Class: %s,Function:%s,"),*GetName(),*FString(__FUNCTION__))
#endif
//静态函数的报错不能直接GetName，所以特殊处理
#if !UE_BUILD_SHIPPING
#define CPP_STATIC_LOG(StaticClassName,Verbosity, Msg) \
UDebug::InternalLog(StaticClassName, FString(__FUNCTION__), Verbosity, Msg)
#define ASSERT_STATIC(Expr,StaticClassName) \
checkf(Expr, TEXT("ASSERT FAILED:Class: %s,Function:%s,"), *StaticClassName, *FString(__FUNCTION__))
#endif