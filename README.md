# Phase Transition w/ Varying Clause Size

## The only important file
`combined.csv`: summarizes data for the 400 benchmarks used in the [SAT 2025 competition](https://satcompetition.github.io/2025/index.html). 
See full description below.

## Files
- `*.tar.xz`: solvers source code. Untar and follow compilation instructions to compile.
- `combined.csv`: Final data. Each line has the following format: `<filename>,<clause-data>,<result>,<time>` where:
  - `<filename>` is self-explanatory
  - `<clause-data>` is a comma-separated list where each element has the form `<c:l>`. It reads ``There are `c` clauses with `l` literals each.". If the list terminates with `;`, it means that we have succesfully computed this data fully. Otherwise, the time limit did not allow us to conclude the computation.
  - `<result>` can be sat/unsat/none, the latter means the solver could not solve it.
  - `<time>` is the number of seconds the solver ran.
- `count.csv` and `results.csv`: raw data used to construct `combined.csv`. The first was computed using `count.sh`. The second using some cluster scripts.
- `fetch.sh`: a script that creates a `bms` directory and populates it with the 400 benchmarks used in the SAT 2025 competition. For a description of the benchmarks, go [here](https://repositum.tuwien.at/handle/20.500.12708/218424).
- `track_main_2025.uri`: Helps `fetch.sh` to get the benchmarks. Originates from [this](https://satcompetition.github.io/2025/downloads.html) website.
- `unify.sh` A script that generates `combined.csv` from `count.csv` and `resuls.csv`

## Examples
- The first line of `combined.csv` says that the file `00d5a43a481477fa4d56a2ce152a6cfb-st_890_86_9_572.normalised.cnf` has `159` unit clauses, `76908` clauses with `2` literals, `2` clauses with `27` literals, `2` clauses with `29` literals, `886` clauses with `3` literals, etc. Further, the clauses-literal-count data was computed fully. The solver couldn't solve it within the time-limit, and the process terminated aftter a tiny bit more than 5000 seconds.
- For 4 files, we weren't able to compute clause-literal data at all within the time limit. For three of them, the solver was able to report that they are satisfiable. For one, it failed to do so and the process was terminated after ~4529 seconds (possibly due to memout).

## Evaluation setting
- time limit used: 5000 seconds for every task (solving / counting clauses-literals).
- The solver used was KISSAT, winner of SAT 2025 competition.
- We were able to compute complete data for 396 files out of 400.
