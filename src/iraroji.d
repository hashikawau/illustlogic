import std.algorithm;
import std.array;
import std.conv;
import std.math;
import std.range;
import std.string;

import std.stdio;

class Problem
{
    public LineSolver[] rowSolvers;
    public LineSolver[] colSolvers;

    private this(int[][] rowHints, int[][] colHints) {
        int numRows = rowHints.length;
        int numCols = colHints.length;
        this.rowSolvers = rowHints
            .map!(lineHint => new LineSolver(lineHint, numCols))
            .array;
        this.colSolvers = colHints
            .map!(lineHint => new LineSolver(lineHint, numRows))
            .array;
    }

    public @property int numRows() {
        return rowSolvers.length; }
    public @property int numCols() {
        return colSolvers.length; }
    public @property const (int[])[] rowHints() {
        return rowSolvers
            .map!(solver => solver.hints)
            .array; }
    public @property const (int[])[] colHints() {
        return colSolvers
            .map!(solver => solver.hints)
            .array; }

    public static Problem open(string filePath) {
        auto fin = new File(filePath, "r");

        int numRows = 1;
        int numCols = 2;
        for (auto line = fin.readln().strip();
            !fin.eof;
            line = fin.readln().strip()
        ) {
            if (!line.empty) {
                auto digits = line.split()
                    .filter!(word => word.isNumeric())
                    .map!(word => word.to!int())
                    .array;
                if (digits.length < 2)
                    throw new Exception("format error");
                numRows = digits[0];
                numCols = digits[1];
                break;
            }
        }

        auto hints = fin.byLine()
            .filter!(line => !line.strip().empty())
            .map!(line => line.split().map!(word => word.to!int()).array)
            .take(numRows + numCols)
            .array;
        auto rows = hints.take(numRows);
        auto cols = hints.drop(numRows).take(numCols);

        return new Problem(rows, cols);
    }

    public void solve() {
        int n = 80;
        for (int i = 0; i < n; ++i) {
            solveRows();
            solveCols();
        }

        int maxNumHints = rowSolvers
            .map!(solver => solver.hints.length)
            .fold!max;
        foreach(solver; rowSolvers) {
            writeln(solver.toString(maxNumHints));
        }
        foreach(solver; colSolvers) {
            writeln(solver.toString(colSolvers
                .map!(solver => solver.hints.length)
                .fold!max));
        }
    }

    private void solveRows() {
        foreach (i; iota(numRows)) {
            rowSolvers[i].solve();
            foreach (j; iota(numCols)) {
                colSolvers[j].states[i] = rowSolvers[i].states[j];
            }
        }
    }

    private void solveCols() {
        foreach (j; iota(numCols)) {
            colSolvers[j].solve();
            foreach (i; iota(numRows)) {
                rowSolvers[i].states[j] = colSolvers[j].states[i];
            }
        }
    }
}

unittest {
    auto problem = Problem.open("data/test_01.txt");
    assert(problem.numRows == 1, "numRows");
    assert(problem.numCols == 2, "numCols");
    assert(problem.rowHints == [[2]], "rowHints");
    assert(problem.colHints == [[1],[1]], "colHints");
    assert(
        problem.rowSolvers
            .map!(solver => solver.hints)
            .array == [[2]],
        "rowSolvers");
    assert(
        problem.colSolvers
            .map!(solver => solver.hints)
            .array == [[1],[1]],
        "colSolvers");
}

class LineSolver
{
    public const int[] hints;
    public State[] states;

    public this(int[] hints, int numSpaces) {
        this.hints = hints;
        this.states = new State[numSpaces];
        this.states[] = State.UNKNOWN;
    }

    public @property int numHints() {
        return hints.length; }

    public @property int numSpaces() {
        return states.length; }

