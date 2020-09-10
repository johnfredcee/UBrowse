
#include "UBrowseGraph.h"
#include "UObject/UObjectHash.h"
#include "UBrowsePrivatePCH.h"
#include "UBrowseNode.h"

#define LOCTEXT_NAMESPACE "UBrowseGraph"

UBrowseGraph::UBrowseGraph(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}


void UBrowseGraph::RefreshGraph(UObject* pRoot)
{
	constexpr int32 nodeYSpacing = 150;
	constexpr int32 nodeYStart = 50;
	constexpr int32 nodeXPos = 50;

	/* clear previous graph */
	RemoveAllNodes();

	/* Walk the outer chain */
	TArray<UObject*> Outers;
	UObject* Root = pRoot == nullptr ? GEngine : pRoot;
	UObject *Outermost = nullptr;
	Outermost = Root;
	while (Outermost != nullptr) 
	{
		Outers.Push(Outermost);
		Outermost = Outermost->GetOuter();
	}
	// start building the graph, outermost first
	uint32 NodeY = nodeYStart;
	int32 OuterCount = Outers.Num() - 1;
	UBrowseNode*  ThisNode; 
	UBrowseNode*  PrevNode = nullptr;
	for (int32 i = OuterCount; i >= 0; i--) {
		FGraphNodeCreator<UBrowseNode> NodeBuilder(*this);
		UObject *NodeObject = Outers[i];
		ThisNode = NodeBuilder.CreateNode(false);
		ThisNode->SetupNode(FIntPoint(nodeXPos, NodeY), NodeObject);
		ThisNode->FixInPlace();
		NodeBuilder.Finalize();
		if (PrevNode != nullptr)
		{
			PrevNode->GetChildrenPin()->MakeLinkTo(ThisNode->GetParentPin());
		}
		if (!ThisNode->GetCDOPin()->bHidden)
		{
			UClass* NodeClass = Cast<UClass>(NodeObject);
			if (NodeClass == nullptr)
			{
				NodeClass = NodeObject->GetClass();
			}
			if (NodeClass)
			{
				TArray<UEdGraphNode*> Siblings;
				Siblings.Add(ThisNode);
				FIntRect Bounds = FEdGraphUtilities::CalculateApproximateNodeBoundaries(Siblings);
				UObject *NodeCDO = NodeClass->ClassDefaultObject;
				FGraphNodeCreator<UBrowseNode> CDONodeBuilder(*this);
				auto CDONode = CDONodeBuilder.CreateNode(false);
				CDONode->SetupNode(FIntPoint(nodeXPos, NodeY), NodeCDO);
				CDONode->GetChildrenPin()->PinName = TEXT("Default");
				ThisNode->GetCDOPin()->MakeLinkTo(CDONode->GetChildrenPin());
				CDONodeBuilder.Finalize();
			}
		}
		if (!ThisNode->GetOwnerPin()->bHidden)
		{
			UActorComponent *Component = Cast<UActorComponent>(NodeObject);
			if (Component != nullptr)
			{
				AActor* NodeOwner = Component->GetOwner();
				if (NodeOwner != nullptr)
				{	
					TArray<UEdGraphNode*> Siblings;
					Siblings.Add(ThisNode);
					FIntRect Bounds = FEdGraphUtilities::CalculateApproximateNodeBoundaries(Siblings);
					FGraphNodeCreator<UBrowseNode> OwnerNodeBuilder(*this);
					auto OwnerNode = OwnerNodeBuilder.CreateNode(false);
					OwnerNode->SetupNode(FIntPoint(nodeXPos, NodeY), NodeOwner);
					OwnerNode->GetChildrenPin()->PinName = TEXT("Actor");
					OwnerNode->GetChildrenPin()->Direction = EGPD_Output;
					OwnerNode->GetChildrenPin()->MakeLinkTo(ThisNode->GetOwnerPin());
					OwnerNodeBuilder.Finalize();
				}
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
				UObject* BP = NodeBPClass->ClassGeneratedBy;
				FGraphNodeCreator<UBrowseNode> BPNodeBuilder(*this);
				auto BPNode = BPNodeBuilder.CreateNode(false);
				BPNode->SetupNode(FIntPoint(nodeXPos, NodeY), BP);
				ThisNode->GetGeneratedByPin()->Direction = EGPD_Input;
				BPNode->GetGeneratesPin()->Direction = EGPD_Output;
				ThisNode->GetGeneratedByPin()->MakeLinkTo(BPNode->GetGeneratesPin());
				BPNodeBuilder.Finalize();
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
				GeneratedNode->SetupNode(FIntPoint(nodeXPos, NodeY), GeneratedClass);
				if (GeneratedNode->GetGeneratedByPin() != nullptr)
				{
					ThisNode->GetGeneratesPin()->Direction = EGPD_Output;
					GeneratedNode->GetGeneratedByPin()->Direction = EGPD_Input;
					ThisNode->GetGeneratesPin()->MakeLinkTo(GeneratedNode->GetGeneratedByPin());
				}
				GeneratedNodeBuilder.Finalize();
			}
		}

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