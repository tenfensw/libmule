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

#ifndef MULESTL_H
#define MULESTL_H

// Uncomment the line below if you have installed libMule without mulestlconfig.h file
//#define MULESTL_INTERNAL_NOSEPERATECONFIG

#ifndef MULESTL_INTERNAL_NOSEPERATECONFIG
#include <mulestlconfig.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <math.h>
#ifndef MULESTL_HEADERS_NOSIGNALH
#include <signal.h>
#endif
#ifdef MULESTL_FEATURES_DOCXX11
#include <initializer_list>
#endif

#ifdef DEBUG
#define MULESTL_FEATURES_DEBUG
#endif
#ifdef MULESTL_FEATURES_DEBUG
#define MULESTL_DEBUG_XTERMDEFAULTCOLOR "\x1b[0m"
#define MULESTL_DEBUG_XTERMALERTCOLOR "\x1b[33;1m"
#endif
#define MULESTL_VECTOR_MAXELEMENTCOUNT 100
#define MULESTL_VECTOR_REQUIREDELEMENTCOUNT 1
#define MULESTL_VECTOR_MINIMALISTICVECTORSTART 3
#define MULESTL_STREAMS_COUTSTR "devcout"
#define MULESTL_STREAMS_CINSTR "devcin"
#define MULESTL_STREAMS_CERRSTR "devcout"

#ifndef NULL
#define NULL 0
#endif

#define MULE_INTERNAL_AVOIDEXCEPTIONS
#define MULE_INTERNAL_AVOIDSTRINGSTREAMS

#define MULESTL_FEATURES_STRINGS
#define MULESTL_INTERNAL_SIZETDEFINITION unsigned int

inline void* operator new(MULESTL_INTERNAL_SIZETDEFINITION size) { return malloc(size); }
inline void operator delete(void* todel) { free(todel); }
inline void operator delete(void* todel, unsigned int toint) { free(todel); }

#ifdef MULESTL_ARDUINO_REDEFINEGCCSTUFF
  static void* __gxx_personality_sj0;
  static void* __cxa_begin_catch;
  static void* __cxa_end_catch;
#endif

namespace std {

	const char endl = '\n';
	
	static void* mulestlcalloc(int arraymembers, int arraysize) { return calloc(arraymembers, arraysize); }
	static void mulestlcfree(void* pointer) { free(pointer); }

	static void mulestldebug(const char* message, ...) {
		va_list everythingelse;
		va_start(everythingelse, message);
#ifdef MULESTL_FEATURES_DEBUG
		printf(MULESTL_DEBUG_XTERMALERTCOLOR);
		printf("[MuleSTL minimalistic C++ library/DEBUG] ");
		vprintf(message, everythingelse);
		printf(MULESTL_DEBUG_XTERMDEFAULTCOLOR);
		printf("\n");
#endif
		va_end(everythingelse);
	}
	
#ifdef MULESTL_FEATURES_NEEDFAKESYSTEM
	static int system(const char* cmd) { return -1; }
#endif
#ifdef MULESTL_FEATURES_NEEDFAKEEXIT
	static void exit(int status) { mulestldebug("status = %d", status); }
#endif
	
	static char* mulestl_chartochararray(char in) {
		char* toreturn = (char*)(mulestlcalloc(1, sizeof(char)));
		toreturn[0] = in;
		return toreturn;
	}
	
	static int mulestl_strlen(const char* str) {
		int result = 0;
		while (str[result] != '\0') {
			if (str[result] != '\0')
				result = result + 1;
		}
		return result;
	}
	
	static bool mulestl_streq(const char* str1, const char* str2) {
		int str1_len = mulestl_strlen(str1);
		int str2_len = mulestl_strlen(str2);
		if (str1_len != str2_len)
			return false;
		for (int i = 0; i < str2_len; i++) {
			if (str1[i] != str2[i])
				return false;
		}
		return true;
	}
	
	
	
