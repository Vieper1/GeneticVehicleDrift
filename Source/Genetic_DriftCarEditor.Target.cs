// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Genetic_DriftCarEditorTarget : TargetRules
{
	public Genetic_DriftCarEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.Add("Genetic_DriftCar");
	}
}
