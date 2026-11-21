# C++ to Rust conversion record

## Scope and source reference

This note records the conversion work performed in this repository. The C++ source used as the reference was read from commit [`23202f8cb4644aff0d9cf85ce9605283e1a4f9b2`](https://github.com/neurodine-com/conixer/tree/23202f8cb4644aff0d9cf85ce9605283e1a4f9b2), especially `src/main.cpp`, `src/fighting_structures.h`, `src/BaseBlock.*`, `src/TPath.*`, `src/FieldSpace.*`, `src/CPathFinder.*`, and `src/tel.*`.

The Rust conversion replaced the C++ implementation rather than retaining it behind a wrapper. It was implemented as a standalone Cargo binary with no third-party dependencies. **It is not a line-by-line or behavior-equivalent port of the C++ tactical strategy.** The current Rust bot uses a simpler move-scoring strategy, and the original source itself has an important incomplete direction table (see [Known gaps and differences](#known-gaps-and-differences)).

## Conversion actions taken

1. Added `Cargo.toml` for the `conixer` package (Rust 2021 edition, no dependencies).
2. Replaced `src/main.cpp` with `src/main.rs` and implemented:
   - `Point` for integer board coordinates and board-bound validation.
   - `Bot` state for a 10×10 board, four friendly and four enemy units, concrete/blocked cells, and a turn counter.
   - movement candidate enumeration, a simple enemy reachable-area estimate, and a weighted move selector.
   - opponent-move state recording and stdin/stdout protocol handling.
   - three focused unit tests for initial coordinates, returned coordinate bounds, and opponent concrete-field recording.
3. Replaced the C++ `Makefile` recipes with Cargo-based `all`, `test`, `clean`, and `run` targets.
4. Added Rust build/run instructions to `README.md`.
5. Removed the old C++ `.cpp` and `.h` files under `src/` so that the active implementation is Rust.
6. Added `/target/` to `.gitignore` for Cargo build artifacts.
7. Ran `cargo test` and release build validation. The first test run exposed an incorrect expected board index in the new opponent-field test; the test expectation was corrected to match the code's row-major `[y * 10 + x]` indexing. All three tests and the release build then passed.

## C++ to Rust concept mapping

| C++ concept / file | Rust implementation | Conversion approach |
| --- | --- | --- |
| `TPoint` in `fighting_structures.h` | `Point` in `src/main.rs` | Two integer coordinates plus `valid()` and row-major `index()` helpers. |
| `TBufor` (six integers) | `[i32; 6]` | Fixed-size Rust array represents the protocol's old position, new position, and concrete position. |
| `TPath`, `VV`, `SimpPath` | `Bot` unit arrays and `Vec<Point>` from `all_moves()` | Replaced the C++ path-vector structures with on-demand candidate generation rather than recreating the nested C++ structs. |
| `BaseBlock` and `FieldSpace::MOF` | `Bot::blocked: [bool; 100]` plus unit-position arrays | Fixed-size board state; no manual allocation or object-per-square hierarchy. Current unit positions are also checked in `occupancy()`. |
| `FieldSpace::RecordMovement` | `Bot::record_enemy_move()` | Converts the six received coordinates into opponent position and concrete-cell state updates. |
| `CPathFinder` tactical/pathfinding methods | `Bot::choose_move()` | Replaced the C++ tactical procedures with candidate scoring and a flood-fill estimate. This is a strategy redesign, not a direct translation. |
| `tel::readLine` / `writeLine` and `main()` | `read_ints()` and Rust `main()` | Reads whitespace-separated integers; converts incoming protocol coordinates from one-based to zero-based, and adds one to the bot's outgoing coordinates. Flushes after each response. |
| C++ `Makefile` | Cargo recipes in `Makefile` | `cargo build --release`, `cargo test`, `cargo clean`, and run the release executable. |

## Current move-selection logic

`Bot::choose_move()` increments the turn counter, considers each friendly unit, each generated destination, and each orthogonal adjacent concrete target. It rejects target cells outside the board or occupied by a known unit/concrete. Each candidate is scored as:

```text
-20 * sum(enemy reachable-cell counts after placing concrete)
+ friendly unit's current candidate count
- Manhattan distance from the destination to the nearest enemy
+ a small turn-based periodic bonus
- friendly unit index
```

The highest-scoring candidate becomes the move. State is then updated by freeing the old position, occupying the new position, and marking the concrete target. If there are no candidates, the fallback is the first friendly unit staying in place and targeting `(0, 0)`; this fallback is not guaranteed to be a legal Arbiter move and requires correction before treating it as production-safe.

`enemy_reachable_count()` performs a four-neighbor flood fill. It is an area estimate, not the C++ mutual-threat/doublet analysis. Likewise, `all_moves()` includes staying in place and computes candidates from orthogonal neighbors and their neighbors; it is not the C++ ray/range pathfinder.

## Protocol handling

The Rust program accepts a starting player identifier (`1` or `2`). For player 1 it reads the initial time value, outputs a move, and then reads the opponent's six coordinates. For player 2 it first reads the opponent's move. Subsequent turns read a time integer, output a move, then read the opponent's six integers. Incoming coordinates are decremented and outgoing coordinates incremented, matching `tel.cpp`'s one-based Arbiter convention. Output is flushed after each move. On EOF the Rust program exits and writes `Program end.` as the C++ `tel` destructor did.

## Original C++ strategy inspected

The historical C++ code represented movement/range possibilities as eight directional `SimpPath` vectors. `FieldSpace` computed reachable paths and shooting ranges, `CPathFinder` counted move/shoot threats, searched for opportunities to move and cement a field that also constrains an enemy, selected units based on restrictions, and fell back to moving/placing concrete when a tactical opportunity was not found. The top-level loop alternated reading time, emitting a move, and recording the opponent's move.

This original behavior was **not ported** into `choose_move()`. The Rust conversion retained broad concepts (board memory, four units per side, movement plus concrete placement, Arbiter text protocol) but replaced the detailed strategy with the simpler score described above.

## Known gaps and differences

- **Not strategy-equivalent:** threat maps, mutual move/shoot counts, doublet search, `WNJS`, connection-field selection, old-position cementing, and the C++ randomized tactical choices are absent from Rust.
- **Original direction data is zero-filled:** at the reference commit, `cMoveBlur[8]` in `fighting_structures.h` contains eight `{0, 0}` entries. The C++ pathfinder relies on that array for movement and shooting directions, so its apparent eight-direction path logic cannot work as intended with those values. The Rust code substitutes four hard-coded orthogonal directions, based on the documented game rules; it does not resolve or emulate the original source defect.
- **Movement semantics differ:** the documented game permits movement of up to two cells along a valid orthogonal route. Rust `all_moves()` builds paths by extending orthogonal neighbors, which may include bent two-step destinations and does not verify each path against a rule-accurate path model.
- **Initial enemy positions differ from historical C++:** Rust hard-codes an opposite-side set of four coordinates. The C++ constants at the inspected revision define `constEnemyLoc` as four `(0, 0)` points, and the constructor initializes teams differently depending on player number. Rust's positions were an assumption, not a verified match to Arbiter initialization.
- **Reachability score is approximate:** the flood fill sees only the concrete bitmap, not the locations of all units as impassable barriers; it also starts from each enemy coordinate even if that location is flagged blocked. Its score should not be read as a true legal move count.
- **Fallback legality is unverified:** the no-candidate output uses `(0, 0)` as a concrete target and does not validate it.
- **Opponent-state assumptions:** the Rust receiver updates an enemy only when the received old coordinate matches a tracked unit; otherwise, its fallback checks for an invalid-position slot, but the currently initialized enemy entries are valid. Unexpected protocol coordinates can therefore fail to update any enemy record.
- **Validation is limited:** the tests check initialization validity, output coordinate bounds, and concrete bitmap indexing. They do not compare Rust decisions against C++ decisions or validate complete game traces against an Arbiter.
- **Debug logging removed:** the C++ `DEBUGIN` traces and file-based logs were not reproduced; standard output is reserved for the Arbiter protocol.

## Build and test

```sh
 cargo test
 cargo build --release
 ./target/release/conixer
```

`make`, `make test`, and `make run` wrap these Cargo commands.

## Recommended next step for a faithful port

Before claiming equivalent gameplay, first establish the intended eight direction vectors and the authoritative starting positions from the Arbiter/game rules. Then translate and test the original candidate generation, tactical scoring, concrete placement validation, and all fallback paths against a set of recorded legal game turns. The present Rust version is best described as an initial functional rewrite of the executable/protocol and a simplified replacement strategy, not a completed semantic port.
