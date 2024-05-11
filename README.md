# Simple C++ Port Scanner

This repository contains a simple console-based port scanner application written in C++. The application utilizes the ASIO standalone library to perform asynchronous network requests. It checks for open TCP ports on a specified IP address within a user-defined range.

## Features

- Validates IP addresses and port ranges before scanning.
- Scans multiple ports concurrently using a thread pool.
- Outputs the status of each port (open/closed).

## Prerequisites

To compile and run this application, you will need:

- C++20 compatible compiler
- [ASIO standalone library](https://think-async.com/Asio/) (non-Boost version)
- CMake (optional, for building)

Ensure that the ASIO library is properly set up in your environment as this application depends on it for network operations.
