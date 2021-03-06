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

#include <type_traits>

namespace Molten::Shader::Visual
{

    // Structure base implementations.
    template<typename TMetaData>
    template<typename ... TMetaDataParams>
    inline StructureMetaBase<TMetaData>::StructureMetaBase(TMetaDataParams ... metaDataParameters) :
        TMetaData(metaDataParameters...)
    {}


    // Structure implementations.
    template<template<typename TVariableTypeDataType, typename TVariableTypeMetaData> typename TVariableType, 
        typename TVariableMetaData, typename TMetaData>
    template<typename ... TMetaDataParams>
    inline Structure<TVariableType, TVariableMetaData, TMetaData>::Structure(Script& script, TMetaDataParams ... metaDataParameters) :
        StructureMetaBase<TMetaData>(metaDataParameters...),
        m_script(script),
        m_sizeOf(0)
    {}

    template<template<typename TVariableTypeDataType, typename TVariableTypeMetaData> typename TVariableType,
        typename TVariableMetaData, typename TMetaData>
    inline Structure<TVariableType, TVariableMetaData, TMetaData>::~Structure()
    {
        for (auto& member : m_members)
        {
            delete member;
        }
    }

    template<template<typename TVariableTypeDataType, typename TVariableTypeMetaData> typename TVariableType,
        typename TVariableMetaData, typename TMetaData>
    inline typename Structure<TVariableType, TVariableMetaData, TMetaData>::VariableContainer::iterator 
        Structure<TVariableType, TVariableMetaData, TMetaData>::begin()
    {
        return m_members.begin();
    }
    template<template<typename TVariableTypeDataType, typename TVariableTypeMetaData> typename TVariableType,
        typename TVariableMetaData, typename TMetaData>
    inline typename Structure<TVariableType, TVariableMetaData, TMetaData>::VariableContainer::const_iterator
        Structure<TVariableType, TVariableMetaData, TMetaData>::begin() const
    {
        return m_members.begin();
    }

    template<template<typename TVariableTypeDataType, typename TVariableTypeMetaData> typename TVariableType,
        typename TVariableMetaData, typename TMetaData>
    inline typename Structure<TVariableType, TVariableMetaData, TMetaData>::VariableContainer::iterator 
        Structure<TVariableType, TVariableMetaData, TMetaData>::end()
    {
        return m_members.end();
    }
    template<template<typename TVariableTypeDataType, typename TVariableTypeMetaData> typename TVariableType,
        typename TVariableMetaData, typename TMetaData>
    inline typename Structure<TVariableType, TVariableMetaData, TMetaData>::VariableContainer::const_iterator 
        Structure<TVariableType, TVariableMetaData, TMetaData>::end() const
    {
        return m_members.end();
    }

    template<template<typename TVariableTypeDataType, typename TVariableTypeMetaData> typename TVariableType,
        typename TVariableMetaData, typename TMetaData>
    template<typename TDataType, typename ... TMetaDataParams>
    inline typename Structure<TVariableType, TVariableMetaData, TMetaData>::template VariableType<TDataType>*
        Structure<TVariableType, TVariableMetaData, TMetaData>::AddMember(TMetaDataParams ... metaData)
    {
        auto* member = new VariableType<TDataType>(m_script, metaData...);
        m_members.push_back(member);
        m_sizeOf += member->GetSizeOf();
        return member;
    }

    template<template<typename TVariableTypeDataType, typename TVariableTypeMetaData> typename TVariableType,
        typename TVariableMetaData, typename TMetaData>
    inline void Structure<TVariableType, TVariableMetaData, TMetaData>::RemoveMember(const size_t index)
    {
        if (index >= m_members.size())
        {
            return;
        }
        auto it = m_members.begin() + index;
        auto* variablebase = *it;
        m_sizeOf -= variablebase->GetSizeOf();
        delete variablebase;
        m_members.erase(it);
    }

    template<template<typename TVariableTypeDataType, typename TVariableTypeMetaData> typename TVariableType,
        typename TVariableMetaData, typename TMetaData>
    inline void Structure<TVariableType, TVariableMetaData, TMetaData>::RemoveAllMembers()
    {
        for (auto& member : m_members)
        {
            delete member;
        }
        m_members.clear();
        m_sizeOf = 0;
    }

    template<template<typename TVariableTypeDataType, typename TVariableTypeMetaData> typename TVariableType,
        typename TVariableMetaData, typename TMetaData>
    inline size_t Structure<TVariableType, TVariableMetaData, TMetaData>::GetMemberCount() const
    {
        return m_members.size();
    }

    template<template<typename TVariableTypeDataType, typename TVariableTypeMetaData> typename TVariableType,
        typename TVariableMetaData, typename TMetaData>
    template<typename TDataType>
    inline typename Structure<TVariableType, TVariableMetaData, TMetaData>::template VariableType<TDataType>*
        Structure<TVariableType, TVariableMetaData, TMetaData>::GetMember(const size_t index)
    {
        return static_cast<VariableType<TDataType>*>(GetMember(index));
    }
    template<template<typename TVariableTypeDataType, typename TVariableTypeMetaData> typename TVariableType,
        typename TVariableMetaData, typename TMetaData>
    template<typename TDataType>
    inline const typename Structure<TVariableType, TVariableMetaData, TMetaData>::template VariableType<TDataType>*
        Structure<TVariableType, TVariableMetaData, TMetaData>::GetMember(const size_t index) const
    {
        return static_cast<const VariableType<TDataType>*>(GetMember(index));
    }
    
