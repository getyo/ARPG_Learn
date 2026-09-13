#include "UNTLink.h"

#include "BlueprintEditorModule.h"
#include "HTTPManager.h"
#include "Editor/EditorEngine.h"
#include "EdGraph/EdGraphNode.h"
#include "EdGraphUtilities.h"
#include "Editor/EditorEngine.h"
// #include "WebSocketManager.h"
#include "Dom/JsonValue.h"
#include "BlueprintEditor.h"
#include "GraphEditor.h"
#include "Editor.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Framework/Application/SlateApplication.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"


#define LOCTEXT_NAMESPACE "FUNTLinkModule"

DEFINE_LOG_CATEGORY_STATIC(LogMyModule, Log, All);


void FUNTLinkModule::StartupModule()
{
	// 初始化 WebSocket 管理器
	// FWebSocketManager::Get().Initialize(); 
	// ExtensionInstance.Install(); // 安装扩展


	//使用HTTP交互
	FHTTPManager::Get().Initialize();


	// 注册内容浏览器菜单扩展
	AddContentBrowserMenuExtensions();

	// 注册蓝图节点右键菜单扩展
	AddGraphEditorMenuExtension();
}

void FUNTLinkModule::ShutdownModule()
{
	// 关闭并清理 WebSocket 管理器
	// FWebSocketManager::Get().Shutdown();

	// ExtensionInstance.Uninstall(); // 卸载扩展


	FHTTPManager::Get().Shutdown(); 
	// 移除内容浏览器菜单扩展
	RemoveContentBrowserMenuExtensions();
	// 移除蓝图节点右键菜单扩展
	RemoveGraphEditorMenuExtension();
}


void FUNTLinkModule::AddContentBrowserMenuExtensions()
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(
		"ContentBrowser");
	TArray<FContentBrowserMenuExtender_SelectedAssets>& MenuExtenders = ContentBrowserModule.
		GetAllAssetViewContextMenuExtenders();
	MenuExtenders.Add(
		FContentBrowserMenuExtender_SelectedAssets::CreateRaw(
			this, &FUNTLinkModule::OnExtendContentBrowserAssetContextMenu));
	MyContentBrowserExtenderDelegateHandle = MenuExtenders.Last().GetHandle();

	// 文件夹上下文菜单扩展
	TArray<FContentBrowserMenuExtender_SelectedPaths>& FolderMenuExtenders = ContentBrowserModule.
		GetAllPathViewContextMenuExtenders();
	FolderMenuExtenders.Add(
		FContentBrowserMenuExtender_SelectedPaths::CreateRaw(
			this, &FUNTLinkModule::OnExtendContentBrowserFolderContextMenu));
	MyFolderContentBrowserExtenderDelegateHandle = FolderMenuExtenders.Last().GetHandle();
}

void FUNTLinkModule::RemoveContentBrowserMenuExtensions()
{
	if (FModuleManager::Get().IsModuleLoaded("ContentBrowser"))
	{
		FContentBrowserModule& ContentBrowserModule = FModuleManager::GetModuleChecked<FContentBrowserModule>(
			"ContentBrowser");
		ContentBrowserModule.GetAllAssetViewContextMenuExtenders().RemoveAll(
			[this](const FContentBrowserMenuExtender_SelectedAssets& Extender)
			{
				return Extender.GetHandle() == MyContentBrowserExtenderDelegateHandle;
			});

		// 移除文件夹上下文菜单扩展
		ContentBrowserModule.GetAllPathViewContextMenuExtenders().RemoveAll(
			[this](const FContentBrowserMenuExtender_SelectedPaths& Extender)
			{
				return Extender.GetHandle() == MyFolderContentBrowserExtenderDelegateHandle;
			});
	}
}

