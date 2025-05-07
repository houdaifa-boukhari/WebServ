/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 18:19:42 by hel-bouk          #+#    #+#             */
/*   Updated: 2025/04/30 18:33:11 by hel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/logger.hpp"

static std::string currentTime()
{
    time_t now = time(0);
    char buf[80];
    strftime(buf, sizeof(buf), "[%Y-%m-%d %H:%M:%S]", localtime(&now));
    return std::string(buf);
}

void logger::logInfo(const std::string& msg)
{
    std::cout << currentTime() << " [INFO] " << msg << std::endl;
}

void logger::logError(const std::string& msg)
{
    std::cerr << currentTime() << " [ERROR] " << msg << std::endl;
}

void logger::logDebug(const std::string& msg)
{
    std::cout << currentTime() << " [DEBUG] " << msg << std::endl;
}