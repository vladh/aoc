use std::fs::File;
use std::io::{self, BufRead};

fn read_lines(filename: &str) -> io::Result<io::Lines<io::BufReader<File>>> {
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

fn main() {
    let lines = read_lines("data/05").expect("could not open file");
    let mut fresh_ranges: Vec<(u64, u64)> = vec![];
    let mut n_fresh = 0;
    for line in lines.map_while(Result::ok) {
        if line == "" {
            continue;
        }
        if line.contains("-") {
            let parts: Vec<_> = line
                .split("-")
                .map(|it| it.parse::<u64>().expect("couldn't parse number"))
                .collect();
            let start = *parts.get(0).expect("couldn't get range start");
            let end = *parts.get(1).expect("couldn't get range end");
            fresh_ranges.push((start, end));
        } else {
            let n = line.parse::<u64>().expect("couldn't parse number");
            let mut is_fresh = false;
            for range in &fresh_ranges {
                if n >= range.0 && n <= range.1 {
                    is_fresh = true;
                    break;
                }
            }
            if is_fresh {
                n_fresh += 1;
            }
        }
    }
    println!("{}", n_fresh);
}
