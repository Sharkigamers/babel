// #include "CFileManager.hpp"
// #include <iostream>

// int main(void)
// {
//     try
//     {
//         babel::server::CFileManager lopes(DEFAULT_DB_PATH);
//         babel::server::CServerClient client(2, "eydou", "lopes", false);

//         lopes.addUser(client);
//         std::vector<babel::server::CServerClient> row =  lopes.getListUser();        

//         for (const auto &x : row) {
//             std::cout << x.toSQLString() << std::endl;
//         }
//     }
//     catch(const std::exception& e)
//     {
//         std::cerr << e.what() << '\n';
//     }
//     return 0;
// }

#include <iostream>

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "CServer.hpp"

int main()
{
	try
	{
		babel::server::CServer lopes;

		lopes.init();
		lopes.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	return 0;
}