
#include "SUBrowsePanel.h"

#include "UBrowseGraph.h"
#include "UBrowseSchema.h"
#include "Widgets/Text/STextBlock.h"


#define LOCTEXT_NAMESPACE "SUBrowsePanel"

void SUBrowsePanel::Construct(const FArguments& InArgs)
{
    BrowserGraphPtr = NewObject<UBrowseGraph>(UBrowseGraph::StaticClass());
    BrowserGraphPtr->Schema = UBrowseSchema::StaticClass();
    BrowserGraphPtr->AddToRoot();
    OnNewObjectView.BindSP(this, &SUBrowsePanel::OnNewRootNode);
    // appearance
    // TODO: Name of object at root
    AppearanceInfo.CornerText = LOCTEXT("UBrowseGraphCornerText", "UBROWSE");
    // events
    SGraphEditor::FGraphEditorEvents GraphEvents;
    GraphEvents.OnNodeDoubleClicked = InArgs._OnNodeDoubleClicked;

    // clang-format off
	GraphEditorPtr = SNew(SGraphEditor)
		.GraphToEdit(BrowserGraphPtr)
		.IsEditable(false) 
		.TitleBar(SNew(SBorder).HAlign(HAlign_Center)
		[
			SNew(STextBlock).Text(LOCTEXT("UBrowseGraphTitle", "UBrowse Graph"))
		])
		.GraphEvents(GraphEvents)
		.DisplayAsReadOnly(false)
		.Appearance(this, &SUBrowsePanel::GetAppearanceInfo);
		ChildSlot
		[
			GraphEditorPtr.ToSharedRef()
		];
    // clang-format on
    BrowserGraphPtr->RefreshGraph();
}

/*
void SUBrowsePanel::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
    SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
}
*/

FGraphAppearanceInfo SUBrowsePanel::GetAppearanceInfo() const
{
    return AppearanceInfo;
}

void SUBrowsePanel::OnNewRootNode(TSharedPtr<FBrowserObject> InObject)
{
    AppearanceInfo.CornerText = FText::FromString(GetNameSafe(InObject->Object.Get()));
    AppearanceInfo.ReadOnlyText = FText::FromString(GetNameSafe(InObject->Object.Get()));
    BrowserGraphPtr->RefreshGraph(InObject->Object.Get());
}

#undef LOCTEXT_NAMESPACE