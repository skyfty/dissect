set(SQLITE_THREADSAFE 1)

add_library(sqlite_orm STATIC "sqlite_orm/c.cpp")
target_include_directories(sqlite_orm PUBLIC
    "sqlite_orm"
     "${VTK_INCLUDE_DIRS}/vtksqlite"
)
