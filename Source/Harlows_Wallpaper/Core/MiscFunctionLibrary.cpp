// Fill out your copyright notice in the Description page of Project Settings.

#include "MiscFunctionLibrary.h"

// TODO the compiler refused to compile when I defined the function here seperately from it's 
// declaration in the header. If someone knows why this doesn't work here, feel free to fix it and
// remove the definition in the header so the header is neater. Also, let DJ know what the fix was!!
// template<typename T>
// FString UMiscFunctionLibrary::EnumToString(const char* EnumName, const T EnumValue)
// {
// 	UEnum* enumObj = FindObject<UEnum>(ANY_PACKAGE, UTF8_TO_TCHAR(EnumName));
// 	return *(enumObj ? enumObj->GetNameStringByIndex(static_cast<uint8>(EnumValue)) : "null");
// }