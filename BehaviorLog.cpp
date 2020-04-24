#include <api/Loader.h>
#include <string>
#include <iostream>
#include <time.h>
#include <chrono>
#include <stdio.h>
#include <fstream>
#include "Minecraft.hpp"
#include <future>
#include<api\types\helper.h>
#include<sstream>
#include "behavior.h"
extern LangPack LP;
using namespace std;
extern bool mode;

using namespace std;

extern bool LogUseItem;
extern bool LogChest;
extern bool LogPlace;
extern bool LogDestory;
extern bool LogCDim;

extern bool showLogUseItem;
extern bool showLogChest;
extern bool showLogBlock;
extern bool showLogCDim;

extern bool logfd;
extern bool logfh;

extern std::string logfilec;


//use item
THook(
	bool, "?useItemOn@GameMode@@UEAA_NAEAVItemStack@@AEBVBlockPos@@EAEBVVec3@@PEBVBlock@@@Z", void* _this,
	ItemStack& item, BlockPos const& blkpos, unsigned char a4, Vec3 const& v5, Block const* blk)
{
	auto sp = *reinterpret_cast<Player**>(reinterpret_cast<unsigned long long>(_this) + 8);
	std::string mstr = item.getName();
	bool vaule = original(_this, item, blkpos, a4, v5, blk);
	if (mode) {
		if (LogUseItem) {
			if (vaule) {
				logger::Item("Operate", sp->getNameTag(), sp->isStand(), sp->getDimension(), _TRS("word.operate"), mstr, blkpos.getPosition());
			}
		}
	}
	else
		mode = true;
	return vaule;
}
//dest block
THook(bool, "?_destroyBlockInternal@GameMode@@AEAA_NAEBVBlockPos@@E@Z", void* _this, BlockPos const& blkpos) {
	auto sp = *reinterpret_cast<Player**>(reinterpret_cast<unsigned long long>(_this) + 8);
	auto bs = *(BlockSource**)(*((__int64*)_this + 1) + 800i64);
	auto& blk = bs->getBlock(blkpos);
	auto bname = blk.getLegacyBlock().getFullName();
	bool vaule = original(_this, blkpos);
	if (LogDestory) {
		if (vaule) {
			if (bname.find("minecraft:") != string::npos)
				bname = bname.replace(bname.find("minecraft:"), 10, "");
			logger::Block("Destroy", sp->getNameTag().c_str(), sp->isStand(), sp->getDimension(), _TRS("word.destroyed"), bname, blkpos.getPosition());
		}
	}
	return vaule;
}

//place block
THook(
	__int64,
	"?onBlockPlacedByPlayer@VanillaServerGameplayEventListener@@UEAA?AW4EventResult@@AEAVPlayer@@AEBVBlock@@"
	"AEBVBlockPos@@_N@Z",
	void* _this, Player* sp, const Block* blk, const BlockPos* blkpos, bool _bool) {
	if (LogPlace) {
		mode = false;
		string bname = blk->getLegacyBlock().getFullName();
		if (bname.find("minecraft:") != string::npos)
			bname = bname.replace(bname.find("minecraft:"), 10, "");
		logger::Block(
			"Place", sp->getNameTag().c_str(), sp->isStand(), sp->getDimension(), _TRS("word.placed"),
			bname, blkpos->getPosition());
	}
	return original(_this, sp, blk, blkpos, _bool);
}
//open Chest
THook(void, "?startOpen@ChestBlockActor@@UEAAXAEAVPlayer@@@Z", void* _this, Player* sp) {
	auto real_this = reinterpret_cast<void*>(reinterpret_cast<unsigned long long>(_this) - 248);
	auto blkpos = reinterpret_cast<BlockActor*>(real_this)->getPosition();
	if (LogChest) {
		logger::Interaction(
			"Chest", sp->getNameTag().c_str(), sp->isStand(), sp->getDimension(), _TRS("word.open"), _TRS("word.chest"),
			blkpos->getPosition());
		//std::printf("onopenchest\n");
		mode = false;
		original(_this, sp);
	}
}
//close chest
THook(__int64, "?stopOpen@ChestBlockActor@@UEAAXAEAVPlayer@@@Z", void* _this, Player* sp) {
	auto real_this = reinterpret_cast<void*>(reinterpret_cast<unsigned long long>(_this) - 248);
	auto blkpos = reinterpret_cast<BlockActor*>(real_this)->getPosition();
	if (LogChest) {
		logger::Interaction(
			"Chest", sp->getNameTag().c_str(), sp->isStand(), sp->getDimension(), _TRS("word.close"), _TRS("word.chest"),
			blkpos->getPosition());
		//std::printf("oncchest\n");
	}
	return original(_this, sp);
}


