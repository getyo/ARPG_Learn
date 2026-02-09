#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NativeGameplayTags.h"
#include "QuestRelativeInterface.generated.h"

//以下两个类的类名，除了前缀(U/I)以外必须一字不差
UINTERFACE(MinimalAPI, Blueprintable)
class UQuestRelativeInterface: public UInterface
{
	GENERATED_BODY()
};

//实现接口时继承I开头的类
class FIRSTRPG_API IQuestRelativeInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest")
	bool IsQuestRelative();
	virtual bool IsQuestRelative_Implementation() = 0;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest")
	TArray<FGameplayTag> GetRelativeQuest();
	virtual TArray<FGameplayTag> GetRelativeQuest_Implementation() = 0;
};