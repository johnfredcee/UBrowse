// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

/*=============================================================================
SoundCueGraphSchema.cpp
=============================================================================*/

#include "UBrowseSchema.h"
#include "Widgets/Input/SButton.h"
#include "ToolMenu.h"
#include "EdGraph/EdGraphNode.h"
#include "GraphEditorActions.h"
#include "GraphEditor.h"
#include "ConnectionDrawingPolicy.h"
#include "UBrowseNode.h"
#include "UBrowseEditorCommands.h"

#define LOCTEXT_NAMESPACE "UBrowseSchema"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
class SUBrowseContextMenu : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SUBrowseContextMenu) {}
	SLATE_END_ARGS()

		/**
		* Construct the widget
		*
		* @param InArgs   Declaration from which to construct the widget.
		*/
	void Construct(const FArguments& InArgs)
	{

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
			.ToolTipText(LOCTEXT("MemersButtonToolTip", "Show members graph."))
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
	if ((Node != nullptr) && (Node->GetUObject() != nullptr) && (Node->GetUObject()->IsAsset()))
	{
		FToolMenuSection& Section = Menu->AddSection("UBrowseGraphSchemaNodeActions", LOCTEXT("UBrowseActionsMenuHeader", "UBrowse Node Actions"));
		Section.AddMenuEntry(
			"MarkDependentCompilableAssetsDirty",
			LOCTEXT("MarkDependentCompilableAssetsDirtyLabel", "Mark dependent compilable assets dirty"),
			LOCTEXT("MarkDependentCompilableAssetsDirtyToolTip", "Finds all niagara assets which depend on this asset either directly or indirectly,\n and marks them dirty so they can be saved with the latest version."),
			FSlateIcon(),
			FUIAction(FExecuteAction::CreateStatic(&UBrowseSchema::OpenNodeAsset, Node->GetUObject())));		
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
#undef LOCTEXT_NAMESPACE