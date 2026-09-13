// CharacterStatusComponent.h
// 角色状态组件：只负责“数据 + 规则”，表现（UI/特效/音效）通过事件交给蓝图。
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Delegates/DelegateCombinations.h"
#include "CharacterStatusComponent.generated.h"

/** 数值变化（当前值 / 上限），用于血条、耐力条、经验条 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCharacterStatusValueChanged, float, Current, float, Max);

/** 等级变化 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterStatusLevelChanged, int32, NewLevel);

/** 状态类变化（死亡/复活等） */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterStatusStateChanged);

UCLASS(ClassGroup = (Character), meta = (BlueprintSpawnableComponent))
class FIRSTRPG_API UCharacterStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterStatusComponent();

	// ================= 数据 =================

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Status|Health")
	float CurHealth = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Status|Health")
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Status|Stamina")
	float CurStamina = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Status|Stamina")
	float MaxStamina = 100.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Status|Stamina")
	bool IsStaminaRunOut = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Status|XP")
	int32 Level = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Status|XP")
	bool IsLevelUp = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Status|XP")
	float CurXP = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Status|XP")
	float MaxXP = 100.f;
	/** 每次升级提升的经验上限 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Status|XP")
	float MaxXPIncrement = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Status")
	bool IsDead = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Status")
	bool CanBeKilled = true;

	// ================= 事件（蓝图接表现：UI / 特效 / 音效） =================

	UPROPERTY(BlueprintAssignable, Category = "Character Status|Events")
	FCharacterStatusValueChanged OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "Character Status|Events")
	FCharacterStatusValueChanged OnStaminaChanged;
	UPROPERTY(BlueprintAssignable, Category = "Character Status|Events")
	FCharacterStatusValueChanged OnXPChanged;
	UPROPERTY(BlueprintAssignable, Category = "Character Status|Events")
	FCharacterStatusLevelChanged OnLevelChanged;
	UPROPERTY(BlueprintAssignable, Category = "Character Status|Events")
	FCharacterStatusStateChanged OnDeath;
	UPROPERTY(BlueprintAssignable, Category = "Character Status|Events")
	FCharacterStatusStateChanged OnRevive;
	UPROPERTY(BlueprintAssignable, Category = "Character Status|Events")
	FCharacterStatusStateChanged OnStaminaRunOut;

	// ================= 规则 =================
	// 说明：与蓝图同名同参，蓝图子类里已有的 override 继续生效；它们调用 Parent 时落到这里的实现。
	// 返回值与原蓝图一致：DecreaseHealth 返回 IsDead，DecreaseStamina 返回是否耐力耗尽，IncreaseXP 返回是否升级。

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character Status|Health")
	void DecreaseHealth(double Damage, UPARAM(DisplayName = "Is Dead") bool& OutIsDead);
	virtual void DecreaseHealth_Implementation(double Damage, bool& OutIsDead);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character Status|Health")
	void IncreaseHealth(double Heal);
	virtual void IncreaseHealth_Implementation(double Heal);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character Status|Health")
	void PromoteMaxHealth(double MaxHealthIncrement);
	virtual void PromoteMaxHealth_Implementation(double MaxHealthIncrement);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character Status|Stamina")
	void DecreaseStamina(double StaminaDecrement, UPARAM(DisplayName = "Is Stamina Run Out") bool& OutIsStaminaRunOut);
	virtual void DecreaseStamina_Implementation(double StaminaDecrement, bool& OutIsStaminaRunOut);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character Status|Stamina")
	void IncreaseStamina(double StaminaIncrement);
	virtual void IncreaseStamina_Implementation(double StaminaIncrement);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character Status|Stamina")
	void PromoteMaxStamina(double MaxStaminaIncrement);
	virtual void PromoteMaxStamina_Implementation(double MaxStaminaIncrement);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character Status|XP")
	bool IncreaseXP(double XpIncrement);
	virtual bool IncreaseXP_Implementation(double XpIncrement);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character Status|XP")
	void IncreaseLevel(int64 LevelIncrement);
	virtual void IncreaseLevel_Implementation(int64 LevelIncrement);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character Status")
	bool CharacterDie();
	virtual bool CharacterDie_Implementation();
	/** 直接向蓝图广播一次当前所有数值（供 UI 初始化/刷新用） */
	UFUNCTION(BlueprintCallable, Category = "Character Status|Events")
	void BroadcastStatusValues();

	/** 数据变化后要求蓝图刷新表现（UI/特效）——蓝图里实现它即可，C++ 不碰 UI */
	UFUNCTION(BlueprintImplementableEvent, Category = "Character Status|Events")
	void UpdateStatusUI();

protected:
	virtual void BeginPlay() override;

	/** 数值钳制到 [0, Max] */
	void ClampValues();
};
