#pragma once

#include "SGraphNode.h"

class UBrowseNode;

class SUBrowseNode : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SUBrowseNode) { }
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UBrowseNode *InNode);

	FSlateColor GetNodeTitleColor() const;

	// SGraphNode Implementation
	virtual void UpdateGraphNode() override;
	// SGraphNode Implementation
};