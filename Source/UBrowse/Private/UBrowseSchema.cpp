//

#include "UBrowseSchema.h"

#include "ConnectionDrawingPolicy.h"
#include "EdGraph/EdGraphNode.h"
#include "Editor.h"
#include "GraphEditor.h"
#include "GraphEditorActions.h"
#include "SlateOptMacros.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "ToolMenu.h"
#include "UBrowse.h"
#include "UBrowseEditorCommands.h"
#include "UBrowseGraph.h"
#include "UBrowseNode.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SCompoundWidget.h"


#define LOCTEXT_NAMESPACE "UBrowseSchema"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
class SUBrowseContextMenu : public SCompoundWidget
{
  public:
    // clang-format off
	SLATE_BEGIN_ARGS(SUBrowseContextMenu) {}
	SLATE_END_ARGS()
    // clang-format on

    /**
     * Construct the widget
     *
     * @param InArgs   Declaration from which to construct the widget.
     */
    void Construct(const FArguments& InArgs)
    {
        // clang-format off
		this->ChildSlot
		[
			SNew(SBorder)
			.BorderImage(FCoreStyle::Get().GetBrush("Menu.Background"))
			.Padding(FMargin(5))
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SButton)
					.Text(LOCTEXT("MembersButton", "Members"))
					.ToolTipText(LOCTEXT("MembersButtonToolTip", "Show members graph."))
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SAssignNew(PopupAnchor, SMenuAnchor)
					.Placement(MenuPlacement_MenuRight)
					.OnGetMenuContent(this, &SUBrowseContextMenu::OnGetContent)
					[
						SNew(SButton)
						.Text(LOCTEXT("InstancesButton", "List Instances"))
						.ToolTipText(LOCTEXT("InstancesTooltip", "List of Instances of this class/object."))
						.OnClicked(this, &SUBrowseContextMenu::OpenSubmenu)
					]
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SButton)
					.Text(LOCTEXT("ClassHierarchyButton", "Class Hierarchy"))
					.ToolTipText(LOCTEXT("ClassHierarchyButton", "Show Class Hierarchy Graph related to this instance/class"))
				]
			]
		];
	// clang-format ON
	}

	TSharedPtr<SMenuAnchor> PopupAnchor;
	FReply OpenSubmenu()
	{
		PopupAnchor->SetIsOpen(!PopupAnchor->IsOpen());
		return FReply::Handled();
	}

	TSharedRef<SWidget> OnGetContent() const
	{
		return SNew(SUBrowseContextMenu);
	}
};
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

UEdGraphNode* FBrowseGraphSchemaAction_BrowseMode::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode/* = true*/)
{
	/* Here be dragons */
	/* FSoundClassEditorUtilities::CreateSoundClass(ParentGraph, FromPin, Location, NewSoundClassName); */
	return NULL;
}

// Overridden connection drawing policy to use less curvy lines between nodes
class FUBrowseConnectionDrawingPolicy : public FConnectionDrawingPolicy
{
public:
	FUBrowseConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements)
		: FConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, InZoomFactor, InClippingRect, InDrawElements)
	{
	}

	virtual FVector2D ComputeSplineTangent(const FVector2D& Start, const FVector2D& End) const override
	{
		const int32 Tension = FMath::Abs<int32>(Start.X - End.X);
		return Tension * FVector2D(1.0f, 0);
	}
};

UBrowseSchema::UBrowseSchema(FObjectInitializer const &Args)
{

}

void UBrowseSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	const FText Name = LOCTEXT("ChangeUBrowseMode", "Change UBrowse Mode");
	const FText ToolTip = LOCTEXT("ChangeUBrowseModeTooltip", "Change viewing mode of UBrowse Graph");

	TSharedPtr<FBrowseGraphSchemaAction_BrowseMode> NewAction(new FBrowseGraphSchemaAction_BrowseMode(FText::GetEmpty(), Name, ToolTip, 0));

	ContextMenuBuilder.AddAction(NewAction);
}


void UBrowseSchema::GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	Super::GetContextMenuActions(Menu, Context);

	const UBrowseNode* Node = Cast<UBrowseNode>(Context->Node);
	if ((Node != nullptr) && (Node->GetUObject() != nullptr)) 
	{
		FToolMenuSection& Section = Menu->AddSection("UBrowseGraphSchemaNodeActions", LOCTEXT("UBrowseActionsMenuHeader", "UBrowse Node Actions"));
		if (Node->GetUObject()->GetClass() != nullptr)
		{
			Section.AddMenuEntry(
				"UBrowseEditAsset",
				LOCTEXT("UBrowseBrowseClassLabel", "Browse UClass"),
				LOCTEXT("UBrowseBrowseClassToolTip", "Browse the class object of this object."),
				FSlateIcon(),
				FUIAction(FExecuteAction::CreateStatic(&UBrowseSchema::BrowseClass, Node->GetUObject()->GetClass())));		
		}
		if (Node->GetUObject()->IsAsset())
		{

			Section.AddMenuEntry(
				"UBrowseEditAsset",
				LOCTEXT("UBrowseEditAssetLabel", "Edit Asset"),
				LOCTEXT("UBrowseEditAssetToolTip", "Opens asset Editor for this Asset"),
				FSlateIcon(),
				FUIAction(FExecuteAction::CreateStatic(&UBrowseSchema::OpenNodeAsset, Node->GetUObject())));		
		}
	}
};

FConnectionDrawingPolicy* UBrowseSchema::CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const
{
	return new FUBrowseConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, InZoomFactor, InClippingRect, InDrawElements);
}

void UBrowseSchema::OpenNodeAsset(const UObject* Obj)
{
	if ((Obj != nullptr) && (Obj->IsAsset()))
	{
		GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(const_cast<UObject*>(Obj));
	}
	return;
}

void UBrowseSchema::BrowseClass(UClass* ClassObj)
{
	FUBrowseModule& UBrowseModule = FModuleManager::LoadModuleChecked<FUBrowseModule>("UBrowse");
	if (ClassObj != nullptr)
	{
		UBrowseModule.ViewInUBrowse(ClassObj);	
	}
	return;
}

void UBrowseSchema::BrowseInstances(UObject* Obj)
{
	UClass* ClassObj = Obj->GetClass();
	// TO DO 
	return;
}

// void UBrowseSchema::GetGraphDisplayInformation(const UEdGraph& Graph, /*out*/ FGraphDisplayInfo& DisplayInfo) const
// {
// // 	UBrowseGraph
// // 	DisplayInfo.PlainName = FText::FromString( Graph.GetName() );
	
// // 	if (const UAnimStateConduitNode* ConduitNode = Cast<const UAnimStateConduitNode>(Graph.GetOuter()))
// // 	{
// // 		FFormatNamedArguments Args;
// // 		Args.Add(TEXT("NodeTitle"), ConduitNode->GetNodeTitle(ENodeTitleType::FullTitle) );

// // 		DisplayInfo.PlainName = FText::Format( NSLOCTEXT("Animation", "ConduitRuleGraphTitle", "{NodeTitle} (conduit rule)"), Args);
// // 	}

// // 	DisplayInfo.DisplayName = DisplayInfo.PlainName;
// // }


#undef LOCTEXT_NAMESPACE