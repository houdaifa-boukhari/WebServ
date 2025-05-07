/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-bouk <hel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 18:14:22 by hel-bouk          #+#    #+#             */
/*   Updated: 2025/04/30 18:33:32 by hel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

class logger
{
	public :
		static void logInfo(const std::string& msg);
    	static void logError(const std::string& msg);
    	static void logDebug(const std::string& msg);	
};
