#include "CharacterStatusComponent.h"

UCharacterStatusComponent::UCharacterStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	ClampValues();
	// 只广播数值事件；不在这里驱动 UI。
	// 原因：组件的 BeginPlay 早于 Owner 角色的 BeginPlay，此时 Owner 的控件还没创建，
	// 提前刷新会读到 None（Accessed None）。UI 的首次刷新由 Owner 建好控件后自行触发。
	OnHealthChanged.Broadcast(CurHealth, MaxHealth);
	OnStaminaChanged.Broadcast(CurStamina, MaxStamina);
	OnXPChanged.Broadcast(CurXP, MaxXP);
	OnLevelChanged.Broadcast(Level);
}

void UCharacterStatusComponent::ClampValues()
{
	CurHealth = FMath::Clamp(CurHealth, 0.f, MaxHealth);
	CurStamina = FMath::Clamp(CurStamina, 0.f, MaxStamina);
	CurXP = FMath::Clamp(CurXP, 0.f, MaxXP);
}

void UCharacterStatusComponent::BroadcastStatusValues()
{
	OnHealthChanged.Broadcast(CurHealth, MaxHealth);
	OnStaminaChanged.Broadcast(CurStamina, MaxStamina);
	OnXPChanged.Broadcast(CurXP, MaxXP);
	OnLevelChanged.Broadcast(Level);

	// 让蓝图刷新表现（蓝图里实现 UpdateStatusUI 即可，C++ 不碰 UI）
	// 注意：只由“数据变化”路径触发；BeginPlay 不调用它（见 BeginPlay 注释）。
	UpdateStatusUI();
}

// ================= 生命值 =================

void UCharacterStatusComponent::DecreaseHealth_Implementation(double Damage, bool& OutIsDead)
{
	const float Dmg = static_cast<float>(Damage);
	const float Before = CurHealth;

	if (CurHealth - Dmg > 0.f)
	{
		CurHealth = CurHealth - Dmg;
		this->IsDead = false;
	}
	else
	{
		CurHealth = 0.f;
		if (CanBeKilled)
		{
			this->IsDead = true;
		}
	}

	OnHealthChanged.Broadcast(CurHealth, MaxHealth);
	UpdateStatusUI();
	if (this->IsDead)
	{
		OnDeath.Broadcast();
	}
	OutIsDead = this->IsDead;
	UE_LOG(LogTemp, Log, TEXT("[Status] %s DecreaseHealth: Cur %.1f -> %.1f (Dmg %.1f, Max %.1f, IsDead %d)"),
		*GetOwner()->GetName(), Before, CurHealth, Dmg, MaxHealth, this->IsDead ? 1 : 0);
}

void UCharacterStatusComponent::IncreaseHealth_Implementation(double Heal)
{
	CurHealth = FMath::Min(CurHealth + static_cast<float>(Heal), MaxHealth);
	OnHealthChanged.Broadcast(CurHealth, MaxHealth);
	UpdateStatusUI();
}

void UCharacterStatusComponent::PromoteMaxHealth_Implementation(double MaxHealthIncrement)
{
	MaxHealth = MaxHealth + static_cast<float>(MaxHealthIncrement);
	OnHealthChanged.Broadcast(CurHealth, MaxHealth);
	UpdateStatusUI();
}

// ================= 耐力 =================

void UCharacterStatusComponent::DecreaseStamina_Implementation(double StaminaDecrement, bool& OutIsStaminaRunOut)
{
	const float Dec = static_cast<float>(StaminaDecrement);
	if (CurStamina - Dec > 0.f)
	{
		CurStamina = CurStamina - Dec;
		this->IsStaminaRunOut = false;
	}
	else
	{
		CurStamina = 0.f;
		this->IsStaminaRunOut = true;
	}

	OnStaminaChanged.Broadcast(CurStamina, MaxStamina);
	if (this->IsStaminaRunOut)
	{
		OnStaminaRunOut.Broadcast();
	}
	UpdateStatusUI();
	OutIsStaminaRunOut = this->IsStaminaRunOut;
}

void UCharacterStatusComponent::IncreaseStamina_Implementation(double StaminaIncrement)
{
	CurStamina = FMath::Min(CurStamina + static_cast<float>(StaminaIncrement), MaxStamina);
	IsStaminaRunOut = CurStamina <= 0.f;
	OnStaminaChanged.Broadcast(CurStamina, MaxStamina);
	UpdateStatusUI();
}

void UCharacterStatusComponent::PromoteMaxStamina_Implementation(double MaxStaminaIncrement)
{
	MaxStamina = MaxStamina + static_cast<float>(MaxStaminaIncrement);
	OnStaminaChanged.Broadcast(CurStamina, MaxStamina);
	UpdateStatusUI();
}

// ================= 经验 / 等级 =================

bool UCharacterStatusComponent::IncreaseXP_Implementation(double XpIncrement)
{
	CurXP = CurXP + static_cast<float>(XpIncrement);

	if (CurXP >= MaxXP)
	{
		// 与原蓝图行为一致：只扣一次经验上限，并升一级
		CurXP = CurXP - MaxXP;
		IncreaseLevel(1);
		IsLevelUp = true;
	}

	OnXPChanged.Broadcast(CurXP, MaxXP);
	UpdateStatusUI();
	return IsLevelUp;
}

void UCharacterStatusComponent::IncreaseLevel_Implementation(int64 LevelIncrement)
{
	Level = Level + static_cast<int32>(LevelIncrement);
	MaxXP = MaxXP + MaxXPIncrement;

	OnLevelChanged.Broadcast(Level);
	OnXPChanged.Broadcast(CurXP, MaxXP);
	UpdateStatusUI();
}

// ================= 生死 =================

bool UCharacterStatusComponent::CharacterDie_Implementation()
{
	return IsDead;
}