    public bool solve() {
        if (isCompleted())
            return true;

        HintSolver[] hintSolvers = iota(numHints)
            .map!(i => new HintSolver(hints, i, states))
            .array;

        foreach (k; iota(1)) {
        auto allResults = hintSolvers
            .map!(solver => solver.result)
            .array;

        foreach (j; iota(numSpaces)) {
            if (states[j] == State.BLACK) {
                foreach (i, hintSolver; hintSolvers) {
                    if (hintSolver.isPossible(j)
                    && hintSolvers[0..max(0, i)].all!(s => !s.isPossible(j))
                    ) {
                        foreach (s; hintSolvers[0..max(0, i+1)]) {
                            s.assignRightPossibles(j, false);
                        }
                        //hintSolver.assignRightPossibles(j, false);
                    }

                }
            }
        }
        foreach (j; iota(numSpaces).array.reverse) {
            if (states[j] == State.BLACK) {
                //foreach (i, hintSolver; hintSolvers) {
                foreach (tup; zip(iota(numHints), hintSolvers).array.reverse) {
                    int i = tup[0];
                    auto hintSolver = tup[1];
                    if (hintSolver.isPossible(j)
                    && hintSolvers[min(numHints, i + 1)..$].all!(s => !s.isPossible(j))
                    ) {
                        foreach (s; hintSolvers[min(numHints, i)..$]) {
                            s.assignLeftPossibles(j, false);
                        }
                        //hintSolver.assignLeftPossibles(j, false);
                    }
                }
            }
        }

        foreach (j; iota(numSpaces)) {
            if (states[j] == State.BLACK) {
                foreach (i, hintSolver; hintSolvers) {
                    auto aaa = zip(iota(numSpaces), hintSolver.possibles)
                        .filter!(tup => tup[1])
                        .array;
                    if (aaa.all!(tup =>
                        tup[0] <= j
                        && tup[0] + hintSolver.hint > j)
                    ) {
                        foreach (ii, s; hintSolvers) {
                            if (ii != i)
                                s.assignPosPossibles(j, false);
                        }
                    }
                }
            }
        }

        }

        auto allResults = hintSolvers
            .map!(solver => solver.result)
            .array;

        //writeln("       : %s".format(statesString()));
        //foreach (s; hintSolvers)
        //writeln("hint=%2d: %s".format(
        //    s.hint,
        //    s.possibles.map!(p => p ? "o" : ".").join("")));

        foreach (i; iota(numSpaces)) {
            if (states[i] == State.UNKNOWN) {
                if (allResults.any!(r => r[i] == State.BLACK))
                    states[i] = State.BLACK;
                else if (allResults.all!(r => r[i] == State.WHITE))
                    states[i] = State.WHITE;
            }
        }

        // ----------------------------
        foreach (j; iota(numSpaces)) {
            if (states[j] == State.BLACK) {
                auto ps = hintSolvers
                    .filter!(p => p.isPossible(j))
                    .array;
                if (ps.length > 1) {
                    auto pps = ps.map!(p =>p.results(j)).array;
                    for (int jj = 0; jj < numSpaces; ++jj) {
                        for (int ii = 0; ii < ps.length; ++ii) {
                            if (pps[ii][jj] != State.BLACK)
                                goto aaaa;
                        }
                        states[jj] = State.BLACK;
                        aaaa: ;
                    }
                }
            }
        }

        return isCompleted();
    }

    private bool isCompleted() {
        return states.all!(s => s != State.UNKNOWN);
    }

    public string statesString() {
        return states
            .map!(s => s.toString())
            .join("");
    }

    public string toString(int paddingSize) {
        auto paddedHints
            = new int[max(0, paddingSize - numHints)]
            ~ hints;
        auto hintString = paddedHints
            .map!(h => h == 0? "  ": "%2d".format(h))
            .join(" ");
        return "%s|%s|".format(
            hintString,
            statesString());
    }

}

