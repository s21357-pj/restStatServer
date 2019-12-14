#include <algorithm>
#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>
#include "sys/types.h"
#include "headers.h"

using namespace Pistache;

#include <string>
#include <limits.h>
#include <unistd.h>

std::string getpath()
{
    char result[ PATH_MAX ];
    int count = readlink( "/proc/self/exe", result, PATH_MAX );
    if (count > 0) {
      	return std::string(result, count-14);
    }
}

std::string currentPath = getpath();

std::ifstream t(currentPath + "../share/restStatServer/Chart.min.js");
std::string chartfile((std::istreambuf_iterator<char>(t)),
                 std::istreambuf_iterator<char>());
std::ifstream t2(currentPath + "../share/restStatServer/utils.js");
std::string utilsfile((std::istreambuf_iterator<char>(t2)),
                 std::istreambuf_iterator<char>());
std::ifstream t3(currentPath + "../share/restStatServer/jquery.min.js");
std::string jqueryfile((std::istreambuf_iterator<char>(t3)),
                 std::istreambuf_iterator<char>());
std::ifstream t4(currentPath + "../share/restStatServer/index.htm");
std::string indexfile((std::istreambuf_iterator<char>(t4)),
                 std::istreambuf_iterator<char>());

namespace Generic {

    void handleStat(const Rest::Request&, Http::ResponseWriter response) {
        std::string ans = ""; 
        for (size_t j = 0; j<qCpu.size(); j++) {
            ans += "{\"mem\" : \""+std::to_string(qMem[j])+"\", \"cpu\" :"+ std::to_string(qCpu[j]) +"}";
            if (j != countProbes-1) {
                ans += ",";
            }
            ans += "\n";
        }
        response.send(Http::Code::Ok, "[\n"+ans+"]");
    }

    void handleIndex(const Rest::Request&, Http::ResponseWriter response) {
        response.send(Http::Code::Ok, indexfile);
    }
    void handleJquerymin(const Rest::Request&, Http::ResponseWriter response) {
        response.send(Http::Code::Ok, jqueryfile);
    }
    void handleUtils(const Rest::Request&, Http::ResponseWriter response) {
        response.send(Http::Code::Ok, utilsfile);
    }
    void handleChartjs(const Rest::Request&, Http::ResponseWriter response) {
        response.send(Http::Code::Ok, chartfile);
    }
}



class StatsEndpoint {
public:
    StatsEndpoint(Address addr)
    : httpEndpoint(std::make_shared<Http::Endpoint>(addr))
    { }

    void init(size_t thr = 2) {
        auto opts = Http::Endpoint::options()
        .threads(thr);
        httpEndpoint->init(opts);
        setupRoutes();
    }

    void start() {
        httpEndpoint->setHandler(router.handler());
        httpEndpoint->serve();
    }

private:
    void setupRoutes() {
        using namespace Rest;
        Routes::Get(router, "/stats", Routes::bind(&Generic::handleStat));
        Routes::Get(router, "/", Routes::bind(&Generic::handleIndex));
        Routes::Get(router, "/jquery.min.js", Routes::bind(&Generic::handleJquerymin));
        Routes::Get(router, "/Chart.min.js", Routes::bind(&Generic::handleChartjs));
        Routes::Get(router, "/utils.js", Routes::bind(&Generic::handleUtils));
    }
    typedef std::mutex Lock;
    typedef std::lock_guard<Lock> Guard;
    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;
};

int main(int argc, char *argv[]) {
    std::thread t1(infoLoop);
    t1.detach();
    Port port(80);
    int thr = 2;    
    if (argc >= 2) {
    	std::string first_arg = argv[1];
	    if (first_arg == "--version" || first_arg == "-v") {
    	    std::cout << versionOutput;
    	    return 0;
        }
	    if (first_arg == "--help" || first_arg == "-h") {
    	    std::cout << helpOutput;
    	    return 0;
        }        
        port = std::stol(argv[1]);
        if (argc == 3)
            thr = std::stol(argv[2]);
    }

    Address addr(Ipv4::any(), port);
    std::cout << "Cores = " << hardware_concurrency() << std::endl;
    std::cout << "Using " << thr << " threads" << std::endl;

    StatsEndpoint stats(addr);

    stats.init(thr);
    stats.start();
}