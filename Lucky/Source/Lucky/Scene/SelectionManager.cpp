#include "lcpch.h"
#include "SelectionManager.h"

namespace Lucky
{
    void SelectionManager::Select(Object selectionID)
    {
        s_Context = selectionID;
    }

    bool SelectionManager::IsSelected(Object selectionID)
    {
        return s_Context == selectionID;
    }

    void SelectionManager::Deselect()
    {
        s_Context = {};
    }

    Object SelectionManager::GetSelection()
    {
        return s_Context;
    }
}