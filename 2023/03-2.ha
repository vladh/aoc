use ascii;
use bufio;
use fmt;
use io;
use os;
use strconv;
use strings;
use types;

type linenum = struct {
	val: int,
	idx_start: int,
	idx_end: int,
};

type lineparts = struct {
	numbers: []linenum,
	symbol_idxs: []int,
};


fn lineparts_print(line: lineparts) void = {
	fmt::print("(numbers")!;
	for (let idx = 0z; idx < len(line.numbers); idx += 1) {
		fmt::printf(" ({} {} {})",
			line.numbers[idx].val,
			line.numbers[idx].idx_start,
			line.numbers[idx].idx_end)!;
	};
	fmt::print(") (symbol_idxs")!;
	for (let idx = 0z; idx < len(line.symbol_idxs); idx += 1) {
		fmt::printf(" {}", line.symbol_idxs[idx])!;
	};
	fmt::println(")")!;
};


fn lineparts_get_sum(prev_line: lineparts, curr_line: lineparts, next_line: lineparts) int = {
	let sum = 0;
	for (let idx_symb = 0z; idx_symb < len(curr_line.symbol_idxs); idx_symb += 1) {
		const symbol_idx = curr_line.symbol_idxs[idx_symb];
		let n_adj_numbers = 0;
		let product = 1;
		let lines = [prev_line, curr_line, next_line];
		for (let idx_line = 0z; idx_line < len(lines); idx_line += 1) {
			const line = lines[idx_line];
			for (let idx_num = 0z; idx_num < len(line.numbers); idx_num += 1) {
				const number = line.numbers[idx_num];
				if (symbol_idx >= number.idx_start - 1 && symbol_idx <= number.idx_end + 1) {
					product *= number.val;
					n_adj_numbers += 1;
				};
			};
		};
		if (n_adj_numbers == 2) {
			sum += product;
		};
	};
	return sum;
};


fn lineparts_parse(line_str: str) lineparts = {
	let iter = strings::iter(line_str);
	let line = lineparts { ... };
	let curr_num = linenum { ... };
	let are_parsing_num = false;
	for (let idx = 0z; true; idx += 1) {
		const next = strings::next(&iter);
		if (next is rune) {
			const r = next as rune;
			if (ascii::isdigit(r)) {
				if (!are_parsing_num) {
					curr_num.idx_start = idx: int;
				};
				are_parsing_num = true;
				const digit = strconv::stoi(
					strings::fromrunes([r]))!;
				curr_num.val *= 10;
				curr_num.val += digit;
				continue;
			} else if (r == '*') {
				append(line.symbol_idxs, idx: int);
			};
		};
		if (are_parsing_num) {
			curr_num.idx_end = (idx: int) - 1;
			append(line.numbers, curr_num);
			are_parsing_num = false;
			curr_num = linenum { ... };
		};
		if (next is void) {
			break;
		};
	};
	return line;
};


export fn main() void = {
	const f = os::open("./data/03")!;
	const scanner = bufio::newscanner(f, types::SIZE_MAX);
	defer bufio::finish(&scanner);

	let prev_line = lineparts{ ... };
	let curr_line = lineparts { ... };
	let next_line = lineparts { ... };
	let sum = 0;

	for (let idx = 0z; true; idx += 1) {
		const line_str = match (bufio::scan_line(&scanner)!) {
		case let s: const str => yield s;
		case io::EOF          => break;
		};
		if (len(line_str) == 0) { continue; };
		prev_line = curr_line;
		curr_line = next_line;
		next_line = lineparts_parse(line_str);
		sum += lineparts_get_sum(prev_line, curr_line, next_line);
	};

	prev_line = curr_line;
	curr_line = next_line;
	next_line = lineparts { ... };
	sum += lineparts_get_sum(prev_line, curr_line, next_line);

	fmt::println(sum)!;
};
