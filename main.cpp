/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 15:47:22 by yel-moun          #+#    #+#             */
/*   Updated: 2025/05/06 21:27:45 by hel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./configParsing/fileParser.hpp"
#include "./CoreServer/include/Server.hpp"

int main(int argc, char *argv[])
{
	try
	{
		FileParser fp(argc, argv);
		fp.start();
		Server sv(fp.getServerConfiguration().getServers());
		
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
		std::exit(1);
	}
	catch (const char *e)
	{
		std::cerr << e << std::endl;
		std::exit(1);
	}
	return 0;
}