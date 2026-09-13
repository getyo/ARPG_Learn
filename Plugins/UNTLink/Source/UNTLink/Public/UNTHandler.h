#pragma once

#include "CoreMinimal.h"
#include "UNTHandler.generated.h"

UCLASS()
class UNTLINK_API UUNTHandler : public UObject
{
    GENERATED_BODY()

public:
    // 主处理函数
    void HandleMessage(const FString& Message);
};
