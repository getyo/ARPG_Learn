#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"
#include "GraphEditorModule.h" 

class FToolBarBuilder;
class FMenuBuilder;


class FUNTLinkModule : public IModuleInterface
{
public:
	/** IModuleInterface实现 */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:  
	// 内容浏览器和蓝图编辑器菜单扩展相关方法
	void AddContentBrowserMenuExtensions();
	void RemoveContentBrowserMenuExtensions();

	// 为内容浏览器和蓝图编辑器菜单创建扩展的方法
	TSharedRef<FExtender> OnExtendContentBrowserAssetContextMenu(const TArray<FAssetData>& SelectedAssets);
	TSharedRef<FExtender> OnExtendContentBrowserFolderContextMenu(const TArray<FString>& SelectedFolders);

	// 存储菜单扩展句柄
	FDelegateHandle MyContentBrowserExtenderDelegateHandle;
	FDelegateHandle MyFolderContentBrowserExtenderDelegateHandle;


	// 蓝图节点右键菜单扩展相关方法
	void AddGraphEditorMenuExtension();
	void RemoveGraphEditorMenuExtension();

	TSharedRef<FExtender> OnExtendGraphEditorContextMenu(const TSharedRef<FUICommandList> CommandList,
														 const UEdGraph* Graph,
														 const UEdGraphNode* Node,
														 const UEdGraphPin* Pin,
														 bool bIsConst);

	void OnCustomActionClicked();
	TSharedPtr<SGraphEditor> GetActiveGraphEditor() const;
	void OnAICustomActionClicked();


	// 存储 GraphEditor 菜单扩展句柄
	FDelegateHandle GraphEditorExtenderHandle;
	
	
};
