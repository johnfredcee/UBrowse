// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

/*=============================================================================
SoundCueGraphSchema.cpp
=============================================================================*/

#include "UBrowsePrivatePCH.h"
#include "UnrealEd.h"
#include "GraphEditorActions.h"
#include "GraphEditor.h"
#include "UBrowseSchema.h"

#define LOCTEXT_NAMESPACE "UBrowseSchema"

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

/*
void UBrowseSchema::GetContextMenuActions(const UEdGraph* CurrentGraph, const UEdGraphNode* InGraphNode, const UEdGraphPin* InGraphPin, class FMenuBuilder* MenuBuilder, bool bIsDebugging) const
{
	if (InGraphNode)
	{
		const UBrowseNode* UBrowseNodeNode = Cast<const UBrowseNode*>(InGraphNode);

		MenuBuilder->BeginSection("UBrowseGraphSchemaNodeActions", LOCTEXT("UBrowseActionsMenuHeader", "UBrowse Node Actions"));
		{
			MenuBuilder->AddMenuEntry(FGraphEditorCommands::Get().BreakNodeLinks);
			MenuBuilder->AddMenuEntry(FGenericCommands::Get().Properties);
		}
		MenuBuilder->EndSection();
	}
}
*/

#undef LOCTEXT_NAMESPACE