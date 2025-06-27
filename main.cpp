/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 15:47:22 by yel-moun          #+#    #+#             */
/*   Updated: 2025/06/26 11:45:08 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./configParsing/fileParser.hpp"
#include "./CoreServer/include/Server.hpp"

int main(int argc, char *argv[])
{
	try
	{
		FileParser *fp = new FileParser(argc, argv);
		fp->start();
		fp->getServerConfiguration().printConfig();
		Server sv(fp->getServerConfiguration().getServers());
		sv.run();
		delete fp;
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