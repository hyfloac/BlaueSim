cmake_minimum_required(VERSION 3.25)

include(CheckCCompilerFlag)

file(GLOB SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp" "include/*.h" "include/*.inl")
file(GLOB HEADERS_BASE "include/*.hpp" "include/*.inl")

set(BLAU_SOURCE_FILES ${SOURCES} ${HEADERS_BASE})

add_executable(BlaueSim ${BLAU_SOURCE_FILES})

foreach(_source IN ITEMS ${HEADERS})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "${CMAKE_SOURCE_DIR}" "" _group_path "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_group_path}")
    source_group("include\\${_group_path}" FILES "${_source}")
endforeach()

source_group("src" FILES ${SOURCES})

target_sources(BlaueSim PRIVATE ${SOURCES})
target_sources(BlaueSim PUBLIC FILE_SET "HEADERS" BASE_DIRS "include" FILES ${HEADERS})

# Set the include directory.
target_include_directories(BlaueSim PUBLIC include)
# Set the source directory.
target_include_directories(BlaueSim PRIVATE src)

find_package(glm REQUIRED)
find_package(ICU REQUIRED)
find_package(Vulkan REQUIRED)
find_package(EASTL REQUIRED)

target_link_libraries(BlaueSim glm::glm icu::icu Vulkan::Vulkan SDL3::SDL3 EASTL::EASTL)

if(Vulkan_MoltenVK_FOUND)
    target_link_libraries(BlaueSim Vulkan::MoltenVK)
endif()

# Reference EASTL as a shared library.
# Otherwise we have to implement some rather painful new[] functions.
target_compile_definitions(BlaueSim PRIVATE -DEASTL_DLL)

# Set C++20
target_compile_features(BlaueSim PUBLIC cxx_std_20)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
        # using clang with clang-cl front end

        target_compile_options(BlaueSim PRIVATE -Wno-unknown-attributes)
    elseif(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
        # using clang with regular front end

        target_compile_options(BlaueSim PRIVATE -Wno-unknown-attributes)

        # Enable PIC
        set_target_properties(BlaueSim PROPERTIES POSITION_INDEPENDENT_CODE ON)

        # Attempt to enable Link Time Optimization
        set_target_properties(BlaueSim PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON)
    endif()
endif()

if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
    # Disable exceptions and ignore some CRT warnings
    target_compile_definitions(BlaueSim PRIVATE -D_CRT_SECURE_NO_WARNINGS -D_HAS_EXCEPTIONS=1)

    set_target_properties(BlaueSim PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    
    target_compile_options(BlaueSim PRIVATE "$<$<NOT:$<CONFIG:Debug>>:/Zi>")
    target_link_options(BlaueSim PRIVATE "$<$<NOT:$<CONFIG:Debug>>:/DEBUG>")
    target_link_options(BlaueSim PRIVATE "$<$<NOT:$<CONFIG:Debug>>:/OPT:REF>")
    target_link_options(BlaueSim PRIVATE "$<$<NOT:$<CONFIG:Debug>>:/OPT:ICF>")

    check_c_compiler_flag(/wd5030 HAS_UNRECOGNIZED_ATTRIBUTES_WARNING)
    check_c_compiler_flag(/wd4251 HAS_DLL_INTERFACE_WARNING)

    if(HAS_UNRECOGNIZED_ATTRIBUTES_WARNING)
        target_compile_options(BlaueSim PRIVATE /wd5030)
    endif()

    if(HAS_DLL_INTERFACE_WARNING)
        target_compile_options(BlaueSim PRIVATE /wd4251)
    endif()
endif()

install(
    TARGETS ${PROJECT_NAME}
    RUNTIME_DEPENDENCY_SET BlaueDeps
)

install(
    RUNTIME_DEPENDENCY_SET BlaueDeps
    PRE_EXCLUDE_REGEXES
        [=[api-ms-]=]
        [=[ext-ms-]=]
        [[kernel32\.dll]]
        [[libc\.so\..*]] [[libgcc_s\.so\..*]] [[libm\.so\..*]] [[libstdc\+\+\.so\..*]]
    POST_EXCLUDE_REGEXES
        [=[.*system32\/.*\.dll]=]
        [=[^\/(lib|usr\/lib|usr\/local\/lib)]=]
)
