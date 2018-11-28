//
// This file is a part of libMule - Microcontroller-Universal 
// Library (that is extendable)
// Copyright (C) 2018 Tim K <timprogrammer@rambler.ru>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//

#include <mule.h>
#include <iostream>

int main() {
	MuleApplication mApp;
	MuleFile mf("./popcorn.txt");
	if (mf.exists())
		std::cout << "./popcorn.txt exists" << std::endl;
	else
		std::cout << "./popcorn.txt does not exist" << std::endl;
	std::cout << "./popcorn.txt contents are: " << mf.read() << std::endl;
	if (mf.write("MuleFile tests"))
		std::cout << "MuleFile::write returned true" << std::endl;
	else
		std::cout << "MuleFile::write returned false" << std::endl;
	std::cout << "New ./popcorn.txt contents are: " << mf.read() << std::endl;
	if (mf.append("MuleFile tests again"))
		std::cout << "MuleFile::append returned true" << std::endl;
	else
		std::cout << "MuleFile::append returned false" << std::endl;
	std::cout << "Updated ./popcorn.txt contents are: " << mf.read() << std::endl;
	std::cout << "./popcorn.txt parent directory is " << mf.getParentDirectory() << std::endl;
	if (mf.remove())
		std::cout << "MuleFile::remove returned true" << std::endl;
	else
		std::cout << "MuleFile::remove returned false" << std::endl;
	std::cout << "Goodbye" << std::endl;
	return 0;
}
