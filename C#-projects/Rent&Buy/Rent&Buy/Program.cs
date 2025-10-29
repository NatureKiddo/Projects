internal class Program
{
    static void Main(string[] args)
    {
        double income = SetExpensenAndIncome(); //The income is the following catergories or the expenses such as : water & lights
        SetAccommodation(income);
        Console.ReadKey();// read the next line of characters from the standard input stream
    }
    static void SetAccommodation(double income)//income can be with double dights at the end (0.00)
    {
        while (true)//loop of the whole program
        {
            Console.Write("Are you buying a property or renting an accommodation: ");//Question for the user if the buying or renting after the income/expenses
            switch (Console.ReadLine())// read the next line of characters from the standard input stream
            {
                case "rent"://rent section
                    {   
                        Console.WriteLine("\nEnter the following for a home loan.");//home loan
                        Console.Write("Enter the monthly rental amount: ");//the montly rental amount
                        double rentalAmount = double.Parse(Console.ReadLine());//input of the amount can be a double (0.00)
                        Console.WriteLine("The deposit amount: ");//The deposit before rentong out the accommodation
                        double depositAmount = double.Parse(Console.ReadLine());//input of the amount can be a double (0.00)
                        Console.WriteLine("What is the interest rate: ");//the value added tax
                        double interest = double.Parse(Console.ReadLine());//input of the amount can be a double (0.00)
                        Console.WriteLine("Enter the number of months you would like to rent: ");//Number of month renting 
                            int numberOfMonths = int.Parse(Console.ReadLine());//input is an INT variable
                            double monthlyLoans = 0;//home loan is 0 currently
                            if (numberOfMonths >= 1)//if it equals or is more than 1 month
                            {
                                interest = interest / 100 / 12;//The interest rate 
                                monthlyLoans = (rentalAmount - depositAmount) * (interest * Math.Pow(1 + interest, numberOfMonths)//calculations
                                    / (Math.Pow(1 + interest, numberOfMonths) - 1));
                                if (monthlyLoans > income / 3)//if statmnet about whether it approved or not
                                {
                                    Console.WriteLine("Approval of the home loan is unlikely");
                                    return;//if the conditions matched the return and display this.
                                }
                                Console.WriteLine("Approval of the home loan is likely");
                                return;//if the conditions matched the return and display this.
                        }
                            Console.WriteLine("Error. Enter again");
                        return;
                        break;//stop this section program
                    }
                case "buy"://buy section
                    {
                        Console.WriteLine("\nEnter the following values for a home loan");//home loan
                        Console.Write("What is the price of the propetry you want to purchase:");//What is the price 
                        double priceOfProperty = double.Parse(Console.ReadLine());//input of the amount can be a double (0.00)
                        Console.Write("Total deposit:");//the final amount of the deposit
                        double totalDeposit = double.Parse(Console.ReadLine());//input of the amount can be a double (0.00)
                        Console.Write("What is the Interest Rate (percentage):");//the interest rate
                        double rate = double.Parse(Console.ReadLine());//input of the amount can be a double (0.00)
                        while (true)//loop of the buy section
                        {
                            Console.Write("Number of months to repay (between 240 and 360):");//months for the payment to be finished
                            int monthsToRepay = int.Parse(Console.ReadLine());//input is an INT variable
                            double monthlyHomeLoans = 0;//home loan is 0
                            if (monthsToRepay >= 240 && monthsToRepay <= 360)//if statment about the monthly payment between 240 and 360 months
                            {
                                rate = rate / 100 / 12;//percentage rate
                                monthlyHomeLoans = (priceOfProperty - totalDeposit) * (rate * Math.Pow(1 + rate, monthsToRepay)//calculations
                                    / (Math.Pow(1 + rate, monthsToRepay) - 1));
                                if (monthlyHomeLoans > income / 3)//if statmnet about whether it approved or not
                                {
                                    Console.WriteLine("Approval of the home loan is unlikely");
                                    return;//if the conditions matched the retuen and display this.
                                }
                                Console.WriteLine("Approval of the home loan is likely");
                                return;//if the conditions matched the retuen and display this.
                            }
                            Console.WriteLine("Error. Enter again");//if type incorrectly
                        }
                        break;
                    }
                default:
                    {
                        Console.WriteLine("You must enter either buy or rent");//when miss spelling or not typing "rent" or "buy"
                        break;
                    }
            }
        }
    }
    static double SetExpensenAndIncome()//Questions for infomation of the income being received and expenses being spent 
    {
        Console.WriteLine("Welcome To The Rent or Buy Application.");//Welcoming message
        Console.WriteLine("Enter your expenses and income");//income and expense
        Console.Write("Gross monthly income (before deductions): ");//enter the gross monthly salary before deductions
        double income = double.Parse(Console.ReadLine());//input can be a double 
        Console.Write("Estimated monthly tax deducted: ");//give or take on your monthly tax deductions
        Console.ReadLine();// read the next line of characters from the standard input stream
        Console.WriteLine("\nEnter your esitmated amounts of expense in the following categories:");//Monthly expenses
        Console.Write("Groceries: ");//Shopping for the house
        Console.ReadLine();// read the next line of characters from the standard input stream
        Console.Write("Water & Lights: ");//drinking/bathing or charging/using appliances
        Console.ReadLine();// read the next line of characters from the standard input stream
        Console.Write("Travel Costs (including petrol): ");//traveling to work or shops
        Console.ReadLine();// read the next line of characters from the standard input stream
        Console.Write("Cellphone & Telephone: ");//phone on contract/how much you pay to get airtime or mega bits
        Console.ReadLine();// read the next line of characters from the standard input stream
        Console.Write("Other Expenses: ");//could be your monthly subscription on things like news or fashion or makeup 
        Console.ReadLine();// read the next line of characters from the standard input stream
        return income;//transfer the income totals to the other sections
    }
}