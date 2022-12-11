#pragma once

#include "../Items/ItemBaseStruct.h"
#include "ButtonClick.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FButtonClicked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShopSignature, FItemBaseStruct, Item);
