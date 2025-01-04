#pragma once

#include "EdGraph/EdGraph.h"
#include "EdGraph/EdGraphNode.h"
#include "GraphEditor.h"

#include "UBrowseGraph.generated.h"


UCLASS()
class UBrowseGraph : public UEdGraph
{
    GENERATED_UCLASS_BODY()
  public:
    // Build the graph based on the current object
    void RefreshGraph(UObject* object = nullptr);

   private:
    // Clear the graph
    void RemoveAllNodes();
};