unittest {
    LineSolver solver = null;

    solver = new LineSolver([10], 10);
    assert(solver.solve() == true, "solve()");
    assert(solver.statesString() == "##########", "solve()");

    solver = new LineSolver([10], 10);
    solver.states[4] = State.BLACK;
    assert(solver.solve() == true, "solve()");
    assert(solver.statesString() == "##########", "solve()");

    solver = new LineSolver([9], 10);
    assert(solver.solve() == false, "solve()");
    assert(solver.statesString() == "?########?", "solve()");

    solver = new LineSolver([9], 10);
    solver.states[0] = State.WHITE;
    assert(solver.solve() == true, "solve()");
    assert(solver.statesString() == ".#########", "solve()");

    solver = new LineSolver([9], 10);
    solver.states[9] = State.WHITE;
    assert(solver.solve() == true, "solve()");
    assert(solver.statesString() == "#########.", "solve()");

    solver = new LineSolver([8], 10);
    assert(solver.solve() == false, "solve()");
    assert(solver.statesString() == "??######??", "solve()");

    solver = new LineSolver([7], 10);
    assert(solver.solve() == false, "solve()");
    assert(solver.statesString() == "???####???", "solve()");

    solver = new LineSolver([6], 10);
    assert(solver.solve() == false, "solve()");
    assert(solver.statesString() == "????##????", "solve()");

    solver = new LineSolver([5], 10);
    assert(solver.solve() == false, "solve()");
    assert(solver.statesString() == "??????????", "solve()");

    solver = new LineSolver([5], 10);
    solver.states[0] = State.BLACK;
    assert(solver.solve() == true, "solve()");
    assert(solver.statesString() == "#####.....", "solve()");

    solver = new LineSolver([5], 10);
    solver.states[1] = State.BLACK;
    assert(solver.solve() == false, "solve()");
    assert(solver.statesString() == "?####?....", "solve()");

    solver = new LineSolver([5], 10);
    solver.states[8] = State.BLACK;
    assert(solver.solve() == false, "solve()");
    assert(solver.statesString() == "....?####?", "solve()");

    solver = new LineSolver([5], 10);
    solver.states[9] = State.BLACK;
    assert(solver.solve() == true, "solve()");
    assert(solver.statesString() == ".....#####", "solve()");

    solver = new LineSolver([5], 10);
    solver.states[4] = State.WHITE;
    assert(solver.solve() == true, "solve()");
    assert(solver.statesString() == ".....#####", "solve()");

    solver = new LineSolver([5], 10);
    solver.states[5] = State.WHITE;
    assert(solver.solve() == true, "solve()");
    assert(solver.statesString() == "#####.....", "solve()");

    solver = new LineSolver([4], 10);
    assert(solver.solve() == false, "solve()");
    assert(solver.statesString() == "??????????", "solve()");

    solver = new LineSolver([3], 10);
    assert(solver.solve() == false, "solve()");
    assert(solver.statesString() == "??????????", "solve()");

    solver = new LineSolver([2], 10);
    assert(solver.solve() == false, "solve()");
    assert(solver.statesString() == "??????????", "solve()");

    solver = new LineSolver([1], 10);
    assert(solver.solve() == false, "solve()");
    assert(solver.statesString() == "??????????", "solve()");

    solver = new LineSolver([1], 10);
    solver.states[0] = State.BLACK;
    assert(solver.solve() == true, "solve()");
    assert(solver.statesString() == "#.........", "solve()");

    solver = new LineSolver([1], 10);
    solver.states[9] = State.BLACK;
    assert(solver.solve() == true, "solve()");
    assert(solver.statesString() == ".........#", "solve()");

    solver = new LineSolver([0], 10);
    assert(solver.solve() == true, "solve()");
    assert(solver.statesString() == "..........", "solve()");
}

unittest {
    LineSolver solver = null;

    solver = new LineSolver([4, 5], 10);
    assert(solver.solve() == true, "solve()");
    assert(solver.statesString() == "####.#####", "solve()");

    solver = new LineSolver([2, 2], 10);
    solver.states[4] = State.BLACK;
    solver.states[6] = State.BLACK;
    assert(solver.solve() == true, "solve()");
    assert(solver.statesString() == "...##.##..", "solve()");
}

unittest {
    LineSolver solver = null;

    solver = new LineSolver([1, 6, 6, 1, 7], 30);
    assert(solver.solve() == false, "solve()");
    assert(solver.statesString() == "???????#??????#????????##?????", "solve()");

    solver = new LineSolver([1, 1, 4, 2, 3, 1, 1, 1, 1], 30);
    assert(solver.solve() == false, "solve()");
    assert(solver.statesString() == "??????????????????????????????", "solve()");

    solver = new LineSolver([1, 1, 1, 1], 30);
    assert(solver.solve() == false, "solve()");
    assert(solver.statesString() == "??????????????????????????????", "solve()");

    solver = new LineSolver([2, 1, 3, 2], 15);
    solver.states = "##.??#???#??.##".from();
    assert(solver.solve() == false, "solve()");
    assert(solver.statesString() == "##...#.??#??.##", "solve()");

    solver = new LineSolver([2, 3], 10);
    solver.states = "??.#??????".from();
    assert(solver.solve() == false, "solve()");
    assert(solver.statesString() == "??.##?????", "solve()");

    solver = new LineSolver([4, 3], 15);
    solver.states = "????.?#????????".from();
    assert(solver.solve() == false, "solve()");
    assert(solver.statesString() == "????.?##???????", "solve()");

    solver = new LineSolver([6, 2, 3, 2, 2, 2, 2, 1], 40);
    solver.states = "######...?????#?.##..????????????.????.#".from();
    assert(solver.solve() == false, "solve()");
    assert(solver.statesString() == "######...????##?.##..????????????.????.#", "solve()");
    //assert(solver.statesString() == "######...?#??##?.##..????????????.????.#", "solve()");

    solver = new LineSolver([1, 1, 1, 1, 1, 1, 1, 1, 1], 60);
    solver.states = ".?.?...????????.#.#.#....#.#..#...............#.#....#......".from();
    assert(solver.solve() == true, "solve()");
    assert(solver.statesString() == "................#.#.#....#.#..#...............#.#....#......", "solve()");

}

