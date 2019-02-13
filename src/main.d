import std.stdio;
import std.string;

import iraroji;

int main(string[] args)
{
    if (args.length < 2) {
        "Usage: command file_path".writeln();
        return 0;
    }

    auto filePath = args[1];
    auto problem = Problem.open(filePath);
    problem.solve();

    return 0;
}

