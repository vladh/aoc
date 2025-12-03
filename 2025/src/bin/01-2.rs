use std::fs::File;
use std::io::{self, BufRead};

fn read_lines(filename: &str) -> io::Result<io::Lines<io::BufReader<File>>> {
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

fn main() {
    let mut n_zeros = 0;
    let mut pos = 50;
    let lines = read_lines("data/01").expect("could not open file");
    for line in lines.map_while(Result::ok) {
        let mut chars = line.chars();
        let dir = chars.next().expect("did not get direction character");
        let mut amount: i32 = chars.as_str().parse().expect("did not get amount");
        if dir == 'L' {
            amount *= -1;
        }
        let new_pos = pos + amount;
        let new_pos_mod = new_pos.rem_euclid(100);
        print!("{:2} ({}) + {:2}\t= {:2} ({})\t%= {:2} ({})",
            pos,
            pos.signum(),
            amount,
            new_pos,
            new_pos.signum(),
            new_pos_mod,
            new_pos_mod.signum());

        let mut n_crosses = (new_pos / 100).abs();
        print!(" : {}", n_crosses);
        if new_pos == 0 {
            n_crosses += 1;
            print!("+1");
        } else {
            print!("+0");
        }
        if pos.signum() == -new_pos.signum() {
            n_crosses += 1;
            print!("+1");
        } else {
            print!("+0");
        }
        print!("={} crosses", n_crosses);
        n_zeros += n_crosses;
        pos = new_pos_mod;
        println!("");
    }
    println!("{}", n_zeros);
}
