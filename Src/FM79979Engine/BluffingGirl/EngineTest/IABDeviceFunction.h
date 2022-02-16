#ifndef _IAB_DEVICE_FUNCTION_H_
#define _IAB_DEVICE_FUNCTION_H_

#include "IABSetup.h"
//in Android the code written in AndroidDeviceFunction.cpp
//
//

bool		CallIABRCreate();
bool		AddSKUData(const char* e_strSKUID,const char* e_strName,int e_iType,bool e_bConsumable);
bool		PurchaseProduct(char*e_strProductionID,ePurchaseType e_ePurchaseType,char*e_strPayLoadDataForCheck);
bool		IsProductsPurchased(char*e_strProductionID,ePurchaseType e_ePurchaseType);
bool		IsWaitingForRespond();

#endif