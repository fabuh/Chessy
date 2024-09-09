function GetScriptAbsoluteDir()
	local str = debug.getinfo(2, "S").source:sub(2)
	return str:match("(.*/)")
end
local checkout_abs_dir = GetScriptAbsoluteDir() .. "../../"
-----------------------------------------------------------------------------------
function AddConfig()
	systemversion("latest")
	
	configurations { "Debug", "Release" }
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
	filter {}
	
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
	filter {}
	
	platforms { "x32", "x64" }
	filter "platforms:x32"
		architecture "x32"
	filter{}
	
	filter "platforms:x64"
		architecture "x64"
	filter{}
end
-----------------------------------------------------------------------------------
workspace "Chessy"
	startproject "Chessy"
	location(checkout_abs_dir)
	AddConfig()
	
	project("Chessy")
		language "C++"
		cppdialect "c++20"
		--kind "ConsoleApp"
		kind "WindowedApp"
		targetname("Chessy")
		------------------------------------------------------------------------------
		AddConfig()
		
		location(checkout_abs_dir .. "prj/" .. _ACTION)
		objdir (checkout_abs_dir .. "prj/" .. _ACTION .. "/obj/%{cfg.buildcfg}_%{cfg.platform}")
		targetdir (checkout_abs_dir .. "build/%{cfg.platform}")
		------------------------------------------------------------------------------
		-- files
		------------------------------------------------------------------------------
		files
		{
			checkout_abs_dir .. "src/**"
		}
		------------------------------------------------------------------------------
		-- includes
		------------------------------------------------------------------------------
		includedirs
		{
			checkout_abs_dir .. "src",
		}
		------------------------------------------------------------------------------
		-- pch
		------------------------------------------------------------------------------
		pchheader("pch.h")
		pchsource("../../src/pch.cpp") -- relative to current lua file
		------------------------------------------------------------------------------
		-- links
		------------------------------------------------------------------------------
		includedirs
		{
			checkout_abs_dir .. "/libs/SDL2-2.30.3/include",
			checkout_abs_dir .. "/libs/SDL2_image-2.8.2/include"
		}
		libdirs
		{
			checkout_abs_dir .. "/libs/SDL2-2.30.3/lib/x64",
			checkout_abs_dir .. "/libs/SDL2_image-2.8.2/lib/x64"
		}
		
		links { "SDL2", "SDL2_image" }
	------------------------------------------------------------------------------
	------------------------------------------------------------------------------