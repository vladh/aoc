use ascii;
use bufio;
use fmt;
use io;
use os;
use strconv;
use strings;
use types;

def MAX_RED: size = 12z;
def MAX_GREEN: size = 13z;
def MAX_BLUE: size = 14z;


fn get_game_value(line: str) size = {
	let is_possible = true;

	const tok_header = strings::tokenize(line, ": ");
	const header = strings::next_token(&tok_header) as str;
	const rest = strings::remaining_tokens(&tok_header);

	const header = strings::replace(header, "Game ", "");
	const id = strconv::stoz(header)!;

	const tok_games = strings::tokenize(rest, "; ");
	for (true) {
		const game = match (strings::next_token(&tok_games)) {
		case let game: str => yield game;
		case void          => break;
		};

		const tok_picks = strings::tokenize(game, ", ");
		for (true) {
			const pick = match (strings::next_token(&tok_picks)) {
			case let pick: str => yield pick;
			case void          => break;
			};

			const parts = strings::split(pick, " ");
			const count = strconv::stoz(parts[0])!;
			const color = parts[1];

			if (
				(color == "red" && count > MAX_RED) ||
				(color == "green" && count > MAX_GREEN) ||
				(color == "blue" && count > MAX_BLUE)
			) {
				is_possible = false;
			};
		};
	};

	return if (is_possible) { yield id; } else { yield 0; };
};


export fn main() void = {
	const f = os::open("./data/02")!;
	const scanner = bufio::newscanner(f, types::SIZE_MAX);
	defer bufio::finish(&scanner);
	let possible_sum = 0z;
	for (true) {
		const line = match (bufio::scan_line(&scanner)!) {
		case let s: const str => yield s;
		case io::EOF          => break;
		};
		if (len(line) == 0) { continue; };
		possible_sum += get_game_value(line);
	};
	fmt::println(possible_sum)!;
};
