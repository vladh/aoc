use std::fs::File;
use std::io::{self, BufRead};

fn read_lines(filename: &str) -> io::Result<io::Lines<io::BufReader<File>>> {
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

fn main() {
    let mut operations: Vec<&str> = Vec::new();
    let mut results: Vec<u64> = Vec::new();

    let lines: Vec<String> = read_lines("data/06")
        .expect("could not open file")
        .collect::<Result<_, _>>()
        .expect("could not collect lines");

    for line in lines.iter() {
        let first_char = line.chars().nth(0).expect("could not get first character");
        if first_char == '+' || first_char == '*' {
            operations = line.split('\t').collect();
        }
    }

    results.resize(operations.len(), 0);

    for (idx_line, line) in lines.iter().enumerate() {
        let first_char = line.chars().nth(0).expect("could not get first character");
        if first_char == '+' || first_char == '*' {
            continue;
        }
        let numbers: Vec<u64> = line
            .split('\t')
            .map(|it| it.parse::<u64>().expect("couldn't parse number"))
            .collect();
        for (idx_num, num) in numbers.iter().enumerate() {
            if idx_line == 0 {
                results[idx_num] = *num;
            } else {
                if operations[idx_num] == "+" {
                    results[idx_num] = results[idx_num] + *num;
                } else if operations[idx_num] == "*" {
                    results[idx_num] = results[idx_num] * *num;
                }
            }
            results[0] = results[0]
        }
    }

    let answer: u64 = results.iter().sum();
    println!("{}", answer);
}
