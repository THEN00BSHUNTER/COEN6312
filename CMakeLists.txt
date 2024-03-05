cmake_minimum_required(VERSION 3.27)
project(Store)

set(CMAKE_CXX_STANDARD 17)

add_executable(Store main.cpp
sqlite/sqlite3.c
sqlite/sqlite3.h
Branch.cpp
Branch.h
Category.cpp
Category.h
Customer.cpp
Customer.h
Database.cpp
Database.h
Discount.cpp
Discount.h
Employee.cpp
Employee.h
Inventory.cpp
Inventory.h
Manager.cpp
Manager.h
Menu.cpp
Menu.h
Order.cpp
Order.h
OrderItem.cpp
OrderItem.h
Product.cpp
Product.h
Promo.cpp
Promo.h
Return.cpp
Return.h
ReturnItem.cpp
ReturnItem.h
Subcategory.cpp
Subcategory.h
Supplier.cpp
Supplier.h
Transaction.cpp
Transaction.h
User.cpp
User.h
Warehouse.cpp
Warehouse.h
)
