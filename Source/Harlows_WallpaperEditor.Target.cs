// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class Harlows_WallpaperEditorTarget : TargetRules
{
	public Harlows_WallpaperEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "Harlows_Wallpaper" } );
	}
}
