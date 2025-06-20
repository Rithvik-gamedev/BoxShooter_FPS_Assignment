// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHttpRequest.h"
#include "BoxSpawner.generated.h"


class ABoxActor;

// structs for JSON data based on the provided schema
USTRUCT(BlueprintType)
struct FBoxTypeData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box Type")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box Type")
	
	FLinearColor Color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box Type")
	int32 Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box Type")
	int32 Score;
};

USTRUCT(BlueprintType)
struct FBoxTransform
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box Transform")
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box Transform")
	FRotator Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box Transform")
	FVector Scale;
	
};

USTRUCT(BlueprintType)
struct FBoxObjectData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box Object Data")
	FString Type; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box Object Data")
	FBoxTransform Transform;
};


USTRUCT(BlueprintType)
struct FRootJsonData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Root Data")
	TArray<FBoxTypeData> Types;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Root Data")
	TArray<FBoxObjectData> Objects;
};

UCLASS()
class FPS_ASSIGNMENT_API ABoxSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values Box properties
	ABoxSpawner();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Function to initiate the JSON data download
	void FetchData();

	//Callback function for when the HTTP request completes
	void OnDataReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully);

	//Function to parse and spawn the box
	void ParseAndSpawnActors(const FString& String);

	//Function to spawn BoxActor
	ABoxActor* SpawnBoxActor(const FBoxObjectData& ObjectData);


protected:

	// URL to fetch JSON Data
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = " Spawn Data")
	FString DataURL;

	//BoxActor Class to spawn at runtime
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Data")
	TSubclassOf<AActor> BoxActorClass;
	
	TMap<FString, FBoxTypeData> BoxTypeMap;

};
