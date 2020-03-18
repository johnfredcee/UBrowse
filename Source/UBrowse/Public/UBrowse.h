// Some copyright should be here...

#pragma once


#include "AssetData.h"
#include "ScopedTransaction.h"
#include "EdGraph/EdGraphPin.h"
#include "EdGraph/EdGraphNode.h"
#include "GraphEditor.h"
#include "EdGraphUtilities.h"
#include "EdGraph/EdGraphNodeUtils.h"
#include "Modules/ModuleManager.h"
#include "Editor/PropertyEditor/Public/PropertyEditing.h"


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
	void PluginButtonClicked();
	
private:

	void AddToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);

	TSharedRef<SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);
	TSharedPtr<FGraphPanelNodeFactory> m_NodeFactory;
	TSharedPtr<class FUICommandList> PluginCommands;
};