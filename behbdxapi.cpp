#include <lbpch.h>
#include <stl\KVDB.h>
#include <api\myPacket.h>
#include <mcapi/Player.h>
#include <stl\varint.h>
#include "behavior.h"
#include <api\xuidreg\xuidreg.h>
#include <api\types\helper.h>
#include <api\command\commands.h>
#include <mcapi/Core.h>
#include <mcapi/Actor.h>
#include <mcapi/Level.h>
#include <mcapi\Certificate.h>
#include <mcapi/BlockSource.h>
#include <mcapi/core.h>
#include <mcapi/mass.h>
#include <mcapi/BlockSource.h>
#include <stl\langPack.h>
extern bool mode;
using namespace std;
typedef signed int          INT32, * PINT32;

extern bool logfd;
extern bool logfh;

extern bool logCmdMlock;
extern bool CmdBlockWriteable;
extern std::string logfilec;
extern bool LogCMD;
extern bool LogJoin;
extern bool LogLeft;
extern bool LogChat;
extern LangPack LP;

THook(void, "?_setCommand@BaseCommandBlock@@AEAAXAEAVBlockSource@@AEBVCommandOrigin@@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z",
	void* _this, BlockSource& a1, CommandOrigin const& a2, string const& a3) {
	//auto sp = MakeSP(a2);
	//cout << sp->getNameTag() << endl;
	if (CmdBlockWriteable)
		original(_this, a1, a2, a3);
	if (logCmdMlock)
		logger::cmdbw("WriteCommandBlock",_TRS("word.writecmdblock"), a3, CmdBlockWriteable ? _TRS("word.success") : _TRS("word.denied"));
}

THook(void, "?openCommandBlockMinecart@ServerPlayer@@UEAAXAEBUActorUniqueID@@@Z",
	Player* _this, struct ActorUniqueID *a1) {
	Actor* Minecrat = LocateS<ServerLevel>()->fetchEntity(*a1, false);
	original(_this, a1);
	if (logCmdMlock)logger::cmdbm("OpenCommandBlock", _this->getNameTag(), Minecrat->getPos(),Minecrat->getDimensionId());
	mode = false;
}
THook(void, "?openCommandBlock@ServerPlayer@@UEAAXAEBVBlockPos@@@Z",
	Player* _this, const BlockPos* a1) {
	if (logCmdMlock)logger::cmdbb("OpenCommandBlock", _this->getNameTag(), ("(" + to_string(a1->x) + ", " + to_string(a1->y) + ", " + to_string(a1->z) + ")"),_this->getDimensionId());
	original(_this, a1);
	mode = false;
}
THook(void,
	"?die@Mob@@UEAAXAEBVActorDamageSource@@@Z",
	Mob* _this, ActorDamageSource* a2) {
	if(LogDie)
	if (!_this->getNameTag().empty()) {
		string Mob_name = _this->getNameTag();
		string Mob_type_name = SymCall("?getEntityName@@YA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AEBVActor@@@Z", string, Mob*)(_this);
		ActorUniqueID src_id = a2->getEntityUniqueID();
		Actor* Src = LocateS<ServerLevel>()->fetchEntity(src_id, false);
		string src_name, mob_name;
		if (Src) {
			string Src_name = Src->getNameTag();
			string Src_type_name = SymCall("?getEntityName@@YA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AEBVActor@@@Z", string, Actor*)(Src);
			if (!Mob_type_name.empty()) {
				if (Mob_name == Mob_type_name) {
					mob_name = Mob_name;
				}
				else {
					mob_name = Mob_name;
				}
			}
			if (!Src_type_name.empty()) {
				if (!Src_name.empty())
					src_name = Src_name;
				else
					src_name = Src_type_name;
			}
			else {
				src_name = "unknow";
			}
		}
		else {
			if (!Mob_type_name.empty()) {
				if (Mob_name == Mob_type_name) {
					mob_name = Mob_name;
				}
				else {
					mob_name = Mob_type_name;
				}
			}
			src_name = "unknow";
		}
		logger::showDie("DeathInfo", _this->getDimensionId(), _this->getPos(), src_name, mob_name);	}
	original(_this, a2);
}

void Chat() {
	addListener([](PlayerChatEvent& event) {
		wlog(gettime(), (string)"Chat", event.getPlayer().getName(), (string)"text", event.getPlayer().getDimID(), event.getChat());
	});
}
void Join() {
	addListener([](PlayerJoinEvent& event) {
		wlog(gettime(), (string)"Join", event.getPlayer().getName(), event.getPlayer().getDimID() , event.getPlayer().getIP(), to_string(event.getPlayer().getXuid()));
	});
}
void Left() {
	addListener([](PlayerLeftEvent& event) {
		wlog(gettime(), (string)"Left", event.getPlayer().getName(), event.getPlayer().getDimID(), "unknow", to_string(event.getPlayer().getXuid()));
	});
}
void CMD() {
	addListener([](PlayerCMDEvent& event) {
		wlog(gettime(), (string)"CMD", event.getPlayer().getName(), (string)"text", event.getPlayer().getDimID(), event.getCMD());
	});
}

void reg() {
	if (LogCMD)
		CMD();
	if (LogJoin)
		Join();
	if (LogLeft)
		Left();
	if (LogChat)
		Chat();
}
void bh_bdxapi_entery() {
	thread listener_thread(reg);
	listener_thread.detach();
	loadconf();
}