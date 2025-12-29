import sys

def main():
    # 1. Determine Input Source
    # We use a context manager helper to handle both file and stdin seamlessly
    input_source = None
    should_close = False

    if len(sys.argv) > 1:
        try:
            input_source = open(sys.argv[1], 'r')
            should_close = True
        except FileNotFoundError:
            sys.exit(f"Error: File '{sys.argv[1]}' not found.")
    else:
        input_source = sys.stdin

    try:
        # 2. Parse Header
        # Read exactly one line for the header
        try:
            header_line = next(input_source)
        except StopIteration:
            sys.exit("Error: File is empty.")

        header_parts = header_line.strip().split()

        # Basic header format validation
        if len(header_parts) < 4 or header_parts[0] != 'p' or header_parts[1] != 'cnf':
            sys.exit("Error: Invalid header format. Expected 'p cnf V C'.")

        try:
            V = int(header_parts[2])
            C = int(header_parts[3])
        except ValueError:
            sys.exit("Error: V and C in header must be integers.")

        # Dictionary to store counts of spaces: { number_of_spaces : count_of_lines }
        space_counts = {}
        
        # Track the number of clause lines processed
        clauses_processed = 0

        # 3. Process Clause Lines (Line-by-line)
        # We start enumeration at 2 because the header was line 1
        for line_num, line_content in enumerate(input_source, start=2):
            clauses_processed += 1
            
            # Optimization: If we have already read more lines than C, we can stop early
            # because the file size is already invalid (C+1 expected, now at C+2 or more).
            if clauses_processed > C:
                sys.exit(f"Error: Line count mismatch. Header specifies {C} clauses, but file has extra lines (at least {clauses_processed}).")

            clean_line = line_content.strip()
            tokens = clean_line.split()

            if not tokens:
                sys.exit(f"Error: Line {line_num} is empty or missing data.")

            # Check if line ends with '0'
            if tokens[-1] != '0':
                sys.exit(f"Error: Line {line_num} does not end with 0.")

            # Validate Bounds [-V, +V]
            for token in tokens:
                try:
                    num = int(token)
                except ValueError:
                    sys.exit(f"Error: Line {line_num} contains non-integer value '{token}'.")

                if num > V or num < -V:
                    sys.exit(f"Error: Line {line_num} contains number {num} out of bounds [-{V}, {V}].")

            # Calculate Spaces
            # Logic: Spaces = Number of Tokens - 1
            num_spaces = len(tokens) - 1

            if num_spaces in space_counts:
                space_counts[num_spaces] += 1
            else:
                space_counts[num_spaces] = 1

        # 4. Final Validation of Line Count
        # If the loop finishes, we must check if we read fewer lines than expected.
        if clauses_processed < C:
             sys.exit(f"Error: Line count mismatch. Header specifies {C} clauses, but file only has {clauses_processed} clause lines.")

        # 5. Generate Output
        sorted_L = sorted(space_counts.keys())
        output_parts = [str(V), str(C)]

        for L in sorted_L:
            K = space_counts[L]
            output_parts.append(f"{L}:{K}")

        print(" ".join(output_parts))

    finally:
        # Close the file if we opened one (stdin usually doesn't need closing here)
        if should_close and input_source:
            input_source.close()

if __name__ == "__main__":
    main()
