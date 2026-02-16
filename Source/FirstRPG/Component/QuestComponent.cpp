#include "QuestComponent.h"
#include "FirstRPG/Character/Player/ThirdPersonPlayerController.h"
#include "FirstRPG/Character/GeneralCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UQuestComponent::UQuestComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}


// Called when the game starts
void UQuestComponent::BeginPlay()
{
    Super::BeginPlay();
    if (QuestIdentifyTag.IsValid()) return;
    if (auto OwnerCharacter = Cast<AGeneralCharacter>(GetOwner()))
    {
        QuestIdentifyTag = OwnerCharacter->GetCharacterTag();
    }
    // ...
	
}

void UQuestComponent::SendTargetFinish(const FGameplayTag& ActionTag, int Cnt, AController* TargetInstigator)
{
    if (!PlayerController)
        PlayerController = Cast<AThirdPersonPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (!QuestSubsystem)
    {
        UGameInstance* GI = GetWorld()->GetGameInstance();
        QuestSubsystem= GI->GetSubsystem<UQuestionSubsystem>();
    }
    for (auto &Pair : RelativeQuestMap)
    {
        for (auto &Target : Pair.Value.Targets)
        {
            if (Target.StageInt == PlayerController->GetQuestStageInt(Pair.Key))
            {
                if (ActionTag == Target.ActionTag){
                    QuestSubsystem->BroadcastFinish(FS_QuestTargetData(
                        Pair.Key,
                        Target.StageInt,
                        Target.ActionTag,
                        QuestIdentifyTag,
                        Cnt,
                        TargetInstigator,
                        nullptr
                        ));
                }
            }
        }
    }
}


// Called every frame
void UQuestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

