// Fill out your copyright notice in the Description page of Project Settings.


#include "BigCoinItem.h"

ABigCoinItem::ABigCoinItem()
{
	PointValue=50;
	ItemType = "BigCoin";
}

void ABigCoinItem::ActivateItem(AActor* activator)
{
	Super::ActivateItem(activator);
}