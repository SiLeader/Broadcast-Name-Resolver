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

#include <mutex>
#include <algorithm>

#include <bnr/client.hpp>

#if defined(__unix__) || defined(__APPLE__)
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#elif defined(__WIN32__)
#error "Windows is not supported."
#endif

#include <bnr/server.hpp>

namespace bnr {
    bool Query(std::vector<Response> &res, const Request &req, std::size_t maxResponseCount) {
        res.clear();

        auto sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock < 0) {
            perror("bnr::Query: socket: ");
            return false;
        }

        int y;
        setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &y, sizeof(y));
        timeval tv = {};
        tv.tv_sec = 0;
        tv.tv_usec = bnr::PollingSpan * 1000;
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv));

        sockaddr_in addr = {};
        addr.sin_addr.s_addr = inet_addr("255.255.255.255");
        addr.sin_port = htons(bnr::ServerPort);
        addr.sin_family = AF_INET;

        std::vector<std::uint8_t> buf;
        req.datagram(buf);
        if (sendto(sock, buf.data(), buf.size(), 0,
                   static_cast<const sockaddr *>(static_cast<const void *>(&addr)),
                   sizeof(addr)) < 0) {
            perror("bnr::Query: sendto: ");
            return false;
        }

        buf.resize(Response::MaxDatagramLength);
        for (std::size_t i = 0; i < maxResponseCount; ++i) {
            auto rs = recv(sock, buf.data(), buf.size(), 0);

            if (rs < 0) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    // OK: cannot receive datagram in timeout.
                    continue;
                } else {
                    perror("bnr::Query: recv: ");
                    return false;
                }
            }
            res.emplace_back(Response::FromDatagram(buf));
        }
        return !res.empty();
    }

    bool Lookup(std::vector<std::string> &address, const std::string &name,
                std::size_t maxResponseCount) {
        std::vector<Response> res;
        if (!Query(res, Request(Type::ResolveV4, name), maxResponseCount)) {
            return false;
        }
        address.clear();
        address.resize(res.size());
        std::transform(std::begin(res), std::end(res), std::begin(address),
                       [](const Response &r) { return r.value(); });
        return true;
    }

    bool Lookup(std::string &address, const std::string &name, std::size_t maxResponseCount) {
        std::vector<std::string> res;
        if (!Lookup(res, name, maxResponseCount)) {
            return false;
        }
        address = res[0];
        return true;
    }
} // namespace bnr