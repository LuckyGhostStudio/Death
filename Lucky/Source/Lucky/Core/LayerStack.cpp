#include "lcpch.h"
#include "LayerStack.h"

namespace Lucky
{
    LayerStack::LayerStack()
    {

    }

    LayerStack::~LayerStack()
    {
        // 删除所有层
        for (Layer* layer : m_Layers) {
            delete layer;
        }
    }

    void LayerStack::PushLayer(Layer* layer)
    {
        m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);     // 插入 layer 到 m_LayerInsertIndex 位置
        m_LayerInsertIndex++;
        layer->OnAttach();
    }

    void LayerStack::PushOverlay(Layer* overlay)
    {
        m_Layers.emplace_back(overlay);     // 插入 overlay 到 栈顶
        overlay->OnAttach();
    }

    void LayerStack::PopLayer(Layer* layer)
    {
        std::vector<Layer*>::iterator it = std::find(m_Layers.begin(), m_Layers.end(), layer);      // 查找 layer

        // 找到 layer
        if (it != m_Layers.end()) {
            layer->OnDetach();
            m_Layers.erase(it);         // 移除 it 指向的 layer
            m_LayerInsertIndex--;
        }
    }

    void LayerStack::PopOverlay(Layer* overlay)
    {
        std::vector<Layer*>::iterator it = std::find(m_Layers.begin(), m_Layers.end(), overlay);    // 查找 overlay

        if (it != m_Layers.end()) {
            overlay->OnDetach();
            m_Layers.erase(it);         // 移除 it 指向的 overlay
        }
    }
}