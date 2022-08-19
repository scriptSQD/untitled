#pragma once

#include <UntitledPrecomp.hpp>

#include "wx/treebase.h"
#include "wx/treectrl.h"

#include "wx/scrolwin.h"

#include "Components/QuickActionsPanel.hpp"
#include <Structures/DatabaseMetadata.hpp>

class TableNodeData : public wxTreeItemData {
  public:
    explicit TableNodeData(DatabaseMetadata::TableLocation location)
        : m_Location(std::move(location)){};

    [[nodiscard]] DatabaseMetadata::TableLocation GetLocation() const {
        return m_Location;
    }

  private:
    DatabaseMetadata::TableLocation m_Location;
};

class DatabaseTreeCtrl : public wxScrolledWindow {
  public:
    explicit DatabaseTreeCtrl(wxWindow *parent);
    ~DatabaseTreeCtrl() override {
        delete m_IconsList;
        SQD_LOG("Destroying DatabaseTreeCtrl!");
    }

    void RefreshTree() { PopulateDatabaseTree(); }

    void BindTreeEvt(const wxEventTypeTag<wxTreeEvent> &event,
                     const std::function<void(wxTreeEvent &evt)> &functor) {
        m_DatabaseTreeView->Bind(event, functor);
    };

    [[nodiscard]] wxTreeCtrl *GetTreeCtrl() const { return m_DatabaseTreeView; }

  private:
    void IntrospectDatabase();
    void PopulateDatabaseTree();

    void PopulateIcons();
    void ConstructQuickActionsPanel();

    void OnQuickAdd(wxCommandEvent &evt);
    void OnQuickDelete(wxCommandEvent &evt);
    void OnQuickSync(wxCommandEvent &evt);

  private:
    wxBoxSizer *m_Sizer;

    wxImageList *m_IconsList;

    wxTreeCtrl *m_DatabaseTreeView;
    DatabaseMetadata m_DatabaseMeta;

    QAPanel *m_QuickActionsPanel{};

    std::vector<wxBitmapButton *> m_QuickActions;
    wxBitmapButton *m_Add, *m_Delete, *m_Sync;
};