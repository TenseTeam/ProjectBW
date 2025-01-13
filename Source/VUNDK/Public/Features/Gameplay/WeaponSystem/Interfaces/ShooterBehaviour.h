#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ShooterBehaviour.generated.h"

UINTERFACE(BlueprintType)
class VUNDK_API UShooterBehaviour : public UInterface
{
	GENERATED_BODY()
};

class VUNDK_API IShooterBehaviour
{
	GENERATED_BODY()

public:
	/**
	 * @brief Starts the shoot behaviour.
	 * @return True if the shoot was successful, false otherwise.
	 */
	virtual bool Shoot() = 0;

	/**
	 * @brief Refills an amount of ammo.
	 * @param Ammo The amount of ammo to refill.
	 * @return The rest of the ammo that could not be refilled.
	 */
	virtual int32 Refill(const int32 Ammo) = 0;

	/**
	 * @brief Refills the magazine to the maximum.
	 */
	virtual void RefillAllMagazine() = 0;

	/**
	 * @brief Gets the current ammo.
	 * @return The current ammo.
	 */
	virtual int32 GetCurrentAmmo() const = 0;
};
