use std::cmp;
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
            break;
        }
        if line.contains("-") {
            let parts: Vec<_> = line
                .split("-")
                .map(|it| it.parse::<u64>().expect("couldn't parse number"))
                .collect();
            let mut start = *parts.get(0).expect("couldn't get range start");
            let mut end = *parts.get(1).expect("couldn't get range end");
            println!("considering {}-{}", start, end);
            for range in &mut fresh_ranges {
                if (range.0 >= start && range.0 <= end)
                    || (range.1 >= start && range.1 <= end)
                    || (start >= range.0 && start <= range.1)
                    || (end >= range.0 && end <= range.1)
                {
                    let new_start = cmp::min(range.0, start);
                    let new_end = cmp::max(range.1, end);
                    println!("\tchanging {}-{} to {}-{}", range.0, range.1, new_start, new_end);
                    start = new_start;
                    end = new_end;
                    range.0 = 0;
                    range.1 = 0;
                }
            }
            println!("\tadding {}-{}", start, end);
            fresh_ranges.push((start, end));
        }
    }
    for range in &fresh_ranges {
        if range.0 == 0 || range.1 == 0 {
            continue;
        }
        n_fresh += range.1 - range.0 + 1;
    }
    println!("{}", n_fresh);
}
