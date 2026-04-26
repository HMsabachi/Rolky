local RolkyDotNetPath = os.getenv("ROLKY_DOTNET_PATH")

project "Rolky.Native"
    language "C++"
    cppdialect "C++20"
    kind "StaticLib"

    -- Can't specify 64-bit architecture in the workspace level since VS 2022 (see https://github.com/premake/premake-core/issues/1758)
    architecture "x86_64"

    pchheader "RolkyPCH.hpp"
    pchsource "Source/RolkyPCH.cpp"

    forceincludes { "RolkyPCH.hpp" }

    files {
        "Source/**.cpp",
        "Source/**.hpp",

        "Include/Rolky/**.hpp",
    }

    includedirs { "Source/", "Include/Rolky/" }
    externalincludedirs { RolkyDotNetPath }