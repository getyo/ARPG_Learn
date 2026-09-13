#pragma once
 
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WebSocketBlueprintLibrary.generated.h"

UCLASS()
class UNTLINK_API UWebSocketBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "WebSocket")
	static void SendWebSocketData(const FString& Data);
};
