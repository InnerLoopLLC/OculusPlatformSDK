// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class LibOVRPlatform : ModuleRules
{
	public LibOVRPlatform(TargetInfo Target)
	{
		Type = ModuleType.External;		
		
		string OculusThirdPartyDirectory = Path.Combine( ThirdPartyPath, "Oculus/LibOVRPlatform/LibOVRPlatform");

        bool isLibrarySupported = true;
		
		if (Target.Platform == UnrealTargetPlatform.Win32)
		{
			PublicAdditionalLibraries.Add(OculusThirdPartyDirectory + "/lib/LibOVRPlatform32_1.lib");
			isLibrarySupported = true;
		}
		else if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PublicAdditionalLibraries.Add(OculusThirdPartyDirectory + "/lib/LibOVRPlatform64_1.lib");
			isLibrarySupported = true;
		}
		else
		{
			System.Console.WriteLine("Oculus Platform SDK not supported for this platform");
		}
		
		if (isLibrarySupported)
		{
			PublicIncludePaths.Add(Path.Combine( OculusThirdPartyDirectory, "include" ));
		}
	}
	
	private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath( Path.Combine( ModulePath, "../../../ThirdParty" ) ); }
    }
}
