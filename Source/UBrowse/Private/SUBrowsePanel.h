#pragma once

#include "GraphEditor.h"
#include "UBrowse.h"

class UBrowseGraph;

class SUBrowsePanel : public SCompoundWidget
{
public:
    DECLARE_DELEGATE_OneParam(FOnNewObjectView, TSharedPtr<FBrowserObject>);

    // clang-format off
    SLATE_BEGIN_ARGS(SUBrowsePanel) {}
        SLATE_EVENT(FSingleNodeEvent, OnNodeDoubleClicked)
    SLATE_END_ARGS()
    // clang-format on

    FOnNewObjectView OnNewObjectView;

    FSingleNodeEvent OnNodeDoubleClicked;

    /**
     * Construct this widget
     *
     * @param InArgs The declaration data for this widget.
     */
    void Construct(const FArguments& InArgs);

    /* Called when a new root node is selected */
    void OnNewRootNode(TSharedPtr<FBrowserObject> InObject);

    FGraphAppearanceInfo GetAppearanceInfo() const;

private:
    FGraphAppearanceInfo AppearanceInfo;
    TSharedPtr<SGraphEditor> GraphEditorPtr;
    UBrowseGraph* BrowserGraphPtr = nullptr;
};