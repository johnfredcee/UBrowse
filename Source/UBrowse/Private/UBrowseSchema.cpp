// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

/*=============================================================================
SoundCueGraphSchema.cpp
=============================================================================*/

#include "UBrowsePrivatePCH.h"
#include "UnrealEd.h"
#include "EdGraph/EdGraphNode.h"
#include "GraphEditorActions.h"
#include "GraphEditor.h"
#include "UBrowseSchema.h"

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

UBrowseSchema::UBrowseSchema(FObjectInitializer const &Args)
{

}

void UBrowseSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	const FText Name = LOCTEXT("ChangeUBrowseMode", "Change UBrowse Mode");
	const FText ToolTip = LOCTEXT("ChangeUBrowseModeTooltip", "Change viewing mode of UBrowse Graph");

	TSharedPtr<FSoundClassGraphSchemaAction_NewNode> NewAction(new FSoundClassGraphSchemaAction_NewNode(FText::GetEmpty(), Name, ToolTip.ToString(), 0));

	ContextMenuBuilder.AddAction(NewAction);
}


void UBrowseSchema::GetContextMenuActions(const UEdGraph* CurrentGraph, const UEdGraphNode* InGraphNode, const UEdGraphPin* InGraphPin, class FMenuBuilder* MenuBuilder, bool bIsDebugging) const
{
	if (InGraphNode)
	{
		MenuBuilder->BeginSection("UBrowseGraphSchemaNodeActions", LOCTEXT("UBrowseActionsMenuHeader", "UBrowse Node Actions"));
		{
			MenuBuilder->AddWidget(SNew(SUBrowseContextMenu), LOCTEXT("UBrowseGraphOptions", "Graph Options"));
		}
		MenuBuilder->EndSection();
	}
}


#undef LOCTEXT_NAMESPACE