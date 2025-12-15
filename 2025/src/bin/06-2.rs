use std::fs::File;
use std::io::{self, BufRead};

fn read_lines(filename: &str) -> io::Result<io::Lines<io::BufReader<File>>> {
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

fn main() {
    let lines: Vec<String> = read_lines("data/06")
        .expect("could not open file")
        .collect::<Result<_, _>>()
        .expect("could not collect lines");

    let mut operation = '?';
    let mut subsum: u64 = 0;
    let mut operand: u64;
    let mut sum: u64 = 0;

    for idx_char in 0..lines[0].len() {
        let mut got_only_spaces = true;
        operand = 0;
        for idx_line in 0..lines.len() {
            let c = lines[idx_line]
                .chars()
                .nth(idx_char)
                .expect("could not get char");
            if c == ' ' {
                continue;
            }
            got_only_spaces = false;
            if c == '+' || c == '*' {
                operation = c;
            } else {
                let digit = u64::from(
                    c.to_digit(10).expect("could not get digit")
                );
                operand = operand * 10 + digit;
            }
        }
        if !got_only_spaces {
            if subsum == 0 {
                subsum = operand;
            } else {
                if operation == '+' {
                    println!("{} + {} = {}",
                        subsum,
                        operand,
                        subsum + operand);
                    subsum += operand;
                } else if operation == '*' {
                    println!("{} * {} = {}",
                        subsum,
                        operand,
                        subsum * operand);
                    subsum *= operand;
                } else {
                    panic!("unknown operation");
                }
            }
        }
        if got_only_spaces || idx_char == lines[0].len() - 1 {
            println!("");
            sum += subsum;
            subsum = 0;
        }
    }

    println!("{}", sum);
}
