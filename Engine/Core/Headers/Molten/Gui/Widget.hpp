/*
* MIT License
*
* Copyright (c) 2021 Jimmie Bergmann
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

#ifndef MOLTEN_CORE_GUI_WIDGET_HPP
#define MOLTEN_CORE_GUI_WIDGET_HPP

#include "Molten/Gui/GuiTypes.hpp"
#include "Molten/Gui/SpacingTypes.hpp"
#include "Molten/Gui/WidgetData.hpp"

namespace Molten::Gui
{

    template<typename TTheme>
    class Widget
    {

    public:

        static constexpr bool overrideChildrenMouseEvents = false;

        enum class State{ };

        Vector2f32 size;
        MarginType margin;
        PaddingType padding;

        explicit Widget(WidgetData<TTheme>& data);
        Widget(
            WidgetData<TTheme>& data,
            const Vector2f32& size);

        virtual ~Widget() = default;

        virtual bool GetOverrideChildrenMouseEvents() const;

        virtual void Update();

        template<template<typename> typename TWidgetType, typename ... TArgs>
        WidgetTypePointer<TWidgetType<TTheme>> CreateChild(TArgs ... args);

    protected:

        virtual void OnAddChild(WidgetData<TTheme>& childData);

        virtual void OnRemoveChild(WidgetData<TTheme>& childData); // NOT IN USE YET.

        WidgetData<TTheme>& GetData();
        const WidgetData<TTheme>& GetData() const;

        typename WidgetData<TTheme>::TreeNormalLane GetChildrenNormalLane();
        typename WidgetData<TTheme>::TreePartialLane GetChildrenPartialLane();

        const Bounds2f32& GetGrantedBounds() const;

        void SetGrantedBounds(const Bounds2f32& grantedBounds);

        void ApplyMarginsToGrantedBounds();

    private:
        
        template<typename TLayerSkin>
        friend class Canvas;

        template<typename TLayerSkin>
        friend class Layer; // Delete due to obsolete template?

        friend TTheme;

        Widget(const Widget&) = delete;
        Widget(Widget&&) = delete;
        Widget& operator= (const Widget&) = delete;
        Widget& operator= (Widget&&) = delete;

        WidgetData<TTheme>& m_data;

    };


    template<typename TTheme, template<typename> typename TWidget>
    class WidgetMixin : public Widget<TTheme>
    {

    public:
   
        bool GetOverrideChildrenMouseEvents() const override;

    protected:

        explicit WidgetMixin(WidgetDataMixin<TTheme, TWidget>& data);

        WidgetMixin(
            WidgetDataMixin<TTheme, TWidget>& data,
            const Vector2f32& size);

        template<typename TState>
        const TState& GetSkinState() const;

        template<typename TState>
        void SetSkinState(const TState& state);

        WidgetDataMixin<TTheme, TWidget>& GetDataMixin();
        const WidgetDataMixin<TTheme, TWidget>& GetDataMixin() const;

        using WidgetSkinType = WidgetSkin<TTheme, TWidget>;
        
    private:  

        WidgetDataMixin<TTheme, TWidget>& m_dataMixin;

    };

}

#include "Molten/Gui/Widget.inl"

#endif