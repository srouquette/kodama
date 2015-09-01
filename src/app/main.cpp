// Copyright (c) 2014 Sylvain Rouquette
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "platform/pragma.h"
WARNING_DISABLE_MSVC(4100)       // unreferenced variable

WARNING_PUSH
WARNING_DISABLE_MSVC(4512)       // assignment operator could not be generated
#include <boost/filesystem/path.hpp>
#include <boost/locale.hpp>
#include <boost/locale/generator.hpp>
WARNING_POP

int main(int argc, char* argv[]) {
    std::locale::global(boost::locale::generator().generate(""));
    boost::filesystem::path::imbue(std::locale());

    return 0;
}