	template <class in, class on>
	on* copy(const in* ifirst, const in* ilast, on* ofirst) {
		while (ifirst != ilast)
			*(ofirst)++ = *(ifirst)++;
		return ofirst;
	}
	
	class MuleSTLNonCopyable {
		public:
		  MuleSTLNonCopyable() {}

		private:
		  MuleSTLNonCopyable(const MuleSTLNonCopyable& cp);
		  MuleSTLNonCopyable& operator=(const MuleSTLNonCopyable& cp);
	};
	
	template <class T>
	class MuleSTLNonPOSIXAllocator : private MuleSTLNonCopyable {
		public:
		  MuleSTLNonPOSIXAllocator() {}
		  ~MuleSTLNonPOSIXAllocator() {}
		  
		  T* allocate(int data_size) { return (T*)(mulestlcalloc(data_size, sizeof(T))); }
		  void deallocate(T* todel) { mulestlcfree(todel); }
	};
	
	template <class somekindofclass, class nonstlallocat = MuleSTLNonPOSIXAllocator<somekindofclass>>
	class vector {
		public:
		  vector() : 
			contents(nullptr), 
			contents_size(MULESTL_VECTOR_MINIMALISTICVECTORSTART),
			contents_els(0) {
			
			mulestldebug("this is an empty vector constructor");
			//notthefirsttime = false;
			//contents = nullptr;
			//contents_size = MULESTL_VECTOR_MINIMALISTICVECTORSTART;
			if (this->reserve(MULESTL_VECTOR_MINIMALISTICVECTORSTART) == false)
				mulestldebug("line %d, planning to crash the whole program", __LINE__);
			;
			//contents_els = 0;
			mulestldebug("line %d, contents_size = %d, contents_els = %d", __LINE__, contents_size, contents_els);
			//notthefirsttime = true;
		  }
		  
		  vector(const vector<somekindofclass>& orig) : 
			contents(nullptr), 
			contents_size(MULESTL_VECTOR_MINIMALISTICVECTORSTART),
			contents_els(0) {
			
			mulestldebug("this copies an existing vector into this vector");
			//notthefirsttime = false;
			//contents = nullptr;
			contents_size = orig.size();
			if (this->reserve(contents_size) == false)
				mulestldebug("line %d, planning to crash the whole program", __LINE__);
			copy(orig.begin(), orig.end(), this->begin());
			contents_els = contents_size;
			mulestldebug("line %d, contents_size = %d, contents_els = %d", __LINE__, contents_size, contents_els);
			//notthefirsttime = true;
		  }
		  
		  vector(const somekindofclass* orig, int origlen) : 
			contents(nullptr), 
			contents_size(MULESTL_VECTOR_MINIMALISTICVECTORSTART),
			contents_els(0) {
			
			mulestldebug("this converts a const pointer array to a vector");
			contents = nullptr;
			//notthefirsttime = false;
			contents_size = origlen;
			if (this->reserve(contents_size) == false)
				mulestldebug("line %d, planning to crash the whole program", __LINE__);
			copy(orig, (orig + (origlen - 1)), this->begin());
			contents_els = contents_size;
			mulestldebug("line %d, contents_size = %d, contents_els = %d", __LINE__, contents_size, contents_els);
			//notthefirsttime = true;
		  }
		  
#ifdef MULESTL_FEATURES_DOCXX11
		  vector(const std::initializer_list<somekindofclass>& inlist) : 
			contents(nullptr), 
			contents_size(MULESTL_VECTOR_MINIMALISTICVECTORSTART),
			contents_els(0) {

			mulestldebug("this allows importing std::initializer_list");
			contents = nullptr;
			contents_size = inlist.size();
			if (this->reserve(contents_size) == false)
				mulestldebug("line %d, planning to crash the whole program", __LINE__);
			copy(inlist.begin(), inlist.end(), this->begin());
			contents_els = contents_size;
			mulestldebug("line %d, contents_size = %d, contents_els = %d", __LINE__, contents_size, contents_els);
		  }
#endif
		  
