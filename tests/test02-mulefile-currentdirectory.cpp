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
