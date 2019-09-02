// Copyright 2019 SiLeader and Cerussite.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <bnr/server.hpp>

#include <string>
#include <unordered_map>
#include <iostream>

#if defined(__unix__) || defined(__APPLE__)
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#elif defined(__WIN32__)
#error "Windows is not supported."
#else  // ESP32
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#endif

#include <bnr/detail/data.hpp>

namespace {
    void Process(const std::string &name,
                 const std::unordered_map<bnr::Type, std::string, bnr::detail::TypeHash> &values,
                 int sock) {
        sockaddr_in peerAddr = {};
        socklen_t addrLen = sizeof(peerAddr);

        std::vector<std::uint8_t> buf(bnr::Request::MaxDatagramLength);

        if (recvfrom(sock, buf.data(), buf.size(), 0,
                     static_cast<sockaddr *>(static_cast<void *>(&peerAddr)), &addrLen) < 0) {
            perror("Process: recvfrom: ");
            return;
        }

        auto request = bnr::Request::FromDatagram(buf);
        if (values.find(request.type()) == std::end(values)) {
            return;
        }
        if (request.key() != name) {
            return;
        }

        bnr::Response response(request.type(), values.at(request.type()));
        response.datagram(buf);
        sendto(sock, buf.data(), buf.size(), 0,
               static_cast<const sockaddr *>(static_cast<const void *>(&peerAddr)),
               sizeof(peerAddr));
    }
} // namespace

namespace bnr {
    std::unordered_map<Type, std::string, detail::TypeHash>
    CreateValues(const std::vector<std::pair<Type, std::string>> &values) {
        std::unordered_map<Type, std::string, detail::TypeHash> h;
        h.reserve(values.size());

        for (const auto &v : values) {
            h[v.first] = v.second;
        }
        return h;
    }

    bool StartServer(const std::string &name,
                     const std::unordered_map<Type, std::string, detail::TypeHash> &values) {
        auto sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock < 0) {
            perror("bnr::Query: socket: ");
            return false;
        }

        sockaddr_in bindAddr = {};
        bindAddr.sin_family = AF_INET;
        bindAddr.sin_port = htons(bnr::ServerPort);
        bindAddr.sin_addr.s_addr = INADDR_ANY;

        bind(sock, static_cast<const sockaddr *>(static_cast<const void *>(&bindAddr)),
             sizeof(bindAddr));

        std::cout<<"Waiting for client contact."<<std::endl;

        while (true) {
            Process(name, values, sock);
        }
    }
} // namespace bnr
