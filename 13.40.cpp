#include<map>
#include<string>
#include<iostream>
#include<utility>
#include<memory>
#include<algorithm>

using namespace std;


struct Strvec {
	Strvec():element(nullptr),first_free(nullptr),last_end(nullptr){}
	Strvec(const Strvec&);
	Strvec(const initializer_list<string>&);
	Strvec& operator=(const Strvec&);
	~Strvec();
	void push_back(const string&);
	size_t size() const{ return first_free - element; };
	size_t capacity() const{ return last_end - element; };
	string* begin() const{ return element; };
	string* end() const{ return first_free; };


private:
	std::allocator<std::string> alloc;
	void chk_n_alloc() {                                   
		if (size() == capacity()) reallocate();
	};
	pair<string*, string*> alloc_n_copy(const string*, const string*);
	void free();
	void reallocate();
	string *element;
	string *first_free;
	string *last_end;
};
void Strvec::push_back(const string& num) {
	chk_n_alloc();
	alloc.construct(first_free++, num);
};

Strvec::Strvec(const Strvec& tar) {
	auto p = alloc_n_copy(tar.element, tar.first_free);
	element = p.first;
	first_free = last_end = p.second;
};
Strvec& Strvec::operator=(const Strvec& tar) {
	auto p = alloc_n_copy(tar.element, tar.first_free);
	free();
	element = p.first;
	first_free = last_end = p.second;
	return *this;
};
Strvec::~Strvec() {
	free();
};
Strvec::Strvec(const initializer_list<string>& p) {
	auto size = p.end() - p.begin();
	auto b = alloc.allocate(size);
	element = first_free = last_end = b;
	for (auto a:p) push_back(a);
};

pair<string*, string*> Strvec::alloc_n_copy(const string* a, const string* b) {
	auto data = alloc.allocate(b - a);
	auto temp = b - a;
	return { data, data+temp};
};
void Strvec::free() {
	if (element)
		for (auto p = first_free; p != element; )		alloc.destroy(--p);
	
	alloc.deallocate(element, last_end - element);
};
void Strvec::reallocate() {
	auto newcapacity = size() ? 2*size() : 1;
	auto newdata = alloc.allocate(newcapacity);
	auto dest = newdata;
	auto elen = element;
	for (; elen != first_free;)
		alloc.construct(dest++, move(*elen++));
	free();
	element = newdata;
	first_free = dest;
	last_end = element + newcapacity;

};




int main() {
	Strvec test;
	test.push_back("fsefsef");
	test.push_back("fsefsef");
	test.push_back("fseef");
	test.push_back("fsef");
	for (auto p = test.begin(); p != test.end(); p++)
		cout << *p << endl;

	Strvec test2{"fesff","fesdf"};

	for (auto p = test2.begin(); p != test2.end(); p++)
		cout << *p << endl;

	system("pause");


}

