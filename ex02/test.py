#!/usr/bin/env python3
import random
import subprocess
import sys

def run_test(values):
    args = [str(v) for v in values]
    result = subprocess.run(["./PmergeMe"] + args, capture_output=True, text=True, timeout=30)
    if result.returncode != 0:
        return False, f"non-zero exit: {result.returncode}, stderr={result.stderr}"

    lines = result.stdout.strip().split("\n")
    if len(lines) != 4:
        return False, f"expected 4 lines, got {len(lines)}: {lines}"

    before_line, after_line = lines[0], lines[1]
    if not before_line.startswith("Before:") or not after_line.startswith("After:"):
        return False, f"bad line prefixes: {before_line[:20]!r} / {after_line[:20]!r}"

    after_vals = [int(x) for x in after_line.split(":", 1)[1].split()]
    expected = sorted(values)

    if after_vals != expected:
        return False, f"MISMATCH\n  input:    {values}\n  expected: {expected}\n  got:      {after_vals}"

    if len(after_vals) != len(values):
        return False, f"length mismatch: {len(after_vals)} vs {len(values)}"

    return True, "ok"

def main():
    random.seed(1234)
    failures = 0
    total = 0

    sizes_to_test = list(range(1, 50)) + [63, 64, 65, 100, 127, 128, 129, 200, 500, 1000, 1500, 2999, 3000, 3001, 5000]

    for size in sizes_to_test:
        for trial in range(3 if size > 100 else 8):
            total += 1
            values = random.sample(range(1, 2_000_000), size)
            ok, msg = run_test(values)
            if not ok:
                failures += 1
                print(f"FAIL size={size} trial={trial}: {msg}")
                if failures > 5:
                    print("Too many failures, stopping early.")
                    sys.exit(1)

    print(f"\n{total - failures}/{total} tests passed.")
    if failures == 0:
        print("ALL TESTS PASSED")
    sys.exit(0 if failures == 0 else 1)

if __name__ == "__main__":
    main()