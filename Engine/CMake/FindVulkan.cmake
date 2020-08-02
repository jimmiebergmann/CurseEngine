if(WIN32)
	find_path(Vulkan_INCLUDE NAMES "vulkan/vulkan.h" PATHS "$ENV{VULKAN_SDK}/Include")	
	if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
		find_library(Vulkan_LIBRARY NAMES "vulkan-1" PATHS "$ENV{VULKAN_SDK}/Lib32" NO_SYSTEM_ENVIRONMENT_PATH)
	elseif("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
		find_library(Vulkan_LIBRARY NAMES "vulkan-1" PATHS "$ENV{VULKAN_SDK}/Lib" NO_SYSTEM_ENVIRONMENT_PATH)
	endif()
else()
	find_path(Vulkan_INCLUDE NAMES "vulkan/vulkan.h" PATHS "$ENV{VULKAN_SDK}/include")	
	find_library(Vulkan_LIBRARY NAMES "vulkan-1" PATHS "$ENV{VULKAN_SDK}/lib" NO_SYSTEM_ENVIRONMENT_PATH)
endif()

if("${Vulkan_LIBRARY}" STREQUAL "Vulkan_LIBRARY-NOTFOUND")
	set(Vulkan_NOTFOUND "TRUE")
else()
	set(Vulkan_FOUND "TRUE")
endif()