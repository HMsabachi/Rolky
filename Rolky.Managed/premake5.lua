include "../Premake/CSExtensions.lua"

project "Rolky.Managed"
    language "C#"
    dotnetframework "net9.0"
    kind "SharedLib"

    -- Don't specify architecture here. (see https://github.com/premake/premake-core/issues/1758)

    propertytags { { "AppendTargetFrameworkToOutputPath", "false" } }

    files {
        "Source/**.cs"
    }