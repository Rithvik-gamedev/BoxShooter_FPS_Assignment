// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxActor/BoxSpawner.h"

#include "HttpModule.h"
#include "BoxActor/BoxActor.h"
#include "Interfaces/IHttpResponse.h"

// Sets default values
ABoxSpawner::ABoxSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoxSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (!DataURL.IsEmpty())
	{
		FetchData();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DataURL is not set."));
	}
	
}

void ABoxSpawner::FetchData()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("GET");
	HttpRequest->SetURL(DataURL);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &ABoxSpawner::OnDataReceived);
	HttpRequest->ProcessRequest();
}

void ABoxSpawner::OnDataReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully)
{
	if (bProcessedSuccessfully && Response.IsValid() && Response->GetResponseCode() == 200)
	{
		FString ResponseString = Response->GetContentAsString();
		UE_LOG(LogTemp, Log, TEXT("JSON Data Received:\n%s"), *ResponseString);
		ParseAndSpawnActors(ResponseString);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to retrieve JSON data. HTTP Status Code: %d"), Response.IsValid() ? Response->GetResponseCode() : -1);
	}
}

void ABoxSpawner::ParseAndSpawnActors(const FString& String)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(String);

	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		const TArray<TSharedPtr<FJsonValue>>* BoxTypesArray;
		if (JsonObject->TryGetArrayField(TEXT("types"), BoxTypesArray))
		{
			for (const TSharedPtr<FJsonValue>& TypesArray : *BoxTypesArray)
			{
				TSharedPtr<FJsonObject> TypeObject = TypesArray->AsObject();
				if (TypeObject.IsValid())
				{
					FBoxTypeData TypeData;
					TypeData.Name = TypeObject->GetStringField(TEXT("name"));
					TypeData.Health = TypeObject->GetIntegerField(TEXT("health"));
					TypeData.Score = TypeObject->GetIntegerField(TEXT("score"));
					
					const TArray<TSharedPtr<FJsonValue>>* ColorArray;
					if (TypeObject->TryGetArrayField(TEXT("color"), ColorArray))
					{
						TypeData.Color.R = ColorArray->operator[](0)->AsNumber() / 255.f;
						TypeData.Color.G = ColorArray->operator[](1)->AsNumber() / 255.f;
						TypeData.Color.B = ColorArray->operator[](2)->AsNumber() / 255.f;
						TypeData.Color.A = 1.f;
					}
					else
					{
						TypeData.Color = FLinearColor::White;
						UE_LOG(LogTemp, Warning, TEXT("Color data missing or invalid. Defaulting to white"))
					}
					BoxTypeMap.Add(TypeData.Name, TypeData);
					
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("JSON does not contain 'types' array"));
			return;
		}

		const TArray<TSharedPtr<FJsonValue>>* ObjectsArray;
		if (JsonObject->TryGetArrayField(TEXT("objects"), ObjectsArray))
		{
			for (const TSharedPtr<FJsonValue>& ObjectEntry : *ObjectsArray)
			{
				TSharedPtr<FJsonObject> ObjectJson = ObjectEntry->AsObject();
				if (ObjectJson.IsValid())
				{
					FBoxObjectData ObjectData;
					ObjectData.Type = ObjectJson->GetStringField(TEXT("type"));
					
					const TSharedPtr<FJsonObject>* TransformObjectPtr;
					if (ObjectJson->TryGetObjectField(TEXT("transform"), TransformObjectPtr) && TransformObjectPtr->IsValid())
					{
						TSharedPtr<FJsonObject> TransformObject = *TransformObjectPtr;

						// Parse Location array
						const TArray<TSharedPtr<FJsonValue>>* LocationArray;
						if (TransformObject->TryGetArrayField(TEXT("location"), LocationArray) && LocationArray->Num() == 3)
						{
							ObjectData.Transform.Location.X = LocationArray->operator[](0)->AsNumber();
							ObjectData.Transform.Location.Y = LocationArray->operator[](1)->AsNumber();
							ObjectData.Transform.Location.Z = LocationArray->operator[](2)->AsNumber();
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("Location data missing or invalid for object type: %s. Defaulting to (0,0,0)."), *ObjectData.Type);
							ObjectData.Transform.Location = FVector::ZeroVector;
						}

						// Parse Rotation array [roll, pitch, yaw]
						const TArray<TSharedPtr<FJsonValue>>* RotationArray;
						if (TransformObject->TryGetArrayField(TEXT("rotation"), RotationArray) && RotationArray->Num() == 3)
						{
							// FRotator constructor takes (Pitch, Yaw, Roll)
							ObjectData.Transform.Rotation.Roll = RotationArray->operator[](0)->AsNumber();
							ObjectData.Transform.Rotation.Pitch = RotationArray->operator[](1)->AsNumber();
							ObjectData.Transform.Rotation.Yaw = RotationArray->operator[](2)->AsNumber();
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("Rotation data missing or invalid for object type: %s. Defaulting to (0,0,0)."), *ObjectData.Type);
							ObjectData.Transform.Rotation = FRotator::ZeroRotator;
						}

						// Parse Scale array
						const TArray<TSharedPtr<FJsonValue>>* ScaleArray;
						if (TransformObject->TryGetArrayField(TEXT("scale"), ScaleArray) && ScaleArray->Num() == 3)
						{
							ObjectData.Transform.Scale.X = ScaleArray->operator[](0)->AsNumber();
							ObjectData.Transform.Scale.Y = ScaleArray->operator[](1)->AsNumber();
							ObjectData.Transform.Scale.Z = ScaleArray->operator[](2)->AsNumber();
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("Scale data missing or invalid for object type: %s. Defaulting to (1,1,1)."), *ObjectData.Type);
							ObjectData.Transform.Scale = FVector::OneVector;
						}

						SpawnBoxActor(ObjectData);
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Transform data missing or invalid for object type: %s. Skipping this object."), *ObjectData.Type);
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Invalid object entry found in JSON 'objects' array."));
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("JSON data does not contain an 'objects' array at the root."));
		}
	}
}

ABoxActor* ABoxSpawner::SpawnBoxActor(const FBoxObjectData& ObjectData)
{
	if (!BoxActorClass)
	{
		UE_LOG(LogTemp, Error, TEXT("BoxActorClass is not set in the Blueprint."));
		return nullptr;
	}

	const FBoxTypeData* TypeData = BoxTypeMap.Find(ObjectData.Type);
	if (!TypeData)
	{
		UE_LOG(LogTemp, Warning, TEXT("TypeData is not found."));
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ABoxActor* SpawnedActor = GetWorld()->SpawnActor<ABoxActor>(BoxActorClass, ObjectData.Transform.Location, ObjectData.Transform.Rotation, SpawnParams);
	if (SpawnedActor)
	{
		SpawnedActor->SetBoxProperties(TypeData->Name, TypeData->Color, TypeData->Health, TypeData->Score);
		return SpawnedActor;
	}
	else return nullptr;
}

// Called every frame
void ABoxSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

