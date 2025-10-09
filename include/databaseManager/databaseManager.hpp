#pragma once

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/logger.hpp>

#include <memory>
#include <string>

/**
 * @brief Singleton class responsible for managing MongoDB connections.
 * 
 * This class handles initialization of the MongoDB driver instance and maintains
 * a connection pool for efficient database access. It follows a thread-safe
 * Singleton pattern for consistent global access.
 */
class DatabaseManager {
public:
    /**
     * @brief Returns the global instance of the DatabaseManager.
     * 
     * @return DatabaseManager& The singleton instance.
     */
    static DatabaseManager& Get();

    /**
     * @brief Initializes the MongoDB driver and connection pool.
     * 
     * Must be called once before any database operation.
     * 
     * @param uri MongoDB URI string (e.g. "mongodb://localhost:27017")
     */
    void Initialize();

    /**
     * @brief Acquires a connection from the MongoDB connection pool.
     * 
     * @return mongocxx::pool::entry Active connection entry.
     */
    mongocxx::pool::entry GetConnect();

    /**
     * @brief Attempts to acquire a connection without blocking.
     * 
     * @return Optional connection entry.
     */
    bsoncxx::stdx::optional<mongocxx::pool::entry> TryConnect();

    /**
     * @brief Verifies if a connection can be successfully acquired.
     * 
     * @return true if the connection is valid.
     * @return false otherwise.
     */
    bool TestConnection();

private:
    DatabaseManager() = default;
    ~DatabaseManager() = default;

    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    std::unique_ptr<mongocxx::instance> instance_;
    std::unique_ptr<mongocxx::pool> pool_;
};
