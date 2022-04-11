#include "pch.h"
#include <EventAPI.h>
#include <LoggerAPI.h>
#include <MC/Level.hpp>
#include <MC/BlockInstance.hpp>
#include <MC/Block.hpp>
#include <MC/BlockSource.hpp>
#include <MC/Actor.hpp>
#include <MC/Player.hpp>
#include <MC/ItemStack.hpp>
#include <LLAPI.h>
#include <MC/BinaryStream.hpp>
#include <MC/MinecraftPackets.hpp>
//#include <MC/Command.hpp>
//#include <MC/CommandRegistry.hpp>
#include <regCommandAPI.h>

Logger logger("Fakecmd By starkc");

class FakecmdCommand : public Command {
public:
    std::string rawfakecmd;
    void execute(CommandOrigin const& ori, CommandOutput& output) const override {//执行部分
        ServerPlayer* sp = ori.getPlayer();
        auto packet = MinecraftPackets::createPacket(0x4d);
        dAccess<string, 48>(packet.get()) = rawfakecmd;
        //*((char*)packet.get() + 16) = sp->getClientSubId();
        sp->sendCommandRequestPacket("/"+rawfakecmd);
        //std::cout << rawfakecmd << std::endl;
    }

    static void setup(CommandRegistry* registry) {//注册部分(推荐做法)
        registry->registerCommand("fakecmd", "run fakecmd as inputs", CommandPermissionLevel::Any, { (CommandFlagValue)0 }, { (CommandFlagValue)0x80 });
        registry->registerOverload<FakecmdCommand>("fakecmd", RegisterCommandHelper::makeMandatory(&FakecmdCommand::rawfakecmd, "Rawcmd"));
    }
};
bool regcmd(const Event::RegCmdEvent& ev) {
    FakecmdCommand::setup(ev.mCommandRegistry);
    return true;
}

void PluginInit()
{
    LL::registerPlugin("Fakecmd", "a plugin allows you to use fakecmd", LL::Version(1, 0, 0), {
                { "Note", "note:(" },
                { "Author", "Starkc" },
                { "Key", "123456" }
        }
    );

    Event::RegCmdEvent::subscribe(regcmd);
}
