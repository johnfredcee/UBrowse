#pragma once

#include "ClassIconFinder.h"

#define LOCTEXT_NAMESPACE "SUBrowsePropertyTableRow.h"


class SUBrowsePropertyTableRow 
    : public SMultiColumnTableRow<TSharedPtr<int32>>
{
public:
    SLATE_BEGIN_ARGS(SUBrowsePropertyTableRow) { }
        SLATE_ARGUMENT(TSharedPtr<FBrowserObject>, Object)
        SLATE_ARGUMENT(FText, HighlightText)
    SLATE_END_ARGS()
public:
    SUBrowsePropertyTableRow() 
        : ClassIcon(nullptr)
    {
    }

    void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView)
    {
        Object = InArgs._Object;
		if (UObject* Obj = InArgs._Object->Object.Get())
		{
			// Get selection icon based on actor(s) classes and add before the selection label
			ClassIcon = FClassIconFinder::FindIconForClass(Obj->GetClass());
		}
		SMultiColumnTableRow<TSharedPtr<int32> >::Construct(FSuperRowType::FArguments(), InOwnerTableView);
    }

    private:
        TSharedPtr<FBrowserObject> Object;

        const FSlateBrush* ClassIcon;

        FText Name;
        FText ClassName;
        FText Package;

        FText HighlightText;        

public:

	BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override
	{
		return SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SImage)
				.Image(ClassIcon)
			]

		+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(STextBlock)
				.Text(LOCTEXT("Placeholder", "Placeholder"))
			];
	}
	END_SLATE_FUNCTION_BUILD_OPTIMIZATION
};

#undef LOCTEXT_NAMESPACE
