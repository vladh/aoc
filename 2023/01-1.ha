use ascii;
use bufio;
use fmt;
use io;
use os;
use strconv;
use strings;
use types;


fn get_digit(iter: strings::iterator) int = {
	for (true) {
		match (strings::next(&iter)) {
		case void =>
			return 0;
		case let r: rune =>
			if (ascii::isdigit(r)) {
				const s = strings::fromrunes([r]);
				return strconv::stoi(s)!;
			};
		};
	};
};


fn get_line_value(line: str) int = {
	let iter = strings::iter(line);
	let first_digit = get_digit(iter);
	let riter = strings::riter(line);
	let second_digit = get_digit(riter);
	return first_digit * 10 + second_digit;
};


export fn main() void = {
	const f = os::open("./data/01")!;
	const scanner = bufio::newscanner(f, types::SIZE_MAX);
	defer bufio::finish(&scanner);
	let sum = 0;
	for (true) {
		const line = match (bufio::scan_line(&scanner)!) {
		case let s: const str => yield s;
		case io::EOF          => break;
		};
		if (len(line) == 0) { continue; };
		sum += get_line_value(line);
	};
	fmt::println(sum)!;
};