class HintSolver
{
    public const int[] hints;
    public const int index;
    public State[] states;

    public int leftMost;
    public int rightMost;
    public bool[] possibles;

    public this(const int[] hints, int index, State[] states) {
        this.hints = hints;
        this.index = index;
        this.states = states;

        possibles = new bool[states.length];

        if (hints[index] == 0) {
            possibles[] = true;
            return;
        }

        leftMost = findLeftMost();
        rightMost = findRightMost();

        foreach (i; iota(leftMost, rightMost + 1)) {
            if (isCompatible(hints[index], i))
                possibles[i] = true;
        }
    }

    public bool isPossible(int pos) {
        int left = max(0, pos - hints[index] + 1);
        int right = min(numSpaces, pos + 1);
        return possibles[left .. right]
            .any!(p => p);
    }

    @property int hint() {
        return hints[index];
    }

    @property int numHints() {
        return hints.length;
    }

    @property int numSpaces() {
        return states.length;
    }

    int findLeftMost() {
        int pos = 0;
        foreach (hint; hints[0..index]) {
            while (!isCompatible(hint, pos))
                ++pos;
            pos += hint + 1;
        }
        while (!isCompatible(hints[index], pos))
            ++pos;
        return pos;
    }

    int findRightMost() {
        int pos = numSpaces;
        foreach (i; iota(numHints - 1, index, -1)) {
            pos -= hints[i];
            while (!isCompatible(hints[i], pos)) {
                --pos;
            }
            --pos;
        }
        pos -= hints[index];
        while (!isCompatible(hints[index], pos))
            --pos;
        return pos;
    }

    private bool isCompatible(int hint, int headPos) {
        if (hint <= 0)
            return true;
        int firstCell = 0;
        int lastCell = numSpaces - 1;
        int tailPos = headPos + hint - 1;
        if (headPos < firstCell || tailPos > lastCell)
            throw new Exception("error: pos range violation");
        if (headPos > firstCell && states[headPos - 1] == State.BLACK)
            return false;
        if (tailPos < lastCell && states[tailPos + 1] == State.BLACK)
            return false;
        return iota(headPos, tailPos + 1)
            .all!(i => states[i] != State.WHITE);
    }

    public State[] solve() {
        return null;
    }

    public void assignPosPossibles(int pos, bool possible) {
        possibles[max(0, pos - hints[index])..pos + 1] = possible;
    }

    public void assignLeftPossibles(int pos, bool possible) {
        possibles[0..max(0, pos - hints[index])] = possible;
    }

    public void assignRightPossibles(int pos, bool possible) {
        possibles[min(numSpaces, pos + 1)..$] = possible;
    }

    public @property State[] result() {
        //writeln("hint=%s, index=%s: %s".format(
        //    hints[index],
        //    index,
        //    possibles.map!(p => p ? "o" : ".").join("")));
        int maxCount = possibles.filter!(p => p).array.length;
        int[] counts = new int[states.length];
        foreach (i, p; possibles) {
            if (p)
                counts[i..i + hints[index]] += 1;
        }
        //writeln("max=%s, counts=%s".format(
        //    maxCount,
        //    counts));
        return counts
            .map!(count =>
                count == maxCount ? State.BLACK :
                count == 0 ? State.WHITE :
                State.UNKNOWN
            )
            .array;
    }

    public State[] results(int pos) {
        //writeln("---- pos=%s, hint=%s".format(pos, hints[index]));
        bool[] ps = zip(iota(numSpaces), possibles)
            .map!(tup => tup[1]
                && tup[0] <= pos
                && tup[0] + hints[index] > pos)
            .array;
        //writeln("---- next=%s".format(ps));

        int maxCount = ps.filter!(p => p).array.length;
        int[] counts = new int[states.length];
        foreach (i, p; ps) {
            if (p)
                counts[i..i + hints[index]] += 1;
        }
        //writeln("max=%s, counts=%s".format(
        //    maxCount,
        //    counts));
        return counts
            .map!(count =>
                count == maxCount ? State.BLACK :
                count == 0 ? State.WHITE :
                State.UNKNOWN
            )
            .array;
    }

}

