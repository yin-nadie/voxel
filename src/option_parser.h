/* Copyright 2017 Luis Sanz <luis.sanz@gmail.com> */

#ifndef SRC_OPTION_PARSER_H_
#define SRC_OPTION_PARSER_H_

#include <string>
#include <vector>
#include <memory>

namespace option_parser {

struct ValueBase {
	virtual ~ValueBase() {}
	virtual int parse(char **p) = 0;
};

template<typename T> class Value : public ValueBase {
 public:
	explicit Value(T *val) : val(val) {}
	~Value() {}
	int parse(char **p);

 private:
	T *val;
};

class Option {
 public:
	template<typename T> Option(char sh, const char *lo, const char *help, T *val)
		: sh(sh), lo(lo), help(help), val(new Value<T>(val)) { }

	~Option() { }

	int parse(char **p) const;
	void print_help();

 private:
	char sh;
	const char *lo;
	const char *help;
	std::shared_ptr<ValueBase> val;
};

int parse(char *argv[], int argc, Option *optv, int optc);

void print_help(Option *optv, int optc);

};  // namespace option_parser

#endif  // SRC_OPTION_PARSER_H_
