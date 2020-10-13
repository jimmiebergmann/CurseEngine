/*
* MIT License
*
* Copyright (c) 2020 Jimmie Bergmann
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files(the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions :
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
*/

#include "Molten/Gui/Widget.hpp"
#include "Molten/Gui/Layer.hpp"
#include <algorithm>

namespace Molten::Gui
{

    Widget::Widget()
    {}

    void Widget::Update(const Time&)
    {}

    void Widget::Draw(CanvasRenderer& )
    {}

    Vector2f32 Widget::CalculateSize(const Vector2f32& grantedSize)
    {
        return grantedSize;
    }

    void Widget::CalculateChildrenGrantedSize(WidgetTreeData::Tree::ConstLane<WidgetTreeData::Tree::PartialLaneType> /*children*/)
    {
    }

    bool Widget::OnAddChild(WidgetPointer)
    {
        return false;
    }

    const WidgetRenderData& Widget::GetRenderData() const
    {
        return m_renderData;
    }

    const Vector2f32& Widget::GetGrantedSize() const
    {
        return m_renderData.grantedSize;
    }

    void Widget::SetRenderData(
        WidgetTreeData::Tree::ConstIterator< WidgetTreeData::Tree::PartialLaneType> it,
        const Vector2f32& position,
        const Vector2f32& grantedSize)
    {
        const auto& parentGrantedSize = GetGrantedSize();
        auto& childRenderData = (*it).GetValue()->GetWidgetRenderData();

        childRenderData.position = {
            std::clamp(position.x, 0.0f, parentGrantedSize.x),
            std::clamp(position.y, 0.0f, parentGrantedSize.y)
        };

        childRenderData.grantedSize = {
            std::clamp(grantedSize.x, 0.0f, parentGrantedSize.x - childRenderData.position.x),
            std::clamp(grantedSize.y, 0.0f, parentGrantedSize.y - childRenderData.position.y)
        };
    }

    bool Widget::AddChild(WidgetPointer parent, WidgetPointer child)
    {
        return Layer::AddChild(parent, child);
    }

    WidgetTreeData& Widget::GetWidgetTreeData()
    {
        return m_treeData;
    }

    WidgetRenderData& Widget::GetWidgetRenderData()
    {
        return m_renderData;
    }

}