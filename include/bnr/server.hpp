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

#ifndef BROADCAST_NAME_RESOLUTION_SERVER_HPP
#define BROADCAST_NAME_RESOLUTION_SERVER_HPP

#include <string>
#include <unordered_map>

#include <bnr/detail/data.hpp>

namespace bnr {
    constexpr std::uint16_t ServerPort = 12345;

    std::unordered_map<Type, std::string, detail::TypeHash>
    CreateValues(const std::vector<std::pair<Type, std::string>> &values);

    bool StartServer(const std::string &name,
                     const std::unordered_map<Type, std::string, detail::TypeHash> &values);
} // namespace bnr

#endif // BROADCAST_NAME_RESOLUTION_SERVER_HPP
