/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-moun <yel-moun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 15:47:22 by yel-moun          #+#    #+#             */
/*   Updated: 2025/03/08 15:47:27 by yel-moun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./configParsing/fileParser.hpp"

int main(int argc, char *argv[])
{
	try
	{
		FileParser fp(argc, argv);
		fp.start();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
		std::exit(1);
	}
	std::cout << "File is correct" << std::endl;
	return 0;
}