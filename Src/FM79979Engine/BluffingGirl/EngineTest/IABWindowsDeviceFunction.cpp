#include "stdafx.h"
#include "IABSetup.h"

bool		CallIABRCreate()
{

	return true;
}

bool		AddSKUData(const char* e_strSKUID,const char* e_strName,int e_iType,bool e_bConsumable)
{
	return true;
}

bool			PurchaseProduct(char*e_strProductionID,ePurchaseType e_ePurchaseType,char*e_strPayLoadDataForCheck)
{
	return true;
}


bool		IsProductsPurchased(char*e_strProductionID,ePurchaseType e_ePurchaseType)
{
	return false;
}

bool		IsWaitingForRespond()
{
	return false;
}
