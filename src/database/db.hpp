#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <memory>
#include <bsoncxx/stdx/optional.hpp>
#include <bsoncxx/stdx/string_view.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/logger.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/uri.hpp>

#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <unistd.h>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

using bsoncxx::builder::basic::sub_array;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

using namespace std;

namespace {
class mongo_access {
   public:
    static mongo_access& instance() {
        static mongo_access instance;
        return instance;
    }

    void configure(std::unique_ptr<mongocxx::instance> instance,
                   std::unique_ptr<mongocxx::pool> pool) {
        _instance = std::move(instance);
        _pool = std::move(pool);
    }

    using connection = mongocxx::pool::entry;

    connection get_connection() {
        return _pool->acquire();
    }

    bsoncxx::stdx::optional<connection> try_get_connection() {
        return _pool->try_acquire();
    }

   private:
    mongo_access() = default;

    std::unique_ptr<mongocxx::instance> _instance = nullptr;
    std::unique_ptr<mongocxx::pool> _pool = nullptr;
};
}  

void configure(mongocxx::uri uri) {
    class noop_logger : public mongocxx::logger {
       public:
        virtual void operator()(mongocxx::log_level,
                                bsoncxx::stdx::string_view,
                                bsoncxx::stdx::string_view) noexcept {}
    };

    auto instance =
        std::make_unique<mongocxx::instance>(std::make_unique<noop_logger>());

    mongo_access::instance().configure(std::move(instance),
                                       std::make_unique<mongocxx::pool>(std::move(uri)));
}

bool do_work() {
    auto connection = mongo_access::instance().get_connection();
    if (!connection)
        return false;
    return true;
}

int dbmain(string mongo_uri) {
    auto uri = mongocxx::uri{mongo_uri};
    configure(std::move(uri));
    return do_work() ? EXIT_SUCCESS : EXIT_FAILURE;
}