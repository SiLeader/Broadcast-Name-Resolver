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
#include <iostream>

int main(int argc, char **argv) {
    if (argc < 3) {
        std::cerr << "too few arguments." << std::endl;
        std::cerr << "usage: " << argv[0] << " NAME IP" << std::endl;
        return EXIT_FAILURE;
    }

    bnr::StartServer(argv[1], bnr::CreateValues({{bnr::Type ::ResolveV4, argv[2]}}));
    return EXIT_SUCCESS;
}
