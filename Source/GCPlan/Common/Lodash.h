#pragma once

class Lodash {
	bool seeded = false;

public:
	Lodash();
	~Lodash();

	static FString GetInstanceId(FString prefix = "", int numChars = 10);
	static int RandomRangeInt(int min, int max);
	static float RandomRangeFloat(float min, float max);
	static float RangeValue(float value, float valueMin, float valueMax, float newStart, float newEnd);
	static FString ToFixed(float value, int digits = 2);
	// static T Max(TArray<T> values);
	// static T Min(TArray<T> values);
};