TSharedRef<FExtender> FUNTLinkModule::OnExtendContentBrowserAssetContextMenu(const TArray<FAssetData>& SelectedAssets)
{
	TSharedRef<FExtender> Extender = MakeShareable(new FExtender());
	Extender->AddMenuExtension(
		"CommonAssetActions", // 尝试使用"AssetContext"作为插入点
		EExtensionHook::Before, // 在现有菜单项之后添加
		nullptr,
		FMenuExtensionDelegate::CreateLambda([this, SelectedAssets](FMenuBuilder& MenuBuilder)
		{
			// 只有当选中至少一个资产时才显示此菜单项
			if (SelectedAssets.Num() > 0)
			{
				MenuBuilder.BeginSection(NAME_None, LOCTEXT("SectionName", "虚幻盒子"));

				MenuBuilder.AddMenuEntry(
					LOCTEXT("ActionLocKeyAsset", "导入到虚幻盒子资产库"),
					LOCTEXT("ActionTooltipLockKeyAsset", "将选中的文件夹及其内容导入到虚幻盒子中（虚幻盒子需要处于打开状态）"),
					FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Import"), // 使用一个合适的图标
					FUIAction(FExecuteAction::CreateLambda([SelectedAssets]()
					{
						// 创建一个JSON对象来存储资产路径信息
						TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

						// 设置动作类型为导入资产
						JsonObject->SetStringField(TEXT("action"), TEXT("import-assets"));


						// 用于存储所有选中资产的软路径数组
						TArray<TSharedPtr<FJsonValue>> AssetsArray;

						for (const FAssetData& AssetData : SelectedAssets)
						{
							FString AssetRealPath;
							// 尝试将长路径名转换为文件系统上的真实路径
							if (FPackageName::TryConvertLongPackageNameToFilename(
								AssetData.PackageName.ToString(), AssetRealPath))
							{
								AssetRealPath = FPaths::ConvertRelativePathToFull(AssetRealPath);
								// 判断资产类型，如果是关卡，则使用.umap扩展名
								FString FileExtension;
								if (AssetData.AssetClassPath == FTopLevelAssetPath(TEXT("/Script/Engine.World")))
								{
									FileExtension = TEXT(".umap");
								}
								else
								{
									// 对于其他类型的资产，默认使用.uasset扩展名
									FileExtension = TEXT(".uasset");
								}

								// 将正确的文件扩展名添加到路径
								AssetRealPath += FileExtension;
								AssetsArray.Add(MakeShareable(new FJsonValueString(AssetRealPath)));
							}
						}


						// 将资产数组添加到JSON对象
						JsonObject->SetArrayField(TEXT("assets"), AssetsArray);

						// 将JSON对象序列化为字符串
						FString OutputString;
						TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
						if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
						{
							// 使用FWebSocketManager发送序列化后的JSON字符串
							// FWebSocketManager::Get().SendData(OutputString);

							FHTTPManager::Get().SendData(OutputString);
						}
					}))
				);
			}
		})
	);

	return Extender;
}

