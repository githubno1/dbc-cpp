#include "DbConnectionFactory.h"
#include <dbccpp/DbExceptions.h>
#include "sqlite/SQLiteConnection.h"

namespace dbc
{

dbconnection_transferable_ptr createSQLiteConnection(const std::string& params)
{
    return dbconnection_transferable_ptr(new SQLiteConnection(params));
}

DbConnectionFactory::DbConnectionFactory() :
    _callbacks_registry()
{
    // see doc/README-factory.rst why this unneccessary coupling is needed
    _callbacks_registry["sqlite"] = createSQLiteConnection;
}

dbconnection_transferable_ptr DbConnectionFactory::createDbConnection(const std::string& driverName,
        const std::string& params)
{
    CallbackMap::const_iterator it = _callbacks_registry.find(driverName);
    if (it == _callbacks_registry.end())
        throw DbErrorBase(driverName + ": database driver not found");

    return (it->second)(params);
}

DbConnectionFactory& DbConnectionFactory::instance()
{
    static DbConnectionFactory factory;
    return factory;
}

}
