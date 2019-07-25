# Copyright 2019 SiLeader and Cerussite.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.5)
project(broadcast_name_resolution)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_subdirectory(client)
add_subdirectory(server)

add_library(bnr src/client.cpp include/bnr/client.hpp include/bnr/detail/data.hpp src/detail_data.cpp src/server.cpp include/bnr/server.hpp)
