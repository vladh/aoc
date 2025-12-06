use std::fs::File;
use std::io::{self, BufRead};

fn read_lines(filename: &str) -> io::Result<io::Lines<io::BufReader<File>>> {
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

fn main() {
    let mut max_joltage_sum = 0;
    let lines = read_lines("data/03-test1").expect("could not open file");
    for line in lines.map_while(Result::ok) {
        let mut max_digit = 0;
        let mut max_digit_idx = 0;
        for (idx, c) in line[..(line.len() - 1)].chars().enumerate() {
            let digit = c.to_digit(10).expect("couldn't get digit");
            if digit > max_digit {
                max_digit = digit;
                max_digit_idx = idx;
            }
        }
        let mut submax_digit = 0;
        for c in line[(max_digit_idx + 1)..].chars() {
            let digit = c.to_digit(10).expect("couldn't get digit");
            if digit > submax_digit {
                submax_digit = digit;
            }
        }
        let max_joltage = max_digit * 10 + submax_digit;
        println!("{}", max_joltage);
        max_joltage_sum += max_joltage;
    }
    println!("{}", max_joltage_sum);
}
