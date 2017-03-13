
#include "UBrowsePrivatePCH.h"
#include "UObjectHash.h"
#include "UBrowseNode.h"
#include "UBrowseGraph.h"

#define LOCTEXT_NAMESPACE "UBrowseGraph"

FVector2D GetNodeSize(const SGraphEditor& GraphEditor, const UEdGraphNode* Node)
{
	FSlateRect Rect;
	if (GraphEditor.GetBoundsForNode(Node, Rect, 0.f))
	{
		return FVector2D(Rect.Right - Rect.Left, Rect.Bottom - Rect.Top);
	}

	return FVector2D(Node->NodeWidth, Node->NodeHeight);
}

UBrowseGraph::UBrowseGraph(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UBrowseGraph::RefreshGraph(UObject* pRoot)
{
	RemoveAllNodes();
	// walk from GEngine
	TArray<UObject*> Outers;
	UObject* Root = pRoot == nullptr ? GEngine : pRoot;
	UObject *Outermost = nullptr;
	Outermost = Root;
	while (Outermost != nullptr) 
	{
		Outers.Push(Outermost);
		Outermost = Outermost->GetOuter();
	}
	uint32 NodeY = 50;
	int32 OuterCount = Outers.Num() - 1;
	FGraphNodeCreator<UBrowseNode> NodeBuilder(*this);
	UBrowseNode*  ThisNode = NodeBuilder.CreateNode(false);
	ThisNode->SetupNode(FIntPoint(50, NodeY), Outers[OuterCount]);
	NodeBuilder.Finalize();
	UBrowseNode*  PrevNode = ThisNode;
	for (int32 i = OuterCount-1; i >= 0; i--) {
		NodeY += 100;
		FGraphNodeCreator<UBrowseNode> NodeBuilder(*this);
		ThisNode = NodeBuilder.CreateNode(false);
		ThisNode->SetupNode(FIntPoint(50, NodeY), Outers[i]);
		NodeBuilder.Finalize();
		PrevNode->GetChildrenPin()->MakeLinkTo(ThisNode->GetParentPin());
		/*
		TArray<UObject*> Results;
		GetObjectsWithOuter(PrevNode->GetUObject(), Results, false);
		uint32 SiblingNodeY = NodeY - 100;
		uint32 SiblingNodeX = 50;
		TArray<UEdGraphNode*> Siblings;
		Siblings.Push(PrevNode);
		for (int32 j = 0; j < Results.Num(); j++) {
			// we don't want to duplicate the node this is
			if (Results[j] == ThisNode->GetUObject())
				continue;
			// we don't want to include browser nodes in the results!
			if (Results[j]->GetClass()->IsChildOf(UEdGraphNode::StaticClass()))
				continue;
			if (Results[j]->GetClass()->IsChildOf(UEdGraphPin::StaticClass()))
				continue;
			FIntRect Bounds = FEdGraphUtilities::CalculateApproximateNodeBoundaries(Siblings);
			SiblingNodeX = Bounds.Width() + 200;
			UBrowseNode* SiblingNode = Cast<UBrowseNode>(CreateNode(UBrowseNode::StaticClass(), false));
			SiblingNode->SetupNode(FIntPoint(SiblingNodeX, SiblingNodeY), Results[j]);
			//SiblingNodeX += SiblingNode->NodeWidth;
			Siblings.Push(SiblingNode);
		}
		*/
		PrevNode = ThisNode;
	}
}


void UBrowseGraph::RemoveAllNodes()
{
	TArray< UEdGraphNode* > NodesToRemove = Nodes;
	for (int32 NodeIndex = 0; NodeIndex < NodesToRemove.Num(); ++NodeIndex)
	{
		RemoveNode(NodesToRemove[NodeIndex]);
	}
}

#undef LOCTEXT_NAMESPACE