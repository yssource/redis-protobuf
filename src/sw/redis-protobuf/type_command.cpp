/**************************************************************************
   Copyright (c) 2019 sewenew

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 *************************************************************************/

#include "type_command.h"
#include "errors.h"
#include "redis_protobuf.h"
#include "utils.h"

namespace sw {

namespace redis {

namespace pb {

int TypeCommand::run(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) const {
    try {
        assert(ctx != nullptr);

        auto args = _parse_args(argv, argc);

        auto &module = RedisProtobuf::instance();

        auto key = api::open_key(ctx, args.key_name, api::KeyMode::READONLY);
        if (!api::key_exists(key.get(), module.type())) {
            return RedisModule_ReplyWithNull(ctx);
        }

        auto *msg = api::get_msg_by_key(key.get());
        assert(msg != nullptr);

        return RedisModule_ReplyWithSimpleString(ctx, msg->GetTypeName().data());
    } catch (const WrongArityError &err) {
        return RedisModule_WrongArity(ctx);
    } catch (const Error &err) {
        return api::reply_with_error(ctx, err);
    }
}

TypeCommand::Args TypeCommand::_parse_args(RedisModuleString **argv, int argc) const {
    assert(argv != nullptr);

    if (argc != 2) {
        throw WrongArityError();
    }

    return Args{argv[1]};
}

}

}

}
