#define MULESTL_DEBUG_XTERMDEFAULTCOLOR "\x1b[0m"
#define MULESTL_DEBUG_XTERMALERTCOLOR "\x1b[43;1m"
#define MULESTL_DEBUG_XTERMFINALCOLOR "\x1b[42;1m"
#define MULESTL_DEBUG_XTERMNOTICECOLOR "\x1b[46;1m"

#define TESTFUNC(retval, awaitedval) \
	if ((retval) == awaitedval) { \
		printf(MULESTL_DEBUG_XTERMFINALCOLOR); \
		printf("Test %s passed", #retval); \
	} \
	else { \
		printf(MULESTL_DEBUG_XTERMALERTCOLOR); \
		printf("Test %s failed", #retval); \
		retc = retc + 1; \
	} \
	printf(MULESTL_DEBUG_XTERMDEFAULTCOLOR); \
	printf("\n");

#define NOTIFYTEST(num) \
	printf(MULESTL_DEBUG_XTERMNOTICECOLOR); \
	printf("TEST №%d", num); \
	printf(MULESTL_DEBUG_XTERMDEFAULTCOLOR); \
	printf("\n");

#include <mulestl.h>

int test_initlist();
int test_emptyconst();
int test_arrayconst();

int main() {
	int toret = test_initlist() + test_emptyconst() + test_arrayconst();
	std::cout << "error code: " << toret;
	return toret;
}

int test_initlist() {
	NOTIFYTEST(1);
	int retc = 0;
	std::vector<int> myVec{3,1,2};
	printf("Created a new vector myVec\n");
	TESTFUNC(myVec.size(), 3);
	myVec.push_back(3);
	myVec.push_back(1);
	myVec.push_back(2);
	TESTFUNC(myVec.size(), 6);
	myVec.erase(1);
	TESTFUNC(myVec.size(), 5);
	for (int i = 0; i < myVec.size(); i++)
		printf("myVec[%d] = %d\n", i, myVec[i]);
	printf("Goodbye\n");
	return retc;
}

int test_emptyconst() {
	NOTIFYTEST(2);
	int retc = 0;
	std::vector<int> myVec;
	printf("Created a new vector myVec\n");
	TESTFUNC(myVec.size(), 0);
	myVec.push_back(3);
	myVec.push_back(1);
	myVec.push_back(2);
	TESTFUNC(myVec.size(), 3);
	myVec.erase(1);
	TESTFUNC(myVec.size(), 2);
	for (int i = 0; i < myVec.size(); i++)
		printf("myVec[%d] = %d\n", i, myVec[i]);
	printf("Goodbye\n");
	return retc;
}

int test_arrayconst() {
	NOTIFYTEST(3);
	int arr[3] = {3,1,2};
	int retc = 0;
	std::vector<int> myVec(arr, 3);
	printf("Created a new vector myVec\n");
	TESTFUNC(myVec.size(), 3);
	myVec.push_back(3);
	myVec.push_back(1);
	myVec.push_back(2);
	TESTFUNC(myVec.size(), 6);
	myVec.erase(1);
	TESTFUNC(myVec.size(), 5);
	for (int i = 0; i < myVec.size(); i++)
		printf("myVec[%d] = %d\n", i, myVec[i]);
	printf("Goodbye\n");
	return retc;
}
