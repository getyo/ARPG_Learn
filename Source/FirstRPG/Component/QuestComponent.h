#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NativeGameplayTags.h"
#include "FirstRPG/QuestSystem/QuestionSubsystem.h"
#include "QuestComponent.generated.h"

USTRUCT(BlueprintType)
struct FS_ActorHoldQuestTargets
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TArray<FS_ActorHoldTarget> Targets;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIRSTRPG_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetFocusedQuest(const FGameplayTag & QuestTag)
	{
		this->FocusedQuestTag = QuestTag;
	}
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Quest")
	FGameplayTag QuestIdentifyTag = FGameplayTag::EmptyTag;
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Quest")
	TMap<FGameplayTag,FS_ActorHoldQuestTargets> RelativeQuestMap;
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Quest")
	FGameplayTag FocusedQuestTag = FGameplayTag::EmptyTag;
	// Called when the game starts
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable,Category = "Quest")
	void SendTargetFinish(const FGameplayTag& ActionTag, int Cnt, AController * TargetInstigator);

private:
	AThirdPersonPlayerController * PlayerController = nullptr;
	UQuestionSubsystem * QuestSubsystem = nullptr;
};
