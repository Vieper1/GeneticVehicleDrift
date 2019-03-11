// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Genetic_DriftCarTarget : TargetRules
{
	public Genetic_DriftCarTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.Add("Genetic_DriftCar");
	}
}