unittest {
    HintSolver solver = null;
    int[] hints = [1, 1, 2, 1, 1];
    State[] states = "??????????".from();

    solver = new HintSolver(hints, 0, states);
    solver.solve();
    assert(solver.result.map!(toString).join("") == "#.........");

    solver = new HintSolver(hints, 1, states);
    solver.solve();
    assert(solver.result.map!(toString).join("") == "..#.......");

    solver = new HintSolver(hints, 2, states);
    solver.solve();
    assert(solver.result.map!(toString).join("") == "....##....");

    solver = new HintSolver(hints, 3, states);
    solver.solve();
    assert(solver.result.map!(toString).join("") == ".......#..");

    solver = new HintSolver(hints, 4, states);
    solver.solve();
    assert(solver.result.map!(toString).join("") == ".........#");

    hints = [2, 2];
    states = "????#?#???".from();
    solver = new HintSolver(hints, 0, states);
    solver.solve();
    assert(solver.result.map!(toString).join("") == "?????.....");
    solver = new HintSolver(hints, 1, states);
    solver.solve();
    assert(solver.result.map!(toString).join("") == "...??.????");

    hints = [0];
    states = "??????????".from();
    solver = new HintSolver(hints, 0, states);
    solver.solve();
    assert(solver.result.map!(toString).join("") == "..........");

    hints = [5];
    states = "????.?????".from();
    solver = new HintSolver(hints, 0, states);
    solver.solve();
    assert(solver.result.map!(toString).join("") == ".....#####");

    hints = [9];
    states = ".?????????".from();
    solver = new HintSolver(hints, 0, states);
    solver.solve();
    assert(solver.result.map!(toString).join("") == ".#########");

}

unittest {
    HintSolver solver = null;
    int[] hints = [1, 1, 4, 2, 3, 1, 1, 1, 1];
    State[] states = "??????????????????????????????".from();
    //writeln(states);

    solver = new HintSolver(hints, 0, states);
    auto s = solver.solve();
    //writeln(s);
}

enum State
{
    UNKNOWN,
    WHITE,
    BLACK,
}

string toString(State state) {
    switch(state) {
        case State.UNKNOWN: return "?";
        case State.WHITE: return ".";
        case State.BLACK: return "#";
        default: throw new Exception("no State");
    }
}

string toString(State[] states) {
    return states.map!(toString).join("");
}

State[] from(string stateString) {
    return stateString
        .map!(ch => 
            ch == '#' ? State.BLACK:
            ch == '.' ? State.WHITE:
            State.UNKNOWN)
        .array;
}

class Combination(T)
{
    private T[] indexPattern;

    public this(T[][] patterns) {
        if (patterns == null
        || patterns.length == 0
        || patterns.any!(array => array.length == 0)
        )
            throw new Exception("patterns must not be null or empty");
        this.patterns = patterns;
        this.indexPattern = null;
    }

    public const T[][] patterns;
    public @property const(T)[] next() {
        if (indexPattern == null) {
            indexPattern = new T[patterns.length];
            indexPattern[] = T.init;
            return current;
        }

        if (indexPattern[0] > patterns[0][$-1])
            return null;
        foreach (i; iota(patterns.length).array.reverse) {
            ++indexPattern[i];
            if (indexPattern[i] < patterns[i].length) {
                break;
            }
            if (i == 0)
                break;
            indexPattern[i] = 0;
        }

        return current;
    }

    public @property const(T)[] current() {
        if (indexPattern[0] >= patterns[0].length)
            return null;
        return zip(patterns, indexPattern)
            .map!(tup => tup[0][tup[1]])
            .array;
    }
}

unittest {
    auto c = new Combination!int([
        [1, 2],
        [3],
        [4, 5, 6]
    ]);
    assert(c.next == [1,3,4], "pattern: 1");
    assert(c.next == [1,3,5], "pattern: 2");
    assert(c.next == [1,3,6], "pattern: 3");
    assert(c.next == [2,3,4], "pattern: 4");
    assert(c.next == [2,3,5], "pattern: 5");
    assert(c.next == [2,3,6], "pattern: 6");
    assert(c.next == null, "pattern: 7");
}

