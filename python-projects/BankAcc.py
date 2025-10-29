# Input Section
account_number = input("Enter your account number: ")
account_type = input("Enter your account type (s for savings, c for checking): ").lower()
min_balance = float(input("Enter the minimum balance: "))
current_balance = float(input("Enter your current balance: "))

# Display header
print("\n---Bank Account Summary ---")
print("Account Number:", account_number)

# Processing
if account_type == 's': # Savings account
    print("Account Type: Savings Account")
    # Savings account has a flat interest rate of 4%
    if current_balance < min_balance:
        current_balance -= 10.00
        print("Service charge of $10.00 applied (Below minimum balance).")
    else:
        # Savings account earns interest based on the balance
        interest = current_balance * 0.04
        current_balance += interest
        print("Interest of 4% applied.")

elif account_type == 'c':  # Checking account
    print("Account Type: Check Account") 
    if current_balance < min_balance:
        current_balance -= 25.00 # Service charge for checking account below minimum balance
        print("Service charge of $25.00 applied (Below minimum balance).")
    else:
        # Checking account earns interest based on the balance
        if current_balance - min_balance <= 5000:
            interest = current_balance * 0.03
            print("Interest of 3% applied.")
        else:
            # Checking account earns 5% interest if balance exceeds $5000
            interest = current_balance * 0.05
            print("Interest of 5% applied.")
        current_balance += interest

else:
    # Invalid account type handling
    print("Invalid account type entered. Please enter 's' for savings or 'c' for checking.")

# Final balance output
print(f"Final Balance: ${current_balance:.2f}")
print("Thank you for using our banking service!") 
