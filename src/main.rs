use std::io::{self, BufRead, Write};

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
struct Point {
    x: i32,
    y: i32,
}

impl Point {
    fn valid(self) -> bool {
        (0..10).contains(&self.x) && (0..10).contains(&self.y)
    }

    fn index(self) -> usize {
        (self.y * 10 + self.x) as usize
    }
}

struct Bot {
    blocked: [bool; 100],
    mine: [Point; 4],
    enemy: [Point; 4],
    turn: u32,
}

const ORTHOGONAL: [Point; 4] = [
    Point { x: 1, y: 0 },
    Point { x: -1, y: 0 },
    Point { x: 0, y: 1 },
    Point { x: 0, y: -1 },
];

const MINE_START: [Point; 4] = [
    Point { x: 3, y: 0 },
    Point { x: 6, y: 0 },
    Point { x: 0, y: 3 },
    Point { x: 9, y: 3 },
];

impl Bot {
    fn new(starting_with: i32) -> Self {
        let (mine, enemy) = if starting_with == 1 {
            (
                MINE_START,
                [
                    Point { x: 6, y: 9 },
                    Point { x: 3, y: 9 },
                    Point { x: 9, y: 6 },
                    Point { x: 0, y: 6 },
                ],
            )
        } else {
            (
                [
                    Point { x: 6, y: 9 },
                    Point { x: 3, y: 9 },
                    Point { x: 9, y: 6 },
                    Point { x: 0, y: 6 },
                ],
                MINE_START,
            )
        };

        let blocked = [false; 100];
        Self {
            blocked,
            mine,
            enemy,
            turn: 0,
        }
    }

    fn occupancy(&self, p: Point) -> bool {
        p.valid()
            && (self.blocked[p.index()]
                || self.mine.contains(&p)
                || self.enemy.contains(&p))
    }

    fn all_moves(&self, start: Point) -> Vec<Point> {
        let mut out = vec![start];
        for d in ORTHOGONAL {
            let n = Point { x: start.x + d.x, y: start.y + d.y };
            if n.valid() && !self.occupancy(n) {
                out.push(n);
                for d2 in ORTHOGONAL {
                    let n2 = Point { x: n.x + d2.x, y: n.y + d2.y };
                    if n2.valid() && n2 != start && !self.occupancy(n2) {
                        if !out.contains(&n2) {
                            out.push(n2);
                        }
                    }
                }
            }
        }
        out
    }

    fn choose_move(&mut self) -> [i32; 6] {
        self.turn += 1;

        let mut best: Option<(i32, Point, Point, Point)> = None;
        for (unit_index, &old) in self.mine.iter().enumerate() {
            for &new in &self.all_moves(old) {
                for d in ORTHOGONAL {
                    let shoot = Point { x: new.x + d.x, y: new.y + d.y };
                    if !shoot.valid()
                        || self.occupancy(shoot)
                        || self.mine.contains(&shoot)
                        || self.enemy.contains(&shoot)
                    {
                        continue;
                    }

                    let enemy_mobility: i32 = self
                        .enemy
                        .iter()
                        .map(|&enemy| {
                            let mut provisional = self.blocked;
                            provisional[shoot.index()] = true;
                            enemy_reachable_count(&provisional, enemy)
                        })
                        .sum();

                    let own_mobility = self.all_moves(old).len() as i32;
                    let distance_to_enemy = self
                        .enemy
                        .iter()
                        .map(|&e| (new.x - e.x).abs() + (new.y - e.y).abs())
                        .min()
                        .unwrap_or(20);
                    let shoot_priority = if (self.turn % 3) == 0 { 1 } else { 0 };
                    let score = -enemy_mobility * 20
                        + own_mobility
                        - distance_to_enemy
                        + shoot_priority
                        - unit_index as i32;

                    if best.map_or(true, |(best_score, _, _, _)| score > best_score) {
                        best = Some((score, old, new, shoot));
                    }
                }
            }
        }

        let (old, new, shoot) = match best {
            Some((_, old, new, shoot)) => (old, new, shoot),
            None => (self.mine[0], self.mine[0], Point { x: 0, y: 0 }),
        };

        let unit = self.mine.iter().position(|&p| p == old).unwrap_or(0);
        self.blocked[old.index()] = false;
        self.mine[unit] = new;
        self.blocked[new.index()] = true;
        self.blocked[shoot.index()] = true;

        [old.x, old.y, new.x, new.y, shoot.x, shoot.y]
    }