    template<template<typename TVariableTypeDataType, typename TVariableTypeMetaData> typename TVariableType,
        typename TVariableMetaData, typename TMetaData>
    inline typename Structure<TVariableType, TVariableMetaData, TMetaData>::VariableBaseType*
        Structure<TVariableType, TVariableMetaData, TMetaData>::GetMember(const size_t index)
    {
        if (index >= m_members.size())
        {
            return nullptr;
        }
        return m_members[index];
    }

    template<template<typename TVariableTypeDataType, typename TVariableTypeMetaData> typename TVariableType,
        typename TVariableMetaData, typename TMetaData>
    inline const typename Structure<TVariableType, TVariableMetaData, TMetaData>::VariableBaseType*
        Structure<TVariableType, TVariableMetaData, TMetaData>::GetMember(const size_t index) const
    {
        if (index >= m_members.size())
        {
            return nullptr;
        }
        return m_members[index];
    }

    template<template<typename TVariableTypeDataType, typename TVariableTypeMetaData> typename TVariableType,
        typename TVariableMetaData, typename TMetaData>
    inline typename Structure<TVariableType, TVariableMetaData, TMetaData>::VariableBaseType*
        Structure<TVariableType, TVariableMetaData, TMetaData>::operator[](const size_t index)
    {
        return GetMember(index);
    }
    template<template<typename TVariableTypeDataType, typename TVariableTypeMetaData> typename TVariableType,
        typename TVariableMetaData, typename TMetaData>
    inline const typename Structure<TVariableType, TVariableMetaData, TMetaData>::VariableBaseType*
        Structure<TVariableType, TVariableMetaData, TMetaData>::operator[](const size_t index) const
    {
        return GetMember(index);
    }

    template<template<typename TVariableTypeDataType, typename TVariableTypeMetaData> typename TVariableType,
        typename TVariableMetaData, typename TMetaData>
    inline typename Structure<TVariableType, TVariableMetaData, TMetaData>::VariableContainer
        Structure<TVariableType, TVariableMetaData, TMetaData>::GetMembers()
    {
        return { m_members.begin(), m_members.end() };
    }

    template<template<typename TVariableTypeDataType, typename TVariableTypeMetaData> typename TVariableType,
        typename TVariableMetaData, typename TMetaData>
    inline typename Structure<TVariableType, TVariableMetaData, TMetaData>::ConstVariableContainer
        Structure<TVariableType, TVariableMetaData, TMetaData>::GetMembers() const
    {
        return { m_members.begin(), m_members.end() };
    }

    template<template<typename TVariableTypeDataType, typename TVariableTypeMetaData> typename TVariableType,
        typename TVariableMetaData, typename TMetaData>
    inline size_t Structure<TVariableType, TVariableMetaData, TMetaData>::GetSizeOf() const
    {
        return m_sizeOf;
    }

    template<template<typename TVariableTypeDataType, typename TVariableTypeMetaData> typename TVariableType,
        typename TVariableMetaData, typename TMetaData>
    template<template<typename TOtherVariableTypeDataType, typename TOtherVariableTypeMetaData> typename TOtherVariableType,
        typename TOtherVariableMetaData, typename TOtherMetaData>
    bool Structure<TVariableType, TVariableMetaData, TMetaData>::
        CheckCompability(const Structure<TOtherVariableType, TOtherVariableMetaData, TOtherMetaData>& other) const
    {
        
        return Private::CheckStructureCompability(m_members, other.m_members);
    }

}

namespace Molten::Shader::Visual::Private
{

    template<>
    inline bool CheckStructureCompability(
        const Structure<InputVariable>::VariableContainer& lhs, 
        const Structure<OutputVariable>::VariableContainer& rhs)
    {
        if (lhs.size() != rhs.size())
        {
            return false;
        }

        for (size_t i = 0; i < lhs.size(); i++)
        {
            auto* memberA = lhs[i];
            auto* memberB = rhs[i];

            if (memberA->GetOutputPin()->GetDataType() != memberB->GetInputPin()->GetDataType())
            {
                return false;
            }
        }
        
        return true;
    }

    template<>
    inline bool CheckStructureCompability(
        const Structure<OutputVariable>::VariableContainer& lhs,
        const Structure<InputVariable>::VariableContainer& rhs)
    {
        return CheckStructureCompability(rhs, lhs);
    }

    template<>
    inline bool CheckStructureCompability(
        const Structure<InputVariable>::VariableContainer& lhs,
        const Structure<InputVariable>::VariableContainer& rhs)
    {
        if (lhs.size() != rhs.size())
        {
            return false;
        }

        for (size_t i = 0; i < lhs.size(); i++)
        {
            auto* memberA = lhs[i];
            auto* memberB = rhs[i];

            if (memberA->GetOutputPin()->GetDataType() != memberB->GetOutputPin()->GetDataType())
            {
                return false;
            }
        }

        return true;
    }

    template<>
    inline bool CheckStructureCompability(
        const Structure<OutputVariable>::VariableContainer& lhs,
        const Structure<OutputVariable>::VariableContainer& rhs)
    {
        if (lhs.size() != rhs.size())
        {
            return false;
        }

        for (size_t i = 0; i < lhs.size(); i++)
        {
            auto* memberA = lhs[i];
            auto* memberB = rhs[i];

            if (memberA->GetInputPin()->GetDataType() != memberB->GetInputPin()->GetDataType())
            {
                return false;
            }
        }

        return true;
    }

}