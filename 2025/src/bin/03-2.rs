use std::fs::File;
use std::io::{self, BufRead};

const TARGET_LEN: usize = 12;

fn read_lines(filename: &str) -> io::Result<io::Lines<io::BufReader<File>>> {
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

fn get_max_digit(s: &str) -> (i64, usize) {
    let mut max_digit = 0;
    let mut max_digit_idx = 0;
    for (idx, c) in s.chars().enumerate() {
        let digit = c.to_digit(10).expect("couldn't get digit") as i64;
        if digit > max_digit {
            max_digit = digit;
            max_digit_idx = idx;
        }
    }
    return (max_digit, max_digit_idx);
}

fn main() {
    let mut max_joltage_sum: i64 = 0;
    let lines = read_lines("data/03-test1").expect("could not open file");
    for line in lines.map_while(Result::ok) {
        let mut pivot_idx = 0;
        let mut max_joltage = 0;
        let mut n_digits_found = 0;
        while n_digits_found < TARGET_LEN {
            let n_digits_left = TARGET_LEN - n_digits_found;
            // println!("{} to {}", pivot_idx, line.len() - n_digits_left);
            let s_slice = &line[pivot_idx..(line.len() - n_digits_left + 1)];
            let (max_digit, max_digit_idx) = get_max_digit(s_slice);
            // println!("s_slice {}", s_slice);
            // println!("max_digit {}", max_digit);
            // println!("max_digit_idx {}", max_digit_idx);
            // println!("pivot_idx {}", pivot_idx);
            // println!("n_digits_found {}", n_digits_found);
            // println!("");
            pivot_idx += max_digit_idx + 1;
            max_joltage *= 10;
            max_joltage += max_digit;
            n_digits_found += 1;
        }
        println!("{}", max_joltage);
        max_joltage_sum += max_joltage;
    }
    println!("{}", max_joltage_sum);
}