		  ~vector() { 
			clear();
			contents = NULL;
		  }
		  
		  int max_size() const { return MULESTL_VECTOR_MAXELEMENTCOUNT; }
		  
		  void clear() {
			mulestldebug("contents_els = %d", contents_els);
			if (contents_els > 0) {
				if (contents)
					allocat.deallocate(contents);
				else
					mulestldebug("contents is not allocated, what is even going on?");
			}
			contents = nullptr;
			this->reserve(contents_size);
			contents_els = 0;
		  }
		  
		  bool empty() const { return (this->size() == 0); }
		  
		  vector<somekindofclass> operator=(const vector<somekindofclass>& invec) {
			contents_size = invec.size();
			contents_els = contents_size;
			if (this->reserve(contents_els) == false)
				mulestldebug("line %d, planning to crash the whole program", __LINE__);
			copy(invec.begin(), invec.end(), this->begin());
		  }
		  
		  bool reserve(int elc) {
			if (contents != nullptr) {
				if (contents_els > 0) 
					allocat.deallocate(contents);
			}
			//contents = (somekindofclass*)(mulestlcalloc(elc, sizeof(somekindofclass)));
			contents = allocat.allocate(elc);
			if (contents == NULL)
				return false;
			contents_size = elc;
			return true;
		  }
		  
		  int mulegetallocatedelements() const { return contents_els; }
		  
		  somekindofclass& at(int pos) const { 
			if (pos > contents_els || pos < 0) {
				mulestldebug("line %d, planning to crash the whole program", __LINE__);
				somekindofclass* nullgenerator = NULL;
				return *(nullgenerator);
			}
			return contents[pos]; 
		  }
		  
		  somekindofclass& operator[](int pos) { return this->at(pos); }
		  
		  int size() const { return contents_els; }
		  
		  void erase(int pos) {
			if (pos > contents_els || pos < 0)
				return;
			bool reachedel = false;
			int old_cels = contents_els;
			mulestldebug("pos = %d, old_cels = %d", pos, old_cels);
			for (int i = 0; i < old_cels; i++) {
				mulestldebug("i = %d", i);
				if (i == pos) {
					mulestldebug("reached %d == %d", i, pos);
					reachedel = true;
					contents[i] = NULL;
					contents_els = contents_els - 1;
				}
				if (reachedel == true && (i + 1) < old_cels) {
					somekindofclass nextitem = contents[i + 1];
					contents[i] = nextitem;
					contents[i + 1] = NULL;
				}
			}
		  }
		  
		  void resize(int size, somekindofclass el) {
			if (size > MULESTL_VECTOR_MAXELEMENTCOUNT || size == contents_els || size < contents_els)
				return;
			if (contents_els > 0) {
				int old_cels = contents_els;
				mulestldebug("line %d, contents_els = %d\n", __LINE__, contents_els);
				somekindofclass origcont[old_cels];
				for (int i = 0; i < old_cels; i++) {
					mulestldebug("i = %d");
					origcont[i] = this->at(i);
					mulestldebug("success");
				}
				if (this->reserve(size) == true)
					mulestldebug("this->reserve(%d) success", size);
				else
					mulestldebug("this->reserve(%d) fail", size);
				for (int j = 0; j < old_cels; j++)
					contents[j] = origcont[j];
			}
			else
				this->reserve(size);
			mulestldebug("size = %d", size);
			contents[size - 1] = el;
			contents_els = contents_els + 1;
			
		  }
		  
		  void push_back(const somekindofclass& el) { this->resize(this->size() + 1, el); }
		  
		  somekindofclass& back() const { return this->at(this->size() - 1); }
		  
		  somekindofclass& front() const { return this->at(0); }
		  
		  void pop_back() { this->erase(this->size() - 1); }
		  
		  somekindofclass* begin() const { return contents; }
		  
