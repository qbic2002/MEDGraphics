//
// Created by danil on 27.10.2022.
//

#ifndef MEDGRAPHICS_LOGGING_H
#define MEDGRAPHICS_LOGGING_H

#include <iostream>
#include <string>
#include <utils/OsLock.h>

utils::OsLock info(std::ostream& = std::cout);

void info(const std::string& s);

void exceptional_exit(const std::string& msg);

#endif //MEDGRAPHICS_LOGGING_H
