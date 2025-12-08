use std::fs::File;
use std::io::{self, BufRead};

use ndarray::Array2;

fn read_lines(filename: &str) -> io::Result<io::Lines<io::BufReader<File>>> {
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

fn get_entry_or_zero(a: &Array2<i8>, width: usize, height: usize, xi: isize, yi: isize) -> i8 {
    let widthi = width as isize;
    let heighti = height as isize;
    if xi >= 0 &&
        xi < widthi &&
        yi >= 0 &&
        yi < heighti &&
        a[[xi as usize, yi as usize]] == 1
    {
        return 1;
    }
    return 0;
}

fn main() {
    let lines = read_lines("data/04").expect("could not open file");

    let mut map = Array2::<i8>::zeros((200, 200));
    let mut adj_counts = Array2::<i8>::zeros((200, 200));

    let mut x = 0;
    let mut y = 0;
    for line in lines.map_while(Result::ok) {
        x = 0;
        for c in line.chars() {
            if c == '@' {
                map[[x, y]] = 1;
            }
            x += 1;
        }
        y += 1;
    }

    let width = x;
    let height = y;

    let mut n_removed = 0;
    loop {
        let mut did_iter_remove = false;
        for y in 0..height {
            for x in 0..width {
                let mut adj_count = 0;
                let xi = x as isize;
                let yi = y as isize;
                adj_count += get_entry_or_zero(&map, width, height, xi - 1, yi - 1);
                adj_count += get_entry_or_zero(&map, width, height, xi    , yi - 1);
                adj_count += get_entry_or_zero(&map, width, height, xi + 1, yi - 1);
                adj_count += get_entry_or_zero(&map, width, height, xi - 1, yi);
                adj_count += get_entry_or_zero(&map, width, height, xi + 1, yi);
                adj_count += get_entry_or_zero(&map, width, height, xi - 1, yi + 1);
                adj_count += get_entry_or_zero(&map, width, height, xi    , yi + 1);
                adj_count += get_entry_or_zero(&map, width, height, xi + 1, yi + 1);
                adj_counts[[x, y]] = adj_count;
            }
        }
        for y in 0..height {
            for x in 0..width {
                if map[[x, y]] == 1 && adj_counts[[x, y]] < 4 {
                    n_removed += 1;
                    map[[x, y]] = 0;
                    did_iter_remove = true;
                }
            }
        }
        if !did_iter_remove {
            break;
        }
    }

    println!("{}", n_removed);
}
