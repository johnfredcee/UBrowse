
#include "UBrowsePrivatePCH.h"
#include "GraphEditorSettings.h"
#include "EdGraph/EdGraphNode.h"
#include "EditorClassUtils.h"
#include "UBrowseNode.h"
#include "SUBrowseNode.h"

#define LOCTEXT_NAMESPACE "SUBrowseNode" 

void SUBrowseNode::Construct(const FArguments& InArgs, UBrowseNode* InNode)
{
	GraphNode = InNode;
	UpdateGraphNode();
}

FSlateColor SUBrowseNode::GetNodeTitleColor() const
{
	const UObject* Obj = static_cast<UBrowseNode*>(GraphNode)->GetUObject();
	if (Obj == nullptr)
	{
		return GetErrorColor();
	}
	if (Obj->IsA(UClass::StaticClass()))
	{
		return GetDefault<UGraphEditorSettings>()->ClassPinTypeColor;
	}
	UClass* Class = Obj->GetClass();
	if (Class->IsChildOf(UBlueprint::StaticClass()))
	{
		return GetDefault<UGraphEditorSettings>()->StructPinTypeColor;
	}
	if (Class->IsChildOf(UBlueprintGeneratedClass::StaticClass()))
	{
		return GetDefault<UGraphEditorSettings>()->SoftClassPinTypeColor;
	}
	if (Class->IsChildOf(UBlueprint::StaticClass()))
	{
		return GetDefault<UGraphEditorSettings>()->SoftObjectPinTypeColor;
	}
	if (Class->IsChildOf(UPackage::StaticClass()))
	{
		return GetDefault<UGraphEditorSettings>()->WildcardPinTypeColor;
	}
	if (Class->IsChildOf(ULevel::StaticClass()))
	{
		return GetDefault<UGraphEditorSettings>()->BooleanPinTypeColor;
	}
	if (Class->IsChildOf(AActor::StaticClass()))
	{
		return GetDefault<UGraphEditorSettings>()->VectorPinTypeColor;
	}
	return GetDefault<UGraphEditorSettings>()->ObjectPinTypeColor;
}


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SUBrowseNode::UpdateGraphNode()
{
	OutputPins.Empty();
	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	UpdateErrorInfo();

	TSharedPtr<SVerticalBox>    MainVerticalBox;
	TSharedPtr<SErrorText>      ErrorText;
	TSharedPtr<SNodeTitle>      NodeTitle = SNew(SNodeTitle, GraphNode);

	ContentScale.Bind(this, &SUBrowseNode::GetContentScale);

	UBrowseNode *EdGraphNode = static_cast<UBrowseNode *>(GraphNode);

	GetOrAddSlot(ENodeZone::Center)
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	[
		SAssignNew( MainVerticalBox, SVerticalBox )
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("Graph.Node.Body"))
			.Padding(0)
			[
				SNew(SVerticalBox)
				.ToolTipText(this, &SUBrowseNode::GetNodeTooltip)
				+ SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Top)
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
					[
						SNew(SImage)
						.Image(FEditorStyle::GetBrush("Graph.Node.TitleGloss"))
					]
					+ SOverlay::Slot()
					.HAlign(HAlign_Left)
					.VAlign(VAlign_Center)
					[
						SNew(SBorder)
						.BorderImage(FEditorStyle::GetBrush("Graph.Node.ColorSpill"))
						.Padding(FMargin(10, 5, 30, 3))
						.BorderBackgroundColor(this, &SUBrowseNode::GetNodeTitleColor)
						[
							SNew(SVerticalBox) 
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								SNew(SHorizontalBox)
								+ SHorizontalBox::Slot()
								.AutoWidth()
								[
									FEditorClassUtils::GetDocumentationLinkWidget(EdGraphNode->GetUObject()->GetClass())
								]
								+ SHorizontalBox::Slot()
								.AutoWidth()
								[
									SNew(STextBlock)
									.Text(NodeTitle.Get(), &SNodeTitle::GetHeadTitle)
								]
							]
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								NodeTitle.ToSharedRef()
							]
						]
					]
					+ SOverlay::Slot()
					.VAlign(VAlign_Top)
					[
						SNew(SBorder)
						.BorderImage(FEditorStyle::GetBrush("Graph.Node.TitleHighlight"))
						.Visibility(EVisibility::HitTestInvisible)
						[
							SNew(SSpacer)
							.Size(FVector2D(20, 20))
						]
					]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(1.0f)
			[
				// POPUP ERROR MESSAGE : DO I need this?
				SAssignNew(ErrorText, SErrorText)
				.BackgroundColor(this, &SUBrowseNode::GetErrorColor)
				.ToolTipText(this, &SUBrowseNode::GetErrorMsgToolTip)
			]
			+ SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Top)
				[
					// NODE CONTENT AREA
					SNew(SBorder)
					.BorderImage(FEditorStyle::GetBrush("NoBorder"))
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					.Padding(FMargin(0, 3))
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.VAlign(VAlign_Center)
						[
							// LEFT
							SNew(SBox)
							.WidthOverride(120)
							[
								SAssignNew(LeftNodeBox, SVerticalBox)
							]
						]
						+ SHorizontalBox::Slot()
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Center)
						.FillWidth(1.0f)
						[
							SNew(SVerticalBox)
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								SNew(STextBlock)
								.Text(EdGraphNode->GetShortDesc())
							]
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								SNew(STextBlock)
								.Text(EdGraphNode->GetLongDesc())
							]
						]
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.VAlign(VAlign_Center)
						[
							// RIGHT
							SNew(SBox)
							.WidthOverride(120)
							[
								SAssignNew(RightNodeBox, SVerticalBox)
							]
						]
					]
				]
			]
		]
	];
	CreateBelowWidgetControls(MainVerticalBox);
	CreatePinWidgets();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