//open Barrel
THook(void, "?startOpen@BarrelBlockActor@@UEAAXAEAVPlayer@@@Z", void* _this, Player* sp) {
	auto real_this = reinterpret_cast<void*>(reinterpret_cast<unsigned long long>(_this) - 248);
	auto blkpos = reinterpret_cast<BlockActor*>(real_this)->getPosition();
	if (LogChest) {
		logger::Interaction(
			"Barrel", sp->getNameTag().c_str(), sp->isStand(), sp->getDimension(), _TRS("word.open"), _TRS("word.barrel"),
			blkpos->getPosition());
		mode = false;
	}
	original(_this, sp);
}
//close Barrel
THook(__int64, "?stopOpen@BarrelBlockActor@@UEAAXAEAVPlayer@@@Z", void* _this, Player* sp) {
	auto real_this = reinterpret_cast<void*>(reinterpret_cast<unsigned long long>(_this) - 248);
	auto blkpos = reinterpret_cast<BlockActor*>(real_this)->getPosition();
	if (LogChest) {
		logger::Interaction(
			"Barrel", sp->getNameTag().c_str(), sp->isStand(), sp->getDimension(), _TRS("word.close"), _TRS("word.barrel"),
			blkpos->getPosition());
	}
	return original(_this, sp);
}


//open Dispenser
THook(void, "?startOpen@DispenserBlockActor@@UEAAXAEAVPlayer@@@Z", void* _this, Player* sp) {
	auto real_this = reinterpret_cast<void*>(reinterpret_cast<unsigned long long>(_this) - 248);
	auto blkpos = reinterpret_cast<BlockActor*>(real_this)->getPosition();
	if (LogChest) {
		logger::Interaction(
			"Dispenser", sp->getNameTag().c_str(), sp->isStand(), sp->getDimension(), _TRS("word.open"), _TRS("word.dispenser"),
			blkpos->getPosition());
		mode = false;
	}
	original(_this, sp);
}

//open Furnace
THook(void, "?startOpen@FurnaceBlockActor@@UEAAXAEAVPlayer@@@Z", void* _this, Player* sp) {
	auto real_this = reinterpret_cast<void*>(reinterpret_cast<unsigned long long>(_this) - 248);
	auto blkpos = reinterpret_cast<BlockActor*>(real_this)->getPosition();
	if (LogChest) {
		logger::Interaction(
			"Furnace", sp->getNameTag().c_str(), sp->isStand(), sp->getDimension(), _TRS("word.open"), _TRS("word.furnace"),
			blkpos->getPosition());
		mode = false;
	}
	original(_this, sp);
}

//cotainer i/o
THook(
	void, "?setSlot@LevelContainerManagerModel@@UEAAXHAEBUContainerItemStack@@_N@Z", LevelContainerManagerModel* _this,
	int slot, ContainerItemStack* itemst) {
	auto pItemStack = itemst;
	auto id = pItemStack->getId();
	auto size = pItemStack->getStackSize();
	auto sp = _this->getPlayer();
	if (LogChest) {
		if (id >= 0) {
			std::string object_name = pItemStack->getName();
			if (size == 0) {
				logger::Container_Out("Chest", sp->getNameTag().c_str(), sp->getDimension(), slot);
			}
			else
				logger::Container_In(
					"Chest", sp->getNameTag().c_str(), sp->getDimension(), slot, size, object_name);
			//std::printf("on put");
		}
		else
			logger::Error("Warning", sp->getNameTag().c_str(), sp->getDimension(), _TRS("unknow.block"));
		//std::printf("on put ?");
	}
	original(_this, slot, itemst);
}

//切换维度
THook(
	bool, "?_playerChangeDimension@Level@@AEAA_NPEAVPlayer@@AEAVChangeDimensionRequest@@@Z", void* _this,
	Player* sp, void* req) {
	bool vaule = original(_this, sp, req);
	if (LogCDim) {
		if (vaule)
			logger::ChangeDimension("Dimension", sp->getNameTag().c_str(), sp->getDimension(), sp->getPos());
		//std::printf("onchangedim\n");
	}

	return vaule;

}
void bh_ownapi_entery(){
	loadconf();
}