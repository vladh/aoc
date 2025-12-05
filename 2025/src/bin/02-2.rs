use std::fs::File;
use std::io::{self, BufRead};

fn read_lines(filename: &str) -> io::Result<io::Lines<io::BufReader<File>>> {
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

fn is_multi_repeated(n: i64) -> bool {
    let s = n.to_string();
    for divisor in 2..=8 {
        if s.len() % divisor == 0 {
            let mut found = true;
            let chunk_len = s.len() / divisor;
            let n_chunks = s.len() / chunk_len;
            let first_chunk = &s[..chunk_len];
            for idx_chunk in 1..n_chunks {
                let chunk_start = idx_chunk * chunk_len;
                let chunk_end = (idx_chunk + 1) * chunk_len;
                let chunk = &s[chunk_start..chunk_end];
                if chunk != first_chunk {
                    found = false;
                    break;
                }
            }
            if found {
                return true;
            }
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
                if is_multi_repeated(n) {
                    n_repeated += 1;
                    repeated_sum += n;
                }
            }
        }
    }
    println!("n_repeated\t{}", n_repeated);
    println!("repeated_sum\t{}", repeated_sum);
}
