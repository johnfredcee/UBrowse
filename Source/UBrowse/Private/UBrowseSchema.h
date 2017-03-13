
#pragma once

#include "EdGraph/EdGraphSchema.h"
#include "UBrowseSchema.generated.h"

/** Action to switch visualisation mode of the graph **/
USTRUCT()
struct FBrowseGraphSchemaAction_BrowseMode : public FEdGraphSchemaAction
{
	GENERATED_USTRUCT_BODY();

	// Simple type info
	static FName StaticGetTypeId() { static FName Type("UBrowseGraphSchemaAction_BrowseMode"); return Type; }

	FBrowseGraphSchemaAction_BrowseMode()
		: FEdGraphSchemaAction()
	{}

	FBrowseGraphSchemaAction_BrowseMode(const FText& InNodeCategory, const FText& InMenuDesc, const FString& InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping)
	{}

	//~ Begin FEdGraphSchemaAction Interface
	virtual FName GetTypeId() const override { return StaticGetTypeId(); }
	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
	//~ End FEdGraphSchemaAction Interface

};

UCLASS(MinimalAPI)
class UBrowseSchema : public UEdGraphSchema
{
	GENERATED_UCLASS_BODY()

	// TO DO : Get Graph Display Info
	//virtual void GetGraphDisplayInformation(const UEdGraph& Graph, /*out*/ FGraphDisplayInfo& DisplayInfo) const;


	//~ Begin EdGraphSchema Interface
	/** @return     The type of graph (function vs. ubergraph) that this that TestEdGraph is. */
	//@TODO: This is too K2-specific to be included in EdGraphSchema and should be refactored
		virtual EGraphType GetGraphType(const UEdGraph* TestEdGraph) const override { return GT_Ubergraph; }

	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	//virtual void GetContextMenuActions(const UEdGraph* CurrentGraph, const UEdGraphNode* InGraphNode, const UEdGraphPin* InGraphPin, class FMenuBuilder* MenuBuilder, bool bIsDebugging) const override;
	//~ End EdGraphSchema Interface

};