		  somekindofclass* end() const { return (contents + contents_els); }
		  
		  
		protected:
		  somekindofclass* contents;
		  int contents_size;
		  int contents_els;
		  nonstlallocat allocat;
		  //bool notthefirsttime;
		  
	};
	
#ifdef MULESTL_FEATURES_STRINGS
	class string {
	
		public:
		  static const int npos = -1;
		  
		  string(const char* str)	{ muleStringInternalImport(str); }
		  
		  string(char* str)		{ muleStringInternalImport(str); }
		  
		  string(const string& str)	{ muleStringInternalImport(str); }
		  
		  string(char character)	{ muleStringInternalImport(character); }
		  
		  string(string* str)		{ muleStringInternalImport(str->c_str()); }
		  
		  ~string() {
			this->clear();
			delete strcnt;
		  }
		  
		  char* c_str() const {
			char* returnedval = (char*)(mulestlcalloc(strcnt->size() + 1, sizeof(char)));
			for (int i = 0; i < strcnt->size(); i++)
				returnedval[i] = strcnt->at(i);
			returnedval[strcnt->size()] = '\0';
			return returnedval; 
		  }
		  int size() const { return strcnt->size(); }
		  int length() const { return this->size(); }
		  int find(const string& str, int pos = 0) const {
			int argstrlength = str.size();
			for (int i = pos; i < this->size(); i++) {
				if (str.substr(i, argstrlength) == str)
					return i;
			}
			return -1;
		  }
		  char* begin() const { return strcnt->begin(); }
		  char* end() const { return strcnt->end(); }
		  void clear() {
			strcnt->clear();
			delete strcnt;
			strcnt = new vector<char>();
		  }
		  int max_size() const { return strcnt->max_size(); }
		  bool empty() const { return strcnt->empty(); }
		  char& operator[](int pos) const { return this->at(pos); }
		  string& operator=(const string& instr) { muleStringInternalImport(instr); }
		  string& operator=(const char* instr) { muleStringInternalImport(instr); }
		  string& operator+ (const string& right) {
			this->append(right);
			return *(this);
		  }
		  string& operator+ (const char* right) { 
			this->append(right);
			return *(this);
		  }
		  string& operator+ (char right) {
			this->push_back(right);
			return *(this);
		  }
		  bool operator==(const string& right) const {
			int rightlen = right.length();
			int leftlen = this->length();
			if (rightlen != leftlen)
				return false;
			while (true) {
				rightlen = rightlen - 1;
				leftlen = leftlen - 1;
				if (this->at(leftlen) != right.at(rightlen))
					return false;
				if (rightlen == 0 || leftlen == 0)
					break;
			}
			return true;
		  }
		  char& at(int pos) const { return strcnt->at(pos); }
		  string& append(const string& str) {
			if ((str.length() + this->length()) > strcnt->max_size())
				return *(this);
			for (int i = 0; i < str.length(); i++)
				this->push_back(str.at(i));
			return *(this);
		  }
		  string& append(const char* cstr) { return this->append(string(cstr)); }
		  string& append(char character) {
			this->push_back(character);
			return *(this);
		  }
		  string& replace(int pos, int len, const string& replstr) {
			string p1 = this->muleStringLeft(pos);
			int n2 = pos + len;
			n2 = this->length() - n2;
			string p2 = this->muleStringRight(n2);
			string toset = p1 + replstr + p2;
			this->clear();
			muleStringInternalImport(toset);
			return *(this);
		  }
		  void push_back(char toadd) { strcnt->push_back(toadd); }
		  string muleStringLeft(int end) const {
			string result("");
			int rend = end;
			if (rend > (this->length() - 1) || rend < 0)
				rend = this->length();
			for (int i = 0; i < end; i++)
				result.push_back(this->at(i));
			return result;
		  }
		  string muleStringRight(int end) const {
			string result("");
			int rstart = this->length() - end;
			if (rstart < 0 || rstart > (this->length() - 1))
				rstart = this->length() - 1;
			for (int i = rstart; i < this->length(); i++)
				result.push_back(this->at(i));
			return result;
				
		  }
		  string& erase(int pos = 0, int len = -1) {
			mulestldebug("pos = %d", pos);
			if (pos < 0 || pos > (this->length() - 1))
				return *(this);
			int times = len;
			if (times < 1)
				times = this->length() - pos;
			for (int i = 0; i < times; i++) {
				mulestldebug("i = %d", i);
				if (pos < this->length())
					strcnt->erase(pos);
			}
			return *(this);
		  }
		  int copy(char* out, int len, int pos = 0) const {
			int buffermove = -1;
			for (int i = pos; i < pos + len; i++) {
				buffermove = buffermove + 1;
				out[buffermove] = this->at(i);
			}
			return len;
		  }
		  string substr(int pos = 0, int len = -1) const {
			int end = pos + len;
			if (len == -1 || len > (this->length() - pos - 1))
				end = this->length() - pos;
			string result("");
			for (int i = pos; i < end; i++)
				result.push_back(this->at(i));
			return result;
		  }
		  
		  
		protected:
		  vector<char>* strcnt;
		  int strlength;
		  void muleStringInternalImport(const char* in) {
			strlength = mulestl_strlen(in) + 1;
			strcnt = new vector<char>(in, strlength);
		  }
		  void muleStringInternalImport(char* in) {
			strlength = mulestl_strlen(in) + 1;
			strcnt = new vector<char>(in, strlength);
		  }
		  void muleStringInternalImport(char character) { muleStringInternalImport(mulestl_chartochararray(character)); }
		  void muleStringInternalImport(const string& in) { muleStringInternalImport((const char*)(in.c_str())); }
		
	};
#endif

#ifdef MULESTL_FEATURES_STREAMS	
	class istream {
		public:
		  istream(const char* nm) {
			muleIStreamInternalInit();
		  }
		  istream(const istream& in) {}
		  ~istream() {}
		  istream& operator>>(const char& in) { return *(this); }
		  istream& operator>>(const int& in) { return *(this); }
		  
