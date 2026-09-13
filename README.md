  这个仓库作为我做游戏的第一个实验性项目而存在，主要参考了这位博主的视频：https://www.youtube.com/watch?v=FNTyIWkv5k8&list=PLiSlOaRBfgkcPAhYpGps16PT_9f28amXi

  但是实际来讲，我和他做得东西只有基本目标相同，我添加了动作优先级管理，霸体系统等一系列东西，其中一些内容也不是用蓝图而是C++实现的。
  同时也为之后的扩展留好了接口，按照这位博主的思路我做到一般就发现继续开发和维护极为困难，于是进行了重构，25.12.27，即使视频内容还未过半，
  两个人做的东西已经很不一样了，这位博主出于基本教学而做得内容对于大多数有自驱力的人来讲是有很多扩展空间的，当然，这是我见过最好的教程，没有之一，还请不要误解我的意思。

至于具体的文档，在`Document`文件夹中，不过也不是全部代码都有说明，只是一部分架构比较复杂的C++代码我会有所记录方便日后继续开发。


---

## 仓库内容与忽略规则

本仓库只入库「我自己做的东西」：**代码、项目配置、蓝图/动画/状态机/关卡/UI/数据资产、文档**；下载或导入的第三方与模板资源、素材本体、引擎生成物一律不入。

### 入库

- 代码：`Source/`
- 项目配置：`Config/`、`FirstRPG.uproject`、`.gitignore`、`.editorconfig`、`.vsconfig`
- 关卡本身：`Content/Levels/`
- 自己做的资产：`Content/Player`、`Content/NPC`、`Content/QusetionSystem`、`Content/EquipmentSystem`、`Content/Item`、`Content/Component`、`Content/MountSystem`、`Content/Dialogue`、`Content/Interfaces`
- UI：`Content/UI/EquipmentUI`、`Content/UI/PlayerInGameUI`（字体不入）
- 宝箱蓝图（编辑过的）：`Content/WorldEntity/Blueprints`
- 角色动画与状态机：`Content/Characters/RPG_Character/Animation`
- 插件源码：`Plugins/`（其 `Binaries/`、`Intermediate/` 不入）
- 文档：`Document/`

### 不入

- 第三方 / 模板：`Content/Fab`、`Content/StarterContent`、`Content/LevelPrototyping`、`Content/Characters/Mannequins`、`Content/Characters/Mannequin_UE4`、`Content/Collections`、`Content/Developers`
- 别人资源的本体：`Content/WorldEntity/Asset`、`Content/Characters/RPG_Character`（除 `Animation` 外的模型/材质/贴图/音效）、`Content/UI/Font`
- 原始素材：`*.fbx *.psd *.tga *.wav *.mp3 *.mp4 *.mov *.blend *.max *.ztl *.spp`
- 引擎/编辑器生成物：`Binaries/ Intermediate/ Saved/ DerivedDataCache/ .vs/ .idea/ *.sln*`、`*_BuiltData.uasset`
- UE 外部 Actor/Object 数据：`Content/__ExternalActors__`、`Content/__ExternalObjects__`
- 同步/临时文件：`*baiduyun*`、`*.tmp`、`*.bak` 等

### 维护注意

`.gitignore` 是**白名单式**：先忽略一切，再逐目录放行。**以后新增一个"自己做"的目录，必须在 `.gitignore` 里补一条 `!/Content/你的目录/`**，否则不会被跟踪。

首次入库后如需调整（例如某个资源要入/不入），用 `git add -f <路径>` 强制加入，或用 `git rm --cached <路径>` 停止跟踪（后者不会删本地文件）。

---

## CHANGELOG

### 2026-09-13

- **存档支持任务进度**：新增 `FQuestSaveData`（`ActiveQuests` / `FocusedQuest` / 各条件剩余数），挂在 `FPlayerSaveData` 下；读档恢复当前阶段并回写计数（中途进度不会退回阶段开头）。
- **存档结构版本**：`USaveGameContainer.SaveVersion`（当前 1，旧档读入时给出 Warning）。
- **修复**：`CharacterID` / `ItemID` 改为 `UPROPERTY`（此前不随关卡序列化，运行时为空串 → 所有对象挤在同一个 key 上互相覆盖）。
- **修复**：注销由"Map 置空"改为"移出注册表 + 死亡集合"，`SaveGame` 据此写 `Dead`；`SpawnActors` 生成失败不再覆盖已有映射；读档回填跳过死亡/空 key 条目。
- **修复**：角色存档补写 `CharacterClass` 与位置（对齐道具侧）；装备组件（`UEquipmentComponent`）改为可选依赖，马/象等无装备组件的角色不再整段跳过。
- **修复**：任务条件对象被 GC 回收导致存档崩溃（`UQuestionSubsystem::_QuestInfos` 加 `UPROPERTY`，解引用统一用 `IsValid`）。
- **修复**：蓝图侧"重建存档结构体"导致的字段静默丢失（`BP_PlayerControler.SavePlayerData` 的 `MakePlayerSaveData`、`BP_AICharacter.SaveCharacterData` 的 Return 节点接线）。
- **修复**：对话系统取阶段对话由下标访问改为 `Find/FindRef` + 判空；找不到对话时按级别记录日志（缺任务 Warning / 缺阶段 Verbose）。
- **文档**：`Document/` 五篇（存档系统、任务系统、对话系统、角色系统、角色组件）正文更新为当前状态。
- **仓库**：`.gitignore` 改为白名单式（只入库代码、项目配置与自制资产）；新增本节 CHANGELOG；初始化 git（分支 `ue58`）并连接 `origin`。