    fn record_enemy_move(&mut self, values: [i32; 6]) {
        let old = Point { x: values[0], y: values[1] };
        let new = Point { x: values[2], y: values[3] };
        let concrete = Point { x: values[4], y: values[5] };

        if let Some(unit) = self.enemy.iter().position(|&p| p == old) {
            self.enemy[unit] = new;
        } else if old.valid() {
            if let Some(unit) = self.enemy.iter().position(|&p| !p.valid()) {
                self.enemy[unit] = new;
            }
        }

        if old.valid() {
            self.blocked[old.index()] = false;
        }
        if new.valid() {
            self.blocked[new.index()] = true;
        }
        if concrete.valid() {
            self.blocked[concrete.index()] = true;
        }
    }
}

fn enemy_reachable_count(blocked: &[bool; 100], start: Point) -> i32 {
    let mut seen = [false; 100];
    let mut queue = vec![start];
    seen[start.index()] = true;
    let mut count = 0;
    while let Some(p) = queue.pop() {
        count += 1;
        for d in ORTHOGONAL {
            let n = Point { x: p.x + d.x, y: p.y + d.y };
            if n.valid() && !blocked[n.index()] && !seen[n.index()] {
                seen[n.index()] = true;
                queue.push(n);
            }
        }
    }
    count
}

fn read_ints<T: BufRead>(reader: &mut T, count: usize) -> io::Result<Option<[i32; 6]>> {
    let mut values = [0_i32; 6];
    let mut read = 0;
    loop {
        let mut line = String::new();
        if reader.read_line(&mut line)? == 0 {
            return Ok(None);
        }
        for token in line.split_whitespace() {
            if read >= count {
                break;
            }
            if let Ok(value) = token.parse::<i32>() {
                values[read] = value;
                read += 1;
            }
        }
        if read >= count {
            break;
        }
    }
    Ok(Some(values))
}

fn main() -> io::Result<()> {
    let stdin = io::stdin();
    let mut input = stdin.lock();
    let stdout = io::stdout();
    let mut output = stdout.lock();

    let starting = match read_ints(&mut input, 1)? {
        Some(values) => values[0],
        None => return Ok(()),
    };
    if starting != 1 && starting != 2 {
        writeln!(output, "ERROR: Program start with bad number: {starting} :D")?;
        output.flush()?;
        return Ok(());
    }

    let mut bot = Bot::new(starting);

    if starting == 1 {
        let _time = read_ints(&mut input, 1)?;
        let move_data = bot.choose_move();
        writeln!(
            output,
            "{} {} {} {} {} {}",
            move_data[0] + 1,
            move_data[1] + 1,
            move_data[2] + 1,
            move_data[3] + 1,
            move_data[4] + 1,
            move_data[5] + 1
        )?;
        output.flush()?;
        if let Some(enemy_move) = read_ints(&mut input, 6)? {
            let mut normalized = enemy_move;
            for value in &mut normalized {
                *value -= 1;
            }
            bot.record_enemy_move(normalized);
        }
    } else {
        if let Some(enemy_move) = read_ints(&mut input, 6)? {
            let mut normalized = enemy_move;
            for value in &mut normalized {
                *value -= 1;
            }
            bot.record_enemy_move(normalized);
        }
    }

    loop {
        if read_ints(&mut input, 1)?.is_none() {
            break;
        }
        let move_data = bot.choose_move();
        writeln!(
            output,
            "{} {} {} {} {} {}",
            move_data[0] + 1,
            move_data[1] + 1,
            move_data[2] + 1,
            move_data[3] + 1,
            move_data[4] + 1,
            move_data[5] + 1
        )?;
        output.flush()?;
        if let Some(enemy_move) = read_ints(&mut input, 6)? {
            let mut normalized = enemy_move;
            for value in &mut normalized {
                *value -= 1;
            }
            bot.record_enemy_move(normalized);
        } else {
            break;
        }
    }

    writeln!(output, "Program end.")?;
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn creates_valid_initial_position() {
        let bot = Bot::new(1);
        assert!(bot.mine.iter().all(|p| p.valid()));
        assert_eq!(bot.mine.len(), 4);
    }

    #[test]
    fn move_is_inside_board_and_valid() {
        let mut bot = Bot::new(1);
        let result = bot.choose_move();
        for i in (0..6).step_by(2) {
            assert!((1..=10).contains(&(result[i] + 1)));
            assert!((1..=10).contains(&(result[i + 1] + 1)));
        }
    }

    #[test]
    fn enemy_record_updates_concrete_field() {
        let mut bot = Bot::new(1);
        bot.record_enemy_move([6, 9, 3, 9, 3, 8]);
        assert!(bot.blocked[8 * 10 + 3]);
    }
}
