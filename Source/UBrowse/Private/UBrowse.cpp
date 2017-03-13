// Some copyright should be here...

#include "UBrowsePrivatePCH.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "UBrowseStyle.h"
#include "UBrowseCommands.h"
#include "LevelEditor.h"
#include "UBrowseNode.h"
#include "SUBrowser.h"

static const FName UBrowseTabName("UBrowse");

#define LOCTEXT_NAMESPACE "FUBrowseModule"

class FGraphPanelNodeFactory_UBrowse : public FGraphPanelNodeFactory
{
	virtual TSharedPtr< class SGraphNode > CreateNode(UEdGraphNode* Node) const override
	{
		if (UBrowseNode* DependencyNode = Cast< UBrowseNode >(Node))
		{
			return SNew(SUBrowseNode, DependencyNode);
		}

		return NULL;
	}
};

void FUBrowseModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FUBrowseStyle::Initialize();
	FUBrowseStyle::ReloadTextures();
	
	FUBrowseCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FUBrowseCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FUBrowseModule::PluginButtonClicked),
		FCanExecuteAction());

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	m_NodeFactory = MakeShareable(new FGraphPanelNodeFactory_UBrowse());
	FEdGraphUtilities::RegisterVisualNodeFactory(m_NodeFactory);

	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands,
			FMenuExtensionDelegate::CreateRaw(this, &FUBrowseModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}

	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Content", EExtensionHook::After, PluginCommands,
			FToolBarExtensionDelegate::CreateRaw(this, &FUBrowseModule::AddToolbarExtension));

		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(UBrowseTabName, FOnSpawnTab::CreateRaw(this, &FUBrowseModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FUBrowseTabTitle", "UBrowse"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

}

void FUBrowseModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FUBrowseStyle::Shutdown();
	FUBrowseCommands::Unregister();
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(UBrowseTabName);
	if (m_NodeFactory.IsValid()) {
		FEdGraphUtilities::UnregisterVisualNodeFactory(m_NodeFactory);
		m_NodeFactory.Reset();
	}
}

TSharedRef<SDockTab> FUBrowseModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	const TSharedRef<SDockTab>  DockTab = SNew(SDockTab).TabRole(ETabRole::MajorTab);
	TSharedRef<SUBrowser> Browser = SNew(SUBrowser);
	DockTab->SetContent(Browser);
	Browser->RefreshList();
	return DockTab;
}

void FUBrowseModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(UBrowseTabName);
}

void FUBrowseModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FUBrowseCommands::Get().OpenPluginWindow);
}

void FUBrowseModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FUBrowseCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUBrowseModule, UBrowse)