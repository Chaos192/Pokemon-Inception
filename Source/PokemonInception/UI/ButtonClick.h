#pragma once

#include "../Items/ItemBaseStruct.h"
#include "../Pokemon/PokemonBaseStruct.h"
#include "../Pokemon/PokemonStruct.h"
#include "ButtonClick.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FButtonClicked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShopSignature, FItemBaseStruct, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPokedexSignature, FPokemonBaseStruct, PokedexData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPokemonSignature, FPokemonStruct, Pokemon);
