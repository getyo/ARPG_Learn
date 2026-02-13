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
#endif
