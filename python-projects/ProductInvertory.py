import os  # Importing os module for file existence checks

# Constant: name of the file where inventory data will be stored
FILE_NAME = "inventory.txt"

# Sample data to initialize the inventory file with some entries
sample_data = [
    "ID,Name,Category,Quantity,Price",         # Header row
    "101,Apples,Fruit,50,0.50",                # Sample product 1
    "102,Shampoo,Toiletries,20,3.75",          # Sample product 2
    "103,Bread,Bakery,30,1.25"                 # Sample product 3
]

# Function to create the inventory file with sample data if it doesn't exist
def initialize_file():
    # Only create file if it does not already exist
    if not os.path.exists(FILE_NAME):
        with open(FILE_NAME, "w") as file:
            for line in sample_data:
                file.write(line + "\n")  # Write each line to the file

# Function to add a new product to the inventory
def add_product():
    print("\n--- Add New Product ---")
    try:
        # Prompt user for product details
        new_id = input("Enter Product ID: ")
        name = input("Enter Product Name: ")
        category = input("Enter Product Category: ")
        quantity = int(input("Enter Quantity: "))
        price = float(input("Enter Price: "))

        # Check for duplicate product ID
        with open(FILE_NAME, "r") as file:
            for line in file.readlines()[1:]:  # Skip header
                if line.strip().split(",")[0] == new_id:
                    print("Product ID already exists!")  # Prevent duplicates
                    return

        # Append new product details to the file
        with open(FILE_NAME, "a") as file:
            file.write(f"{new_id},{name},{category},{quantity},{price:.2f}\n")
        print("Product added successfully.")
    except ValueError:
        print("Invalid input format. Please enter correct data types.")

# Helper function: read the inventory file and return all lines
def read_inventory():
    """Read the inventory file and return its lines."""
    with open(FILE_NAME, "r") as file:
        return file.readlines()

# Helper function: write a list of lines back to the inventory file
def write_inventory(lines):
    """Write the given lines back to the inventory file."""
    with open(FILE_NAME, "w") as file:
        file.writelines(lines)

# Function to update an existing product by its ID
def update_product():
    print("\n--- Update Product ---")
    target_id = input("Enter Product ID to update: ")
    updated = False

    lines = read_inventory()  # Get all inventory lines

    # Loop through lines to find the product by ID
    for i, line in enumerate(lines):
        if line.startswith(target_id + ","):
            print("Enter new details:")
            name = input("New Name: ")
            category = input("New Category: ")
            try:
                quantity = int(input("New Quantity: "))
                price = float(input("New Price: "))
                # Replace the line with updated product information
                lines[i] = f"{target_id},{name},{category},{quantity},{price:.2f}\n"
                updated = True
            except ValueError:
                print("Invalid input format.")
                return

    write_inventory(lines)  # Save the updated data
    print("Product updated." if updated else "Product not found.")

# Function to delete a product by its ID
def delete_product():
    print("\n--- Delete Product ---")
    target_id = input("Enter Product ID to delete: ")
    deleted = False

    lines = read_inventory()
    new_lines = []

    # Filter out the product to delete
    for line in lines:
        if line.startswith(target_id + ","):
            deleted = True  # Mark as deleted
        else:
            new_lines.append(line)  # Keep other lines

    write_inventory(new_lines)  # Save filtered list back to file
    print("Product deleted." if deleted else "Product not found.")

# Function to sort products by price in ascending order
def sort_by_price():
    print("\n--- Products Sorted by Price ---")
    lines = read_inventory()

    header = lines[0]  # Extract header
    products = [line.strip().split(",") for line in lines[1:]]  # Parse product lines
    sorted_products = sorted(products, key=lambda x: float(x[4]))  # Sort by price column

    print(header.strip())  # Print header
    for p in sorted_products:
        print(",".join(p))  # Print each sorted product

# Function to filter and display products by a given category
def filter_by_category():
    print("\n--- Filter Products by Category ---")
    category = input("Enter category to filter: ").lower()
    found = False

    lines = read_inventory()

    print(lines[0].strip())  # Print header
    for line in lines[1:]:
        if line.strip().split(",")[2].lower() == category:
            print(line.strip())  # Print matched product
            found = True

    if not found:
        print("No products found in that category.")

# Function to display the entire inventory contents
def inventory_summary():
    print("\n--- Inventory Summary ---")
    with open(FILE_NAME, "r") as file:
        print(file.read())  # Print entire file contents

# Function to summarize inventory by category: total items and value
def category_summary():
    print("\n--- Category Summary ---")
    category_data = {}

    with open(FILE_NAME, "r") as file:
        next(file)  # Skip header
        for line in file:
            _, _, category, quantity, price = line.strip().split(",")
            quantity = int(quantity)
            price = float(price)
            # Initialize category if not already present
            if category not in category_data:
                category_data[category] = {"count": 0, "value": 0.0}
            category_data[category]["count"] += quantity
            category_data[category]["value"] += quantity * price

    # Display the summarized data
    print("Category | Total Items | Total Value")
    for cat, data in category_data.items():
        print(f"{cat:10} | {data['count']:11} | ${data['value']:.2f}")

# Function to display the main menu options
def display_menu():
    print("\nInventory Management System")
    print("1. Add a new product")
    print("2. Update an existing product")
    print("3. Delete a product")
    print("4. Sort products by price")
    print("5. Filter products by category")
    print("6. Generate inventory summary report")
    print("7. Generate category summary report")
    print("8. Exit")

# Main function to control program flow
def main():
    initialize_file()  # Ensure file is ready before actions
    while True:
        display_menu()
        choice = input("Enter choice (1-8): ")
        if choice == '1':
            add_product()
        elif choice == '2':
            update_product()
        elif choice == '3':
            delete_product()
        elif choice == '4':
            sort_by_price()
        elif choice == '5':
            filter_by_category()
        elif choice == '6':
            inventory_summary()
        elif choice == '7':
            category_summary()
        elif choice == '8':
            print("Exiting. See you again!")
            break
        else:
            print("Invalid choice.")

# Only run main if this script is executed directly
if __name__ == "__main__":
    main()