bool SUBrowseNode::RequiresSecondPassLayout() const
{
	return true;
}

void SUBrowseNode::PerformSecondPassLayout(const TMap< UObject*, TSharedRef<SNode> >& NodeToWidgetLookup) const
{
	UBrowseNode* TransNode = CastChecked<UBrowseNode>(GraphNode);

	// Input (Left) Nodes
	if ((!TransNode->GetOwnerPin()->bHidden) && (TransNode->GetOwnerPin()->LinkedTo.Num() > 0))
	{
		UEdGraphPin* LinkedPin = TransNode->GetOwnerPin()->LinkedTo[0];
		UBrowseNode* RightNode = Cast<UBrowseNode>(LinkedPin->GetOwningNode());
		FVector2D RightNodeSize = NodeToWidgetLookup.FindChecked(RightNode)->GetDesiredSize();
		RightNode->NodePosX = (TransNode->NodePosX - RightNodeSize.X) - 50.0f;
	}

	if ((!TransNode->GetGeneratedByPin()->bHidden) && (TransNode->GetGeneratedByPin()->LinkedTo.Num() > 0))
	{
		UEdGraphPin* LinkedPin = TransNode->GetGeneratedByPin()->LinkedTo[0];
		UBrowseNode* RightNode = Cast<UBrowseNode>(LinkedPin->GetOwningNode());
		FVector2D RightNodeSize = NodeToWidgetLookup.FindChecked(RightNode)->GetDesiredSize();
		RightNode->NodePosX = (TransNode->NodePosX - RightNodeSize.X) - 50.0f;
	}

	// Output (Right) Nodes
	if ((!TransNode->GetCDOPin()->bHidden) && (TransNode->GetCDOPin()->LinkedTo.Num() > 0))
	{
		UEdGraphPin* LinkedPin = TransNode->GetCDOPin()->LinkedTo[0];
		UBrowseNode* RightNode = Cast<UBrowseNode>(LinkedPin->GetOwningNode());
		FVector2D TransNodeSize = NodeToWidgetLookup.FindChecked(TransNode)->GetDesiredSize();
		RightNode->NodePosX = (TransNode->NodePosX + TransNodeSize.X) + 50.0f;
	}

	if ((!TransNode->GetGeneratesPin()->bHidden) && (TransNode->GetGeneratesPin()->LinkedTo.Num() > 0))
	{
		UEdGraphPin* LinkedPin = TransNode->GetGeneratesPin()->LinkedTo[0];
		UBrowseNode* RightNode = Cast<UBrowseNode>(LinkedPin->GetOwningNode());
		FVector2D TransNodeSize = NodeToWidgetLookup.FindChecked(TransNode)->GetDesiredSize();
		RightNode->NodePosX = (TransNode->NodePosX + TransNodeSize.X) + 50.0f;
	}

}

#undef LOCTEXT_NAMESPACE