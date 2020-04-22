#include <api/Loader.h>
template <typename Ret, typename Type> Ret& direct_access(Type* type, size_t offset) {
    union {
        size_t raw;
        Type* source;
        Ret* target;
    } u;
    u.source = type;
    u.raw += offset;
    return *u.target;
}
template <typename A, typename T>
class AutomaticID {
    T id;
public:
    AutomaticID() {
        id = 0;
    }
    AutomaticID(T x) {
        id = x;
    }
    operator T() {
        return id;
    }
};
struct ActorUniqueID {
    unsigned long long id;
public:
    ActorUniqueID() {
        id = -1;
    }
    ActorUniqueID(unsigned long long i) {
        id = i;
    }
    auto get() {
        return id;
    }
    operator unsigned long long() {
        return id;
    }
};
struct BlockLegacy {
    // 获取方块名
    auto getFullName() const { return (std::string&) * (__int64*)((__int64)this + 104); }

    // 获取方块ID号
    inline short getBlockItemId() const {
        short (BlockLegacy:: * rv)() const;
        *((void**)&rv) = dlsym("?getBlockItemId@BlockLegacy@@QEBAFXZ");
        return (this->*rv)();
    }
};

struct BlockPos {
    // 获取坐标数组头
    signed int* getPosition() const { return reinterpret_cast<signed int*>(reinterpret_cast<unsigned long long>(this)); }
};

struct Block {
    // 获取源
    inline BlockLegacy const& getLegacyBlock() const {
        BlockLegacy const& (Block:: * rv)() const;
        *((void**)&rv) = dlsym("?getLegacyBlock@Block@@QEBAAEBVBlockLegacy@@XZ");
        return (this->*rv)();
    }
};

struct BlockActor {

    // 取方块位置
    BlockPos* getPosition() { return reinterpret_cast<BlockPos*>(reinterpret_cast<unsigned long long>(this) + 44); }
};

struct BlockSource {
    // 取方块
    inline Block const& getBlock(BlockPos const& a0) const {
        Block const& (BlockSource:: * rv)(BlockPos const&)const;
        *((void**)&rv) = dlsym("?getBlock@BlockSource@@QEBAAEBVBlock@@AEBVBlockPos@@@Z");
        return (this->*rv)(a0);
    }
};

struct Vec3 {
    float x;
    float y;
    float z;
};

struct Level {
    inline struct Actor* fetchEntity(struct ActorUniqueID a0, bool a1) const {
        struct Actor* (Level:: * rv)(struct ActorUniqueID, bool)const;
        *((void**)&rv) = dlsym("?fetchEntity@Level@@QEBAPEAVActor@@UActorUniqueID@@_N@Z");
        return (this->*rv)(a0, a1);
    }
};
struct Actor : Level{
    // 取维度ID
    inline AutomaticID<class Dimension, int> getDimension() const {
        class AutomaticID<class Dimension, int>(Actor:: * rv)() const;
        *((void**)&rv) = dlsym("?getDimensionId@Actor@@UEBA?AV?$AutomaticID@VDimension@@H@@XZ");
        return (this->*rv)();
    }
    // 取名字标签
    inline std::basic_string<char, struct std::char_traits<char>, class std::allocator<char>> const& getNameTag() const {
        class std::basic_string<char, struct std::char_traits<char>, class std::allocator<char>> const& (Actor:: * rv)() const;
        *((void**)&rv) = dlsym("?getNameTag@Actor@@UEBAAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ");
        return (this->*rv)();
    }
    // 是否悬空
    const unsigned char isStand() {
        return *reinterpret_cast<unsigned char*>(reinterpret_cast<unsigned long long>(this) + 376);
    }
    // 取玩家位置
    inline Vec3 const& getPos() const {
        Vec3 const& (Actor:: * rv)() const;
        *((void**)&rv) = dlsym("?getPos@Actor@@UEBAAEBVVec3@@XZ");
        return (this->*rv)();
    }

};
struct Mob : Actor {};
struct Player : Mob {
    std::string const& Player::getDeviceId() const { return direct_access<std::string>(this, 8360); }

    
//std::string const& Player::getClientPlatformId() const { return direct_access<std::string>(this, 3256); }
};

struct GameMode {};

struct ContainerManagerModel {
    // 取开容者
    Player* getPlayer() { return *reinterpret_cast<Player**>(reinterpret_cast<unsigned long long>(this) + 8); }
};
struct LevelContainerManagerModel : ContainerManagerModel {};
struct ItemStackBase {
    // 取物品名称
    inline class std::basic_string<char, struct std::char_traits<char>, class std::allocator<char>> getName() const {
        class std::basic_string<char, struct std::char_traits<char>, class std::allocator<char>>(ItemStackBase:: * rv)() const;
        *((void**)&rv) = dlsym("?getName@ItemStackBase@@QEBA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ");
        return (this->*rv)();
    }

};
struct ItemStack : ItemStackBase {
    // 取物品ID
    inline int getId() const {
        int (ItemStack:: * rv)() const;
        *((void**)&rv) = dlsym("?getId@ItemStackBase@@QEBAFXZ");
        return (this->*rv)();
    }


    // 取容器内数量

    inline int getStackSize() const {
        int (ItemStack:: * rv)() const;
        *((void**)&rv) = dlsym("?getCount@ContainerItemStack@@QEBAHXZ");
        return (this->*rv)();
    }
};

struct ContainerItemStack : ItemStack {};
