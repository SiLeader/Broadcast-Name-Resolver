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

#include <bnr/detail/data.hpp>
#include <iostream>
#include <cassert>

namespace bnr {
    Request::Request(Type t, std::string k)
        : _type(t)
        , _key(std::move(k)) {
        assert(_key.size() < 256);
    }

    Type Request::type() const noexcept { return _type; }
    const std::string &Request::key() const noexcept { return _key; }

    void Request::datagram(std::vector<std::uint8_t> &buf) const {
        buf.clear();
        buf.reserve(sizeof(type()) + sizeof(std::uint8_t) + sizeof(char) * key().size());

        buf.emplace_back(static_cast<std::uint8_t>(type()));
        buf.emplace_back(static_cast<std::uint8_t>(key().size()));
        buf.insert(std::end(buf), std::begin(key()), std::end(key()));
    }

    Request Request::FromDatagram(const std::vector<std::uint8_t> &data) {
        auto type = static_cast<Type>(data[0]);
        auto size = data[1];
        std::string key(std::begin(data) + 2, std::begin(data) + 2 + size);
        return Request(type, std::move(key));
    }

    Response::Response(Type t, std::string v)
        : _type(t)
        , _value(std::move(v)) {
        assert(_value.size() < 256);
    }
    Type Response::type() const noexcept { return _type; }
    const std::string &Response::value() const noexcept { return _value; }

    void Response::datagram(std::vector<std::uint8_t> &buf) const {
        buf.clear();
        buf.reserve(sizeof(type()) + sizeof(std::uint8_t) + sizeof(char) * value().size());

        std::cout << __func__ << " value: " << value() << std::endl;

        buf.emplace_back(static_cast<std::uint8_t>(type()));
        buf.emplace_back(static_cast<std::uint8_t>(value().size()));
        buf.insert(std::end(buf), std::begin(value()), std::end(value()));
    }

    Response Response::FromDatagram(const std::vector<std::uint8_t> &data) {
        auto type = static_cast<Type>(data[0]);
        auto size = data[1];
        std::string value(std::begin(data) + 2, std::begin(data) + 2 + size);
        return Response(type, std::move(value));
    }
} // namespace bnr
