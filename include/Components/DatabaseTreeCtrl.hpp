#pragma once

#include <UntitledPrecomp.hpp>

#include "wx/treebase.h"
#include "wx/treectrl.h"

#include "wx/scrolwin.h"

#include "Views/DatabaseConnectionDialog.hpp"

#include "Components/QuickActionsPanel.hpp"
#include "Structures/DatabaseMetadata.hpp"

class TableNodeData : public wxTreeItemData {
  public:
    explicit TableNodeData(int identifier)
        : m_ConnectionIdentifier(identifier), m_IsConnectionNode(false){};
    explicit TableNodeData(int identifier, bool isConnectionNode,
                           bool isTableNode)
        : m_ConnectionIdentifier(identifier),
          m_IsConnectionNode(isConnectionNode), m_IsTableNode(isTableNode){};
    explicit TableNodeData(DatabaseMetadata::TableLocation location) {
        m_IsConnectionNode = false;
        m_IsTableNode = true;

        const auto &[id, schema, table] = location;
        m_ConnectionIdentifier = id;

        m_Location = std::move(location);
    };

    [[nodiscard]] DatabaseMetadata::TableLocation GetLocation() const {
        return m_Location;
    }
    [[nodiscard]] int GetConnectionIdentifier() const {
        return m_ConnectionIdentifier;
    }

    [[nodiscard]] bool IsConnection() const { return m_IsConnectionNode; }
    void SetIsConnectionNode(bool is = true) { m_IsConnectionNode = is; }

    [[nodiscard]] bool IsTableNode() const { return m_IsTableNode; }

  private:
    int m_ConnectionIdentifier;

    DatabaseMetadata::TableLocation m_Location;

    bool m_IsConnectionNode;
    bool m_IsTableNode;
};

class DatabaseTreeCtrl : public wxScrolledWindow {
  public:
    explicit DatabaseTreeCtrl(wxWindow *parent);
    ~DatabaseTreeCtrl() override {
        delete m_IconsList;
        SQD_LOG("Destroying DatabaseTreeCtrl!");
    }

    void AppendToTree(const std::string &connectionDisplayName, int id) {
        PopulateDatabaseTree(id, connectionDisplayName);
    };
    void RemoveConnection(int identifier);

    void RefreshTree();

    void BindTreeEvt(const wxEventTypeTag<wxTreeEvent> &event,
                     const std::function<void(wxTreeEvent &evt)> &functor) {
        m_DatabaseTreeView->Bind(event, functor);
    };

    [[nodiscard]] wxTreeCtrl *GetTreeCtrl() const { return m_DatabaseTreeView; }

  private:
    void IntrospectDatabase(int connectionIdentifier);

    /**
     * Appends new node to database tree.
     * @param connectionDisplayName Will be displayed as the top-level node
     * name.
     * @param connectionIdentifier Unique id for the new connection.
     */
    void PopulateDatabaseTree(int connectionIdentifier,
                              const std::string &connectionDisplayName);

    void PopulateIcons();
    void ConstructQuickActionsPanel();

    static void OnQuickAdd(wxCommandEvent &evt);
    void OnQuickDelete(wxCommandEvent &evt);
    void OnQuickSync(wxCommandEvent &evt);

  private:
    wxBoxSizer *m_Sizer;

    wxImageList *m_IconsList;

    wxTreeCtrl *m_DatabaseTreeView;
    DatabaseMetadata m_DatabaseMeta;

    std::vector<std::pair<int, std::string>> m_ActiveConnectionNodes;

    QAPanel *m_QuickActionsPanel{};

    std::vector<wxBitmapButton *> m_QuickActions;
    wxBitmapButton *m_Add, *m_Delete, *m_Sync;

    wxTreeItemId m_RootNodeId;
};