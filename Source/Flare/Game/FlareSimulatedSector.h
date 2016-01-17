
#pragma once

#include "Object.h"
#include "../Spacecrafts/FlareBomb.h"
#include "FlareAsteroid.h"
#include "../Economy/FlarePeople.h"
#include "FlareSimulatedSector.generated.h"

class UFlareSimulatedSpacecraft;
struct FFlareSpacecraftDescription;
class UFlareFleet;
class AFlareGame;
struct FFlarePlayerSave;
struct FFlareResourceDescription;


/** Factory action type values */
UENUM()
namespace EFlareTransportLimitType
{
	enum Type
	{
		Production,
		CargoBay
	};
}

/** Sector friendlyness status */
UENUM()
namespace EFlareSectorFriendlyness
{
	enum Type
	{
		NotVisited, /** The company have not visited the sector yet */
		Neutral, /** The sector have neither friendly spacecraft nor hostile spacecraft */
		Friendly, /** The sector have only friendly spacecraft */
		Contested, /** The sector have both friendly and hostile spacecraft */
		Hostile /** The sector have only hostile spacecraft */
	};
}


/** Sector description */
USTRUCT()
struct FFlareSectorDescription
{
	GENERATED_USTRUCT_BODY()

	/** Name */
	UPROPERTY(EditAnywhere, Category = Content)
	FText Name;

	/** Description */
	UPROPERTY(EditAnywhere, Category = Content)
	FText Description;

	/** Sector identifier */
	UPROPERTY(EditAnywhere, Category = Content)
	FName Identifier;

	/** Orbit phase */
	UPROPERTY(EditAnywhere, Category = Content)
	float Phase;

	/** Peaceful sector */
	UPROPERTY(EditAnywhere, Category = Content)
	bool IsPeaceful;

	/** Is this sector ice-rich */
	UPROPERTY(EditAnywhere, Category = Content)
	bool IsIcy;

	/** Is this sector available for pumping stations */
	UPROPERTY(EditAnywhere, Category = Content)
	bool IsGeostationary;

};


/** Sector orbit description */
USTRUCT()
struct FFlareSectorOrbitDescription
{
	GENERATED_USTRUCT_BODY()

	/** Orbit altitude */
	UPROPERTY(EditAnywhere, Category = Content)
	float Altitude;

	/** Orbit altitude */
	UPROPERTY(EditAnywhere, Category = Content)
	TArray<FFlareSectorDescription> Sectors;
};


/** Sector celestial body description */
USTRUCT()
struct FFlareSectorCelestialBodyDescription
{
	GENERATED_USTRUCT_BODY()
		
	/** Parent celestial body identifier */
	UPROPERTY(EditAnywhere, Category = Content)
	FName CelestialBodyIdentifier;

	/** Name of the celestial body */
	UPROPERTY(EditAnywhere, Category = Content)
	FText CelestialBodyName;

	/** Celestial body radius in pixels on the orbital map */
	UPROPERTY(EditAnywhere, Category = Content)
	int32 CelestialBodyRadiusOnMap;

	/** Celestial body image */
	UPROPERTY(EditAnywhere, Category = Content)
	FSlateBrush CelestialBodyPicture;

	/** Orbit altitude */
	UPROPERTY(EditAnywhere, Category = Content)
	TArray<FFlareSectorOrbitDescription> Orbits;
};


/** Sector orbit parameters */
USTRUCT()
struct FFlareSectorOrbitParameters
{
	GENERATED_USTRUCT_BODY()
	/** Parent celestial body identifier */
	UPROPERTY(EditAnywhere, Category = Save)
	FName CelestialBodyIdentifier;

	/** Orbit altitude */
	UPROPERTY(EditAnywhere, Category = Save)
	double Altitude;

	/** Orbit phase */
	UPROPERTY(EditAnywhere, Category = Save)
	double Phase;
};


/** Sector save data */
USTRUCT()
struct FFlareSectorSave
{
    GENERATED_USTRUCT_BODY()

	/** Given Name */
    UPROPERTY(EditAnywhere, Category = Save)
	FText GivenName;

	/** Sector identifier */
    UPROPERTY(EditAnywhere, Category = Save)
    FName Identifier;

	/*UPROPERTY(VisibleAnywhere, Category = Save)
    TArray<FFlareSpacecraftSave> ShipData;

    UPROPERTY(VisibleAnywhere, Category = Save)
	TArray<FFlareSpacecraftSave> StationData;*/

    UPROPERTY(VisibleAnywhere, Category = Save)
    TArray<FFlareBombSave> BombData;

    UPROPERTY(VisibleAnywhere, Category = Save)
    TArray<FFlareAsteroidSave> AsteroidData;

	UPROPERTY(VisibleAnywhere, Category = Save)
	TArray<FName> FleetIdentifiers;

	UPROPERTY(VisibleAnywhere, Category = Save)
	TArray<FName> SpacecraftIdentifiers;

	/** Last flown identifier */
	UPROPERTY(EditAnywhere, Category = Save)
	FName LastFlownShip;

	/** Local sector time. */
	UPROPERTY(EditAnywhere, Category = Save)
	int64 LocalTime;

	/** Sector peole. */
	UPROPERTY(EditAnywhere, Category = Save)
	FFlarePeopleSave PeopleData;
};


UCLASS()
class FLARE_API UFlareSimulatedSector : public UObject
{
    GENERATED_UCLASS_BODY()

public:
    /*----------------------------------------------------
        Save
    ----------------------------------------------------*/

