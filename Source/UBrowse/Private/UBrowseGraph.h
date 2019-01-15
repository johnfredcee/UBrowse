#pragma once

#include "GraphEditor.h"
#include "EdGraph/EdGraph.h"
#include "EdGraph/EdGraphNode.h"
#include "UBrowseGraph.generated.h"

UCLASS()
class UBrowseGraph : public UEdGraph
{
	GENERATED_UCLASS_BODY()
public:

	void RefreshGraph(UObject* object = nullptr);

private:
	void RemoveAllNodes();
};