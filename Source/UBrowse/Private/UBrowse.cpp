// Some copyright should be here...

#include "UBrowse.h"
#include "GameFramework/Actor.h"
#include "LevelEditor.h"
#include "LevelEditorMenuContext.h"
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"
#include "Interfaces/IMainFrameModule.h"
#include "Selection.h"
#include "ToolMenus.h"
#include "UBrowseStyle.h"
#include "UBrowseCommands.h"
#include "UBrowseEditorCommands.h"
#include "UBrowseNode.h"
#include "SUBrowser.h"
#include "SUBrowseNode.h"

const FName FUBrowseModule::UBrowseTabName("UBrowse");

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

	if (!IsRunningCommandlet())
	{
		FUBrowseStyle::Initialize();
		FUBrowseStyle::ReloadTextures();

		FUBrowseCommands::Register();
		FUBrowseEditorCommands::Register();

		PluginCommands = MakeShareable(new FUICommandList);

		PluginCommands->MapAction(
			FUBrowseCommands::Get().OpenPluginWindow,
			FExecuteAction::CreateRaw(this, &FUBrowseModule::OpenUBrowser),
			FCanExecuteAction());

		PluginCommands->MapAction(
			FUBrowseCommands::Get().BrowseUObject,
			FExecuteAction::CreateRaw(this, &FUBrowseModule::OpenUBrowser),
			FCanExecuteAction());

		FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

		m_NodeFactory = MakeShareable(new FGraphPanelNodeFactory_UBrowse());
		FEdGraphUtilities::RegisterVisualNodeFactory(m_NodeFactory);

		{
			TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
			MenuExtender->AddMenuExtension("Instrumentation", EExtensionHook::Before, PluginCommands,
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

	// Register content browser hook
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));	
	TArray<FContentBrowserMenuExtender_SelectedAssets>& CBAssetMenuExtenderDelegates = ContentBrowserModule.GetAllAssetViewContextMenuExtenders();
	CBAssetMenuExtenderDelegates.Add(FContentBrowserMenuExtender_SelectedAssets::CreateRaw(this, &FUBrowseModule::OnExtendContentBrowserAssetSelectionMenu));
	ContentBrowserAssetExtenderDelegateHandle = CBAssetMenuExtenderDelegates.Last().GetHandle();

	IMainFrameModule& MainFrameModule = IMainFrameModule::Get();
	if (MainFrameModule.IsWindowInitialized())
	{
		TSharedPtr<SWindow> DummyWindow;
		AddSceneOutlinerMenu(DummyWindow, false);
	}
	else
	{
		MainFrameModule.OnMainFrameCreationFinished().AddRaw(this, &FUBrowseModule::AddSceneOutlinerMenu);
	}

}

void FUBrowseModule::AddSceneOutlinerMenu(TSharedPtr<SWindow> InRootWindow, bool bIsNewProjectWindow)
{
	auto AddDynamicSection = [this](UToolMenu* ToolMenu)
	{				
		if (ULevelEditorContextMenuContext* LevelEditorMenuContext = ToolMenu->Context.FindContext<ULevelEditorContextMenuContext>())
		{
			// Use the actor under the cursor if available (e.g. right-click menu).
			// Otherwise use the first selected actor if there's one (e.g. Actor pulldown menu or outliner).
			AActor* ContextActor = LevelEditorMenuContext->HitProxyActor.Get();
			if (!ContextActor && GEditor->GetSelectedActorCount() != 0)
			{
				ContextActor = Cast<AActor>(GEditor->GetSelectedActors()->GetSelectedObject(0));
			}

			if (ContextActor)
			{
				this->CreateBrowseMenu(ToolMenu, ContextActor);
			}
		}
	};

	if (UToolMenu* ToolMenu = UToolMenus::Get()->FindMenu("LevelEditor.LevelEditorSceneOutliner.ContextMenu"))
	{
		ToolMenu->AddDynamicSection("UBrowseModuleDynamicSection", FNewToolMenuDelegate::CreateLambda(AddDynamicSection));
	}

}

void FUBrowseModule::CreateBrowseMenu(UToolMenu* ToolMenu, AActor* ContextActor)
{
		const FName UBrowseSectionName = TEXT("UBrowse");
		FToolMenuSection& Section = ToolMenu->AddSection(UBrowseSectionName, LOCTEXT("UBrowseSectionLabel", "UBrowse"));
		FToolUIAction UBrowseOpenAction;
		UBrowseOpenAction.ExecuteAction.BindLambda([this, ContextActor](const FToolMenuContext&){ this->ViewInUBrowse(ContextActor); });
		Section.AddMenuEntry(
			FName("ActorUBrowse"),
			LOCTEXT("UBrowseActor", "UBrowse"),
			TAttribute<FText>(FText::FromString("UObject Browser")),
			FSlateIcon(FUBrowseStyle::GetStyleSetName(), "UBrowse.ActionGo"), // TO DO -- need an icon
			UBrowseOpenAction);
}

