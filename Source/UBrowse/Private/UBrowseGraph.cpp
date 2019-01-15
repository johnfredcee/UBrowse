
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
	constexpr int32 nodeYSpacing = 150;
	constexpr int32 nodeYStart = 50;
	constexpr int32 nodeXPos = 50;

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
	uint32 NodeY = nodeYStart;
	int32 OuterCount = Outers.Num() - 1;
	//FGraphNodeCreator<UBrowseNode> NodeBuilder(*this);
	UBrowseNode*  ThisNode; // = NodeBuilder.CreateNode(false);
	//ThisNode->SetupNode(FIntPoint(nodeXPos, NodeY), Outers[OuterCount]);
	//NodeBuilder.Finalize();
	UBrowseNode*  PrevNode = nullptr;
	for (int32 i = OuterCount; i >= 0; i--) {
		FGraphNodeCreator<UBrowseNode> NodeBuilder(*this);
		UObject *NodeObject = Outers[i];
		ThisNode = NodeBuilder.CreateNode(false);
		ThisNode->SetupNode(FIntPoint(nodeXPos, NodeY), NodeObject);
		NodeBuilder.Finalize();
		if (PrevNode != nullptr)
		{
			PrevNode->GetChildrenPin()->MakeLinkTo(ThisNode->GetParentPin());
		}
		if (!ThisNode->GetCDOPin()->bHidden)
		{
			UClass* NodeClass = Cast<UClass>(NodeObject);
			if (NodeClass)
			{
				TArray<UEdGraphNode*> Siblings;
				Siblings.Add(ThisNode);
				FIntRect Bounds = FEdGraphUtilities::CalculateApproximateNodeBoundaries(Siblings);
				UObject *NodeCDO = NodeClass->ClassDefaultObject;
				FGraphNodeCreator<UBrowseNode> CDONodeBuilder(*this);
				auto CDONode = CDONodeBuilder.CreateNode(false);
				CDONode->SetupNode(FIntPoint(nodeXPos + Bounds.Width() + 250, NodeY), NodeCDO);
				CDONodeBuilder.Finalize();
				ThisNode->GetCDOPin()->MakeLinkTo(CDONode->GetChildrenPin());
				CDONode->GetChildrenPin()->PinName = TEXT("Class");
			}
		}
		if (!ThisNode->GetOwnerPin()->bHidden)
		{
			UActorComponent *Component = Cast<UActorComponent>(NodeObject);
			if (Component)
			{
				TArray<UEdGraphNode*> Siblings;
				Siblings.Add(ThisNode);
				FIntRect Bounds = FEdGraphUtilities::CalculateApproximateNodeBoundaries(Siblings);
				AActor* NodeOwner = Component->GetOwner();
				FGraphNodeCreator<UBrowseNode> OwnerNodeBuilder(*this);
				auto OwnerNode = OwnerNodeBuilder.CreateNode(false);
				OwnerNode->SetupNode(FIntPoint(nodeXPos - Bounds.Width() - 350, NodeY), NodeOwner);
				OwnerNodeBuilder.Finalize();
				ThisNode->GetOwnerPin()->MakeLinkTo(OwnerNode->GetChildrenPin());
				OwnerNode->GetChildrenPin()->PinName = TEXT("Actor");
			}
		}
		if (!ThisNode->GetGeneratedByPin()->bHidden)
		{
			UBlueprintGeneratedClass *NodeBPClass = Cast<UBlueprintGeneratedClass>(NodeObject);
			if (NodeBPClass)
			{
				TArray<UEdGraphNode*> Siblings;
				Siblings.Add(ThisNode);
				FIntRect Bounds = FEdGraphUtilities::CalculateApproximateNodeBoundaries(Siblings);
				UObject* BP= NodeBPClass->ClassGeneratedBy;
				FGraphNodeCreator<UBrowseNode> BPNodeBuilder(*this);
				auto BPNode = BPNodeBuilder.CreateNode(false);
				BPNode->SetupNode(FIntPoint(nodeXPos - Bounds.Width() - 350, NodeY), BP);
				BPNodeBuilder.Finalize();
				ThisNode->GetGeneratedByPin()->MakeLinkTo(BPNode->GetChildrenPin());
				BPNode->GetChildrenPin()->PinName = TEXT("GeneratedBy");
			}
		}
		if (!ThisNode->GetGeneratesPin()->bHidden)
		{
			UBlueprint* BP = Cast<UBlueprint>(NodeObject);
			if (BP)
			{
				TArray<UEdGraphNode*> Siblings;
				Siblings.Add(ThisNode);
				FIntRect Bounds = FEdGraphUtilities::CalculateApproximateNodeBoundaries(Siblings);
				UClass* GeneratedClass = BP->GeneratedClass.Get();
				FGraphNodeCreator<UBrowseNode> GeneratedNodeBuilder(*this);
				auto GeneratedNode = GeneratedNodeBuilder.CreateNode(false);
				GeneratedNode->SetupNode(FIntPoint(nodeXPos + Bounds.Width() + 250, NodeY), GeneratedClass);
				GeneratedNodeBuilder.Finalize();
				ThisNode->GetGeneratesPin()->MakeLinkTo(GeneratedNode->GetChildrenPin());
				GeneratedNode->GetChildrenPin()->PinName = TEXT("Generates");
			}
		}

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
		NodeY += nodeYSpacing;
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