#pragma once

#include "../Items/ItemBaseStruct.h"
#include "../Pokemon/PokemonStruct.h"
#include "WidgetDelegates.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemSlotSignature, UItemSlotWidget*, ItemSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShopSlotSignature, UItemShopSlotWidget*, ItemShopSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemInfoSignature, UItemInfoWidget*, ItemInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPokemonSlotSignature, UPokemonSlotWidget*, PokemonSlot);
