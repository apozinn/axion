#include <iostream>
#include <string>

#include "databaseManager/databaseManager.hpp"
#include "configManager/configManager.hpp"
#include "i18nManager/i18nManager.hpp"

namespace
{
    class NoopLogger : public mongocxx::logger
    {
    public:
        void operator()(mongocxx::log_level,
                        bsoncxx::stdx::string_view,
                        bsoncxx::stdx::string_view) noexcept override {}
    };
}

DatabaseManager &DatabaseManager::Get()
{
    static DatabaseManager instance;
    return instance;
}

void DatabaseManager::Initialize()
{
    std::string uri = ConfigManager::Get().getValue<std::string>("mongo_db_uri");
    if (!uri.length())
    {
        std::cout << i18nManager::Get().GetString("errors.mongoDbUriNotFound") << "\n";
        return;
    }

    instance_ = std::make_unique<mongocxx::instance>(std::make_unique<NoopLogger>());
    pool_ = std::make_unique<mongocxx::pool>(mongocxx::uri{uri});

    if (TestConnection())
    {
        std::cout << "✅ Connected to MongoDB!" << std::endl;
    }
    else
    {
        std::cerr << "❌ Failed to connect to MongoDB!" << std::endl;
    }
}

mongocxx::pool::entry DatabaseManager::GetConnect()
{
    return pool_->acquire();
}

bsoncxx::stdx::optional<mongocxx::pool::entry> DatabaseManager::TryConnect()
{
    return pool_->try_acquire();
}

bool DatabaseManager::TestConnection()
{
    try
    {
        auto conn = Connect();
        return static_cast<bool>(conn);
    }
    catch (...)
    {
        return false;
    }
}