void FUBrowseModule::ShutdownModule()
{
	if (!IsRunningCommandlet())
	{
		IMainFrameModule& MainFrameModule = IMainFrameModule::Get();
		MainFrameModule.OnMainFrameCreationFinished().RemoveAll(this);
		FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));	
		TArray<FContentBrowserMenuExtender_SelectedAssets>& CBAssetMenuExtenderDelegates = ContentBrowserModule.GetAllAssetViewContextMenuExtenders();
		CBAssetMenuExtenderDelegates.RemoveAll([this](const FContentBrowserMenuExtender_SelectedAssets& Delegate) { return Delegate.GetHandle() == ContentBrowserAssetExtenderDelegateHandle; });
		FUBrowseStyle::Shutdown();
		FUBrowseCommands::Unregister();
		FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(UBrowseTabName);
		if (m_NodeFactory.IsValid()) {
			FEdGraphUtilities::UnregisterVisualNodeFactory(m_NodeFactory);
			m_NodeFactory.Reset();
		}
		
	}

}

TSharedRef<SDockTab> FUBrowseModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	const TSharedPtr<SDockTab>  DockTab = SNew(SDockTab).TabRole(ETabRole::MajorTab);
	TSharedRef<SUBrowser> Browser = SNew(SUBrowser);
	DockTab->SetContent(Browser);
	Browser->RefreshList();
	return DockTab.ToSharedRef();
}

void FUBrowseModule::OpenUBrowser()
{
	FGlobalTabmanager::Get()->TryInvokeTab(UBrowseTabName);
}

void FUBrowseModule::ViewInUBrowse(const TArray<FAssetData>& SelectedAssets)
{
	TSharedPtr<SDockTab> UBrowseTab = FGlobalTabmanager::Get()->TryInvokeTab(UBrowseTabName);
	if (SelectedAssets.Num() >= 1)
	{
		TSharedRef<SUBrowser> UBrowserWidget = StaticCastSharedRef<SUBrowser>(UBrowseTab->GetContent());
		const FAssetData Asset{SelectedAssets[0]};
		UObject* ObjectToView = Asset.GetAsset();
		UBrowserWidget->ViewUObject(ObjectToView);
	}

}

void FUBrowseModule::ViewInUBrowse(UObject* ObjectToView)
{
	TSharedPtr<SDockTab> UBrowseTab = FGlobalTabmanager::Get()->TryInvokeTab(UBrowseTabName);
	TSharedRef<SUBrowser> UBrowserWidget = StaticCastSharedRef<SUBrowser>(UBrowseTab->GetContent());
	UBrowserWidget->ViewUObject(ObjectToView);
}


void FUBrowseModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FUBrowseCommands::Get().OpenPluginWindow);
}

void FUBrowseModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FUBrowseCommands::Get().OpenPluginWindow);
}

TSharedRef<FExtender> FUBrowseModule::OnExtendContentBrowserAssetSelectionMenu(const TArray<FAssetData>& SelectedAssets)
{
	TSharedRef<FExtender> Extender(new FExtender());
	
	Extender->AddMenuExtension(
		"FindInExplorer",
		EExtensionHook::After,
		nullptr,
		FMenuExtensionDelegate::CreateLambda([this, SelectedAssets](FMenuBuilder& MenuBuilder)
		{
			MenuBuilder.AddMenuEntry(
				NSLOCTEXT("UBrowse", "ShowInUBrowse_MenuLabel", "Show In UBrowse"),
				NSLOCTEXT("UBrowse", "ShowInUBrowse_Tooltip", "Open UObject in UBrowse Window"),
				FSlateIcon(FUBrowseStyle::GetStyleSetName(), "UBrowse.ActionGo"), // TODO : Need Icon
				FUIAction(FExecuteAction::CreateLambda([this, SelectedAssets](){ this->ViewInUBrowse(SelectedAssets); }))
			);
		}));

	return Extender;
}

void FUBrowseModule::CreateAssetContextMenu(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddMenuEntry(FUBrowseCommands::Get().BrowseUObject);
}


bool FUBrowseModule::IsGameModule() const
{
	return false;
}


#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUBrowseModule, UBrowse)