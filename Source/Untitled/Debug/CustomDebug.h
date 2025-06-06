// @RC

#pragma once

namespace Debug
{
	static void Print(const FString& Msg, int32 InKey =-1,const FColor Color = FColor::MakeRandomColor() )
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(InKey, 4.f,Color,Msg);
		}

		UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
	}
}