		protected:
		  void muleIStreamInternalInit() {}
	};
	
	class ostream {
		public:
		  ostream(const char* nm) {
			iscout = (mulestl_streq(nm, MULESTL_STREAMS_COUTSTR) || mulestl_streq(nm, MULESTL_STREAMS_CERRSTR));
			muleOStreamInternalInit();
		  }
		  ostream(const ostream& in) {
			iscout = in.iscout;
		  }
		  ~ostream() {}
		  ostream& muleOStreamOutputOperator(const int& in) {
			if (iscout == true)
				printf("%d", in);
			return *(this);
		  }
		  ostream& muleOStreamOutputOperator(const string& in) {
			if (iscout == true)
				printf("%s", in.c_str());
			return *(this);
		  }
		  ostream& muleOStreamOutputOperator(const char& in) {
			if (iscout == true)
				printf("%c", in);
			return *(this);
		  }
		  ostream& muleOStreamOutputOperator(const char* in) {
			if (iscout == true)
				printf("%s", in);
			return *(this);
		  }
		  ostream& operator<<(const char* in) { return this->muleOStreamOutputOperator(in); }
		  ostream& operator<<(const int& in) { return this->muleOStreamOutputOperator(in); }
		  ostream& operator<<(const std::string& in) { return this->muleOStreamOutputOperator(in); }
		  ostream& operator<<(const char& in) { return this->muleOStreamOutputOperator(in); }
		  
		  
		  
		protected:
		  bool iscout;
		  void muleOStreamInternalInit() {
			if (iscout == false)
				return;
		  }
		
	};

	extern istream cin;
	extern ostream cout;
	
#endif

	
	inline string operator+(const char* left, const string& right) { return string(string(left) + right); }
	
};
#endif
