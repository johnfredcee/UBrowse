
#include "SUBrowseNode.h"
#include "Engine/Blueprint.h"
#include "GraphEditorSettings.h"
#include "EdGraph/EdGraphNode.h"
#include "EditorClassUtils.h"
#include "SlateOptMacros.h"
#include "UBrowseNode.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Layout/SBox.h"
#include "EditorStyleSet.h"

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

	UBrowseNode *EdGraphNode = Cast<UBrowseNode>(GraphNode);

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
									EdGraphNode->GetUObject() != nullptr ?
										FEditorClassUtils::GetDocumentationLinkWidget(EdGraphNode->GetUObject()->GetClass()) : 
										SNullWidget::NullWidget
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
	if (!TransNode->IsFixedInPlace())
		return;
	const float NodeGap{50.0f};
	bool bStartedLeftHandLayout{ false };
	float LeftHandSide{ 0.0f };
	bool bStartedRightHandLayout = false;
	float RightHandSide{ 0.0f };

	const TArray<UEdGraphPin*>& Pins{TransNode->GetAllPins()};

	// line them up on the left
	for(auto& Pin : Pins)
	{
		if (Pin->Direction == EGPD_Input)
		{
			if ((Pin->GetName() == TEXT("Inner")) || (Pin->GetName() == TEXT("Outer")))
			{
				continue;
			}
			if (!bStartedLeftHandLayout)
			{
				LeftHandSide = TransNode->NodePosX;
				bStartedLeftHandLayout = true;
			}
			if (Pin->LinkedTo.Num() < 1)
				continue;
			// get the node we want to go on the left
			UEdGraphPin* LinkedPin = Pin->LinkedTo[0];
			UBrowseNode* LeftNode = Cast<UBrowseNode>(LinkedPin->GetOwningNode());
			FVector2D LeftNodeSize = NodeToWidgetLookup.FindChecked(LeftNode)->GetDesiredSize();		
			LeftNode->NodePosX = (LeftHandSide - LeftNodeSize.X) - NodeGap;
			LeftHandSide = LeftNode->NodePosX;
		}
	}

	// line them up on the right
	for(auto Pin : Pins)
	{
		if (Pin->Direction == EGPD_Output)
		{
			if ((Pin->GetName() == "Inner") || (Pin->GetName() == "Outer"))
			{
				continue;
			}
			if (!bStartedRightHandLayout)
			{
				FVector2D TransNodeSize = NodeToWidgetLookup.FindChecked(TransNode)->GetDesiredSize();
				RightHandSide = (TransNode->NodePosX + TransNodeSize.X) + NodeGap;
				bStartedRightHandLayout = true;
			}		
			if (Pin->LinkedTo.Num() < 1)
				continue;
			UEdGraphPin* LinkedPin = Pin->LinkedTo[0];
			UBrowseNode* RightNode = Cast<UBrowseNode>(LinkedPin->GetOwningNode());
			FVector2D RightNodeSize = NodeToWidgetLookup.FindChecked(RightNode)->GetDesiredSize();	
			RightNode->NodePosX = RightHandSide;
			RightHandSide = RightNode->NodePosX + RightNodeSize.X + NodeGap;
		}
	}
}

#undef LOCTEXT_NAMESPACE