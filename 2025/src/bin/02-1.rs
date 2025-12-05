use std::fs::File;
use std::io::{self, BufRead};

fn read_lines(filename: &str) -> io::Result<io::Lines<io::BufReader<File>>> {
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

fn is_repeated(n: i64) -> bool {
    let s = n.to_string();
    if s.len() % 2 == 0 {
        let midpoint = s.len() / 2;
        let start = &s[..midpoint];
        let end = &s[midpoint..];
        if start == end {
            return true;
        }
    }
    return false;
}

fn main() {
    let mut n_repeated = 0;
    let mut repeated_sum: i64 = 0;
    let lines = read_lines("data/02").expect("could not open file");
    for line in lines.map_while(Result::ok) {
        let rangespecs = line.split(",");
        for rangespec in rangespecs {
            let parts: Vec<_> = rangespec
                .split("-")
                .map(|it| it.parse::<i64>().expect("couldn't parse number"))
                .collect();
            let start = *parts.get(0).expect("couldn't get range start");
            let end = *parts.get(1).expect("couldn't get range end");
            for n in start..=end {
                if is_repeated(n) {
                    n_repeated += 1;
                    repeated_sum += n;
                }
            }
        }
    }
    println!("n_repeated\t{}", n_repeated);
    println!("repeated_sum\t{}", repeated_sum);
}
