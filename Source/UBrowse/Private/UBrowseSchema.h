

#pragma once

#include "EdGraph/EdGraphSchema.h"
#include "ToolMenus.h"
#include "K2Node.h"
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

	FBrowseGraphSchemaAction_BrowseMode(const FText& InNodeCategory, const FText& InMenuDesc, const FText& InToolTip, const int32 InGrouping)
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
	static void OpenNodeAsset(const UObject* Obj);


	//~ Begin EdGraphSchema Interface
	virtual EGraphType GetGraphType(const UEdGraph* TestEdGraph) const override { return GT_Ubergraph; }
	virtual	FConnectionDrawingPolicy* CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const override;
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	virtual void GetContextMenuActions(UToolMenu* Menu,  UGraphNodeContextMenuContext* Context) const override;
	//~ End EdGraphSchema Interface

};