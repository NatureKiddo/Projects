import math

def ln_approximation():
    print("Natural Logarithm Approximation: ln(1 + x)")

    # Get valid x
    while True:
        try:
            x = float(input("Enter a value for x (must be between -1 and 1, not inclusive): "))
            if -1 < x < 1:
                break
            else:
                print("x must be between -1 and 1 (exclusive).")
        except ValueError:
            print("Invalid input.")

    # Choose precision or max terms
    mode = input("Do you want to use precision (p) or max terms (m)? ").lower()
    precision = None
    max_terms = None

    if mode == 'p':
        precision = float(input("Enter precision (e.g., 0.000001): "))
        max_terms = 1000  # safety limit
    else:
        max_terms = int(input("Enter max number of terms to use: "))
        precision = 0  # disable precision stopping

    print("\nCalculating ln(1 + x) using Taylor Series...\n")
    print(f"{'Term':>4} {'Current Term':>20} {'Running Total':>20} {'Change':>20}")

    term = x
    total = 0
    prev_total = 0
    n = 1

    while n <= max_terms:
        term = ((-1)**(n+1)) * (x ** n) / n
        total += term
        change = abs(total - prev_total)

        print(f"{n:>4} {term:>20.10f} {total:>20.10f} {change:>20.10f}")

        if change < precision:
            break

        prev_total = total
        n += 1

    # Final results
    exact = math.log(1 + x)
    diff = abs(total - exact)

    print("\nSummary")
    print(f"Approximation: {total}")
    print(f"Exact value  : {exact}")
    print(f"Terms used   : {n}")
    print(f"Difference   : {diff}")

# Run the program
ln_approximation()
