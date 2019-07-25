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

#ifndef BROADCAST_NAME_RESOLUTION_DATA_HPP
#define BROADCAST_NAME_RESOLUTION_DATA_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace bnr {
    enum class Type : std::uint8_t {
        Unknown = 0,
        ResolveV4 = 1,
        ResolveV6 = 2, // not implemented
    };

    namespace detail {
        struct TypeHash {
            std::hash<std::uint8_t> hash;

            std::size_t operator()(Type t) const { return hash(static_cast<std::uint8_t>(t)); }
        };
    } // namespace detail

    class Request {
    public:
        static constexpr std::size_t MaxDatagramLength = 1 + 1 + 256;

    private:
        Type _type = Type::Unknown;
        std::string _key;

    public:
        Request() = default;
        Request(Type t, std::string k);
        Request(const Request &) = default;
        Request(Request &&) = default;

        ~Request() = default;

        Request &operator=(const Request &) = default;
        Request &operator=(Request &&) = default;

    public:
        Type type() const noexcept;
        const std::string &key() const noexcept;

    public:
        void datagram(std::vector<std::uint8_t> &buf) const;

    public:
        static Request FromDatagram(const std::vector<std::uint8_t> &data);
    };

    class Response {
    public:
        static constexpr std::size_t MaxDatagramLength = 1 + 1 + 256;

    private:
        Type _type = Type::Unknown;
        std::string _value;

    public:
        Response() = default;
        Response(Type t, std::string k);
        Response(const Response &) = default;
        Response(Response &&) = default;

        ~Response() = default;

        Response &operator=(const Response &) = default;
        Response &operator=(Response &&) = default;

    public:
        Type type() const noexcept;
        const std::string &value() const noexcept;

    public:
        void datagram(std::vector<std::uint8_t> &buf) const;

    public:
        static Response FromDatagram(const std::vector<std::uint8_t> &data);
    };
} // namespace bnr

#endif // BROADCAST_NAME_RESOLUTION_DATA_HPP
