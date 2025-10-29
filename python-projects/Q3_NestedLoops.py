# This program draws a house using nested loops and string formatting.

# Roof
for i in range(5):  # Outer loop for rows
    print(" " * (34 - 4 * i), end="")  # Print leading spaces
    for j in range(i + 1):  # Inner loop for stars
        print("*   ", end="")  # Print stars with spacing
    print()  # New line after each row

# Walls
for i in range(4):  # Outer loop for rows
    print(" " * 22, end="")  # Print leading spaces
    for j in range(2):  # Inner loop for wall stars
        if j == 0:
            print("*", end="")  # Left wall
        else:
            print("                       *", end="")  # Right wall
    print()  # New line after each row

# Floor
for i in range(1):  # Single row for the floor
    print(" " * 22, end="")  # Print leading spaces
    for j in range(7):  # Inner loop for floor stars
        print("*   ", end="")  # Print stars with spacing
    print()  # New line after the floor