// 声明一个新的方法来扩展文件夹的上下文菜单
TSharedRef<FExtender> FUNTLinkModule::OnExtendContentBrowserFolderContextMenu(const TArray<FString>& SelectedFolders)
{
	TSharedRef<FExtender> Extender = MakeShareable(new FExtender());
	Extender->AddMenuExtension(
		"PathViewFolderOptions", // 插入点为 NewFolder
		EExtensionHook::Before, // 在现有菜单项之后添加
		nullptr,
		FMenuExtensionDelegate::CreateLambda([this, SelectedFolders](FMenuBuilder& MenuBuilder)
		{
			// 只有当选中至少一个文件夹时才显示此菜单项
			if (SelectedFolders.Num() > 0)
			{
				MenuBuilder.BeginSection(NAME_None, LOCTEXT("SectionName", "虚幻盒子"));


				MenuBuilder.AddMenuEntry(
					LOCTEXT("ActionLocKeyFolder", "导入到虚幻盒子资产库"),
					LOCTEXT("ActionTooltipLockKeyFolder", "将选中的文件夹及其内容导入到虚幻盒子中（虚幻盒子需要处于打开状态）"),
					FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Import"), // 使用一个合适的图标
					FUIAction(FExecuteAction::CreateLambda([SelectedFolders]()
					{
						// 创建一个JSON对象来存储文件夹路径信息
						TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

						// 设置动作类型为导入文件夹
						JsonObject->SetStringField(TEXT("action"), TEXT("import-assets"));

						// 用于存储所有选中文件夹的真实路径数组
						TArray<TSharedPtr<FJsonValue>> FoldersArray;

						for (const FString& FolderPath : SelectedFolders)
						{
							FString RealPath;
							FPackageName::TryConvertLongPackageNameToFilename(FolderPath, RealPath);
							// 注意：这里我们假定路径是指向一个目录，所以不需要添加文件扩展名
							RealPath = FPaths::ConvertRelativePathToFull(RealPath);
							// 将文件夹的真实路径添加到数组
							FoldersArray.Add(MakeShareable(new FJsonValueString(RealPath)));
						}

						// 将文件夹数组添加到JSON对象
						JsonObject->SetArrayField(TEXT("assets"), FoldersArray);

						// 将JSON对象序列化为字符串
						FString OutputString;
						TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
						if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
						{
							// 使用FWebSocketManager发送序列化后的JSON字符串
							FHTTPManager::Get().SendData(OutputString);
						}
					}))
				);
			}
		})
	);

	return Extender;
}



void FUNTLinkModule::AddGraphEditorMenuExtension()
{
    FGraphEditorModule& GraphEditorModule = FModuleManager::LoadModuleChecked<FGraphEditorModule>("GraphEditor");
    TArray<FGraphEditorModule::FGraphEditorMenuExtender_SelectedNode>& MenuExtenders = GraphEditorModule.GetAllGraphEditorContextMenuExtender();

    MenuExtenders.Add(FGraphEditorModule::FGraphEditorMenuExtender_SelectedNode::CreateRaw(this, &FUNTLinkModule::OnExtendGraphEditorContextMenu));
    GraphEditorExtenderHandle = MenuExtenders.Last().GetHandle();
}



void FUNTLinkModule::RemoveGraphEditorMenuExtension()
{
    FGraphEditorModule& GraphEditorModule = FModuleManager::GetModuleChecked<FGraphEditorModule>("GraphEditor");
    TArray<FGraphEditorModule::FGraphEditorMenuExtender_SelectedNode>& MenuExtenders = GraphEditorModule.GetAllGraphEditorContextMenuExtender();

    MenuExtenders.RemoveAll([this](const FGraphEditorModule::FGraphEditorMenuExtender_SelectedNode& Extender)
    {
        return Extender.GetHandle() == GraphEditorExtenderHandle;
    });
}



TSharedRef<FExtender> FUNTLinkModule::OnExtendGraphEditorContextMenu(
    const TSharedRef<FUICommandList> CommandList,
    const UEdGraph* Graph,
    const UEdGraphNode* Node,
    const UEdGraphPin* Pin,
    bool bIsConst)
{
    TSharedRef<FExtender> MenuExtender(new FExtender());

    MenuExtender->AddMenuExtension(
        "EdGraphSchemaNodeActions",
        EExtensionHook::After,
        CommandList,
        FMenuExtensionDelegate::CreateLambda([this](FMenuBuilder& MenuBuilder)
        {
            // 虚幻盒子部分
            MenuBuilder.BeginSection("UNTLinkSection", LOCTEXT("UNTLinkSection", "虚幻盒子"));

            // 保存节点按钮
            MenuBuilder.AddMenuEntry(
                LOCTEXT("SaveNodeToUNTLink", "保存节点到虚幻盒子"),
                LOCTEXT("SaveNodeToUNTLinkTooltip", "将选中的节点保存到虚幻盒子蓝图库（虚幻盒子需要处于打开状态）"),
                FSlateIcon(),
                FUIAction(FExecuteAction::CreateRaw(this, &FUNTLinkModule::OnCustomActionClicked))
            );

            // AI解析节点按钮
            MenuBuilder.AddMenuEntry(
                LOCTEXT("ParseNodeWithAI", "AI解析节点"),
                LOCTEXT("ParseNodeWithAITooltip", "使用AI分析当前节点内容（虚幻盒子需要处于打开状态）"),
                FSlateIcon(),
                FUIAction(FExecuteAction::CreateRaw(this, &FUNTLinkModule::OnAICustomActionClicked))
            );

            MenuBuilder.EndSection();
        })
    );

    return MenuExtender;
}




