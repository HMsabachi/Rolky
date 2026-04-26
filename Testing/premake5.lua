local RolkyDotNetPath = os.getenv("ROLKY_DOTNET_PATH")

project "Testing"
    language "C++"
    cppdialect "C++20"
    kind "ConsoleApp"

    -- Can't specify 64-bit architecture in the workspace level since VS 2022 (see https://github.com/premake/premake-core/issues/1758)
    architecture "x86_64"

    files {
        "Source/**.cpp",
        "Source/**.hpp",
    }

    externalincludedirs { "../Rolky.Native/Include/" }

    libdirs { RolkyDotNetPath }

    links {
        "Rolky.Native",

        "nethost",
        "libnethost",
        "ijwhost",
    }

    postbuildcommands {
        '{ECHO} Copying "' .. RolkyDotNetPath .. '/nethost.dll" to "%{cfg.targetdir}"',
        '{COPYFILE} "' .. RolkyDotNetPath .. '/nethost.dll" "%{cfg.targetdir}"',
    }

    filter { "configurations:Debug" }
        defines { "ROLKY_TESTING_DEBUG" }

    filter { "configurations:Release" }
        defines { "ROLKY_TESTING_RELEASE" }