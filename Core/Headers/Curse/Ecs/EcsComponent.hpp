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

#ifndef CURSE_CORE_ECS_ECSCOMPONENT_HPP
#define CURSE_CORE_ECS_ECSCOMPONENT_HPP

#include "Curse/Ecs/Ecs.hpp"
#include <array>
#include <vector>

namespace Curse
{

    namespace Ecs
    {

        template<typename ContextType> class SystemBase; ///< Forward declaration.

        using ComponentTypeId = int16_t; ///< Data type of component type ID.

        /**
        * @brief Component base class.
        */
        class ComponentBase
        {

        };

        /**
        * @brief Component base class for specific context type.
        */
        template<typename ContextType>
        class ComponentContextBase : public ComponentBase
        {

        };

        /**
        * @brief Component class.
        *        Inherit from this class to create component structures.
        *        Components are implicitly attached to entities.
        */
        template<typename ContextType, typename DerivedComponent>
        class Component : public ComponentContextBase<ContextType>
        {

        public:

            /**
            * @brief Id of this component type.
            */
            static inline const ComponentTypeId componentTypeId = ContextType::GetNextComponentTypeId();

        };


        namespace Private
        {

            /**
            * @brief Checks if provided types are explicit component types.
            * @return True if Types inherits from ComponentBase, else false.
            */
            template<typename ... Types>
            constexpr bool AreExplicitComponentTypes();

            /**
            * @brief Checks if provided types are explicit component types.
            * @return True if Types inherits from ComponentContextBase<ContextType>, else false.
            */
            template<typename ContextType, typename ... Types>
            constexpr bool AreExplicitContextComponentTypes();


            /**
            * @brief Structure of components grouped together for systems.
            */
            template<typename ContextType>
            struct ComponentGroup
            {
                /**
                * @brief Constructor of component group.
                */
                ComponentGroup(const size_t componentsPerEntity);

                std::vector<SystemBase<ContextType>*> systems;  ///< Vector of systems interested in this component group.
                const size_t componentsPerEntity;               ///<  Number of components per entity.
                std::vector<ComponentBase*> components;         ///< Vector of all components. The entity stride is defined by componentsPerEntity.
                size_t entityCount;                             ///< Number of entities in this component group.

                ///std::vector<Entity<ContextType>*> entities;
            };


            /**
            * @brief Count the total number of bytes of all passes components.
            */
            template<typename ... Components>
            constexpr size_t GetComponentsSize();


            /**
            * @brief Helper structure, containing offset of a component type id.
            * @see ComponentOffsets
            */
            struct ComponentOffsetItem
            {
                ComponentTypeId componentTypeId;
                size_t offset;
            };

            using ComponentOffsetList = std::vector<ComponentOffsetItem>; ///< Vector of component offset items.

            template<size_t Size>
            using ComponentOffsetArray = std::array<ComponentOffsetItem, Size>; ///< Array of component offset items.


            /**
            * @brief Helper function, for creating an array of component offsets.
            *
            * @tparam Components List of components to extend to offsetList.
            * @param offsetList Offset list to extend by template parameter list.
            * @param newTotalSize Total size in bytes of the new extended offset list.
            */
            template<typename ... Components>
            static void ExtendComponentOffsetList(ComponentOffsetList& offsetList, const size_t oldTotalSize);


            /**
            * @brief Helper function, for creating an array of component offsets.
            */
            template<typename ... Components>
            constexpr ComponentOffsetArray<sizeof...(Components)> CreateComponentOffsets();
        
            /**
            * @brief Helper structure, for retreiving data offset of each component.
            */
            template<typename ... Components>
            struct ComponentOffsets
            {
                constexpr static size_t offsetCount = sizeof...(Components);
                static inline const ComponentOffsetArray<sizeof...(Components)> offsets = CreateComponentOffsets<Components...>();
            };


            /**
            * @brief Return the index of provided Comp, in the template parameter set of Components.
            */
            template<typename Comp, typename ... Components>
            size_t GetComponentIndexOfTypes();

            /**
            * @brief Helper structure, for retreiving the offset of a component in a template parameter set.
            */
            template<typename Comp, typename ... Components>
            struct ComponentIndex
            {
                static inline const size_t index = GetComponentIndexOfTypes<Comp, Components...>();
            };

        }

    }

}

#include "Curse/Ecs/EcsComponent.inl"

#endif