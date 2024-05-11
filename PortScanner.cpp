#include <asio.hpp>
#include <asio/thread_pool.hpp>
#include <functional>
#include <iostream>
#include <regex>
#include <string>
#include <mutex>
#include <vector>

std::mutex output_mutex;

bool isValidIPAddress(const std::string& ip) {
    static const std::regex ip_pattern(
        R"(^((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)\.){3}(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)$)");
    return std::regex_match(ip, ip_pattern);
}

bool isValidPortRange(int start_port, int end_port) {
    return start_port > 0 && start_port <= 65535 && end_port > 0 && end_port <= 65535 && start_port <= end_port;
}

void scan_port(const std::string& address, int port, asio::io_context& context) {
    auto socket = std::make_shared<asio::ip::tcp::socket>(context);
    asio::ip::tcp::endpoint endpoint(asio::ip::make_address(address), port);

    socket->async_connect(endpoint, [socket, port](const asio::error_code& ec) {
        std::lock_guard<std::mutex> lock(output_mutex);
        if (!ec) {
            std::cout << "Port " << port << " is open.\n";
        }
        });
}

int main() {
    std::string target_ip;
    int start_port, end_port;

    std::cout << "Enter target IP address: ";
    std::cin >> target_ip;
    if (!isValidIPAddress(target_ip)) {
        std::cerr << "Invalid IP address format.\n";
        return 1;
    }

    std::cout << "Enter start port: ";
    std::cin >> start_port;
    std::cout << "Enter end port: ";
    std::cin >> end_port;
    if (!isValidPortRange(start_port, end_port)) {
        std::cerr << "Invalid port range.\n";
        return 1;
    }

    asio::io_context context;
    asio::executor_work_guard<asio::io_context::executor_type> work_guard = asio::make_work_guard(context);
    asio::thread_pool pool(std::thread::hardware_concurrency());

    for (int port = start_port; port <= end_port; ++port) {
        asio::post(pool, std::bind(scan_port, target_ip, port, std::ref(context)));
    }

    pool.join();
    work_guard.reset();
    context.run();

    return 0;
}