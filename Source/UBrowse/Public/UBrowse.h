// Some copyright should be here...

#pragma once


#include "AssetData.h"
#include "ScopedTransaction.h"
#include "Framework/MultiBox/MultiBoxExtender.h"
#include "EdGraph/EdGraphPin.h"
#include "EdGraph/EdGraphNode.h"
#include "GraphEditor.h"
#include "EdGraphUtilities.h"
#include "EdGraph/EdGraphNodeUtils.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorDelegates.h"
#include "PropertyEditorModule.h"
#include "PropertyHandle.h"
#include "DetailWidgetRow.h"
#include "IDetailGroup.h"
#include "IDetailCustomNodeBuilder.h"
#include "IDetailPropertyRow.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "IPropertyTypeCustomization.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailCustomization.h"
#include "IDetailsView.h"
#include "IDetailRootObjectCustomization.h"
#include "Widgets/Docking/SDockTab.h"

class FBrowserObject : public IDetailCustomization
{
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<class IDetailCustomization> MakeInstance()
	{
		return MakeShareable(new FBrowserObject);
	}

	FBrowserObject() : Object(nullptr)
	{
	};

	FBrowserObject(TWeakObjectPtr<UObject> InObject) : Object(InObject)
	{
	};

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override;

	TWeakObjectPtr<UObject> Object;
};

class FToolBarBuilder;
class FMenuBuilder;

class FUBrowseModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void OpenUBrowser();

	FDelegateHandle ContentBrowserAssetExtenderDelegateHandle;
	TSharedRef<FExtender> OnExtendContentBrowserAssetSelectionMenu(const TArray<FAssetData>& SelectedAssets);
	void CreateAssetContextMenu(FMenuBuilder& MenuBuilder);
	void ViewInUBrowse(const TArray<FAssetData>& SelectedAssets);

	static const FName UBrowseTabName;

private:

	void AddToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);

	TSharedRef<SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);
	TSharedPtr<FGraphPanelNodeFactory> m_NodeFactory;
	TSharedPtr<class FUICommandList> PluginCommands;
};