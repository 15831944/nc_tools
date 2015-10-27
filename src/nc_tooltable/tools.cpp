#include <boost/program_options.hpp>
#include "print_exception.h"

#include <iostream>
#include <vector>
#include <string>
#include <lua.hpp>

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    po::options_description options("nc_tooltable");
    std::vector<std::string> args(argv, argv + argc);
    args.erase(begin(args));

    options.add_options()
        ("help,h", "display this help and exit")
    ;

    try {
        po::variables_map vm;
        store(po::command_line_parser(args).options(options).run(), vm);

        if(vm.count("help")) {
            std::cout << options << "\n";
            return 0;
        }
        notify(vm);

        // TODO functions to export tool table from nc_tools.conf format to linuxcnc tool table format
        // http://wiki.linuxcnc.org/cgi-bin/wiki.pl?ToolTable
/*
 * ; - opening semicolon, no data following 
 * T - tool number, 0-99999 (you can have a large number of tools in inventory) 
 * P - pocket number, 1-99999 (tool table has a lower number of entries, currently 56.) 
 * X..W - tool offset on specified axis - floating-point 
 * D - tool diameter - floating-point 
 * I - front angle (lathe only) - floating-point 
 * J - back angle (lathe only) - floating-point 
 * Q - tool orientation (lathe only) - integer, 0-9 
 * ; - beginning of comment or remark - text */

    } catch(const po::error& e) {
        print_exception(e);
        std::cout << options << "\n";
        return 1;
    } catch(const std::exception& e) {
        print_exception(e);
        return 1;
    }

    return 0;
}
