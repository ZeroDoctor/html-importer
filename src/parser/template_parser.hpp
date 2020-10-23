#pragma once

// * NOTE: single attributes will be lost 
// * i.e. the 'data-body' in <body data-body> will not appear in dom

// * NOTE: can not place templates as first and/or only attribute
// * i.e. <div {#temp}> this will break its better to do <div class="" {#temp}>

#include <iostream>
#include <bitset>
#include <stack>

#include "read_file.hpp"
#include "dom.hpp"

#define SET_FLAG(n, f) ((n) |= (f))
#define CLR_FLAG(n, f) ((n) &= ~(f))
#define TGL_FLAG(n, f) ((n) ^= (f))
#define CHK_FLAG(n, f) ((n) & (f))

constexpr std::uint_fast8_t _tag  { 1 << 0 }; // 0000 0001 tag state
constexpr std::uint_fast8_t _attr { 1 << 1 }; // 0000 0010 attribute state
constexpr std::uint_fast8_t _temp { 1 << 2 }; // 0000 0100 template state
constexpr std::uint_fast8_t _cont { 1 << 3 }; // 0000 1000 content state

struct parserResult {
	// returning
	genericTag      tag = {};
	std::string     out = "";
	bool temp_bool = false;

	// tracking
	size_t*            col;
	const size_t*      row;
	const std::string* line;
};

typedef void parser(parserResult& result, std::uint_fast8_t& flag);
inline std::unordered_map<char, parser*> parser_map; // I guess it's like a state manager

inline bool is_letter(char character) 
{
	int c = character;

	if((c >= 65 && c <= 90) || (c >= 97 && c <= 122)) {
		return true;
	}

	return false;
}

// parse_tag reads a single line as a potential tag
// so many if statements...
// in the loop there are two main if blocks: if(c == '/') and if(result.tag.name == "")
// the former checks for tag ending condition and
// the latter checks for tag starting condition as well as parseing any attributes and/or templates_ids
void parse_tag(parserResult& result, std::uint_fast8_t& flag);

// parse_attr reads a single line as a potential attribute
void parse_attr(parserResult& result, std::uint_fast8_t& flag);

// parse_template reads a single line as a potential template
void parse_template(parserResult& result, std::uint_fast8_t& flag);

std::uint_fast8_t parse_html(
		std::string line, std::vector<parserResult> &results,
		size_t row, std::string& content // this could change
	);

void create_template(std::vector<std::string> lines);

void init_parser();

Dom* parse_create_template(std::string path);