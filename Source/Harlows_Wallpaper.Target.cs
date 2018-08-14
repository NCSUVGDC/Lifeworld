// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class Harlows_WallpaperTarget : TargetRules
{
	public Harlows_WallpaperTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "Harlows_Wallpaper" } );
	}
}
