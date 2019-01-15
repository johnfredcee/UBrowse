#pragma once

#include "GraphEditor.h"
#include "EdGraph/EdGraphNode.h"
#include "EditorClassUtils.h"
#include "UBrowseNode.generated.h"

UCLASS()
class UBrowseNode : public UEdGraphNode
{
	GENERATED_UCLASS_BODY()
public:
	void AddChild(UBrowseNode* ChildNode);
	void SetupNode(const FIntPoint& NodePosition, UObject* object);
	virtual UEdGraphPin* GetChildrenPin();
	virtual UEdGraphPin* GetParentPin();
	virtual UEdGraphPin* GetCDOPin();
	virtual UEdGraphPin* GetOwnerPin();
	virtual UEdGraphPin* GetGeneratedByPin();
	virtual UEdGraphPin* GetGeneratesPin();

	const FText GetShortDesc() const { return FText::FromString(ShortDesc);  }
	const FText GetLongDesc() const { return FText::FromString(LongDesc);  }
	const UObject* GetUObject() const { return NodeObject; }
	FText GetTooltipText() const override;

	// UEdGraphNode implementation
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void AllocateDefaultPins() override;
	// End UEdGraphNode implementation

private:
	FText NodeTitle;
	FString ShortDesc;
	FString LongDesc;

	UEdGraphPin* ChildrenPin = nullptr;
	UEdGraphPin* ParentPin = nullptr;
	UEdGraphPin* CDOPin = nullptr;
	UEdGraphPin* OwnerPin = nullptr;
	UEdGraphPin* GeneratesPin = nullptr;
	UEdGraphPin* GeneratedByPin = nullptr;
	const UClass*  NodeClass = nullptr;
	const UObject* NodeObject = nullptr;
};