void FUNTLinkModule::OnCustomActionClicked()
{
    TSharedPtr<SGraphEditor> GraphEditor = GetActiveGraphEditor();

    if (GraphEditor.IsValid())
    {
        const FGraphPanelSelectionSet SelectedNodes = GraphEditor->GetSelectedNodes();

        if (SelectedNodes.Num() > 0)
        {
            // 导出节点到文本
            FString ExportedText;
            FEdGraphUtilities::ExportNodesToText(SelectedNodes, ExportedText);

            // 将导出的节点文本复制到剪贴板
            FPlatformApplicationMisc::ClipboardCopy(*ExportedText);

            // UE_LOG(LogMyModule, Log, TEXT("节点已复制到剪贴板"));

            // 创建 JSON 对象并通过 WebSocket 发送
            TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
            JsonObject->SetStringField(TEXT("action"), TEXT("save-node"));
            JsonObject->SetStringField(TEXT("nodeData"), ExportedText);

            FString OutputString;
            TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
            if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
            {
             
            	FHTTPManager::Get().SendData(OutputString);
                UE_LOG(LogMyModule, Log, TEXT("节点数据已通过 WebSocket 发送"));
            }
            else
            {
                UE_LOG(LogMyModule, Warning, TEXT("JSON 序列化失败"));
            }
        }
        else
        {
            UE_LOG(LogMyModule, Warning, TEXT("未选中任何节点"));
        }
    }
    else
    {
        UE_LOG(LogMyModule, Warning, TEXT("无法获取当前的 GraphEditor"));
    }
}


TSharedPtr<SGraphEditor> FUNTLinkModule::GetActiveGraphEditor() const
{
    TSharedPtr<SWidget> Widget = FSlateApplication::Get().GetKeyboardFocusedWidget();
    while (Widget.IsValid())
    {
        if (Widget->GetTypeAsString() == TEXT("SGraphEditor"))
        {
            return StaticCastSharedPtr<SGraphEditor>(Widget);
        }
        Widget = Widget->GetParentWidget();
    }
    return nullptr;
}


// AI解析
void FUNTLinkModule::OnAICustomActionClicked()
{
    TSharedPtr<SGraphEditor> GraphEditor = GetActiveGraphEditor();

    if (GraphEditor.IsValid())
    {
        const FGraphPanelSelectionSet SelectedNodes = GraphEditor->GetSelectedNodes();

        if (SelectedNodes.Num() > 0)
        {
            // 导出节点到文本以进行AI解析
            FString ExportedText;
            FEdGraphUtilities::ExportNodesToText(SelectedNodes, ExportedText);

            // 创建 JSON 对象用于AI解析
            TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
            JsonObject->SetStringField(TEXT("action"), TEXT("ai-node"));
            JsonObject->SetStringField(TEXT("nodeData"), ExportedText);

            FString OutputString;
            TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
            if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
            {
             
            	FHTTPManager::Get().SendData(OutputString);
                UE_LOG(LogMyModule, Log, TEXT("节点数据已通过 WebSocket 发送以进行AI解析"));
            }
        }
        else
        {
            UE_LOG(LogMyModule, Warning, TEXT("未选中任何节点"));
        }
    }
    else
    {
        UE_LOG(LogMyModule, Warning, TEXT("无法获取当前的 GraphEditor"));
    }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUNTLinkModule, UNTLink)