	/** Load the sector from a save file */
	virtual void Load(const FFlareSectorDescription* Description, const FFlareSectorSave& Data, const FFlareSectorOrbitParameters& OrbitParameters);

	/** Load sector people */
	virtual UFlarePeople* LoadPeople(const FFlarePeopleSave& PeopleData);

	/** Save the sector to a save file */
    virtual FFlareSectorSave* Save();


    /*----------------------------------------------------
        Gameplay
    ----------------------------------------------------*/

    /** Create a station in the level  for a specific company */
	UFlareSimulatedSpacecraft* CreateStation(FName StationClass, UFlareCompany* Company, FVector TargetPosition, FRotator TargetRotation = FRotator::ZeroRotator, FName AttachPoint = NAME_None);

    /** Create a ship in the level  for a specific company */
	UFlareSimulatedSpacecraft* CreateShip(FName ShipClass, UFlareCompany* Company, FVector TargetPosition);

	/** Create a ship or station in the level  for a specific company. No null parameter accepted */
	UFlareSimulatedSpacecraft* CreateShip(FFlareSpacecraftDescription* ShipDescription, UFlareCompany* Company, FVector TargetLocation, FRotator TargetRotation = FRotator::ZeroRotator, FName AttachPoint = NAME_None);

	/** Create an asteroid */
	void CreateAsteroid(int32 ID, FName Name, FVector Location);

	/** Add the fleet and its ships from the sector */
	void AddFleet(UFlareFleet* Fleet);

	/** Remove the fleet but let its ships in the sector */
	void DisbandFleet(UFlareFleet* Fleet);

	/** Retire the fleet and its ships from the sector */
	void RetireFleet(UFlareFleet* Fleet);

	int RemoveSpacecraft(UFlareSimulatedSpacecraft* Spacecraft);

	void SetShipToFly(UFlareSimulatedSpacecraft* Ship);

	bool CanBuildStation(FFlareSpacecraftDescription* StationDescription, UFlareCompany* Company);

	bool BuildStation(FFlareSpacecraftDescription* StationDescription, UFlareCompany* Company);

	void SimulateTransport();

	void SimulateTransport(UFlareCompany* Company);

	void AdaptativeTransportResources(UFlareCompany* Company, uint32& TransportCapacity, EFlareTransportLimitType::Type TransportLimitType, uint32 TransportLimit, bool ActiveOnly);

	uint32 TakeUselessResources(UFlareCompany* Company, FFlareResourceDescription* Resource, uint32 QuantityToTake);

	uint32 TakeResources(UFlareCompany* Company, FFlareResourceDescription* Resource, uint32 QuantityToTake);

	uint32 GiveResources(UFlareCompany* Company, FFlareResourceDescription* Resource, uint32 QuantityToGive);

	uint32 AdaptativeGiveResources(UFlareCompany* Company, FFlareResourceDescription* GivenResource, uint32 QuantityToGive, EFlareTransportLimitType::Type TransportLimitType, uint32 TransportLimit, bool ActiveOnly, bool StorageOnly);

protected:

    /*----------------------------------------------------
        Protected data
    ----------------------------------------------------*/

    // Gameplay data
	FFlareSectorSave                        SectorData;

    TArray<UFlareSimulatedSpacecraft*>      SectorStations;
    TArray<UFlareSimulatedSpacecraft*>      SectorShips;
	TArray<UFlareSimulatedSpacecraft*>      SectorSpacecrafts;

	TArray<UFlareFleet*>                    SectorFleets;

	AFlareGame*                             Game;
	FFlareSectorOrbitParameters             SectorOrbitParameters;
	const FFlareSectorDescription*          SectorDescription;

	UPROPERTY()
	UFlarePeople*							People;

	int32                                   PersistentStationIndex;

public:

    /*----------------------------------------------------
        Getters
    ----------------------------------------------------*/

    AFlareGame* GetGame() const
    {
		return Game;
    }

    inline FName GetIdentifier() const
    {
        return SectorData.Identifier;
    }

	inline const FFlareSectorDescription* GetDescription() const
	{
		return SectorDescription;
	}

	inline FFlareSectorOrbitParameters* GetOrbitParameters()
	{
		return &SectorOrbitParameters;
	}

	/** Get the name of this sector */
	FText GetSectorName() const;

	/** Get the description of this sector */
	FText GetSectorDescription() const;

	FString GetSectorCode() const;

    inline TArray<UFlareSimulatedSpacecraft*>& GetSectorStations()
    {
        return SectorStations;
    }

    inline TArray<UFlareSimulatedSpacecraft*>& GetSectorShips()
    {
        return SectorShips;
    }

	inline TArray<UFlareSimulatedSpacecraft*>& GetSectorSpacecrafts()
	{
		return SectorSpacecrafts;
	}

	inline TArray<UFlareFleet*>& GetSectorFleets()
	{
		return SectorFleets;
	}

	inline UFlarePeople* GetPeople()
	{
		return People;
	}



	/** Get the friendlyness status toward a company */
	EFlareSectorFriendlyness::Type GetSectorFriendlyness(UFlareCompany* Company) const;

	/** Get the friendlyness status toward a company, as a text */
	FText GetSectorFriendlynessText(UFlareCompany* Company) const;

	/** Get the friendlyness status toward a company, as a color */
	FLinearColor GetSectorFriendlynessColor(UFlareCompany* Company) const;

	uint32 GetTransportCapacity(UFlareCompany* Company);

	uint32 GetResourceCount(UFlareCompany* Company, FFlareResourceDescription* Resource);

	uint32 GetResourcePrice(FFlareResourceDescription* Resource);
};
