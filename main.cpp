/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 15:47:22 by yel-moun          #+#    #+#             */
/*   Updated: 2025/08/18 18:33:00 by hel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./configParsing/fileParser.hpp"
#include "./CoreServer/include/Server.hpp"
#include "./configParsing/configFileValidator.hpp"

int main(int argc, char *argv[])
{
	try
	{
		FileParser *fp = new FileParser(argc, argv);
		fp->start();
		ServerConfiguration config = fp->getServerConfiguration();
		ConfigFileValidator *validator = new ConfigFileValidator(config.getServers());
		validator->validate();
		// config.printConfig();
		// config.printConfig();
		Server sv(config.getServers());
		sv.run();
		delete fp;
		delete validator;
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