// Copyright 2023 DME Games

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SciFiEscapeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USciFiEscapeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SCIFIESCAPE_API ISciFiEscapeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Making the function = 0 means it does not need an implementation in this class
	virtual void Host() = 0;
};
