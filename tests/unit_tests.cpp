/*
** EPITECH PROJECT, 2020
** OOP_babel_2019
** File description:
** unit_tests
*/

#include <criterion/criterion.h>
#include <criterion/output.h>
#include <cstdio>

#include "CSQLite.hpp"
#include "CServerClient.hpp"
#include "CFileManager.hpp"

void deleteTemporaryDB(void)
{
    try
    {
        std::remove("./test.db");
    }
    catch(const std::exception& e)
    {}
}

Test(test_SQLite, open)
{
    babel::server::CSQLite instance;

    instance.OpenConnection("./test.db");
    cr_assert_eq(instance.IsOpen(), true);
}

Test(test_SQLite, open_error)
{
    babel::server::CSQLite instance;

    try
    {
        instance.OpenConnection(".");
    }
    catch(const babel::server::CSQLiteError& e)
    {
        cr_assert_str_eq(e.what(), "Invalid name");
    }
    
    cr_assert_eq(instance.IsOpen(), false);
}

Test(test_client, client)
{
    babel::server::CServerClient client(1, "test", "testpwd", false);

    cr_assert_eq(client.getId(), 1);
    cr_assert_eq(client.getUsername(), "test");
    cr_assert_eq(client.getPassword(), "testpwd");
    cr_assert_eq(client.isConnected(), false);
    cr_assert_eq(client.toSQLString(), "('test','testpwd');");
}

Test(test_fileManager, simple, .init=deleteTemporaryDB)
{
    babel::server::CFileManager test("test.db", "./files/createDB.sql");

    try
    {
        cr_assert_eq(test.userTableExist(), false);
        test.setupDatabase();
        cr_assert_eq(test.userTableExist(), true);    
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

Test(test_fileManager, insertUsers)
{
    babel::server::CFileManager test("testInsert.db", "./files/createDB.sql");
    babel::server::CServerClient newClient(1, "lopes", "mysecretpassword", false);
    std::string newUsername = "test", password = "testpwd";
    std::vector<std::shared_ptr<babel::server::CServerClient> > res;

    test.setupDatabase();
    cr_assert_eq(test.userTableExist(), true);
    test.addUser(newClient);
    test.addUser(newUsername, password);
    res = test.getListUser();
    cr_assert_eq(res.size(), 2);
    cr_assert_eq(res[0]->getId(), 1);
    cr_assert_eq(res[0]->getUsername(), "lopes");
    cr_assert_eq(res[0]->getPassword(), "mysecretpassword");
    
    cr_assert_eq(res[1]->getId(), 2);
    cr_assert_eq(res[1]->getUsername(), "test");
    cr_assert_eq(res[1]->getPassword(), "testpwd");
    std::remove("testInsert.db");
}

Test(test_fileManager, exist_user)
{
    babel::server::CFileManager test("testExistUser.db", "./files/createDB.sql");
    babel::server::CServerClient newClient(1, "lopes", "mysecretpassword", false);
    babel::server::CServerClient unknowClient(2, "Jean", "test", false);

    test.setupDatabase();
    cr_assert_eq(test.userTableExist(), true);
    test.addUser(newClient);
    cr_assert_eq(test.existUser(newClient), true);
    cr_assert_eq(test.existUser(newClient.getUsername(), newClient.getPassword()), true);
    cr_assert_eq(test.existUser(unknowClient), false);
    cr_assert_eq(test.existUser(unknowClient.getUsername(), unknowClient.getPassword()), false);
    std::remove("testExistUser.db");
}